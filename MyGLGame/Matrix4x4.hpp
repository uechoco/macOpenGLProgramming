//
//  Matrix4x4.hpp
//  MyGLGame
//
//  Created by Yusuke Ueno on 2019/07/08.
//  Copyright © 2019 uechoco. All rights reserved.
//

#ifndef Matrix4x4_hpp
#define Matrix4x4_hpp

#include <cstring>
#include <cmath>

// 前方宣言
class Vector3;
class Vector4;
class Matrix4x4;

class Vector3
{
public:
    union {
        struct {
            float x, y, z;
        };
        float v[3];
    };
};

class Vector4
{
public:
    union {
        struct {
            float x, y, z, w;
        };
        float v[4];
    };
};


//! ゼロ行列
extern const Matrix4x4 Matrix4x4_Zero;
//! 恒等行列
extern const Matrix4x4 Matrix4x4_Identity;


class Matrix4x4
{
public:
    union {
        struct {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        float v[16];
    };
    
public:
    // 生成時のコストを最小化するために、コンストラクタは敢えて実装していない。
    
    //! 生成メソッド
    static Matrix4x4 Create(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33)
    {
        return {
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33,
        };
    }
    
    //! 生成メソッド
    static Matrix4x4 Create(float v[16])
    {
        Matrix4x4 m;
        std::memcpy(m.v, v, 16);
        return m;
    }
    
    //! 生成メソッド
    static Matrix4x4 Create(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
    {
        return {
            v1.v[0], v1.v[1], v1.v[2], v1.v[3],
            v2.v[0], v2.v[1], v2.v[2], v2.v[3],
            v3.v[0], v3.v[1], v3.v[2], v3.v[3],
            v4.v[0], v4.v[1], v4.v[2], v4.v[3],
        };
    }
    
    //! 平行移動行列の生成
    static Matrix4x4 CreateTranslation(float x, float y, float z)
    {
        Matrix4x4 m = Matrix4x4_Identity;
        m.v[12] = x;
        m.v[13] = y;
        m.v[14] = z;
        return m;
    }
    
    //! 平行移動行列の生成
    static Matrix4x4 CreateTranslation(const Vector3& v)
    {
        Matrix4x4 m = Matrix4x4_Identity;
        m.v[12] = v.x;
        m.v[13] = v.y;
        m.v[14] = v.z;
        return m;
    }
    
    //! 拡縮行列の生成
    static Matrix4x4 CreateScale(float scale)
    {
        Matrix4x4 m = Matrix4x4_Identity;
        m.v[0] = scale;
        m.v[5] = scale;
        m.v[10] = scale;
        return m;
    }
    
    //! 拡縮行列の生成
    static Matrix4x4 CreateScale(float sx, float sy, float sz)
    {
        Matrix4x4 m = Matrix4x4_Identity;
        m.v[0] = sx;
        m.v[5] = sy;
        m.v[10] = sz;
        return m;
    }
    
    //! X軸の回転行列の生成
    static Matrix4x4 CreateXRotation(float radians)
    {
        const float cos = std::cosf(radians);
        const float sin = std::sinf(radians);
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  cos,  sin, 0.0f,
            0.0f, -sin,  cos, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }
    
    //! Y軸の回転行列の生成
    static Matrix4x4 CreateYRotation(float radians)
    {
        const float cos = std::cosf(radians);
        const float sin = std::sinf(radians);
        return {
             cos, 0.0f, -sin, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
             sin, 0.0f,  cos, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }
    
    //! Z軸の回転行列の生成
    static Matrix4x4 CreateZRotation(float radians)
    {
        const float cos = std::cosf(radians);
        const float sin = std::sinf(radians);
        return {
             cos,  sin, 0.0f, 0.0f,
            -sin,  cos, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }
    
    //! 任意の軸の周りの回転行列の生成
    static Matrix4x4 CreateRotation(const Vector3& axis, float radians)
    {
        const float cos = std::cosf(radians);
        const float omcos = 1.0f - cos;
        const float sin = std::sinf(radians);
        return {
            axis.x * axis.x * omcos + cos,
            axis.x * axis.y * omcos + axis.z * sin,
            axis.x * axis.z * omcos - axis.y * sin,
            0.f,
            axis.x * axis.y * omcos - axis.z * sin,
            axis.y * axis.y * omcos + cos,
            axis.y * axis.z * omcos + axis.x * sin,
            0.f,
            axis.x * axis.z * omcos + axis.y * sin,
            axis.y * axis.z * omcos - axis.x * sin,
            axis.z * axis.z * omcos + cos,
            0.f,
            0.f,
            0.f,
            0.f,
            1.f,
        };
    }
    
    //! スカラーを掛ける
    Matrix4x4& operator *(float s)
    {
        v[0] *= s;
        v[1] *= s;
        v[2] *= s;
        v[3] *= s;
        v[4] *= s;
        v[5] *= s;
        v[6] *= s;
        v[7] *= s;
        v[8] *= s;
        v[9] *= s;
        v[10] *= s;
        v[11] *= s;
        v[12] *= s;
        v[13] *= s;
        v[14] *= s;
        v[15] *= s;
        return *this;
    }
    
    //! 4x4行列同士の掛け算
    Matrix4x4& operator *(Matrix4x4& m2)
    {
        Matrix4x4 m1;
        std::memcpy(m1.v, v, 16);
        this->m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30;
        this->m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31;
        this->m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32;
        this->m03 = m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33;
        
        this->m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30;
        this->m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31;
        this->m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32;
        this->m13 = m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33;
        
        this->m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30;
        this->m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31;
        this->m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32;
        this->m23 = m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33;
        
        this->m30 = m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30;
        this->m31 = m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31;
        this->m32 = m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32;
        this->m33 = m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33;
        return *this;
    }
    
    //! 転置行列の取得
    Matrix4x4 GetTranspose() const
    {
        return {
            m00, m10, m20, m30,
            m01, m11, m21, m31,
            m02, m12, m22, m32,
            m03, m13, m23, m33,
        };
    }
    
    //! 行列式の取得
    float GetDeterminant() const
    {
        
    }
};

//! スカラーとの掛け算
Matrix4x4& operator * ( float s, Matrix4x4& m )
{
    m.v[0] *= s;
    m.v[1] *= s;
    m.v[2] *= s;
    m.v[3] *= s;
    m.v[4] *= s;
    m.v[5] *= s;
    m.v[6] *= s;
    m.v[7] *= s;
    m.v[8] *= s;
    m.v[9] *= s;
    m.v[10] *= s;
    m.v[11] *= s;
    m.v[12] *= s;
    m.v[13] *= s;
    m.v[14] *= s;
    m.v[15] *= s;
    return m;
}

#endif /* Matrix4x4_hpp */
