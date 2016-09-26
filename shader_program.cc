// Copyright (C) 2016 West Virginia University.
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
//     * Neither the name of West Virginia University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Victor Fragoso (victor.fragoso@mail.wvu.edu)

#include "shader_program.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <GL/glew.h>

namespace wvu {
namespace {
// Buffer size for the error log info.
constexpr int kNumCharsInfoLog = 512;

// Enumeration to select the shader types.
enum ShaderType {
  VERTEX = 0,
  FRAGMENT = 1
};

// Compiles a shader that is contained in shader_src C++ string. The shader type
// determines what shader we should compile. This function retrieves the errors
// in case of compilation errors and stores it into info_log. This function
// returns the shader id if successful, otherwise it returns zero.
GLuint CompileShader(const std::string& shader_src,
                     const ShaderType shader_type,
                     std::string* info_log) {
  // TASK: Implement me!
  return 0;
}

// Creates a shader program. This function requires the ids of the vertex and
// fragment shaders which were successfully compiled. The function can return
// the error info log string in case of a failure. The function returns the
// shader program id if successfull, and returns zero otherwise.
GLuint CreateShaderProgram(const GLuint vertex_shader,
                           const GLuint fragment_shader,
                           std::string* info_log) {
  // TASK: Implement me!
  return 0;
}

// Releases the resources allocated for compilation of shaders.
// Clear the shader sources strings.
void ReleaseShaderResources(const GLuint vertex_shader,
                            const GLuint fragmnet_shader) {
  // TASK: Implement me!
}

// Loads a shader source from a file. The function receives the filepath
// reads it, and returns the content of the file in loaded_file. Returns
// true if successful, otherwise false.
bool LoadShaderFromFile(const std::string& filepath,
                        std::string* loaded_file) {
  // Create an input file stream.
  std::ifstream in(filepath);
  // Verify if it is open and that we have a valid sink C++ string.
  if (!in.is_open() || !loaded_file) {
    return false;
  }
  // Create a string buffer to load the entire file in a single call.
  std::stringstream string_buffer;
  // Load the file in a single read.
  string_buffer << in.rdbuf();
  // Close the file.
  in.close();
  // Extract the string from the string buffer.
  *loaded_file = string_buffer.str();
  return true;
}

}  // namespace

bool ShaderProgram::LoadVertexShaderFromString(
    const std::string& vertex_shader_source) {
  vertex_shader_src_ = vertex_shader_source;
  return true;
}

bool ShaderProgram::LoadFragmentShaderFromString(
    const std::string& fragment_shader_source) {
  fragment_shader_src_ = fragment_shader_source;
  return true;
}

bool ShaderProgram::LoadVertexShaderFromFile(
    const std::string& vertex_shader_path) {
  return LoadShaderFromFile(vertex_shader_path, &vertex_shader_src_);
}

bool ShaderProgram::LoadFragmentShaderFromFile(
    const std::string& fragment_shader_path) {
  return LoadShaderFromFile(fragment_shader_path, &fragment_shader_src_);
}

bool ShaderProgram::Create(std::string* error_info_log) {
  // If an instance of this class already created a shader program, the Create()
  // method will report true. No need to build again. If different shader
  // sources are used, then a different instance should be called.
  if (created_) return true;
  return false;
}

bool ShaderProgram::BuildVertexShader(std::string* info_log) {
  vertex_shader_ = CompileShader(vertex_shader_src_, VERTEX, info_log);
  return vertex_shader_ != 0;
}

bool ShaderProgram::BuildFragmentShader(std::string* info_log) {
  fragment_shader_ = CompileShader(fragment_shader_src_, FRAGMENT, info_log);
  return fragment_shader_ != 0;
}

bool ShaderProgram::LinkProgram(std::string* info_log) {
  shader_program_id_ = CreateShaderProgram(vertex_shader_,
                                           fragment_shader_,
                                           info_log);
  ReleaseShaderResources(vertex_shader_, fragment_shader_);
  return shader_program_id_ != 0;
}

}  // namespace wvu
