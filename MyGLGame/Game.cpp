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

    // 手前の面
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });

    // 奥の面
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });

    // 上の面
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });

    // 下の面
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });

    // 左の面
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });

    // 右の面
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });

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

    cameraPos = GLKVector3Make(2.5f, 2.0f, 5.0f);
    bCameraDirty = true; // 初回はdirty
    bPerspective = true; // 最初は透視投影

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
    } else if (Input::GetKey(KeyCode::P)) {
        bPerspective = !bPerspective;
        bCameraDirty = true;
    }
    // レンダリングに使用するシェーダをセット
    pProgram->Use();

    if (bCameraDirty)
    {
        GLKMatrix4 viewMat = GLKMatrix4MakeLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
                                              0.0f, 0.0f, 0.0f,
                                              0.0f, 1.0f, 0.0f);

        GLKMatrix4 projMat = bPerspective
            ? GLKMatrix4MakePerspective(GLKMathDegreesToRadians(60.0f), 640.f / 480.f, 0.00001f, 50.f)
            : GLKMatrix4MakeOrtho(-8.f, 8.f, -6.f, 6.f, 0.0001f, 50.f);
        projViewMat = GLKMatrix4Multiply(projMat, viewMat);

        bCameraDirty = false;
    }

    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLKMatrix4 modelMat = GLKMatrix4Identity;
    GLKMatrix4 pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    pProgram->SetUniform("mat", pvmMat);
    glDrawElements(GL_TRIANGLES, (GLsizei)data.size(), GL_UNSIGNED_SHORT, (void *)0);

    modelMat = GLKMatrix4Identity;
    modelMat = GLKMatrix4Translate(modelMat, 2.0f, 0.0f, -2.0f);
    pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    pProgram->SetUniform("mat", pvmMat);
    glDrawElements(GL_TRIANGLES, (GLsizei)data.size(), GL_UNSIGNED_SHORT, (void *)0);

    modelMat = GLKMatrix4Identity;
    modelMat = GLKMatrix4Translate(modelMat, -2.0f, 0.0f, -2.0f);
    pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    pProgram->SetUniform("mat", pvmMat);
    glDrawElements(GL_TRIANGLES, (GLsizei)data.size(), GL_UNSIGNED_SHORT, (void *)0);
}
