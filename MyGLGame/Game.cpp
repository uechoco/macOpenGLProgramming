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
#include "StringSupport.hpp"
#include <fstream>
#include <cmath>

Game::Game()
{
    glEnable(GL_DEPTH_TEST);

    // シェーダーのコンパイル
    pProgram = new ShaderProgram("myshader.vsh", "myshader.fsh");

    // 3Dモデルの読み込み
    std::vector<GLKVector3> vertices;
    std::string modelFilepath = GetFilepath("bunny.obj");
    std::ifstream ifs(modelFilepath);
    std::string line;
    int lineCount = 0;
    while (ifs && getline(ifs, line)) {
        lineCount++;
        if (line.length() > 0 && line[0] == '#') {
            continue;
        }
        std::vector<std::string> parts = Split(line, " ");
        if (parts.size() > 0) {
            if (parts[0] == "v") {
                if (parts.size() < 4) {
                    throw GameError("Invalid obj file format (v): [line=%d] %s", lineCount, line.c_str());
                }
                float x = atof(parts[1].c_str());
                float y = atof(parts[2].c_str());
                float z = atof(parts[3].c_str());
                vertices.push_back(GLKVector3Make(x, y, z));
            } else if (parts[0] == "f") {
                if (parts.size() < 4) {
                    throw GameError("Invalid obj file format (f): [line=%d] %s", lineCount, line.c_str());
                }
                int index0 = atoi(parts[1].c_str()) - 1;
                int index1 = atoi(parts[2].c_str()) - 1;
                int index2 = atoi(parts[3].c_str()) - 1;
                if (index0 >= vertices.size() || index1 >= vertices.size() || index2 >= vertices.size()) {
                    throw GameError("Invalid index value (f): [line=%d] %s", lineCount, line.c_str());
                }
                GLKVector3& v0 = vertices[index0];
                GLKVector3& v1 = vertices[index1];
                GLKVector3& v2 = vertices[index2];
                GLKVector3 cross = GLKVector3CrossProduct(GLKVector3Subtract(v1, v0),
                                                          GLKVector3Subtract(v2, v0));
                GLKVector3 normal = GLKVector3Normalize(cross);
                GLKVector4 color = GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f);
                data.push_back( VertexData{ v0, normal, color } );
                data.push_back( VertexData{ v1, normal, color } );
                data.push_back( VertexData{ v2, normal, color } );
            }
        }
    }

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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->normal);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData*)0)->color);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    cameraPos = GLKVector3Make(0.1f, 0.1f, 0.3f);
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

    GLKVector3 lightDir = GLKVector3Make(1.0f, -1.0f, -2.0f);
    pProgram->SetUniform("light_dir", lightDir);

    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLKMatrix4 modelMat = GLKMatrix4Identity;
    pProgram->SetUniform("model_mat", modelMat);
    GLKMatrix4 pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    pProgram->SetUniform("pvm_mat", pvmMat);

    pProgram->SetUniform("diffuse_color", GLKVector4Make(1.0f, 0.9f, 0.7f, 1.0f));
    pProgram->SetUniform("ambient_color", GLKVector4Make(0.2f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, (GLsizei)data.size(), GL_UNSIGNED_SHORT, (void *)0);
}
