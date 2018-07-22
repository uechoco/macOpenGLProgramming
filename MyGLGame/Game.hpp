//
//  Game.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include "Time.hpp"

// Type Dependencies
class ShaderProgram;

// Type Definitions
class Game
{
public:
    //! ctor
    Game();
    //! dtor
    ~Game();

    //! 描画
    void Render();

private:
    float   value1 = 0.f;
    float   value2 = 0.f;
    //! シェーダープログラム
    ShaderProgram* pProgram;
    //! Vertex Buffer Objectのハンドル
    GLuint  vbo;
    //! Vertex Array Objectのハンドル
    GLuint  vao;
    //! Index Buffer Objectのハンドル
    GLuint ibo;
};

#endif /* Game_hpp */
