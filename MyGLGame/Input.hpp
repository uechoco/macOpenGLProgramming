//
//  Input.hpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/16.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#ifndef Input_hpp
#define Input_hpp

#include <GLKit/GLKMath.h>
#include <cstdlib>

struct KeyCode
{
    enum : uint64_t
    {
        UpArrow = 1ULL << 0,
        DownArrow = 1ULL << 1,
        LeftArrow = 1ULL << 2,
        RightArrow = 1ULL << 3,

        Space = 1ULL << 4,
        Escape = 1ULL << 5,
        Return = 1ULL << 6,
        Shift = 1ULL << 7,

        A = 1ULL << 8,
        B = 1ULL << 9,
        C = 1ULL << 10,
        D = 1ULL << 11,
        E = 1ULL << 12,
        F = 1ULL << 13,
        G = 1ULL << 14,
        H = 1ULL << 15,
        I = 1ULL << 16,
        J = 1ULL << 17,
        K = 1ULL << 18,
        L = 1ULL << 19,
        M = 1ULL << 20,
        N = 1ULL << 21,
        O = 1ULL << 22,
        P = 1ULL << 23,
        Q = 1ULL << 24,
        R = 1ULL << 25,
        S = 1ULL << 26,
        T = 1ULL << 27,
        U = 1ULL << 28,
        V = 1ULL << 29,
        W = 1ULL << 30,
        X = 1ULL << 31,
        Y = 1ULL << 32,
        Z = 1ULL << 33,

        Alpha1 = 1ULL << 34,
        Alpha2 = 1ULL << 35,
        Alpha3 = 1ULL << 36,
        Alpha4 = 1ULL << 37,
        Alpha5 = 1ULL << 38,
        Alpha6 = 1ULL << 39,
        Alpha7 = 1ULL << 40,
        Alpha8 = 1ULL << 41,
        Alpha9 = 1ULL << 42,
        Alpha0 = 1ULL << 43,
    };
};
    class Input
    {
        static uint64_t keyState;
        static uint64_t keyStateOld;
        static uint64_t keyDownStateTriggered;
        static uint64_t keyUpStateTriggered;

        static bool     isMouseDown;
        static bool     isMouseDownOld;
        static bool     isMouseDownTriggered;
        static bool     isMouseUpTriggered;

        static bool     isMouseDownRight;
        static bool     isMouseDownOldRight;
        static bool     isMouseDownTriggeredRight;
        static bool     isMouseUpTriggeredRight;

    public:
        static bool GetKey(uint64_t keyCode);
        static bool GetKeyDown(uint64_t keyCode);
        static bool GetKeyUp(uint64_t keyCode);

        static bool         GetMouseButton(int button);
        static bool         GetMouseButtonDown(int button);
        static bool         GetMouseButtonUp(int button);
        static GLKVector2   GetMousePosition();

    public:
        static void ProcessKeyDown(uint64_t keyCode);
        static void ProcessKeyUp(uint64_t keyCode);
        static void ProcessMouseDown();
        static void ProcessMouseUp();
        static void ProcessMouseDownRight();
        static void ProcessMouseUpRight();
        static void Update();

    };

#endif /* Input_hpp */
