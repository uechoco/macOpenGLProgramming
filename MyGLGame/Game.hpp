//
//  Game.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "Time.hpp"
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <GLKit/GLKMath.h>
#include <vector>

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
    struct VertexData
    {
        GLKVector3  pos;
        GLKVector4  color;
    };
    //! シェーダープログラム
    ShaderProgram* pProgram;
    //! 描画データ
    std::vector<VertexData> data;
    //! Vertex Buffer Objectのハンドル
    GLuint  vbo;
    //! Vertex Array Objectのハンドル
    GLuint  vao;
    //! Index Buffer Objectのハンドル
    GLuint ibo;

    //! カメラの位置
    GLKVector3              cameraPos;
    //! ビュー行列のdirtyフラグ
    bool bCameraDirty;
};

#endif /* Game_hpp */
