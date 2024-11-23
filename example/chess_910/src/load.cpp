#include "load.h"

#include <GL/glew.h>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

// MARK: stb/stb_image.h -> stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

using namespace std;

std::vector<Object> Load::loadObject(const std::string& path) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if ((scene == nullptr) || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U) || (scene->mRootNode == nullptr)) {
        std::cout << "Error import.ReadFile " << import.GetErrorString() << std::endl;
        return {};
    }
    directory_ = path.substr(0, path.find_last_of('/'));

    std::vector<Object> objects;
    processNode(scene->mRootNode, scene, objects);

    return objects;
}

void Load::processNode(aiNode* node, const aiScene* scene, std::vector<Object>& objects) {

    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        objects.emplace_back(processMesh(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, objects);
    }
}

Object Load::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<Texture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex {};
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
        vertex.texCoords.y = mesh->mTextureCoords[0][i].y;

        vertices.emplace_back(vertex);
    }

    vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace const face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Texture diffuseMaps = loadTexture(material);

    return Object { indices, vertices, diffuseMaps };
}

Texture Load::loadTexture(aiMaterial* material) {
    Texture texture;
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

        texture.id = textureFromFile(str.C_Str(), directory_);
        texture.path = str.C_Str();
    }

    return texture;
}

unsigned int Load::textureFromFile(const char* path, const string& directory) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID = 0;
    glGenTextures(1, &textureID);

    int width = 0;
    int height = 0;
    int comp = 0;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &comp, 0);
    if (data != nullptr) {
        GLint format = 0;
        if (comp == 3) {
            format = GL_RGB;
        } else if (comp == 4) {
            format = GL_RGBA;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    return textureID;
}
