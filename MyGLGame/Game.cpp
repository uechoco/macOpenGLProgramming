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
    
    pMesh = new Mesh("bunny.obj");

    cameraPos = GLKVector3Make(0.0f, 0.0f, 5.0f);
    bCameraDirty = true; // 初回はdirty
    bPerspective = true; // 最初は透視投影

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Game::~Game()
{
    delete pProgram;
    delete pMesh;
}

void Game::Render()
{
    /*
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
     */

    cameraPos.x = -cosf(Time::time * 0.6f) * 5.0f;
    cameraPos.z = sinf(Time::time * 0.6f) * 5.0f;
    bCameraDirty = true;
    // レンダリングに使用するシェーダをセット
    pProgram->Use();

    GLKVector3 lightDir = GLKVector3Make(1.0f, -1.0f, -2.0f);
    pProgram->SetUniform("light_dir", lightDir);

    GLKVector3 cameraTarget = GLKVector3Make(0.0f, 0.0f, 0.0f);
    if (bCameraDirty)
    {
        GLKMatrix4 viewMat = GLKMatrix4MakeLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
                                              cameraTarget.x, cameraTarget.y, cameraTarget.z,
                                              0.0f, 1.0f, 0.0f);

        GLKMatrix4 projMat = bPerspective
            ? GLKMatrix4MakePerspective(GLKMathDegreesToRadians(60.0f), 640.f / 480.f, 0.00001f, 50.f)
            : GLKMatrix4MakeOrtho(-8.f, 8.f, -6.f, 6.f, 0.0001f, 50.f);
        projViewMat = GLKMatrix4Multiply(projMat, viewMat);

        bCameraDirty = false;
    }
    GLKVector3 eyeDir = GLKVector3Subtract(cameraTarget, cameraPos);
    eyeDir = GLKVector3Normalize(eyeDir);
    pProgram->SetUniform("eye_dir", eyeDir);


    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLKMatrix4 modelMat = GLKMatrix4Identity;
    modelMat = GLKMatrix4Translate(modelMat, 0.0f, -2.0f, 0.0f);
    modelMat = GLKMatrix4Scale(modelMat, 20.0f, 20.0f, 20.0f);
    pProgram->SetUniform("model_mat", modelMat);
    GLKMatrix4 pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    pProgram->SetUniform("pvm_mat", pvmMat);

    pProgram->SetUniform("diffuse_color", GLKVector4Make(1.0f, 0.9f, 0.7f, 1.0f));
    pProgram->SetUniform("ambient_color", GLKVector4Make(0.2f, 0.0f, 0.0f, 1.0f));
    pProgram->SetUniform("specular_color", GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f));
    pProgram->SetUniform("specular_shininess", 10.0f);
    
    pMesh->Draw();
}
