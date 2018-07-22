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
#include "Texture.hpp"
#include <cmath>
#include <vector>

struct VertexData
{
    GLfloat     pos[3];
    GLfloat     color[4];
    GLfloat     uv[2];
};

Game::Game()
{
    // シェーダーのコンパイル
    pProgram = new ShaderProgram("myshader.vsh", "myshader.fsh");

    std::vector<VertexData> data;
    data.push_back({ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } });
    data.push_back({ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } });
    data.push_back({ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } });
    data.push_back({ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } });

    std::vector<GLushort> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    // VBOの生成
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * data.size(), data.data(), GL_STATIC_DRAW);

    // IBOの生成
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), ((VertexData*)0)->pos);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), ((VertexData*)0)->color);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), ((VertexData*)0)->uv);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    pTex = new Texture("photo.jpg");
    // 0番目のテクスチャ画像を使うために、「GL_TEXTURE0」という定数を指定して
    // glActiveTexture()に利用するテクスチャの番号を指定します。
    glActiveTexture(GL_TEXTURE0);
    // バインドすることで、GPU上に格納されている複数のテクスチャの中から、
    // このTextureクラスが管理するテクスチャを
    // GLSLのtexture()関数で参照できるようになるのです。
    pTex->Bind();
    pProgram->Use();
    // glActiveTexture()関数で0番目のテクスチャ画像を使うと宣言したら、
    // シェーダ・プログラムの方でもSetUniform()関数を使って
    // uniform変数のtexに「0」をセットして、
    // フラグメント・シェーダの中で0番目のテクスチャが
    // 参照されるように設定しなければいけません。
    pProgram->SetUniform("tex", 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Game::~Game()
{
    delete pTex;
    delete pProgram;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &ibo);
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

    glBindVertexArray(vao);
    // 0番目と1番目の頂点属性を利用することを指定
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // glDrawArray()関数の第1引数に指定したGL_TRIANGLESで、
    // VBOからストリームされるデータを順番に3個ずつ消費して、
    // 三角形を描画していくことを指定します。
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    // IBOから描画する場合は、第1引数は glDrawArraysと変わらないが、
    // 第2引数・第3引数はインデックスリストの個数とサイズを指定する
    // 第4引数は0をポインタとして渡します。
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
}
