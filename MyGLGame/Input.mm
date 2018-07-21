//
//  Input.cpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/16.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Input.hpp"
#import "MyGLView.h"

uint64_t Input::keyState                = 0ULL;
uint64_t Input::keyStateOld             = 0ULL;
uint64_t Input::keyDownStateTriggered   = 0ULL;
uint64_t Input::keyUpStateTriggered     = 0ULL;

bool Input::isMouseDown = false;
bool Input::isMouseDownOld = false;
bool Input::isMouseDownTriggered = false;
bool Input::isMouseUpTriggered = false;

bool Input::isMouseDownRight = false;
bool Input::isMouseDownOldRight = false;
bool Input::isMouseDownTriggeredRight = false;
bool Input::isMouseUpTriggeredRight = false;

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

bool Input::GetMouseButton(int button)
{
    if (button == 0) {
        return isMouseDown;
    } else {
        return isMouseDownRight;
    }
}

bool Input::GetMouseButtonDown(int button)
{
    if (button == 0) {
        return isMouseDownTriggered;
    } else {
        return isMouseDownTriggeredRight;
    }
}

bool Input::GetMouseButtonUp(int button)
{
    if (button == 0) {
        return isMouseUpTriggered;
    } else {
        return isMouseUpTriggeredRight;
    }
}

GLKVector2 Input::GetMousePosition()
{
    return [[MyGLView sharedInstance] mousePosition];
}

void Input::ProcessKeyDown(uint64_t keyCode)
{
    keyState |= keyCode;
}

void Input::ProcessKeyUp(uint64_t keyCode)
{
    keyState &= ~keyCode;
}

void Input::ProcessMouseDown()
{
    isMouseDown = true;
}

void Input::ProcessMouseUp()
{
    isMouseDown = false;
}

void Input::ProcessMouseDownRight()
{
    isMouseDownRight = true;
}

void Input::ProcessMouseUpRight()
{
    isMouseDownRight = false;
}

void Input::Update()
{
    keyDownStateTriggered = keyState & ~keyStateOld;
    keyUpStateTriggered = ~keyState & keyStateOld;
    keyStateOld = keyState;

    isMouseDownTriggered = isMouseDown & ~isMouseDownOld;
    isMouseUpTriggered = ~isMouseDown & isMouseDownOld;
    isMouseDownOld = isMouseDown;

    isMouseDownTriggeredRight = isMouseDownRight & ~isMouseDownOldRight;
    isMouseUpTriggeredRight = ~isMouseDownRight & isMouseDownOldRight;
    isMouseDownOldRight = isMouseDownRight;
}
