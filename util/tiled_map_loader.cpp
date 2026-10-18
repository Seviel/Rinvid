/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <charconv>
#include <cmath>
#include <limits>
#include <locale>
#include <sstream>
#include <string_view>
#include <utility>

#include <tinyxml2.h>

#include <rinvid/util/error.h>
#include <rinvid/util/tiled_map_loader.h>

namespace rinvid
{
namespace
{

using Element = tinyxml2::XMLElement;
constexpr std::uint32_t GID_MASK{0x0FFFFFFFU};

[[noreturn]] void fail(const std::filesystem::path& file, const std::string& message)
{
    throw ResourceLoadError{"Tiled: '" + file.string() + "': " + message};
}

[[noreturn]] void fail(const std::filesystem::path& file, const Element& element,
                       const std::string& message)
{
    throw ResourceLoadError{"Tiled: '" + file.string() + "', line " +
                            std::to_string(element.GetLineNum()) + " <" + element.Name() +
                            ">: " + message};
}

std::string_view trim(std::string_view text)
{
    const auto begin = text.find_first_not_of(" \t\r\n");
    if (begin == std::string_view::npos)
    {
        return {};
    }
    return text.substr(begin, text.find_last_not_of(" \t\r\n") - begin + 1U);
}

std::uint32_t parse_unsigned(std::string_view text, const std::filesystem::path& file,
                             const Element& element, const char* field)
{
    text = trim(text);
    std::uint32_t value{};
    if (!text.empty())
    {
        const auto result = std::from_chars(text.data(), text.data() + text.size(), value);
        if (result.ec == std::errc{} && result.ptr == text.data() + text.size())
        {
            return value;
        }
    }
    fail(file, element, std::string{field} + " must be an unsigned 32-bit integer");
}

std::uint32_t number(const Element& element, const char* field, const std::filesystem::path& file,
                     std::uint32_t fallback = 0U, bool required = false)
{
    const char* text = element.Attribute(field);
    if (text == nullptr)
    {
        if (required)
        {
            fail(file, element, std::string{"missing "} + field);
        }
        return fallback;
    }
    return parse_unsigned(text, file, element, field);
}

std::uint32_t positive(const Element& element, const char* field, const std::filesystem::path& file)
{
    const auto value = number(element, field, file, 0U, true);
    if (value == 0U || value > static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()))
    {
        fail(file, element, std::string{field} + " must be in [1, INT32_MAX]");
    }
    return value;
}

float real(const Element& element, const char* field, const std::filesystem::path& file,
           float fallback = 0.0F)
{
    const char* text = element.Attribute(field);
    if (text == nullptr)
    {
        return fallback;
    }
    // Floating-point from_chars is unavailable in the oldest supported GCC versions.
    std::istringstream stream{std::string{trim(text)}};
    stream.imbue(std::locale::classic());
    float value{};
    stream >> std::noskipws >> value;
    if (stream && stream.peek() == std::char_traits<char>::eof() && std::isfinite(value))
    {
        return value;
    }
    fail(file, element, std::string{field} + " must be a finite number");
}

void require_value(const Element& element, const char* field, const char* expected,
                   const std::filesystem::path& file)
{
    const char* value = element.Attribute(field);
    if (value != nullptr && std::string_view{value} != expected)
    {
        fail(file, element, std::string{"unsupported "} + field + "='" + value + "'");
    }
}

const Element& read_document(tinyxml2::XMLDocument& document, const std::filesystem::path& file,
                             const char* root_name)
{
    if (document.LoadFile(file.string().c_str()) != tinyxml2::XML_SUCCESS)
    {
        fail(file, std::string{"cannot read XML: "} + document.ErrorStr());
    }
    const auto* root = document.RootElement();
    if (root == nullptr || std::string_view{root->Name()} != root_name ||
        root->NextSiblingElement())
    {
        fail(file, std::string{"requires a single <"} + root_name + "> root");
    }
    return *root;
}

Tileset read_tileset(const Element& element, const std::filesystem::path& file,
                     const TileMapData& map)
{
    if (positive(element, "tilewidth", file) != map.tile_width_ ||
        positive(element, "tileheight", file) != map.tile_height_)
    {
        fail(file, element, "tileset tile dimensions must match the map grid");
    }

    const auto* image = element.FirstChildElement("image");
    if (image == nullptr || image->NextSiblingElement("image") ||
        image->Attribute("source") == nullptr ||
        std::string_view{image->Attribute("source")}.empty())
    {
        fail(file, element, "a single atlas <image source='...'> is required");
    }

    if (image->Attribute("trans") || image->FirstChildElement("data"))
    {
        fail(file, *image,
             "color-key transparency and embedded images are unsupported; use image alpha");
    }

    if (const auto* offset = element.FirstChildElement("tileoffset"))
    {
        if (real(*offset, "x", file) != 0.0F || real(*offset, "y", file) != 0.0F)
        {
            fail(file, *offset, "tileset offsets are unsupported");
        }
    }

    for (auto* tile = element.FirstChildElement("tile"); tile;
         tile       = tile->NextSiblingElement("tile"))
    {
        if (tile->FirstChildElement("animation") || tile->FirstChildElement("image") ||
            tile->Attribute("x") || tile->Attribute("y") || tile->Attribute("width") ||
            tile->Attribute("height"))
        {
            fail(file, *tile, "animated tiles and per-tile images/regions are unsupported");
        }
    }

    Tileset result{};
    if (const char* name = element.Attribute("name"))
    {
        result.name_ = name;
    }
    result.image_        = (file.parent_path() / image->Attribute("source")).lexically_normal();
    result.tile_count_   = positive(element, "tilecount", file);
    result.columns_      = positive(element, "columns", file);
    result.margin_       = number(element, "margin", file);
    result.spacing_      = number(element, "spacing", file);
    result.image_width_  = positive(*image, "width", file);
    result.image_height_ = positive(*image, "height", file);
    const std::uint64_t rows =
        (static_cast<std::uint64_t>(result.tile_count_) + result.columns_ - 1U) / result.columns_;
    const std::uint64_t atlas_width =
        2ULL * result.margin_ + static_cast<std::uint64_t>(result.columns_) * map.tile_width_ +
        static_cast<std::uint64_t>(result.columns_ - 1U) * result.spacing_;
    const std::uint64_t atlas_height =
        2ULL * result.margin_ + rows * map.tile_height_ + (rows - 1U) * result.spacing_;

    if (atlas_width > result.image_width_ || atlas_height > result.image_height_)
    {
        fail(file, element,
             "atlas dimensions do not contain the declared tiles, margin, and spacing");
    }
    return result;
}

TileLayer read_layer(const Element& element, const std::filesystem::path& file,
                     const TileMapData& map)
{
    if (positive(element, "width", file) != map.width_ ||
        positive(element, "height", file) != map.height_)
    {
        fail(file, element, "layer dimensions must match the finite map");
    }

    if (real(element, "x", file) != 0.0F || real(element, "y", file) != 0.0F ||
        real(element, "parallaxx", file, 1.0F) != 1.0F ||
        real(element, "parallaxy", file, 1.0F) != 1.0F || element.Attribute("tintcolor"))
    {
        fail(file, element,
             "tile offsets, parallax, and tint are unsupported; pixel offsets are supported");
    }

    require_value(element, "mode", "normal", file);
    const auto* data = element.FirstChildElement("data");
    if (data == nullptr || data->NextSiblingElement("data"))
    {
        fail(file, element, "a single <data> element is required");
    }

    require_value(*data, "compression", "", file);
    require_value(*data, "encoding", "csv", file);

    if (data->FirstChildElement("chunk"))
    {
        fail(file, *data, "infinite map chunks are unsupported");
    }

    TileLayer result{};
    if (const char* name = element.Attribute("name"))
    {
        result.name_ = name;
    }

    result.offset_     = {real(element, "offsetx", file), real(element, "offsety", file)};
    result.opacity_    = real(element, "opacity", file, 1.0F);
    const auto visible = number(element, "visible", file, 1U);
    if (result.opacity_ < 0.0F || result.opacity_ > 1.0F || visible > 1U)
    {
        fail(file, element, "opacity must be in [0, 1] and visible must be 0 or 1");
    }

    result.visible_              = visible != 0U;
    const std::uint64_t expected = static_cast<std::uint64_t>(map.width_) * map.height_;
    const auto          append   = [&](std::uint32_t raw_gid)
    {
        if (result.tiles_.size() >= expected)
        {
            fail(file, *data, "too many tile IDs for the layer dimensions");
        }

        const auto gid = raw_gid & GID_MASK;
        if (gid != 0U)
        {
            const auto next = std::upper_bound(map.tilesets_.begin(), map.tilesets_.end(), gid,
                                               [](std::uint32_t id, const Tileset& tileset)
                                               { return id < tileset.first_gid_; });
            if (next == map.tilesets_.begin() ||
                gid - (next - 1)->first_gid_ >= (next - 1)->tile_count_)
            {
                fail(file, *data,
                     "tile ID " + std::to_string(gid) + " has no matching tileset tile");
            }
        }
        result.tiles_.push_back(raw_gid);
    };

    if (data->Attribute("encoding"))
    {
        if (data->FirstChildElement())
        {
            fail(file, *data, "CSV data cannot contain child elements");
        }
        std::string text{};

        for (auto* node = data->FirstChild(); node; node = node->NextSibling())
        {
            if (node->ToText())
            {
                text += node->Value();
            }
        }
        std::string_view remaining{text};

        while (true)
        {
            const auto comma = remaining.find(',');
            append(parse_unsigned(remaining.substr(0U, comma), file, *data, "tile ID"));
            if (comma == std::string_view::npos)
            {
                break;
            }
            remaining.remove_prefix(comma + 1U);
        }
    }
    else
    {
        for (auto* node = data->FirstChild(); node; node = node->NextSibling())
        {
            if (auto* tile = node->ToElement())
            {
                if (std::string_view{tile->Name()} != "tile")
                {
                    fail(file, *tile, "expected <tile gid='...'>");
                }
                append(number(*tile, "gid", file, 0U, true));
            }
            else if (node->ToText() && !trim(node->Value()).empty())
            {
                fail(file, *data, "text tile data requires encoding='csv'");
            }
        }
    }

    if (result.tiles_.size() != expected)
    {
        fail(file, *data, "tile count does not match layer dimensions");
    }

    return result;
}

} // namespace

