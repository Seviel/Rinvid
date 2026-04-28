/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <utility>

#include <gtest/gtest.h>

#include <rinvid/core/render_context.h>
#include <rinvid/core/shader.h>

#include "include/opengl_test.h"

namespace
{

constexpr const char* vertex_shader_source =
    "#version 330 core\n"
    "layout(location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(position, 1.0);\n"
    "}\n";

constexpr const char* fragment_shader_source =
    "#version 330 core\n"
    "out vec4 out_color;\n"
    "void main()\n"
    "{\n"
    "    out_color = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n";

} // namespace

using namespace rinvid;

TEST_F(OpenGLTest, ShaderMoveAssignment_LeavesDestinationUsable)
{
    Shader shader_1{vertex_shader_source, fragment_shader_source};
    Shader shader_2{vertex_shader_source, fragment_shader_source};

    shader_2 = std::move(shader_1);

    EXPECT_EQ(shader_1.get_id(), 0U);
    EXPECT_NE(shader_2.get_id(), 0U);
    EXPECT_NO_THROW(shader_2.use());
}

TEST_F(OpenGLTest, RenderContextShutdown_ReleasesDefaultShadersAndAllowsReinit)
{
    EXPECT_NE(render_context_.get_shape_default_shader_id(), 0U);
    EXPECT_NE(render_context_.get_texture_default_shader_id(), 0U);
    EXPECT_NE(render_context_.get_text_default_shader_id(), 0U);

    render_context_.shutdown();

    EXPECT_EQ(render_context_.get_shape_default_shader_id(), 0U);
    EXPECT_EQ(render_context_.get_texture_default_shader_id(), 0U);
    EXPECT_EQ(render_context_.get_text_default_shader_id(), 0U);

    render_context_.set_viewport(0, 0, 32, 32);
    render_context_.init(nullptr);

    EXPECT_NE(render_context_.get_shape_default_shader_id(), 0U);
    EXPECT_NE(render_context_.get_texture_default_shader_id(), 0U);
    EXPECT_NE(render_context_.get_text_default_shader_id(), 0U);
}
