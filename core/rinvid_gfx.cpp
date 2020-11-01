#include "include/rinvid_gfx.h"

namespace
{

static const char* vert_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

static const char* frag_shader_source =
    "#version 330 core\n"
    "out vec4 out_color;\n "
    "uniform vec4 in_color;\n"
    "void main()\n"
    "{\n"
    "   out_color = in_color;\n"
    "}\n";

static void set_default_shader(std::uint32_t& default_shader_ref)
{
    std::uint32_t vert_shader;
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_source, nullptr);
    glCompileShader(vert_shader);

    std::uint32_t frag_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, nullptr);
    glCompileShader(frag_shader);

    default_shader_ref = glCreateProgram();

    glAttachShader(default_shader_ref, vert_shader);
    glAttachShader(default_shader_ref, frag_shader);
    glLinkProgram(default_shader_ref);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glUseProgram(default_shader_ref);
}

} // namespace

namespace rinvid
{

std::uint32_t RinvidGfx::default_shader{};

void RinvidGfx::init()
{
    set_default_shader(RinvidGfx::default_shader);
}

void RinvidGfx::set_viewport(std::int32_t x, std::int32_t y, std::int32_t width,
                             std::int32_t heigth)
{
    glViewport(x, y, width, heigth);
}

void RinvidGfx::clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::uint32_t RinvidGfx::get_default_shader()
{
    return default_shader;
}

} // namespace rinvid
