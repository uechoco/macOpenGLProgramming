//
//  Mesh.hpp
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/09/14.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <GLKit/GLKMath.h>
#include <string>

class Mesh
{
    GLuint vbo;
    GLuint vao;
    GLuint ibo;
    GLenum indexType;
    GLsizei indexCount;
    
public:
    Mesh(const std::string& filename, const GLKVector4& color = {1.0f, 1.0f, 1.0f, 1.0f}, float smoothingLevel = 1.0f);
    ~Mesh();
    
    static Mesh* CreateAsCube();
    static Mesh* CreateAsPlane();
    
    void Draw() const;
    
private:
    Mesh() = default;
};

#endif /* Mesh_hpp */
