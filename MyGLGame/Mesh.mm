//
//  Mesh.mm
//  MyGLGame
//
//  Created by Yusuke Ueno on 2018/09/14.
//  Copyright © 2018年 uechoco. All rights reserved.
//

#include "Mesh.hpp"
#include "StringSupport.hpp"
#include <vector>
#include <ModelIO/ModelIO.h>

namespace
{
    struct VertexData
    {
        GLKVector3 pos;
        GLKVector3 normal;
        GLKVector4 color;
    };

    static int MDLMesh_CountVertexBuffers(MDLMesh* mesh)
    {
        NSArray<id<MDLMeshBuffer>> *buffers = mesh.vertexBuffers;
        int ret = 0;
        for (id<MDLMeshBuffer> buffer in buffers) {
            if ([buffer isKindOfClass:[NSNull class]]) {
                break;
            }
            ret++;
        }
        return ret;
    }
}

Mesh::Mesh(const std::string& filename, const GLKVector4& color, float smoothingLevel)
{
    assert(smoothingLevel >= 0.0f && smoothingLevel <= 1.0f);
    
    
    GLKVector3 aabbMax, aabbMin;
    aabbMax.x = INT_MIN;
    aabbMax.y = INT_MIN;
    aabbMax.z = INT_MIN;
    aabbMin.x = INT_MAX;
    aabbMin.y = INT_MAX;
    aabbMin.z = INT_MAX;
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    NSString *theFilename = [NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding];
    NSURL *theURL = [[NSBundle mainBundle] URLForResource:[theFilename stringByDeletingPathExtension]
                                            withExtension:theFilename.pathExtension];
    if (!theURL) {
        throw GameError("Cannot locate a mesh file with name: %s", filename.c_str());
    }
    MDLAsset *asset = [[MDLAsset alloc] initWithURL:theURL];
    NSArray *assets = [asset childObjectsOfClass:[MDLMesh class]];
    if (assets.count > 0) {
        MDLMesh *mesh = assets[0];
        if (MDLMesh_CountVertexBuffers(mesh) == 1) {
            // 頂点情報しか読み込まれていない場合に、法線ベクトルを追加で作成する（Stanford Bunnyなど）
            [mesh addNormalsWithAttributeNamed:MDLVertexAttributeNormal
                               creaseThreshold:(1.0f - smoothingLevel)]; // 0.0 がスムージングあり、1.0 がスムージングなし
        }
        // VBOに適した並び順にデータを格納し直す（GLKMeshクラスでできるかもしれないがよくわからないらしい）
        std::vector<VertexData> vertices;
        MDLVertexAttributeData *attrPos = [mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributePosition];
        MDLVertexAttributeData *attrNormal = [mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributeNormal];
        float *p0 = (float *)attrPos.dataStart;
        float *p1 = (float *)attrNormal.dataStart;
        for (NSUInteger i = 0; i < mesh.vertexCount; i++) {
            VertexData v;
            v.pos.x = *(p0);
            v.pos.y = *(p0 + 1);
            v.pos.z = *(p0 + 2);
            v.normal.x = *(p1);
            v.normal.y = *(p1 + 1);
            v.normal.z = *(p1 + 2);
            v.color = color;
            vertices.push_back(v);
            p0 += attrPos.stride / sizeof(float);
            p1 += attrNormal.stride / sizeof(float);
            
            if (aabbMax.x < v.pos.x) { aabbMax.x = v.pos.x; }
            if (aabbMax.y < v.pos.y) { aabbMax.y = v.pos.y; }
            if (aabbMax.z < v.pos.z) { aabbMax.z = v.pos.z; }
            if (aabbMin.x > v.pos.x) { aabbMin.x = v.pos.x; }
            if (aabbMin.y > v.pos.y) { aabbMin.y = v.pos.y; }
            if (aabbMin.z > v.pos.z) { aabbMin.z = v.pos.z; }
        }
        for (MDLSubmesh *submesh in mesh.submeshes) {
            if (submesh.geometryType != MDLGeometryTypeTriangles) {
                throw GameError("Mesh data should be composed of triangles");
            }
            id<MDLMeshBuffer> indexBuffer = submesh.indexBuffer;
            GLsizeiptr indexDataSize;
            if (submesh.indexType == MDLIndexBitDepthUInt8) {
                indexType = GL_UNSIGNED_BYTE;
                indexCount = (GLsizei)indexBuffer.length;
                indexDataSize = sizeof(GLubyte);
            } else if (submesh.indexType == MDLIndexBitDepthUInt16) {
                indexType = GL_UNSIGNED_SHORT;
                indexCount = (GLsizei)(indexBuffer.length / 2);
                indexDataSize = sizeof(GLushort);
            } else if (submesh.indexType == MDLIndexBitDepthUInt32) {
                indexType = GL_UNSIGNED_INT;
                indexCount = (GLsizei)(indexBuffer.length / 4);
                indexDataSize = sizeof(GLuint);
            } else {
                throw GameError("Mesh data type is invalid");
            }
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize * indexCount, indexBuffer.map.bytes, GL_STATIC_DRAW);
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->pos);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->normal);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->color);
        
        printf("aabb min(%2f,%2f,%2f)-max(%2f,%2f,%2f)",
               aabbMin.x, aabbMin.y, aabbMin.z,
               aabbMax.x, aabbMax.y, aabbMax.z);
    } else {
        throw GameError("Failed to make an asset from a mesh file: %s", filename.c_str());
    }
}

Mesh::~Mesh()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

Mesh* Mesh::CreateAsCube()
{
    Mesh* mesh = new Mesh();
    
    std::vector<VertexData> data;
    // 手前の面
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    
    // 奥の面
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.75f, 1.0f, 1.0f } });
    
    // 上の面
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    
    // 下の面
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.75f, 1.0f } });
    
    // 左の面
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f,  1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    
    // 右の面
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    data.push_back({ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.75f, 0.0f, 1.0f } });
    
    
    GLKVector3 aabbMax, aabbMin;
    aabbMax.x = INT_MIN;
    aabbMax.y = INT_MIN;
    aabbMax.z = INT_MIN;
    aabbMin.x = INT_MAX;
    aabbMin.y = INT_MAX;
    aabbMin.z = INT_MAX;
    
    std::vector<GLushort> indices;
    for (size_t i = 0; i < data.size(); i++) {
        indices.push_back(i);
        
        const VertexData& v = data[i];
        if (aabbMax.x < v.pos.x) { aabbMax.x = v.pos.x; }
        if (aabbMax.y < v.pos.y) { aabbMax.y = v.pos.y; }
        if (aabbMax.z < v.pos.z) { aabbMax.z = v.pos.z; }
        if (aabbMin.x > v.pos.x) { aabbMin.x = v.pos.x; }
        if (aabbMin.y > v.pos.y) { aabbMin.y = v.pos.y; }
        if (aabbMin.z > v.pos.z) { aabbMin.z = v.pos.z; }
    }
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * data.size(), &data[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->pos);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->normal);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), &((VertexData *)0)->color);
    
    glGenBuffers(1, &mesh->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
    mesh->indexCount = (GLsizei)data.size();
    mesh->indexType = GL_UNSIGNED_SHORT;
    
    printf("aabb min(%2f,%2f,%2f)-max(%2f,%2f,%2f)",
           aabbMin.x, aabbMin.y, aabbMin.z,
           aabbMax.x, aabbMax.y, aabbMax.z);
    
    return mesh;
}

void Mesh::Draw() const
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glDrawElements(GL_TRIANGLES, indexCount, indexType, (void *)0);
}
