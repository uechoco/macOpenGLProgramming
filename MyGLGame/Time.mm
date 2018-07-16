//
//  Time.cpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/16.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Time.hpp"

#import <Foundation/Foundation.h>

static NSTimeInterval   startTime;
static NSTimeInterval   oldTime;
static NSTimeInterval   oldFPSTime;

unsigned Time::frameCount   = 0;
float Time::time            = 0.0f;
float Time::deltaTime       = 0.0f;
float Time::fps             = 0.0f;

void Time::Start()
{
    startTime = [NSDate timeIntervalSinceReferenceDate];
    oldTime = startTime;
    oldFPSTime = 0.0f;
}

void Time::Update()
{
    NSTimeInterval now = [NSDate timeIntervalSinceReferenceDate];
    time = now - startTime;
    deltaTime = now - oldTime;
    oldTime = now;

    frameCount++;
    if (frameCount % 60 == 0)
    {
        fps = 60.0f / (time - oldFPSTime);
        oldFPSTime = time;
    }
}