TileMapData load_tiled_map(const std::filesystem::path& file_name)
{
    std::error_code error{};
    const auto      file = std::filesystem::absolute(file_name, error).lexically_normal();
    if (error)
    {
        fail(file_name, "invalid map path: " + error.message());
    }

    tinyxml2::XMLDocument document{};
    const auto&           root = read_document(document, file, "map");
    if (!root.Attribute("orientation", "orthogonal"))
    {
        fail(file, root, "only orthogonal maps are supported");
    }

    require_value(root, "renderorder", "right-down", file);
    if (number(root, "infinite", file) != 0U)
    {
        fail(file, root, "infinite maps are unsupported");
    }

    TileMapData map{};
    map.width_       = positive(root, "width", file);
    map.height_      = positive(root, "height", file);
    map.tile_width_  = positive(root, "tilewidth", file);
    map.tile_height_ = positive(root, "tileheight", file);
    if (static_cast<std::uint64_t>(map.width_) * map.tile_width_ >
            std::numeric_limits<std::int32_t>::max() ||
        static_cast<std::uint64_t>(map.height_) * map.tile_height_ >
            std::numeric_limits<std::int32_t>::max())
    {
        fail(file, root, "map pixel dimensions exceed INT32_MAX");
    }
    for (auto* element = root.FirstChildElement("tileset"); element;
         element       = element->NextSiblingElement("tileset"))
    {
        Tileset tileset{};
        if (const char* source = element->Attribute("source"))
        {
            const auto            tileset_file = (file.parent_path() / source).lexically_normal();
            tinyxml2::XMLDocument tileset_document{};
            tileset = read_tileset(read_document(tileset_document, tileset_file, "tileset"),
                                   tileset_file, map);
        }
        else
        {
            tileset = read_tileset(*element, file, map);
        }

        tileset.first_gid_ = positive(*element, "firstgid", file);
        if (static_cast<std::uint64_t>(tileset.first_gid_) + tileset.tile_count_ - 1U > GID_MASK ||
            (!map.tilesets_.empty() &&
             tileset.first_gid_ < static_cast<std::uint64_t>(map.tilesets_.back().first_gid_) +
                                      map.tilesets_.back().tile_count_))
        {
            fail(file, *element,
                 "tileset GID ranges must be ascending, non-overlapping, and below flip flags");
        }
        map.tilesets_.push_back(std::move(tileset));
    }

    for (auto* element = root.FirstChildElement(); element; element = element->NextSiblingElement())
    {
        const std::string_view name{element->Name()};
        if (name == "layer")
        {
            map.layers_.push_back(read_layer(*element, file, map));
        }
        else if (name == "group" || name == "objectgroup" || name == "imagelayer")
        {
            fail(file, *element, "only tile layers are supported");
        }
    }

    return map;
}

} // namespace rinvid
