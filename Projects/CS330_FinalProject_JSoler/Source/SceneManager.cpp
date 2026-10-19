///////////////////////////////////////////////////////////////////////////////
// SceneManager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

#include <iostream>

// shader names and scene settings
namespace
{
	// Shader uniform names
	const char* const g_ModelName = "model";
	const char* const g_ColorValueName = "objectColor";
	const char* const g_TextureValueName = "objectTexture";
	const char* const g_UseTextureName = "bUseTexture";
	const char* const g_UseLightingName = "bUseLighting";

	// JESSICA SOLER
	// Texture tags
	const char* const TEXTURE_WOOD = "wood";
	const char* const TEXTURE_CRACKER_FRONT = "crackerFront";
	const char* const TEXTURE_CRACKER_BACK = "crackerBack";
	const char* const TEXTURE_CRACKER_SIDE_1 = "crackerSide1";
	const char* const TEXTURE_CRACKER_SIDE_2 = "crackerSide2";
	const char* const TEXTURE_CRACKER_TOP = "crackerTop";
	const char* const TEXTURE_CHEESE = "cheese";
	const char* const TEXTURE_CHOCOLATE = "chocolate";
	const char* const TEXTURE_GLASS = "glass";
	const char* const TEXTURE_METAL = "metal";
	const char* const TEXTURE_LID_TOP = "lidTop";
	const char* const TEXTURE_WINE_LABEL = "wineLabel";

	// Texture paths
	const char* const TEXTURE_WOOD_PATH = "../../Utilities/textures/rusticwood.jpg";
	const char* const TEXTURE_CRACKER_FRONT_PATH = "../../Utilities/textures/cracker_front.jpg";
	const char* const TEXTURE_CRACKER_BACK_PATH = "../../Utilities/textures/cracker_back.jpg";
	const char* const TEXTURE_CRACKER_SIDE_1_PATH = "../../Utilities/textures/cracker_side_1.jpg";
	const char* const TEXTURE_CRACKER_SIDE_2_PATH = "../../Utilities/textures/cracker_side_2.jpg";
	const char* const TEXTURE_CRACKER_TOP_PATH = "../../Utilities/textures/cracker_top.jpg";
	const char* const TEXTURE_CHEESE_PATH = "../../Utilities/textures/cheddar.jpg";
	const char* const TEXTURE_CHOCOLATE_PATH = "../../Utilities/textures/chocolate.jpg";
	const char* const TEXTURE_GLASS_PATH = "../../Utilities/textures/glass.jpg";
	const char* const TEXTURE_METAL_PATH = "../../Utilities/textures/stainless.jpg";
	const char* const TEXTURE_LID_TOP_PATH = "../../Utilities/textures/stainless_end.jpg";
	const char* const TEXTURE_WINE_LABEL_PATH = "../../Utilities/textures/wine_label.jpg";

	// Material tags
	const char* const MATERIAL_WOOD = "wood";
	const char* const MATERIAL_PAPER = "paper";
	const char* const MATERIAL_CHEESE = "cheese";
	const char* const MATERIAL_APPLE = "apple";
	const char* const MATERIAL_GLASS = "glass";
	const char* const MATERIAL_METAL = "metal";

	// Shared material setting
	const float MATERIAL_AMBIENT_STRENGTH = 0.03f;

	// Shared colors and starting values
	const glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 GREY = glm::vec4(0.40f, 0.40f, 0.40f, 1.0f);
	const glm::vec3 CENTER_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 DEFAULT_ROTATION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec2 DEFAULT_UV_SCALE = glm::vec2(1.0f, 1.0f);

	// TABLE SETTINGS
	const glm::vec3 TABLE_SCALE = glm::vec3(10.0f, 1.0f, 10.0f);
	const glm::vec2 TABLE_UV_SCALE = glm::vec2(4.0f, 4.0f);

	// CRACKER BOX SETTINGS
	const glm::vec3 CRACKER_BOX_SCALE = glm::vec3(2.6f, 4.2f, 1.0f);
	// Raise the box center by half its height to sit on the table
	const glm::vec3 CRACKER_BOX_POSITION = glm::vec3(-3.0f, CRACKER_BOX_SCALE.y / 2.0f, -1.0f);
	const glm::vec4 CRACKER_BOX_COLOR = glm::vec4(0.94f, 0.90f, 0.80f, 1.0f);

	// CHEESE SETTINGS
	const glm::vec3 CHEESE_SCALE = glm::vec3(1.5f, 0.55f, 1.5f);
	const glm::vec3 CHEESE_POSITION = glm::vec3(-3.0f, 0.0f, 2.5f);

