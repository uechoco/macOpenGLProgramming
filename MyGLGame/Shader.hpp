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
#include <GLKit/GLKMath.h>
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
    void    SetUniform(const std::string& name, const GLKVector2& vec);
    void    SetUniform(const std::string& name, const GLKVector3& vec);
    void    SetUniform(const std::string& name, const GLKVector4& vec);
    void    SetUniform(const std::string& name, const GLKMatrix4& mat);

private:
    GLuint  programHandle;
    Shader  *vshader;
    Shader  *fshader;
    std::unordered_map<std::string, GLint> uniformLocationMap;
};


#endif /* Shader_hpp */
