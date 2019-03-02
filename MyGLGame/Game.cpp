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
    stencilShadowProgram = new ShaderProgram("stencilshadow.vsh", "stencilshadow.fsh");

    //mesh = Mesh::CreateAsCube();
    mesh = new Mesh("bunny.obj");
    planeMesh = new Mesh("plane.obj");

    cameraPos = GLKVector3Make(0.0f, 0.0f, 5.0f);
    bCameraDirty = true; // 初回はdirty
    bPerspective = true; // 最初は透視投影

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Game::~Game()
{
    delete stencilShadowProgram;
    delete program;
    delete planeMesh;
    delete mesh;
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

    //cameraPos.x = -cosf(Time::time * 0.6f) * 5.0f;
    //cameraPos.z = sinf(Time::time * 0.6f) * 5.0f;
    //bCameraDirty = true;
    // レンダリングに使用するシェーダをセット
    program->Use();

    GLKVector3 lightPos = GLKVector3Make(-1.f, 1.f, 2.0f);
    GLKVector3 spotDir = GLKVector3Make(0.25f, -1.f, -0.25f);
    program->SetUniform("light_pos", lightPos);
    program->SetUniform("light_attenuation", 0.1f);
    program->SetUniform("spot_dir", spotDir);
    program->SetUniform("spot_phi", GLKMathDegreesToRadians(45.f));
    program->SetUniform("spot_theta", GLKMathDegreesToRadians(30.f));
    program->SetUniform("spot_falloff", 1.f);

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
    
    
    program->SetUniform("diffuse_color", GLKVector4Make(1.0f, 0.9f, 0.7f, 1.0f));
    program->SetUniform("ambient_color", GLKVector4Make(0.2f, 0.0f, 0.0f, 1.0f));
    program->SetUniform("specular_color", GLKVector4Make(1.0f, 1.0f, 1.0f, 1.0f));
    program->SetUniform("specular_shininess", 50.0f);
    

    // まずは普通に描画。深度情報をとっておく

    // 背景の上書き
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // デプスだけ書き込む
    //glDepthMask(GL_TRUE);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//色情報を書き込まない
    GLKMatrix4 modelMat = GLKMatrix4Identity;
    //modelMat = GLKMatrix4Translate(modelMat, 0.0f, 0.f, 0.0f);
    //modelMat = GLKMatrix4Scale(modelMat, 1.0f, 1.0f, 1.0f);
    modelMat = GLKMatrix4Translate(modelMat, 0.0f, -2.0f, 0.0f);
    modelMat = GLKMatrix4Scale(modelMat, 20.0f, 20.0f, 20.0f);
    GLKMatrix4 pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    //program->SetUniform("model_mat", modelMat);
    //program->SetUniform("pvm_mat", pvmMat);
    //program->SetUniform("emissive_color", GLKVector4Make(0.f, 0.f, 0.2f, 0.f));
    stencilShadowProgram->Use();
    stencilShadowProgram->SetUniform("light_pos", lightPos);
    stencilShadowProgram->SetUniform("spot_dir", spotDir);
    stencilShadowProgram->SetUniform("far", 5.f);
    stencilShadowProgram->SetUniform("model_mat", modelMat);
    stencilShadowProgram->SetUniform("pvm_mat", pvmMat);
    mesh->Draw();

    program->Use();
    modelMat = GLKMatrix4Identity;
    modelMat = GLKMatrix4Translate(modelMat, 0.0f, -2.0f, 0.0f);
    modelMat = GLKMatrix4Scale(modelMat, 20.0f, 20.0f, 20.0f);
    pvmMat = GLKMatrix4Multiply(projViewMat, modelMat);
    program->SetUniform("model_mat", modelMat);
    program->SetUniform("pvm_mat", pvmMat);
    program->SetUniform("emissive_color", GLKVector4Make(0.f, 0.f, 0.0f, 0.f));
    planeMesh->Draw();
    /*
    // シャドウボリュームをステンシルバッファに描き込む
    stencilShadowProgram->Use();
    //stencilShadowProgram->SetUniform("light_pos", lightPos);
    //stencilShadowProgram->SetUniform("far", 50.f);
    stencilShadowProgram->SetUniform("model_mat", modelMat);
    stencilShadowProgram->SetUniform("pvm_mat", pvmMat);
    glEnable(GL_STENCIL_TEST);
    // 1回目の描き込みはシャドウボリュームの前面を+1でレンダリング
    // 2回目の描き込みはシャドウボリュームの背面を-1でレンダリング
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//色情報を書き込まない
    glDepthMask(GL_FALSE);//デプス値を書き込まない
    glStencilFunc(GL_ALWAYS, 0, ~0);
    glEnable(GL_CULL_FACE);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    mesh->Draw();
    
    // ステンシルバッファを用いて影を描画する
    program->Use();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);//色書き込みON
    glDisable(GL_CULL_FACE); //カリングOFF
    glStencilFunc(GL_NOTEQUAL, 0, ~0); //ステンシル値が0じゃないの部分が影
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendColor(0.1f, 0.1f, 0.1f, 0.5f);
    glBlendFunc(GL_CONSTANT_COLOR, GL_ZERO);
    mesh->Draw();
    glDisable(GL_BLEND);
    
    glDepthMask(GL_TRUE);
    glDisable(GL_STENCIL_TEST);
    */
}
