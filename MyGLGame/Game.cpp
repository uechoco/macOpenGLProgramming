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
    if (Input::GetKey(KeyCode::LeftArrow)) {
        value -= 0.25f * Time::deltaTime;
    }

    if (Input::GetKey(KeyCode::RightArrow)) {
        value += 0.25f * Time::deltaTime;
    }

    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }

    glClearColor(1.0f - PingPong(value), PingPong(value), 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