	// CHOCOLATE SETTINGS
	// Lay the prism flat and angle it across the table
	const glm::vec3 CHOCOLATE_SCALE = glm::vec3(1.0f, 5.5f, 0.9f);
	const glm::vec3 CHOCOLATE_ROTATION = glm::vec3(-90.0f, 0.0f, -65.0f);
	const glm::vec3 CHOCOLATE_POSITION = glm::vec3(0.3f, CHOCOLATE_SCALE.z / 2.0f, 4.4f);
	const glm::vec4 CHOCOLATE_COLOR = glm::vec4(0.18f, 0.30f, 0.35f, 1.0f);
	// Front cover fitted to the current package size and position
	const glm::vec3 CHOCOLATE_COVER_SCALE = glm::vec3(0.5148f, 1.0f, 2.75f);
	const glm::vec3 CHOCOLATE_COVER_ROTATION = glm::vec3(0.0f, -65.0f, -60.9454f);
	const glm::vec3 CHOCOLATE_COVER_POSITION = glm::vec3(0.4068f, 0.4515f, 4.6290f);

	// APPLE SETTINGS
	// Slightly wider and flatter than a round sphere
	const glm::vec3 APPLE_BODY_SCALE = glm::vec3(1.10f, 0.85f, 1.0f);
	const glm::vec3 APPLE_POSITION = glm::vec3(0.2f, APPLE_BODY_SCALE.y, 2.2f);
	const glm::vec4 APPLE_COLOR = glm::vec4(0.50f, 0.65f, 0.12f, 1.0f);
	// Set the stem slightly inside the apple
	const glm::vec3 APPLE_STEM_SCALE = glm::vec3(0.07f, 0.35f, 0.07f);
	const glm::vec4 APPLE_STEM_COLOR = glm::vec4(0.30f, 0.16f, 0.06f, 1.0f);
	const float APPLE_STEM_INSERT_DEPTH = 0.05f;
	const glm::vec3 APPLE_STEM_POSITION = APPLE_POSITION + glm::vec3(0.0f, APPLE_BODY_SCALE.y - APPLE_STEM_INSERT_DEPTH, 0.0f);

	// BOTTLE SETTINGS
	// Glass color and a less transparent base
	const glm::vec4 BOTTLE_GLASS_COLOR = glm::vec4(0.95f, 0.86f, 0.38f, 0.60f);
	const glm::vec4 BOTTLE_BASE_COLOR = glm::vec4(glm::vec3(BOTTLE_GLASS_COLOR), 0.85f);
	const glm::vec2 BOTTLE_LID_UV_SCALE = glm::vec2(4.0f, 1.0f);

	// Whole bottle position and starting rotation
	const glm::vec3 BOTTLE_POSITION = glm::vec3(2.0f, 0.0f, -1.0f);
	const float BOTTLE_Y_ROTATION = 98.0f;

	// Base and lower body together stay 4 units tall
	const float BOTTLE_BODY_HEIGHT = 4.0f;
	const float BOTTLE_BASE_HEIGHT = 0.15f;

	// Sizes of the bottle parts
	const glm::vec3 BOTTLE_BOTTOM_SCALE = glm::vec3(1.0f, BOTTLE_BODY_HEIGHT - BOTTLE_BASE_HEIGHT, 1.0f);
	const glm::vec3 BOTTLE_BASE_SCALE = glm::vec3(BOTTLE_BOTTOM_SCALE.x, BOTTLE_BASE_HEIGHT, BOTTLE_BOTTOM_SCALE.z);
	const glm::vec3 BOTTLE_MIDDLE_SCALE = glm::vec3(1.0f, 0.8f, 1.0f);
	const glm::vec3 BOTTLE_NECK_SCALE = glm::vec3(0.5f, 1.6f, 0.5f);
	const glm::vec3 BOTTLE_LID_SCALE = glm::vec3(0.58f, 1.0f, 0.58f);
	const glm::vec3 BOTTLE_LID_TOP_SCALE = glm::vec3(0.59f, 0.05f, 0.59f);
	const glm::vec3 LABEL_SCALE = glm::vec3(1.005f, 1.25f, 1.005f);

