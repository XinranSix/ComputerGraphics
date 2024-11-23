#ifndef LOAD_H
#define LOAD_H

#include "object.h"

#include <assimp/scene.h>

class Load {
public:
    std::vector<Object> loadObject(const std::string& path);

private:
    void processNode(aiNode* node, const aiScene* scene, std::vector<Object>& objects);
    Object processMesh(aiMesh* mesh, const aiScene* scene);
    Texture loadTexture(aiMaterial* material);
    unsigned int textureFromFile(const char* path, const std::string& directory);

    std::string directory_;
};
#endif
