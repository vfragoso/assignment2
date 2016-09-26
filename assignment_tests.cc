// Copyright (C) 2016  Victor Fragoso <victor.fragoso@mail.wvu.edu>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of the West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL VICTOR FRAGOSO BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

// C headers.
#include <stdlib.h>  // For random.

// C++ headers.
#include <algorithm>  // For std::reverse.
#include <numeric>  // For std::accumulate.
#include <unordered_set>
#include <vector>

// System specific headers.
#include "assignment.h"
#include "Eigen/Core"
#include "glog/logging.h"
#include "gtest/gtest.h"
#include "shader_program.h"

// These are unit tests using google gtest library. A binary will be created
// automatically to run the tests below.
namespace wvu {
namespace {
// GLSL shaders.
// Every shader should declare its version.
// Vertex shader follows standard 3.3.0.
// This shader declares/expexts an input variable named position. This input
// should have been loaded into GPU memory for its processing. The shader
// essentially sets the gl_Position -- an already defined variable -- that
// determines the final position for a vertex.
// Note that the position variable is of type vec3, which is a 3D dimensional
// vector. The layout keyword determines the way the VAO buffer is arranged in
// memory. This way the shader can read the vertices correctly.
const std::string vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "\n"
    "void main() {\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"
    "}\n";

// Fragment shader follows standard 3.3.0. The goal of the fragment shader is to
// calculate the color of the pixel corresponding to a vertex. This is why we
// declare a variable named color of type vec4 (4D vector) as its output. This
// shader sets the output color to a (1.0, 0.5, 0.2, 1.0) using an RGBA format.
const std::string fragment_shader_src =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";
}  // namespace

TEST(LinearAlgebra, Add3dPoints) {
  const Eigen::Vector3f x = Eigen::Vector3f::Random();
  const Eigen::Vector3f y = Eigen::Vector3f::Random();
  const Eigen::Vector3f result = Add3dPoints(x, y);
  EXPECT_EQ((result - x).norm(), y.norm());
  EXPECT_EQ((result - y).norm(), x.norm());
}

TEST(LinearAlgebra, Add4dPoints) {
  const Eigen::Vector4f x = Eigen::Vector4f::Random();
  const Eigen::Vector4f y = Eigen::Vector4f::Random();
  const Eigen::Vector4f result = Add4dPoints(x, y);
  EXPECT_NEAR((result - x).norm(), y.norm(), 1e-3);
  EXPECT_NEAR((result - y).norm(), x.norm(), 1e-3);
}

TEST(LinearAlgebra, Multiply4x4Matrices) {
  const Eigen::Matrix4f x = Eigen::Matrix4f::Random();
  const Eigen::Matrix4f y = Eigen::Matrix4f::Identity();
  const Eigen::Matrix4f result = Multiply4x4Matrices(x, y);
  EXPECT_NEAR(result.norm(), x.norm(), 1e-3);
}

TEST(LinearAlgebra, MultiplyVectorAndMatrix) {
  const Eigen::Matrix4f x = Eigen::Matrix4f::Identity();
  const Eigen::Vector4f y = Eigen::Vector4f::Random();
  const Eigen::Vector4f result = MultiplyVectorAndMatrix(x, y);
  EXPECT_NEAR(y.norm(), result.norm(), 1e-3);
}

TEST(LinearAlgebra, ComputeDotProduct) {
  const Eigen::Vector3f y = Eigen::Vector3f::Random();
  EXPECT_NEAR(y.squaredNorm(), ComputeDotProduct(y, y), 1e-3);
}

TEST(LinearAlgebra, CalculateAngleBetweenTwoVectors) {
  const Eigen::Vector3f x = Eigen::Vector3f::UnitX();
  const Eigen::Vector3f y = Eigen::Vector3f::UnitY();
  const float angle = CalculateAngleBetweenTwoVectors(x, y);
  constexpr float kSquaredAngleInRadians = 3.1416 / 2.0f;
  EXPECT_NEAR(angle, kSquaredAngleInRadians, 1e-3);
}

TEST(LinearAlgebra, ComputeCrossProduct) {
  const Eigen::Vector3f x = Eigen::Vector3f::UnitX();
  const Eigen::Vector3f y = Eigen::Vector3f::UnitY();
  const Eigen::Vector3f z = Eigen::Vector3f::UnitZ();
  const Eigen::Vector3f result = ComputeCrossProduct(x, y);
  EXPECT_NEAR(1.0f, ComputeDotProduct(z, result), 1e-3);
}

TEST(ShaderProgram, CreateProgramFromValidShaderSources) {
  ShaderProgram shader_program;
  EXPECT_TRUE(shader_program.LoadVertexShaderFromString(vertex_shader_src));
  EXPECT_TRUE(shader_program.LoadFragmentShaderFromString(fragment_shader_src));
  std::string error_info_log;
  EXPECT_TRUE(shader_program.Create(&error_info_log));
  EXPECT_GT(shader_program.shader_program_id(), 0);
}

TEST(ShaderProgram, CreateProgramFromInvalidShaderSources) {
  ShaderProgram shader_program;
  const std::string bad_vertex_shader = vertex_shader_src + "asdasdjqw;rjdekl";
  const std::string bad_fragment_shader = vertex_shader_src + "asdasdjqw;jdekl";
  EXPECT_TRUE(shader_program.LoadVertexShaderFromString(bad_vertex_shader));
  EXPECT_TRUE(shader_program.LoadFragmentShaderFromString(bad_fragment_shader));
  std::string error_info_log;
  EXPECT_FALSE(shader_program.Create(&error_info_log));
  EXPECT_GT(error_info_log.size(), 0);
  EXPECT_GT(shader_program.shader_program_id(), 0);
}

}  // namespace wvu