	// Stack the parts relative to the bottle base
	const glm::vec3 BOTTLE_BASE_OFFSET = CENTER_POSITION;
	const glm::vec3 BOTTLE_BOTTOM_OFFSET = BOTTLE_BASE_OFFSET + glm::vec3(0.0f, BOTTLE_BASE_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_MIDDLE_OFFSET = BOTTLE_BOTTOM_OFFSET + glm::vec3(0.0f, BOTTLE_BOTTOM_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_NECK_OFFSET = BOTTLE_MIDDLE_OFFSET + glm::vec3(0.0f, BOTTLE_MIDDLE_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_LID_OFFSET = BOTTLE_NECK_OFFSET + glm::vec3(0.0f, BOTTLE_NECK_SCALE.y, 0.0f);

	// Line up the textured top with the upper edge of the lid
	const glm::vec3 BOTTLE_LID_TOP_OFFSET = BOTTLE_LID_OFFSET + glm::vec3(0.0f, BOTTLE_LID_SCALE.y - BOTTLE_LID_TOP_SCALE.y, 0.0f);
	const glm::vec3 BOTTLE_LABEL_OFFSET = glm::vec3(0.0f, 1.35f, 0.0f);
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager* pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();

	// JESSICA SOLER
	// Start the texture count at zero
	m_loadedTextures = 0;
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	// JESSICA SOLER
	// Free the loaded textures
	DestroyGLTextures();

	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// JESSICA SOLER
		// Allow image rows that are not a multiple of four bytes
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;

			// JESSICA SOLER
			// Free the texture and image if the format is unsupported
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &textureID);
			stbi_image_free(image);
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// JESSICA SOLER
		// Delete each texture and clear its ID
		glDeleteTextures(1, &m_textureIDs[i].ID);
		m_textureIDs[i].ID = 0;
	}

	// Reset the count after freeing the textures
	m_loadedTextures = 0;
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(bFound);
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

// JESSICA SOLER
/***********************************************************
 *  SetShaderTexture()
 *
 *  Find the texture using its tag
 *  Use the assigned color if the texture did not load
 ***********************************************************/
void SceneManager::SetShaderTexture(std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		int textureID = FindTextureSlot(textureTag);

		// Use the assigned color if the texture is missing
		if (textureID < 0)
		{
			m_pShaderManager->setIntValue(g_UseTextureName, false);
			return;
		}

		m_pShaderManager->setIntValue(g_UseTextureName, true);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/

// JESSICA SOLER
/***********************************************************
 *  LoadSceneTextures()
 *
 *  Load the texture images using their paths and tags
 *  Print a message if an image does not load
 ***********************************************************/
void SceneManager::LoadSceneTextures()
{
	// Keep each image path with its texture tag
	struct TextureSource
	{
		const char* path;
		const char* tag;
	};

	const TextureSource textures[] =
	{
		// Table
		{ TEXTURE_WOOD_PATH, TEXTURE_WOOD },

		// Cracker box
		{ TEXTURE_CRACKER_FRONT_PATH, TEXTURE_CRACKER_FRONT },
		{ TEXTURE_CRACKER_BACK_PATH, TEXTURE_CRACKER_BACK },
		{ TEXTURE_CRACKER_SIDE_1_PATH, TEXTURE_CRACKER_SIDE_1 },
		{ TEXTURE_CRACKER_SIDE_2_PATH, TEXTURE_CRACKER_SIDE_2 },
		{ TEXTURE_CRACKER_TOP_PATH, TEXTURE_CRACKER_TOP },

		// Cheese
		{ TEXTURE_CHEESE_PATH, TEXTURE_CHEESE },

		// Chocolate
		{ TEXTURE_CHOCOLATE_PATH, TEXTURE_CHOCOLATE },

		// Apple uses a color and material

		// Bottle
		{ TEXTURE_GLASS_PATH, TEXTURE_GLASS },
		{ TEXTURE_METAL_PATH, TEXTURE_METAL },
		{ TEXTURE_LID_TOP_PATH, TEXTURE_LID_TOP },
		{ TEXTURE_WINE_LABEL_PATH, TEXTURE_WINE_LABEL }
	};

	// Keep loading the other textures if one fails
	for (const TextureSource& texture : textures)
	{
		if (!CreateGLTexture(texture.path, texture.tag))
		{
			std::cout << "ALERT: texture failed: " << texture.tag << "\n";
		}
	}

	BindGLTextures();
}

// JESSICA SOLER
/***********************************************************
 *  DefineObjectMaterials()
 *
 *  Set the light response for each surface
 *  Give each material its own amount of shine
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	// Clear any previously stored materials
	m_objectMaterials.clear();

	// Table wood
	OBJECT_MATERIAL wood{};
	wood.tag = MATERIAL_WOOD;
	wood.ambientColor = glm::vec3(WHITE);
	wood.ambientStrength = MATERIAL_AMBIENT_STRENGTH;
	wood.diffuseColor = glm::vec3(0.65f);
	wood.specularColor = glm::vec3(WHITE);
	wood.shininess = 0.60f;
	m_objectMaterials.push_back(wood);

	// Paper shared by the packages, apple stem, and wine label
	OBJECT_MATERIAL paper{};
	paper.tag = MATERIAL_PAPER;
	paper.ambientColor = glm::vec3(WHITE);
	paper.ambientStrength = MATERIAL_AMBIENT_STRENGTH;
	paper.diffuseColor = glm::vec3(0.45f);
	paper.specularColor = glm::vec3(WHITE);
	paper.shininess = 0.05f;
	m_objectMaterials.push_back(paper);

	// Cheese with a slight shine
	OBJECT_MATERIAL cheese{};
	cheese.tag = MATERIAL_CHEESE;
	cheese.ambientColor = glm::vec3(WHITE);
	cheese.ambientStrength = MATERIAL_AMBIENT_STRENGTH;
	cheese.diffuseColor = glm::vec3(0.65f);
	cheese.specularColor = glm::vec3(WHITE) * 0.20f;
	cheese.shininess = 0.10f;
	m_objectMaterials.push_back(cheese);

	// Apple skin with a soft, waxy highlight
	OBJECT_MATERIAL apple{};
	apple.tag = MATERIAL_APPLE;
	apple.ambientColor = glm::vec3(WHITE);
	apple.ambientStrength = MATERIAL_AMBIENT_STRENGTH;
	apple.diffuseColor = glm::vec3(0.55f);
	apple.specularColor = glm::vec3(WHITE) * 0.60f;
	apple.shininess = 0.25f;
	m_objectMaterials.push_back(apple);

	// Bottle glass with a stronger highlight
	OBJECT_MATERIAL glass{};
	glass.tag = MATERIAL_GLASS;
	glass.ambientColor = glm::vec3(WHITE);
	glass.ambientStrength = MATERIAL_AMBIENT_STRENGTH;
	glass.diffuseColor = glm::vec3(0.65f);
	glass.specularColor = glm::vec3(0.90f);
	glass.shininess = 0.50f;
	m_objectMaterials.push_back(glass);

	// Metal bottle lid
	OBJECT_MATERIAL metal{};
	metal.tag = MATERIAL_METAL;
	metal.ambientColor = glm::vec3(WHITE);
	metal.ambientStrength = MATERIAL_AMBIENT_STRENGTH;
	metal.diffuseColor = glm::vec3(0.55f);
	metal.specularColor = glm::vec3(WHITE);
	metal.shininess = 0.60f;
	m_objectMaterials.push_back(metal);
}

// JESSICA SOLER
/***********************************************************
 *  SetupSceneLights()
 *
 *  Place four point lights around the scene
 *  Use one warm light and three neutral lights
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// Match the number of lights in the fragment shader
	const int LIGHT_COUNT = 4;

	// Shared highlight settings
	const float LIGHT_FOCAL_STRENGTH = 32.0f;
	const float LIGHT_SPECULAR_INTENSITY = 0.65f;

	// Ambient light to help brighten the darker surfaces
	const glm::vec3 WARM_AMBIENT = glm::vec3(0.03f, 0.02f, 0.01f);
	const glm::vec3 NEUTRAL_AMBIENT = glm::vec3(WHITE) * 0.04f;

	// Warm main light, similar to an indoor lamp
	const glm::vec3 WARM_LIGHT_COLOR = glm::vec3(1.0f, 0.85f, 0.65f);
	const glm::vec3 NEUTRAL_LIGHT_COLOR = glm::vec3(WHITE);

	// Lights above and around the table
	const glm::vec3 lightPositions[LIGHT_COUNT] =
	{
		glm::vec3(-12.0f, 8.0f, 12.0f),
		glm::vec3(12.0f, 6.0f, 10.0f),
		glm::vec3(-10.0f, 4.0f, -12.0f),
		glm::vec3(12.0f, 3.0f, -10.0f)
	};

	const glm::vec3 ambientColors[LIGHT_COUNT] =
	{
		WARM_AMBIENT,
		NEUTRAL_AMBIENT,
		NEUTRAL_AMBIENT,
		NEUTRAL_AMBIENT
	};

	// Send each light's settings to the shader
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		const std::string lightName = "lightSources[" + std::to_string(i) + "]";

		m_pShaderManager->setVec3Value(
			(lightName + ".position").c_str(),
			lightPositions[i]);

		m_pShaderManager->setVec3Value(
			(lightName + ".ambientColor").c_str(),
			ambientColors[i]);

		m_pShaderManager->setFloatValue(
			(lightName + ".focalStrength").c_str(),
			LIGHT_FOCAL_STRENGTH);

		m_pShaderManager->setFloatValue(
			(lightName + ".specularIntensity").c_str(),
			LIGHT_SPECULAR_INTENSITY);

		// First light warm, remaining lights neutral
		m_pShaderManager->setVec3Value(
			(lightName + ".diffuseColor").c_str(),
			(i == 0) ? WARM_LIGHT_COLOR : NEUTRAL_LIGHT_COLOR);
	}

	m_pShaderManager->setBoolValue(g_UseLightingName, true);
}

// JESSICA SOLER
/***********************************************************
 *  PrepareScene()
 *
 *  Load the textures and meshes
 *  Set up the materials and lights
 ***********************************************************/
void SceneManager::PrepareScene()
{
	LoadSceneTextures();
	DefineObjectMaterials();
	SetupSceneLights();

	// Load each mesh once and reuse it for the scene objects
	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadPrismMesh();
	m_basicMeshes->LoadSphereMesh();
}

// JESSICA SOLER
/***********************************************************
 *  RenderScene()
 *
 *  Draw each object in the scene
 *  Keep the transparent bottle last
 ***********************************************************/
void SceneManager::RenderScene()
{
	m_table.Render(*this);
	m_crackerBox.Render(*this);
	m_cheese.Render(*this);
	m_chocolate.Render(*this);
	m_apple.Render(*this);
	m_bottle.Render(*this);
}

// JESSICA SOLER
/***********************************************************
 *  SetTransformations()
 *
 *  Combine the part's settings with the whole object
 *  Send the finished model matrix to the shader
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ,
	const glm::mat4& wholeObject)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// Set the part's scale
	scale = glm::scale(scaleXYZ);

	// Set the part's rotation around each axis
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));

	// Set the part's position
	translation = glm::translate(positionXYZ);

	modelView = wholeObject * translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

// JESSICA SOLER
/***********************************************************
 *  DrawSceneObject()
 *
 *  Apply the part's stored settings and draw its mesh
 *  wholeObject keeps the part with the rest of its object
 ***********************************************************/
void SceneManager::DrawSceneObject(const SceneObject& object, const glm::mat4& wholeObject)
{
	SetTransformations(
		object.scale,
		object.rotation.x,
		object.rotation.y,
		object.rotation.z,
		object.position,
		wholeObject);

	// Set the color first in case the texture is unavailable
	SetShaderColor(
		object.color.r,
		object.color.g,
		object.color.b,
		object.color.a);

	if (object.useTexture)
	{
		SetShaderTexture(object.textureTag);
		SetTextureUVScale(object.uvScale.x, object.uvScale.y);
	}
	else
	{
		SetTextureUVScale(DEFAULT_UV_SCALE.x, DEFAULT_UV_SCALE.y);
	}

	SetShaderMaterial(object.materialTag);

	if (object.meshType == PLANE_MESH)
	{
		m_basicMeshes->DrawPlaneMesh();
	}
	else if (object.meshType == CYLINDER_MESH)
	{
		m_basicMeshes->DrawCylinderMesh(
			object.drawTop,
			object.drawBottom,
			object.drawSides);
	}
	else if (object.meshType == TAPERED_CYLINDER_MESH)
	{
		m_basicMeshes->DrawTaperedCylinderMesh(
			object.drawTop,
			object.drawBottom,
			object.drawSides);
	}
	else if (object.meshType == PRISM_MESH)
	{
		m_basicMeshes->DrawPrismMesh();
	}
	else if (object.meshType == SPHERE_MESH)
	{
		m_basicMeshes->DrawSphereMesh();
	}
}

// JESSICA SOLER
/***********************************************************
 *  Table::Table()
 *
 *  Set up a plane for the table
 *  Apply the wood texture and material
 ***********************************************************/
SceneManager::Table::Table()
{
	m_plane.meshType = PLANE_MESH;
	m_plane.scale = TABLE_SCALE;
	m_plane.rotation = DEFAULT_ROTATION;
	m_plane.position = CENTER_POSITION;
	m_plane.color = WHITE;
	m_plane.textureTag = TEXTURE_WOOD;
	m_plane.uvScale = TABLE_UV_SCALE;
	m_plane.useTexture = true;
	m_plane.materialTag = MATERIAL_WOOD;
}

// JESSICA SOLER
/***********************************************************
 *  Table::Render()
 *
 *  Draw the table using DrawSceneObject()
 ***********************************************************/
void SceneManager::Table::Render(SceneManager& scene)
{
	scene.DrawSceneObject(m_plane);
}

// JESSICA SOLER
/***********************************************************
 *  CrackerBox::CrackerBox()
 *
 *  Build the cracker box from six planes
 *  Give each face its own position, rotation, and texture
 ***********************************************************/
SceneManager::CrackerBox::CrackerBox()
{
	// The plane runs from -1 to 1, so use half the box dimensions
	const glm::vec3 halfSize = CRACKER_BOX_SCALE * 0.5f;
	const glm::vec3 frontBackScale = glm::vec3(halfSize.x, 1.0f, halfSize.y);
	const glm::vec3 sideScale = glm::vec3(halfSize.z, 1.0f, halfSize.y);
	const glm::vec3 topBottomScale = glm::vec3(halfSize.x, 1.0f, halfSize.z);

	// Settings for each face relative to the box center
	struct FaceSettings
	{
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec3 offset;
		const char* textureTag;
	};

	const FaceSettings faces[] =
	{
		// Front
		{ frontBackScale, glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, halfSize.z), TEXTURE_CRACKER_FRONT },

		// Back
		{ frontBackScale, glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(0.0f, 0.0f, -halfSize.z), TEXTURE_CRACKER_BACK },

		// Left
		{ sideScale, glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(-halfSize.x, 0.0f, 0.0f), TEXTURE_CRACKER_SIDE_1 },

		// Right
		{ sideScale, glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(halfSize.x, 0.0f, 0.0f), TEXTURE_CRACKER_SIDE_2 },

		// Top
		{ topBottomScale, DEFAULT_ROTATION, glm::vec3(0.0f, halfSize.y, 0.0f), TEXTURE_CRACKER_TOP },

		// Bottom with a plain cream color
		{ topBottomScale, glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(0.0f, -halfSize.y, 0.0f), "" }
	};

	// Apply the shared settings to each face
	for (const FaceSettings& settings : faces)
	{
		SceneObject face{};
		face.meshType = PLANE_MESH;
		face.scale = settings.scale;
		face.rotation = settings.rotation;
		face.position = CRACKER_BOX_POSITION + settings.offset;
		face.textureTag = settings.textureTag;
		face.useTexture = !face.textureTag.empty();
		face.color = face.useTexture ? WHITE : CRACKER_BOX_COLOR;
		face.uvScale = DEFAULT_UV_SCALE;
		face.materialTag = MATERIAL_PAPER;

		m_faces.push_back(face);
	}
}

// JESSICA SOLER
/***********************************************************
 *  CrackerBox::Render()
 *
 *  Draw each face using DrawSceneObject()
 ***********************************************************/
void SceneManager::CrackerBox::Render(SceneManager& scene)
{
	for (const SceneObject& face : m_faces)
	{
		scene.DrawSceneObject(face);
	}
}

// JESSICA SOLER
/***********************************************************
 *  Cheese::Cheese()
 *
 *  Set up a short cylinder for the cheese
 *  Apply the cheese texture and material
 ***********************************************************/
SceneManager::Cheese::Cheese()
{
	m_cheese.meshType = CYLINDER_MESH;
	m_cheese.scale = CHEESE_SCALE;
	m_cheese.rotation = DEFAULT_ROTATION;
	m_cheese.position = CHEESE_POSITION;
	m_cheese.color = WHITE;
	m_cheese.textureTag = TEXTURE_CHEESE;
	m_cheese.uvScale = DEFAULT_UV_SCALE;
	m_cheese.useTexture = true;
	m_cheese.materialTag = MATERIAL_CHEESE;
}

// JESSICA SOLER
/***********************************************************
 *  Cheese::Render()
 *
 *  Draw the cheese using DrawSceneObject()
 ***********************************************************/
void SceneManager::Cheese::Render(SceneManager& scene)
{
	scene.DrawSceneObject(m_cheese);
}

// JESSICA SOLER
/***********************************************************
 *  Chocolate::Chocolate()
 *
 *  Set up a navy prism for the chocolate package
 *  Add a textured plane for the front cover
 ***********************************************************/
SceneManager::Chocolate::Chocolate()
{
	// Plain navy package
	m_package.meshType = PRISM_MESH;
	m_package.scale = CHOCOLATE_SCALE;
	m_package.rotation = CHOCOLATE_ROTATION;
	m_package.position = CHOCOLATE_POSITION;
	m_package.color = CHOCOLATE_COLOR;
	m_package.textureTag = "";
	m_package.uvScale = DEFAULT_UV_SCALE;
	m_package.useTexture = false;
	m_package.materialTag = MATERIAL_PAPER;

	// Front cover fitted to the sloping side
	m_frontCover = m_package;
	m_frontCover.meshType = PLANE_MESH;
	m_frontCover.scale = CHOCOLATE_COVER_SCALE;
	m_frontCover.rotation = CHOCOLATE_COVER_ROTATION;
	m_frontCover.position = CHOCOLATE_COVER_POSITION;
	m_frontCover.color = WHITE;
	m_frontCover.textureTag = TEXTURE_CHOCOLATE;
	m_frontCover.useTexture = true;
}

// JESSICA SOLER
/***********************************************************
 *  Chocolate::Render()
 *
 *  Draw the package and front cover using DrawSceneObject()
 ***********************************************************/
void SceneManager::Chocolate::Render(SceneManager& scene)
{
	scene.DrawSceneObject(m_package);
	scene.DrawSceneObject(m_frontCover);
}

// JESSICA SOLER
/***********************************************************
 *  Apple::Apple()
 *
 *  Set up a sphere for the apple body
 *  Add a short cylinder for the stem
 ***********************************************************/
SceneManager::Apple::Apple()
{
	// Apple body
	m_body.meshType = SPHERE_MESH;
	m_body.scale = APPLE_BODY_SCALE;
	m_body.rotation = DEFAULT_ROTATION;
	m_body.position = APPLE_POSITION;
	m_body.color = APPLE_COLOR;
	m_body.textureTag = "";
	m_body.uvScale = DEFAULT_UV_SCALE;
	m_body.useTexture = false;
	m_body.materialTag = MATERIAL_APPLE;

	// Apple stem
	m_stem.meshType = CYLINDER_MESH;
	m_stem.scale = APPLE_STEM_SCALE;
	m_stem.rotation = DEFAULT_ROTATION;
	m_stem.position = APPLE_STEM_POSITION;
	m_stem.color = APPLE_STEM_COLOR;
	m_stem.textureTag = "";
	m_stem.uvScale = DEFAULT_UV_SCALE;
	m_stem.useTexture = false;
	m_stem.materialTag = MATERIAL_PAPER;
}

// JESSICA SOLER
/***********************************************************
 *  Apple::Render()
 *
 *  Draw the body and stem using DrawSceneObject()
 ***********************************************************/
void SceneManager::Apple::Render(SceneManager& scene)
{
	scene.DrawSceneObject(m_body);
	scene.DrawSceneObject(m_stem);
}

// JESSICA SOLER
/***********************************************************
 *  Bottle::Bottle()
 *
 *  Set up the whole bottle and its seven parts
 *  Position each part relative to the bottle base
 ***********************************************************/
SceneManager::Bottle::Bottle()
	: m_position(BOTTLE_POSITION),
	m_rotation(DEFAULT_ROTATION),
	m_scale(1.0f)
{
	// Starting rotation for the whole bottle
	m_rotation.y = BOTTLE_Y_ROTATION;

	// Bottle base
	// Add some visible thickness to the bottom of the glass
	SceneObject bottleBase;
	bottleBase.meshType = CYLINDER_MESH;
	bottleBase.scale = BOTTLE_BASE_SCALE;
	bottleBase.rotation = DEFAULT_ROTATION;
	bottleBase.position = BOTTLE_BASE_OFFSET;
	bottleBase.color = BOTTLE_BASE_COLOR;
	bottleBase.textureTag = TEXTURE_GLASS;
	bottleBase.uvScale = DEFAULT_UV_SCALE;
	bottleBase.useTexture = true;
	bottleBase.materialTag = MATERIAL_GLASS;
	bottleBase.drawTop = true;
	bottleBase.drawBottom = true;
	bottleBase.drawSides = true;
	m_parts.push_back(bottleBase);

	// Bottle bottom
	// Open ends where this part meets the base and middle
	SceneObject bottleBottom;
	bottleBottom.meshType = CYLINDER_MESH;
	bottleBottom.scale = BOTTLE_BOTTOM_SCALE;
	bottleBottom.rotation = DEFAULT_ROTATION;
	bottleBottom.position = BOTTLE_BOTTOM_OFFSET;
	bottleBottom.color = BOTTLE_GLASS_COLOR;
	bottleBottom.textureTag = TEXTURE_GLASS;
	bottleBottom.uvScale = DEFAULT_UV_SCALE;
	bottleBottom.useTexture = true;
	bottleBottom.materialTag = MATERIAL_GLASS;
	bottleBottom.drawTop = false;
	bottleBottom.drawBottom = false;
	m_parts.push_back(bottleBottom);

	// Bottle middle
	SceneObject bottleMiddle;
	bottleMiddle.meshType = TAPERED_CYLINDER_MESH;
	bottleMiddle.scale = BOTTLE_MIDDLE_SCALE;
	bottleMiddle.rotation = DEFAULT_ROTATION;
	bottleMiddle.position = BOTTLE_MIDDLE_OFFSET;
	bottleMiddle.color = BOTTLE_GLASS_COLOR;
	bottleMiddle.textureTag = TEXTURE_GLASS;
	bottleMiddle.uvScale = DEFAULT_UV_SCALE;
	bottleMiddle.useTexture = true;
	bottleMiddle.materialTag = MATERIAL_GLASS;
	bottleMiddle.drawTop = false;
	bottleMiddle.drawBottom = false;
	m_parts.push_back(bottleMiddle);

	// Bottle neck
	SceneObject bottleNeck;
	bottleNeck.meshType = CYLINDER_MESH;
	bottleNeck.scale = BOTTLE_NECK_SCALE;
	bottleNeck.rotation = DEFAULT_ROTATION;
	bottleNeck.position = BOTTLE_NECK_OFFSET;
	bottleNeck.color = BOTTLE_GLASS_COLOR;
	bottleNeck.textureTag = TEXTURE_GLASS;
	bottleNeck.uvScale = DEFAULT_UV_SCALE;
	bottleNeck.useTexture = true;
	bottleNeck.materialTag = MATERIAL_GLASS;
	bottleNeck.drawTop = false;
	bottleNeck.drawBottom = false;
	m_parts.push_back(bottleNeck);

	// Bottle lid
	SceneObject bottleLid;
	bottleLid.meshType = CYLINDER_MESH;
	bottleLid.scale = BOTTLE_LID_SCALE;
	bottleLid.rotation = DEFAULT_ROTATION;
	bottleLid.position = BOTTLE_LID_OFFSET;
	bottleLid.color = GREY;
	bottleLid.textureTag = TEXTURE_METAL;
	bottleLid.uvScale = BOTTLE_LID_UV_SCALE;
	bottleLid.useTexture = true;
	bottleLid.materialTag = MATERIAL_METAL;
	bottleLid.drawTop = false;
	bottleLid.drawBottom = false;
	m_parts.push_back(bottleLid);

	// Bottle lid top
	// Only draw the textured top to avoid extra triangles
	SceneObject bottleLidTop;
	bottleLidTop.meshType = CYLINDER_MESH;
	bottleLidTop.scale = BOTTLE_LID_TOP_SCALE;
	bottleLidTop.rotation = DEFAULT_ROTATION;
	bottleLidTop.position = BOTTLE_LID_TOP_OFFSET;
	bottleLidTop.color = WHITE;
	bottleLidTop.textureTag = TEXTURE_LID_TOP;
	bottleLidTop.uvScale = DEFAULT_UV_SCALE;
	bottleLidTop.useTexture = true;
	bottleLidTop.materialTag = MATERIAL_METAL;
	bottleLidTop.drawBottom = false;
	bottleLidTop.drawSides = false;
	m_parts.push_back(bottleLidTop);

	// Wine label
	// Slightly wider than the body to sit just outside the glass
	SceneObject wineLabel;
	wineLabel.meshType = CYLINDER_MESH;
	wineLabel.scale = LABEL_SCALE;
	wineLabel.rotation = DEFAULT_ROTATION;
	wineLabel.position = BOTTLE_LABEL_OFFSET;
	wineLabel.color = WHITE;
	wineLabel.textureTag = TEXTURE_WINE_LABEL;
	wineLabel.uvScale = DEFAULT_UV_SCALE;
	wineLabel.useTexture = true;
	wineLabel.materialTag = MATERIAL_PAPER;
	wineLabel.drawTop = false;
	wineLabel.drawBottom = false;
	m_parts.push_back(wineLabel);
}

// JESSICA SOLER
/***********************************************************
 *  Bottle::SetPosition()
 *
 *  Move the whole bottle using its base position
 ***********************************************************/
void SceneManager::Bottle::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

// JESSICA SOLER
/***********************************************************
 *  Bottle::SetRotation()
 *
 *  Rotate the whole bottle around its base
 ***********************************************************/
void SceneManager::Bottle::SetRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

// JESSICA SOLER
/***********************************************************
 *  Bottle::SetScale()
 *
 *  Change the size of the whole bottle
 ***********************************************************/
void SceneManager::Bottle::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

// JESSICA SOLER
/***********************************************************
 *  Bottle::GetModelMatrix()
 *
 *  Combine the whole bottle's position, rotation, and scale
 *  Share this matrix with all of the bottle parts
 ***********************************************************/
glm::mat4 SceneManager::Bottle::GetModelMatrix() const
{
	glm::mat4 translation = glm::translate(m_position);
	glm::mat4 rotationX = glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationZ = glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(m_scale);

	return translation * rotationX * rotationY * rotationZ * scale;
}

// JESSICA SOLER
/***********************************************************
 *  Bottle::Render()
 *
 *  Draw each part using the whole bottle's matrix
 *  Keep the parts together when the bottle moves
 ***********************************************************/
void SceneManager::Bottle::Render(SceneManager& scene)
{
	const glm::mat4 wholeBottle = GetModelMatrix();

	for (const SceneObject& part : m_parts)
	{
		scene.DrawSceneObject(part, wholeBottle);
	}
}