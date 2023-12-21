//
// Created by tjoms on 12/19/23.
//

#include "ComponentRendererSquare.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <unistd.h>
#include <cmath>
#include "Engine/ResourceManager.h"

#include <fstream>
#include "3rdParty/rapidjson/document.h"
#include "3rdParty/rapidjson/istreamwrapper.h"
#include "3rdParty/rapidjson/stringbuffer.h"
#include "3rdParty/rapidjson/writer.h"


void ComponentRendererSquare::Init(rapidjson::Value &serializedData) {
	const int texture_id = serializedData.GetInt();
	Init(texture_id);
}

void ComponentRendererSquare::Init(int texture_id) {
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

void ComponentRendererSquare::Init(std::string texture_name) {
	std::ifstream fis("data/sprites.json");
	rapidjson::IStreamWrapper isw(fis);
	rapidjson::Document document;
	document.ParseStream(isw);

	auto frames = document["frames"].GetArray();
	int i = 0;
	for (; i < frames.Size(); i++) {
		if (frames[i]["filename"].GetString() == texture_name + ".png") {
			break;
		}
	}

	auto texture_size = glm::vec2 (
		document["meta"]["size"]["w"].GetInt(),
		document["meta"]["size"]["h"].GetInt()
	);
	auto origin = glm::vec2(
		frames[i]["frame"]["x"].GetInt(),
		frames[i]["frame"]["y"].GetInt()
	) / texture_size;
	auto unit = glm::vec2(
		frames[i]["frame"]["w"].GetInt(),
		frames[i]["frame"]["h"].GetInt()
	) / texture_size;

	const std::vector<glm::vec4> uvs = {
			glm::vec4(origin.x, origin.y, 0, 0),   // bottom left
			glm::vec4(origin.x, origin.y + unit.y, 0, 0),   // top left
			glm::vec4(origin.x + unit.x, origin.y + unit.y, 0, 0),   // top right
			glm::vec4(origin.x + unit.x, origin.y, 0, 0)};  // bottom right

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
			.withFile("data/sprites.png")
			.withGenerateMipmaps(false)
			.withFilterSampling(false)
			.build();

	_material->setTexture(_texture);
}

void ComponentRendererSquare::Update(float deltaTime) {

}

void ComponentRendererSquare::SetRotation(bool rotate_x, bool rotate_y, int rotation) {
	_rotate_x = rotate_x;
	_rotate_y = rotate_y;
	_rotation = rotation;
}

void ComponentRendererSquare::Render(sre::RenderPass &renderPass) {
	auto go = GetGameObject().lock();
	renderPass.draw(
			_mesh,
			glm::rotate(
					go->transform,
					glm::pi<float>() * _rotation / 2,
					glm::vec3(_rotate_x, _rotate_y, 0)),
			_material
	);
}