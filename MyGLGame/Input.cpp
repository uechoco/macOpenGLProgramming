//
//  Input.cpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/16.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Input.hpp"

uint64_t Input::keyState                = 0ULL;
uint64_t Input::keyStateOld             = 0ULL;
uint64_t Input::keyDownStateTriggered   = 0ULL;
uint64_t Input::keyUpStateTriggered     = 0ULL;

bool Input::GetKey(uint64_t keyCode)
{
    return (keyState & keyCode)? true: false;
}

bool Input::GetKeyDown(uint64_t keyCode)
{
    return (keyDownStateTriggered & keyCode)? true: false;
}

bool Input::GetKeyUp(uint64_t keyCode)
{
    return (keyUpStateTriggered & keyCode)? true: false;
}

void Input::ProcessKeyDown(uint64_t keyCode)
{
    keyState |= keyCode;
}

void Input::ProcessKeyUp(uint64_t keyCode)
{
    keyState &= ~keyCode;
}

void Input::Update()
{
    keyDownStateTriggered = keyState & ~keyStateOld;
    keyUpStateTriggered = ~keyState & keyStateOld;
    keyStateOld = keyState;
}
