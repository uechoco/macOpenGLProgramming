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

    data.push_back({ {  -1.0f, -1.0f, 0.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {   1.0f, -1.0f, 0.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {   0.0f,  1.0f, 0.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });

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

    cameraPos = GLKVector3Make(0.0f, 2.0f, 5.5f);
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
    } else if (Input::GetKey(KeyCode::Q)) {
        cameraPos.y -= 2.0f * Time::deltaTime;
        bCameraDirty = true;
    } else if (Input::GetKey(KeyCode::E)) {
        cameraPos.y += 2.0f * Time::deltaTime;
        bCameraDirty = true;
    }
    // レンダリングに使用するシェーダをセット
    pProgram->Use();

    if (bCameraDirty)
    {
        GLKMatrix4 viewMat = GLKMatrix4MakeLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
                                              0.0f, 0.0f, 0.0f,
                                              0.0f, 1.0f, 0.0f);

        GLKMatrix4 projMat = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(60.0f), 640.f / 480.f, 0.00001f, 50.f);
        projViewMat = GLKMatrix4Multiply(projMat, viewMat);

        bCameraDirty = false;
    }

    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // for文を使って変数iで0から7までカウントし、
    // 360°/8 = π/4ラジアンずつ角度をずらしながら、
    // コサインとサインでX座標とZ座標を計算していきます。
    // 三角形の1辺の長さが2ですので、八角形に内接する円の半径は
    // およそ2.414になります（1 / tan(45°/2)を計算すると出てきます）。
    // コサインとサインの値にこの半径を掛けて、各モデルの位置を計算します。
    for (int i = 0; i < 8; i++) {
        float x = cosf(M_PI / 4 * i) * 2.414f;
        float z = -sinf(M_PI / 4 * i) * 2.414f;
        // モデル行列(TRS行列(の計算
        GLKMatrix4 modelMat = GLKMatrix4Identity;
        modelMat = GLKMatrix4Translate(modelMat, x, 0.0f, z);
        modelMat = GLKMatrix4RotateY(modelMat, M_PI / 4 * i + M_PI / 2);
        GLKMatrix4 pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
        pProgram->SetUniform("mat", pvmMat);
        glDrawElements(GL_TRIANGLES, (GLsizei)data.size(), GL_UNSIGNED_SHORT, (void *)0);
    }
}
