//
//  Game.cpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Game.hpp"
#include "Input.hpp"
#include <cmath>

Game::Game()
{
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Game::~Game()
{
    // TODO: ここでテクスチャなどの解放処理を行う
}

float Game::PingPong(float t)
{
    t -= std::floorf(t / 2.0f) * 2.0f;
    return 1.0f - std::fabsf(t - 1.0f);
}

void Game::Render()
{
    if (Input::GetMouseButton(0)) {
        GLKVector2 pos = Input::GetMousePosition();
        value1 = (pos.x + 1.0f) / 2.0f;
        value2 = (pos.y + 1.0f) / 2.0f;
    }

    glClearColor(value1, value2, 1.0f - value1 * value2, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
