//
//  Time.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/16.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Time_hpp
#define Time_hpp

struct Time
{
    static unsigned frameCount; //!< フレーム数
    static float    time;       //!< ゲーム開始時からの経過時間（秒）
    static float    deltaTime;  //!< 直前のフレームからの経過時間（秒）
    static float    fps;        //!< FPS

    static void Start();
    static void Update();
};


#endif /* Time_hpp */
