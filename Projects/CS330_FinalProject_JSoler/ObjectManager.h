///////////////////////////////////////////////////////////////////////////////
// ObjectManager.h
//
// Define scene objects: component parts, and layout
//
// AUTHOR: Jessica Soler
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

// Object configuration
namespace ObjectSettings
{
	// Texture repetition
	const glm::vec2 DEFAULT_UV_SCALE = glm::vec2(1.0f, 1.0f);
	const glm::vec2 TABLE_UV_SCALE = glm::vec2(4.0f, 4.0f);
	const glm::vec2 BOTTLE_LID_UV_SCALE = glm::vec2(4.0f, 1.0f);

	// Default Values
	const glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 GREY = glm::vec4(0.40f, 0.40f, 0.40f, 1.0f);
	const glm::vec3 CENTER_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 DEFAULT_ROTATION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 DEFAULT_SCALE = glm::vec3(1.0f, 1.0f, 1.0f);

	// Table configuration
	const glm::vec3 TABLE_SCALE = glm::vec3(10.0f, 1.0f, 10.0f);

	// Bottle appearance  
	const glm::vec4 BOTTLE_GLASS_COLOR = glm::vec4(0.95f, 0.86f, 0.38f, 0.60f);

	// Bottle orientation
	const float BOTTLE_Y_ROTATION = 8.0f;

	// Bottle scales
	const glm::vec3 BOTTLE_BOTTOM_SCALE = glm::vec3(1.0f, 4.0f, 1.0f);
	const glm::vec3 BOTTLE_MIDDLE_SCALE = glm::vec3(1.0f, 0.8f, 1.0f);
	const glm::vec3 BOTTLE_NECK_SCALE = glm::vec3(0.5f, 1.6f, 0.5f);
	const glm::vec3 BOTTLE_LID_SCALE = glm::vec3(0.58f, 1.0f, 0.58f);
	const glm::vec3 BOTTLE_LID_TOP_SCALE = glm::vec3(0.59f, 0.05f, 0.59f);
	const glm::vec3 LABEL_SCALE = glm::vec3(1.03f, 1.25f, 1.03f);

	// Bottle placement
	const glm::vec3 BOTTLE_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 BOTTLE_BOTTOM_OFFSET = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 BOTTLE_MIDDLE_OFFSET = glm::vec3(0.0f, BOTTLE_BOTTOM_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_NECK_OFFSET = BOTTLE_MIDDLE_OFFSET + glm::vec3(0.0f, BOTTLE_MIDDLE_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_LID_OFFSET = BOTTLE_NECK_OFFSET + glm::vec3(0.0f, BOTTLE_NECK_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_LID_TOP_OFFSET = BOTTLE_LID_OFFSET + glm::vec3(0.0f, BOTTLE_LID_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_LABEL_OFFSET = glm::vec3(0.0f, 1.35f, 0.0f);
}

// Meshes used
enum MeshType
{
	PLANE_MESH,
	CYLINDER_MESH,
	TAPERED_CYLINDER_MESH
};

// 
struct ObjectPart
{
	MeshType meshType = PLANE_MESH;

	glm::vec3 scale = ObjectSettings::DEFAULT_SCALE;
	glm::vec3 rotation = ObjectSettings::DEFAULT_ROTATION;
	glm::vec3 position = ObjectSettings::CENTER_POSITION;

	glm::vec4 color = ObjectSettings::WHITE;

	std::string textureTag;
	glm::vec2 uvScale = ObjectSettings::DEFAULT_UV_SCALE;
	bool useTexture = false;

	std::string materialTag;
};

class SceneObject
{
public:
	void AddPart(const ObjectPart& part);

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

	glm::mat4 GetModelMatrix() const;

	const std::vector<ObjectPart>& GetParts() const;

private:
	glm::vec3 m_position = ObjectSettings::CENTER_POSITION;
	glm::vec3 m_rotation = ObjectSettings::DEFAULT_ROTATION;
	glm::vec3 m_scale = ObjectSettings::DEFAULT_SCALE;

	std::vector<ObjectPart> m_parts;
};

// Own scene objects and organize their construction and layout.
class ObjectManager
{
public:
	void PrepareObjects();

	const std::vector<SceneObject>& GetObjects() const;

private:
	void CreateTable();
	void CreateBottle();

	std::vector<SceneObject> m_objects;
};