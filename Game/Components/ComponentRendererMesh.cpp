#include "ComponentRendererMesh.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <unistd.h>
#include <cmath>

void ComponentRendererMesh::Init(rapidjson::Value &serializedData) {
	const int texture_id = serializedData.GetInt();
	Init(texture_id);
}

void ComponentRendererMesh::Init(int texture_id) {
	_id = texture_id;
	const glm::vec2 textureSize_px = glm::vec2(1039, 389);
	const glm::vec2 tileSize_px = glm::vec2(64, 64);
	const glm::vec2 borderSizeBetween_px = glm::vec2(1, 1);
	const glm::vec2 tileSizeWithBorder_px = tileSize_px + borderSizeBetween_px;

	const glm::vec2 origin = glm::vec2(0, 0);
	const glm::vec2 unit = tileSize_px / textureSize_px;
	const glm::vec2 offset = tileSizeWithBorder_px / textureSize_px;

	const int tile_x = (texture_id) % 16;
	const int tile_y = std::floor(texture_id / 16);

	const std::vector<glm::vec4> uvs = {
			glm::vec4(origin.x + (offset.x * tile_x), origin.y + (offset.y * tile_y), 0, 0),   // bottom left
			glm::vec4(origin.x + (offset.x * tile_x), unit.y + (offset.y * tile_y), 0, 0),   // top left
			glm::vec4(unit.x + (offset.x * tile_x), unit.y + (offset.y * tile_y), 0, 0),   // top right
			glm::vec4(unit.x + (offset.x * tile_x), origin.y + (offset.y * tile_y), 0, 0)};  // bottom right

	const std::vector<glm::vec3> positions = {
			glm::vec3(-.5, -.5, .5),
			glm::vec3(-.5, .5, .5),
			glm::vec3(.5, .5, .5),
			glm::vec3(.5, -.5, .5)};

	const std::vector<uint16_t> idxs = {3, 1, 0, 3, 2, 1};

	_mesh = sre::Mesh::create()
			.withPositions(positions)
			.withUVs(uvs)
			.withIndices(idxs, sre::MeshTopology::Triangles, 0)
			.build();

	_material = sre::Shader::getUnlit()->createMaterial();

	_texture = sre::Texture::create()
			.withFile("data/level0.png")
			.withGenerateMipmaps(false)
			.withFilterSampling(false)
			.build();

	_material->setTexture(_texture);
}

void ComponentRendererMesh::Update(float deltaTime) {

}

void ComponentRendererMesh::Render(sre::RenderPass &renderPass) {
	auto go = GetGameObject().lock();
	renderPass.draw(
			_mesh,
			glm::rotate(
					go->transform,
					glm::pi<float>() * 3 / 2,
					glm::vec3(1, 0, 0)),
			_material
	);
	renderPass.draw(
			_mesh,
			glm::rotate(
					go->transform,
					glm::pi<float>() * 1 / 2,
					glm::vec3(1, 0, 0)),
			_material
	);

	for (int i = 0; i < 4; i++) {
		renderPass.draw(
				_mesh,
				glm::rotate(
						go->transform,
						glm::pi<float>() * i / 2,
						glm::vec3(0, 1, 0)),
				_material
		);
	}
}