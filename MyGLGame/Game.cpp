//
//  Game.cpp
//  MyGLGame
//
//  Created by uechoco on 2018/07/15.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Game.hpp"
#include "Shader.hpp"
#include "Input.hpp"
#include <cmath>

Game::Game()
{
    // シェーダーのコンパイル
    pProgram = new ShaderProgram("myshader.vsh", "myshader.fsh");

    // 頂点シェーダに渡すデータ配列を格納するために使うBuffer ObjectをVBOと呼ぶ座標データと色情報データを交互に配置することもできます。

    // BufferObjectの生成
    glGenBuffers(1, &vbo);
    // 頂点シェーダに渡すデータ配列であることを宣言する(=VBOであると認識させる)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat data[] = {
        // 頂点1: 左下、赤
        -0.5f, -0.5f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        // 頂点2: 右下、緑
        0.5f, -0.5f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        // 頂点3: 中央上、青
        0.f, 0.5f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };
    // glBufferData()関数の使い方しだいで、
    // バッファの確保とデータの転送を別々に行うこともできます。
    // また、複数のVBOを用意して、頂点の座標データと色情報のデータを
    // 別々のVBOで管理することもできますが、今回のサンプルのように、
    //
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * 3, data, GL_STATIC_DRAW);

    // glGenVertexArrays()関数を使ってVAOを作成し、
    // glBindVertexArray()関数を使ってOpenGLのコンテキストにバインドすることで、
    // 以降の頂点指定関係の状態変化がすべてVAOに格納されるようになります。
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // 頂点属性0: GL_FLOAT を 3個消費。7個間隔。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (GLfloat*)0);
    // 頂点属性1: GL_FLOAT を 4個消費。7個間隔。
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, ((GLfloat*)0)+3);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Game::~Game()
{
    delete pProgram;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Game::Render()
{
    // レンダリングに使用するシェーダをセット
    pProgram->Use();

    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // VAOのバインド
    glBindVertexArray(vao);
    // 0番目と1番目の頂点属性を利用することを指定
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // glDrawArray()関数の第1引数に指定したGL_TRIANGLESで、
    // VBOからストリームされるデータを順番に3個ずつ消費して、
    // 三角形を描画していくことを指定します。
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // なお、コンストラクタでVBOをバインドした後に
    // VAOをバインドしてglGenVertexArrays()関数を呼び出したことによって、
    // 既にVAOからVBOを参照していますので
    // （厳密にはVAOの中の各頂点属性からVBOを参照します。
    // 頂点属性ごとに別個のVBOを参照できます）、
    // Render()関数で改めてVBOをバインドし直す必要はありません。


}
