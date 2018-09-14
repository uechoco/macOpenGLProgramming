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
#include "Mesh.hpp"
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
    //! シェーダープログラム
    ShaderProgram* pProgram = nullptr;
    //! メッシュ
    Mesh*          pMesh = nullptr;

    //! カメラの位置
    GLKVector3              cameraPos;
    //! PV行列
    GLKMatrix4              projViewMat;
    //! ビュー行列のdirtyフラグ
    bool bCameraDirty;
    //! perspective(透視投影) or orthographic(正射影)
    bool bPerspective;
};

#endif /* Game_hpp */
