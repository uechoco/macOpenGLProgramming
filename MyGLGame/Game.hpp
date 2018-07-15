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
    static float PingPong(float t);

private:
    float value = 0.f;
};

#endif /* Game_hpp */
