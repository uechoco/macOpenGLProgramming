//
//  Shader.cpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/21.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Shader.hpp"
#include "StringSupport.hpp"

/**
 @brief シェーダーのコンパイル
 @param shaderType 頂点シェーダ:GL_VERTEX_SHADER、フラグメントシェーダ:GL_FRAGMENT_SHADER
 @param filename シェーダファイル
 */
Shader::Shader(GLenum shaderType, const std::string& filename)
{
    shaderHandle = glCreateShader(shaderType);
    if (shaderHandle == 0) {
        throw GameError("Shader::Shader() Failed to create a shader: %s", filename.c_str());
    }

    std::string sourceStr = ReadTextFile(filename);
    const GLchar *source = sourceStr.c_str();
    glShaderSource(shaderHandle, 1, &source, NULL);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        throw GameError("Shader::Shader() Failed to glShaderSource(): error=%d", error);
    }

    glCompileShader(shaderHandle);
    GLint result;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *logBuffer = new GLchar[logLength];
            glGetShaderInfoLog(shaderHandle, logLength, &logLength, logBuffer);
            printf("-----\n");
            printf("[Shader Compile Log] %s", logBuffer);
            printf("-----\n");
            delete[] logBuffer;
        }
        throw GameError("Shader::Shader() Failed to compile a shader: %s", filename.c_str());
    }
}

Shader::~Shader()
{
    glDeleteShader(shaderHandle);
}

ShaderProgram::ShaderProgram(const std::string& vshName, const std::string& fshName)
{
    vshader = new Shader(GL_VERTEX_SHADER, vshName);
    fshader = new Shader(GL_FRAGMENT_SHADER, fshName);

    programHandle = glCreateProgram();

    glAttachShader(programHandle, vshader->GetHandle());
    glAttachShader(programHandle, fshader->GetHandle());
    glLinkProgram(programHandle);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programHandle);
    delete vshader;
    delete fshader;
}

void ShaderProgram::Use()
{
    glUseProgram(programHandle);
}
