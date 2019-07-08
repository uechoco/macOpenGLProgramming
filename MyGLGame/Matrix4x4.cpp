//
//  Matrix4x4.cpp
//  MyGLGame
//
//  Created by Yusuke Ueno on 2019/07/08.
//  Copyright © 2019 uechoco. All rights reserved.
//

#include "Matrix4x4.hpp"

const Matrix4x4 Matrix4x4_Zero = {
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
};
const Matrix4x4 Matrix4x4_Identity = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
};
