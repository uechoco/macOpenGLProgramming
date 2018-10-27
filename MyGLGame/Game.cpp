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
    program = new ShaderProgram("myshader.vsh", "myshader.fsh");

    mesh = new Mesh("bunny.obj");
    planeMesh = new Mesh("plane.obj");

    cameraPos = GLKVector3Make(0.0f, 0.0f, 5.0f);
    bCameraDirty = true; // 初回はdirty
    bPerspective = true; // 最初は透視投影

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Game::~Game()
{
    delete program;
    delete planeMesh;
    delete mesh;
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
    program->Use();

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
    program->SetUniform("eye_dir", eyeDir);


    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        // ライト1
        GLKVector3 lightPos = GLKVector3Make(-1.f, 4.f, 2.0f);
        GLKVector3 spotDir = GLKVector3Make(0.27f, -1.0f, -0.6f);
        program->SetUniform("light_pos[0]", lightPos);
        program->SetUniform("light_attenuation[0]", 0.01f);
        program->SetUniform("spot_dir[0]", spotDir);
        program->SetUniform("spot_phi[0]", GLKMathDegreesToRadians(45.f));
        program->SetUniform("spot_theta[0]", GLKMathDegreesToRadians(30.f));
        program->SetUniform("spot_falloff[0]", 1.f);

        program->SetUniform("diffuse_color[0]", GLKVector4Make(0.9f, 0.1f, 0.1f, 1.0f));
        program->SetUniform("ambient_color[0]", GLKVector4Make(0.3f, 0.0f, 0.0f, 1.0f));
        program->SetUniform("specular_color[0]", GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f));
        program->SetUniform("specular_shininess[0]", 50.0f);
    }

    {
        // ライト2
        GLKVector3 lightPos = GLKVector3Make(-3.f, 4.f, -1.0f);
        GLKVector3 spotDir = GLKVector3Make(0.8f, -1.f, 0.3f);
        program->SetUniform("light_pos[1]", lightPos);
        program->SetUniform("light_attenuation[1]", 0.01f);
        program->SetUniform("spot_dir[1]", spotDir);
        program->SetUniform("spot_phi[1]", GLKMathDegreesToRadians(45.f));
        program->SetUniform("spot_theta[1]", GLKMathDegreesToRadians(30.f));
        program->SetUniform("spot_falloff[1]", 1.f);

        program->SetUniform("diffuse_color[1]", GLKVector4Make(0.1f, 0.9f, 0.1f, 1.0f));
        program->SetUniform("ambient_color[1]", GLKVector4Make(0.0f, 0.1f, 0.0f, 1.0f));
        program->SetUniform("specular_color[1]", GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f));
        program->SetUniform("specular_shininess[1]", 50.0f);
    }

    {
        // ライト3
        GLKVector3 lightPos = GLKVector3Make(3.f, 4.f, 2.0f);
        GLKVector3 spotDir = GLKVector3Make(-0.8f, -1.f, -0.6f);
        program->SetUniform("light_pos[2]", lightPos);
        program->SetUniform("light_attenuation[2]", 0.01f);
        program->SetUniform("spot_dir[2]", spotDir);
        program->SetUniform("spot_phi[2]", GLKMathDegreesToRadians(45.f));
        program->SetUniform("spot_theta[2]", GLKMathDegreesToRadians(30.f));
        program->SetUniform("spot_falloff[2]", 1.f);

        program->SetUniform("diffuse_color[2]", GLKVector4Make(0.1f, 0.1f, 0.9f, 1.0f));
        program->SetUniform("ambient_color[2]", GLKVector4Make(0.0f, 0.0f, 0.1f, 1.0f));
        program->SetUniform("specular_color[2]", GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f));
        program->SetUniform("specular_shininess[2]", 50.0f);
    }

    GLKMatrix4 modelMat = GLKMatrix4Identity;
    modelMat = GLKMatrix4Translate(modelMat, 0.0f, -2.0f, 0.0f);
    modelMat = GLKMatrix4Scale(modelMat, 20.0f, 20.0f, 20.0f);
    program->SetUniform("model_mat", modelMat);
    GLKMatrix4 pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    program->SetUniform("pvm_mat", pvmMat);

    program->SetUniform("emissive_color", GLKVector4Make(0.f, 0.f, 0.4f, 0.f));
    mesh->Draw();

    program->SetUniform("emissive_color", GLKVector4Make(0.f, 0.f, 0.0f, 0.f));
    planeMesh->Draw();
}
