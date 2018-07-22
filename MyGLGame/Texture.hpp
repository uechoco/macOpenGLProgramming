//
//  Texture.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/22.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <string>

class Texture
{
public:
    //! ctor
    Texture(const std::string& filename);
    //! dtor
    ~Texture();

    void Bind();
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
private:
    GLuint nameHandle;
    float width;
    float height;
};

#endif /* Texture_hpp */
