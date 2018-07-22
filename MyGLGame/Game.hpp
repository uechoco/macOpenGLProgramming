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
class Texture;

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
    //! シェーダープログラム
    ShaderProgram* pProgram;
    //! Vertex Buffer Objectのハンドル
    GLuint  vbo;
    //! Vertex Array Objectのハンドル
    GLuint  vao;
    //! Index Buffer Objectのハンドル
    GLuint ibo;
    //! テクスチャ
    Texture* pTex1;
    Texture* pTex2;
    float    t;
};

#endif /* Game_hpp */
