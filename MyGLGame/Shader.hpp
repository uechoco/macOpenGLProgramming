//
//  Shader.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/21.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <unordered_map>
#include <string>

class Shader
{
public:
    Shader(GLenum shaderType, const std::string& filename);
    ~Shader();

    GLuint  GetHandle() const { return shaderHandle; }

private:
    GLuint  shaderHandle;

};

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vshName, const std::string& fshName);
    ~ShaderProgram();

    void    Use();
    GLint   GetUniformLocation(const std::string& name);
    void    SetUniform(const std::string& name, int value);
    void    SetUniform(const std::string& name, float value);

private:
    GLuint  programHandle;
    Shader  *vshader;
    Shader  *fshader;
    std::unordered_map<std::string, GLint> uniformLocationMap;
};


#endif /* Shader_hpp */
