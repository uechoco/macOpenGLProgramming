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
    glEnable(GL_DEPTH_TEST);

    // シェーダーのコンパイル
    pProgram = new ShaderProgram("myshader.vsh", "myshader.fsh");

    // キャラクター3体
    data.push_back({ {  0.2f, -1.0f, -2.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.2f, -1.0f, -2.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.2f,  0.0f, -2.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });

    data.push_back({ {  -0.5f, -1.0f, -1.0f }, { 1.0f, 0.4f, 0.7f, 1.0f } });
    data.push_back({ {  0.7f, -1.0f, -1.0f }, { 1.0f, 0.4f, 0.7f, 1.0f } });
    data.push_back({ {  0.7f,  0.0f, -1.0f }, { 1.0f, 0.4f, 0.7f, 1.0f } });

    data.push_back({ { -0.5f, -1.0f, 3.0f }, { 0.6f, 0.2f, 1.0f, 1.0f } });
    data.push_back({ {  0.5f, -1.0f, 3.0f }, { 0.6f, 0.2f, 1.0f, 1.0f } });
    data.push_back({ {  0.0f,  0.0f, 3.0f }, { 0.6f, 0.2f, 1.0f, 1.0f } });

    // 四隅のコーン
    data.push_back({ { -2.8f, -1.0f, -5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { -3.2f, -1.0f, -5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { -3.0f,  1.0f, -5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });

    data.push_back({ { 2.8f, -1.0f, -5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { 3.2f, -1.0f, -5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { 3.0f,  1.0f, -5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });

    data.push_back({ { -2.8f, -1.0f, 5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { -3.2f, -1.0f, 5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { -3.0f,  1.0f, 5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });

    data.push_back({ { 2.8f, -1.0f, 5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { 3.2f, -1.0f, 5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });
    data.push_back({ { 3.0f,  1.0f, 5.0f }, { 0.0f, 1.0f, 0.2f, 1.0f } });

    // 地面
    data.push_back({ {  3.0f, -1.0f,  5.0f }, { 0.0f, 0.0f, 0.7f, 1.0f } });
    data.push_back({ {  3.0f, -1.0f, -5.0f }, { 0.0f, 0.0f, 0.7f, 1.0f } });
    data.push_back({ { -3.0f, -1.0f, -5.0f }, { 0.0f, 0.0f, 0.7f, 1.0f } });

    data.push_back({ {  3.0f, -1.0f,  5.0f }, { 0.0f, 0.0f, 0.7f, 1.0f } });
    data.push_back({ { -3.0f, -1.0f,  5.0f }, { 0.0f, 0.0f, 0.7f, 1.0f } });
    data.push_back({ { -3.0f, -1.0f, -5.0f }, { 0.0f, 0.0f, 0.7f, 1.0f } });

    std::vector<GLushort> indices;
    indices.reserve(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        indices.push_back(i);
    }

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData*)0)->pos);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData*)0)->color);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    GLKMatrix4 projMat = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(60.0f), 640.f / 480.f, 0.00001f, 50.f);
    pProgram->Use();
    pProgram->SetUniform("mat", projMat);

    cameraPos = GLKVector3Make(0.0f, 0.0f, 0.0f);
    cameraAngle = 0.0f;
    bCameraDirty = true; // 初回はdirty

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Game::~Game()
{
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
    if (Input::GetKey(KeyCode::A)) {
        cameraPos.x -= 2.0f * Time::deltaTime;
        bCameraDirty = true;
    } else if (Input::GetKey(KeyCode::D)) {
        cameraPos.x += 2.0f * Time::deltaTime;
        bCameraDirty = true;
    } else if (Input::GetKey(KeyCode::W)) {
        cameraPos.z -= 2.0f * Time::deltaTime;
        bCameraDirty = true;
    } else if (Input::GetKey(KeyCode::S)) {
        cameraPos.z += 2.0f * Time::deltaTime;
        bCameraDirty = true;
    }
    if (Input::GetKey(KeyCode::LeftArrow)) {
        cameraAngle += M_PI / 4 * Time::deltaTime;
        bCameraDirty = true;
    } else if (Input::GetKey(KeyCode::RightArrow)) {
        cameraAngle -= M_PI / 4 * Time::deltaTime;
        bCameraDirty = true;
    }
    // レンダリングに使用するシェーダをセット
    pProgram->Use();

    if (bCameraDirty)
    {
        // 実はカメラを移動したり回転させたりすることはできません。
        // OpenGLでもDirectXでも、カメラの位置と方向は固定されているのです。
        // そこで、カメラの代わりに、その分だけ頂点データの方を
        // 回転させたり移動させたりすることで、
        // 擬似的にカメラを移動・回転したことにします。
        GLKMatrix4 transMat = GLKMatrix4MakeTranslation(cameraPos.x, cameraPos.y, cameraPos.z);
        GLKMatrix4 rotMat = GLKMatrix4MakeRotation(cameraAngle, 0.0f, 1.0f, 0.0f);

        GLKMatrix4 viewMat = GLKMatrix4Multiply(transMat, rotMat);
        viewMat = GLKMatrix4Invert(viewMat, NULL);

        GLKMatrix4 projMat = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(60.0f), 640.f / 480.f, 0.00001f, 50.f);
        GLKMatrix4 mat = GLKMatrix4Multiply(projMat, viewMat);

        pProgram->SetUniform("mat", mat);
        bCameraDirty = false;
    }

    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    // 0番目と1番目の頂点属性を利用することを指定
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glDrawArray()関数の第1引数に指定したGL_TRIANGLESで、
    // VBOからストリームされるデータを順番に3個ずつ消費して、
    // 三角形を描画していくことを指定します。
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    // IBOから描画する場合は、第1引数は glDrawArraysと変わらないが、
    // 第2引数・第3引数はインデックスリストの個数とサイズを指定する
    // 第4引数は0をポインタとして渡します。
    glDrawElements(GL_TRIANGLES, (GLsizei)data.size(), GL_UNSIGNED_SHORT, (void *)0);
}
