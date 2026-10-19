///////////////////////////////////////////////////////////////////////////////
// SceneManager.h
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"

#include <string>
#include <vector>

/***********************************************************
 *  SceneManager
 *
 *  This class contains the code for preparing and rendering
 *  3D scenes, including the shader settings.
 ***********************************************************/
class SceneManager
{
public:
	// constructor
	SceneManager(ShaderManager* pShaderManager);
	// destructor
	~SceneManager();

	struct TEXTURE_INFO
	{
		std::string tag;
		uint32_t ID;
	};

	struct OBJECT_MATERIAL
	{
		float ambientStrength;
		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		std::string tag;
	};

private:
	// pointer to shader manager object
	ShaderManager* m_pShaderManager;
	// pointer to basic shapes object
	ShapeMeshes* m_basicMeshes;
	// total number of loaded textures
	int m_loadedTextures;
	// loaded textures info
	TEXTURE_INFO m_textureIDs[16];
	// defined object materials
	std::vector<OBJECT_MATERIAL> m_objectMaterials;

	// load texture images and convert to OpenGL texture data
	bool CreateGLTexture(const char* filename, std::string tag);
	// bind loaded OpenGL textures to slots in memory
	void BindGLTextures();
	// free the loaded OpenGL textures
	void DestroyGLTextures();
	// find a loaded texture by tag
	int FindTextureID(std::string tag);
	int FindTextureSlot(std::string tag);
	// find a defined material by tag
	bool FindMaterial(std::string tag, OBJECT_MATERIAL& material);

	// JESSICA SOLER
	// set the transformation values into the transform buffer
	// wholeObject moves a part with the rest of its object
	void SetTransformations(
		glm::vec3 scaleXYZ,
		float XrotationDegrees,
		float YrotationDegrees,
		float ZrotationDegrees,
		glm::vec3 positionXYZ,
		const glm::mat4& wholeObject = glm::mat4(1.0f));

	// set the color values into the shader
	void SetShaderColor(
		float redColorValue,
		float greenColorValue,
		float blueColorValue,
		float alphaValue);

	// set the texture data into the shader
	void SetShaderTexture(
		std::string textureTag);

	// set the UV scale for the texture mapping
	void SetTextureUVScale(
		float u, float v);

	// set the object material into the shader
	void SetShaderMaterial(
		std::string materialTag);

	// JESSICA SOLER
	// set how each material responds to light
	void DefineObjectMaterials();

	// JESSICA SOLER
	// set the light positions, colors, and highlights
	void SetupSceneLights();

	// JESSICA SOLER
	// choose the mesh used by each object part
	enum MeshType
	{
		PLANE_MESH,
		CYLINDER_MESH,
		TAPERED_CYLINDER_MESH,
		PRISM_MESH,
		SPHERE_MESH
	};

	// JESSICA SOLER
	// store the drawing settings for one object part
	struct SceneObject
	{
		MeshType meshType;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec3 position;
		glm::vec4 color;
		std::string textureTag;
		glm::vec2 uvScale;
		bool useTexture;
		std::string materialTag;

		// choose which cylinder surfaces to draw
		bool drawTop = true;
		bool drawBottom = true;
		bool drawSides = true;
	};

	// JESSICA SOLER
	// store and draw the table
	class Table
	{
	public:
		Table();
		void Render(SceneManager& scene);

	private:
		SceneObject m_plane;
	};

	Table m_table;

	// JESSICA SOLER
	// store and draw the six cracker box faces
	class CrackerBox
	{
	public:
		CrackerBox();
		void Render(SceneManager& scene);

	private:
		std::vector<SceneObject> m_faces;
	};

	CrackerBox m_crackerBox;

	// JESSICA SOLER
	// store and draw the cheese
	class Cheese
	{
	public:
		Cheese();
		void Render(SceneManager& scene);

	private:
		SceneObject m_cheese;
	};

	Cheese m_cheese;

	// JESSICA SOLER
	// store and draw the chocolate package and front cover
	class Chocolate
	{
	public:
		Chocolate();
		void Render(SceneManager& scene);

	private:
		SceneObject m_package;
		SceneObject m_frontCover;
	};

	Chocolate m_chocolate;

	// JESSICA SOLER
	// store and draw the apple body and stem
	class Apple
	{
	public:
		Apple();
		void Render(SceneManager& scene);

	private:
		SceneObject m_body;
		SceneObject m_stem;
	};

	Apple m_apple;

	// JESSICA SOLER
	// store and draw the bottle parts as one object
	class Bottle
	{
	public:
		Bottle();

		// adjust the whole bottle
		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);

		void Render(SceneManager& scene);

	private:
		// combine the whole bottle's position, rotation, and scale
		glm::mat4 GetModelMatrix() const;

		glm::vec3 m_position;	// position of the bottle base
		glm::vec3 m_rotation;	// rotation around the bottle base
		glm::vec3 m_scale;		// size of the whole bottle

		// parts positioned relative to the bottle base
		std::vector<SceneObject> m_parts;
	};

	Bottle m_bottle;

	// JESSICA SOLER
	// draw an object part using its stored settings
	void DrawSceneObject(const SceneObject& object, const glm::mat4& wholeObject = glm::mat4(1.0f));

public:
	// prepare and draw the scene
	void PrepareScene();
	void RenderScene();

	// JESSICA SOLER
	// load the scene textures
	void LoadSceneTextures();
};