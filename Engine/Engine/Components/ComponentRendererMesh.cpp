#include "ComponentRendererMesh.h"

#include "glm/gtx/transform.hpp"

void ComponentRendererMesh::Init(rapidjson::Value& serializedData) {
	_mesh = sre::Mesh::create()
        .withPositions(positions)
        .withUVs(uvs)
        .withIndices(idxs, sre::MeshTopology::Triangles, 0)
        .build();
    _material = sre::Shader::getUnlit()->createMaterial();

    _texture = sre::Texture::create().withFile("data/level0.png")
        .withGenerateMipmaps(false)
        .withFilterSampling(false)
        .build();
    _material->setTexture(_texture);
}

void ComponentRendererMesh::Update(float deltaTime) {

}

void ComponentRendererMesh::Render(sre::RenderPass& renderPass) {
    auto gameObject = GetGameObject().lock();
    if (!gameObject)
        return;

    renderPass.draw(_mesh, gameObject->transform, _material);
    static auto cube = sre::Mesh::create().withCube(0.5f).build();
    static std::vector<std::shared_ptr<sre::Material> > materials = {
            sre::Shader::getUnlit()->createMaterial(),
            sre::Shader::getUnlit()->createMaterial(),
            sre::Shader::getUnlit()->createMaterial()
    };

    std::vector<glm::vec3> positions = {
            {-1,0,-2},
            { 0,0,-3},
            { 1,0,-4}
    };
    std::vector<sre::Color> colors = {
            {1,0,0,1},
            {0,1,0,1},
            {0,0,1,1},
    };
    for (int i = 0; i < positions.size(); i++) {
        materials[i]->setColor(colors[i]);
        renderPass.draw(cube, glm::translate(positions[i]), materials[i]);
    }
}