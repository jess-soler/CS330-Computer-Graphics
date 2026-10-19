///////////////////////////////////////////////////////////////////////////////
// ViewManager.h
// ============
// manage the viewing of 3D objects within the viewport
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "ViewManager.h"
#include <glm/gtc/matrix_transform.hpp>	// JESSICA SOLER

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

// JESSICA SOLER
// Format the camera movement log
#include <iomanip>
#include <iostream>
#include <cmath>

// declaration of the global variables and defines
namespace
{
	// Variables for window width and height
	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 800;
	const char* g_ViewName = "view";
	const char* g_ProjectionName = "projection";

	// JESSICA SOLER
	// Starting camera view
	const glm::vec3 CAMERA_START_POSITION = glm::vec3(0.0f, 5.0f, 12.0f);
	const float CAMERA_FIELD_OF_VIEW = 80.0f;

	// Window position on my screen
	const int WINDOW_POSITION_X = 1500;
	const int WINDOW_POSITION_Y = 55;

	// Projection settings
	const float ORTHOGRAPHIC_HALF_WIDTH = 10.0f;
	const float ORTHOGRAPHIC_HALF_HEIGHT = 8.0f;
	const float NEAR_CLIP_DISTANCE = 0.1f;
	const float FAR_CLIP_DISTANCE = 100.0f;

	// Camera log settings
	const float CAMERA_LOG_INTERVAL = 0.25f;
	const float FULL_ROTATION_DEGREES = 360.0f;

	// Camera object
	Camera* g_pCamera = nullptr;

	// These variables are used for mouse movement processing
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	// Time between current frame and last frame
	float gDeltaTime = 0.0f;
	float gLastFrame = 0.0f;

	// The following variable is false when orthographic projection
	// is off and true when it is on
	bool bOrthographicProjection = false;

	// JESSICA SOLER
	// Remember the O key state to detect a new press
	bool gOKeyWasPressed = false;
}

/***********************************************************
 *  ViewManager()
 *
 *  The constructor for the class
 ***********************************************************/
ViewManager::ViewManager(
	ShaderManager* pShaderManager)
{
	// initialize the member variables
	m_pShaderManager = pShaderManager;
	m_pWindow = NULL;

	// JESSICA SOLER
	// Create the camera and set the starting view
	g_pCamera = new Camera();
	SetFrontView();
	g_pCamera->Zoom = CAMERA_FIELD_OF_VIEW;
}

/***********************************************************
 *  ~ViewManager()
 *
 *  The destructor for the class
 ***********************************************************/
ViewManager::~ViewManager()
{
	// free up allocated memory
	m_pShaderManager = NULL;
	m_pWindow = NULL;
	if (NULL != g_pCamera)
	{
		delete g_pCamera;
		g_pCamera = NULL;
	}
}

/***********************************************************
 *  CreateDisplayWindow()
 *
 *  This method is used to create the main display window.
 ***********************************************************/
GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle)
{
	GLFWwindow* window = nullptr;

	// try to create the displayed OpenGL window
	window = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		windowTitle,
		NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// JESSICA SOLER
	// Leave room for the console on the left of my screen
	glfwSetWindowPos(window, WINDOW_POSITION_X, WINDOW_POSITION_Y);

	// Receive mouse movement and scroll events
	glfwSetCursorPosCallback(window, &ViewManager::Mouse_Position_Callback);
	glfwSetScrollCallback(window, &ViewManager::Mouse_Scroll_Wheel_Callback);

	// tell GLFW to capture all mouse events
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// enable blending for supporting transparent rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_pWindow = window;

	return(window);
}

// JESSICA SOLER
/***********************************************************
 *  SetFrontView()
 *
 *  Set a level front view of the scene
 *  Keep the current speed, sensitivity, and field of view
 ***********************************************************/
void ViewManager::SetFrontView()
{
	if (NULL == g_pCamera)
	{
		return;
	}

	// Let the camera constructor set matching angles and directions
	Camera frontCamera(CAMERA_START_POSITION);

	// Copy the starting position and viewing direction
	g_pCamera->Position = frontCamera.Position;
	g_pCamera->Yaw = frontCamera.Yaw;
	g_pCamera->Pitch = frontCamera.Pitch;
	g_pCamera->Front = frontCamera.Front;
	g_pCamera->Right = frontCamera.Right;
	g_pCamera->Up = frontCamera.Up;
	g_pCamera->WorldUp = frontCamera.WorldUp;

	// Start fresh with the next mouse position
	gFirstMouse = true;
}

// JESSICA SOLER
/***********************************************************
 *  Mouse_Position_Callback()
 *
 *  Use mouse movement to look around
 *  Change the camera's yaw and pitch
 ***********************************************************/
void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos)
{
	// Convert the mouse coordinates to floats
	float xPosition = (float)xMousePos;
	float yPosition = (float)yMousePos;

	// Save the first position to avoid a sudden camera jump
	if (gFirstMouse)
	{
		gLastX = xPosition;
		gLastY = yPosition;
		gFirstMouse = false;
	}

	// Find how far the mouse moved
	float xOffset = xPosition - gLastX;
	float yOffset = gLastY - yPosition;

	// Save the position for the next mouse event
	gLastX = xPosition;
	gLastY = yPosition;

	// Update the viewing direction
	if (NULL != g_pCamera)
	{
		g_pCamera->ProcessMouseMovement(xOffset, yOffset);
	}
}

// JESSICA SOLER
/***********************************************************
 *  Mouse_Scroll_Wheel_Callback()
 *
 *  Change camera movement speed with the scroll wheel
 *  Scroll up slower and scroll down faster
 ***********************************************************/
void ViewManager::Mouse_Scroll_Wheel_Callback(GLFWwindow* window, double xOffset, double yOffset)
{
	if (NULL != g_pCamera)
	{
		g_pCamera->ProcessMouseScroll(-(float)yOffset);
	}
}

/***********************************************************
 *  ProcessKeyboardEvents()
 *
 *  This method is called to process any keyboard events
 *  that may be waiting in the event queue.
 ***********************************************************/
void ViewManager::ProcessKeyboardEvents()
{
	// close the window if the escape key has been pressed
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_pWindow, true);
	}

	// JESSICA SOLER
	if (NULL == g_pCamera)
	{
		return;
	}

	// Select perspective and keep the current camera view
	if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS)
	{
		bOrthographicProjection = false;
	}

	// Select orthographic on a new O key press
	// Keep the current camera position and direction
	bool oKeyPressed = (glfwGetKey(m_pWindow, GLFW_KEY_O) == GLFW_PRESS);

	if (oKeyPressed && !gOKeyWasPressed)
	{
		bOrthographicProjection = true;
	}

	gOKeyWasPressed = oKeyPressed;

	// Move forward and backward
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(FORWARD, gDeltaTime);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(BACKWARD, gDeltaTime);
	}

	// Move left and right
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(LEFT, gDeltaTime);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(RIGHT, gDeltaTime);
	}

	// Move up and down
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(UP, gDeltaTime);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(DOWN, gDeltaTime);
	}
}

// JESSICA SOLER
/***********************************************************
 *  PrepareSceneView()
 *
 *  Update camera movement and the console log
 *  Send the view and selected projection to the shader
 ***********************************************************/
void ViewManager::PrepareSceneView()
{
	glm::mat4 view;
	glm::mat4 projection;

	// per-frame timing
	// calculate time since previous frame
	float currentFrame = (float)glfwGetTime();
	gDeltaTime = currentFrame - gLastFrame;
	gLastFrame = currentFrame;

	// process any keyboard events that may be waiting in the
	// event queue
	ProcessKeyboardEvents();

	// JESSICA SOLER
	// Update the same console line four times per second
	static float lastStatusTime = 0.0f;

	if (currentFrame - lastStatusTime >= CAMERA_LOG_INTERVAL)
	{
		lastStatusTime = currentFrame;

		const float yawOffset = std::remainder(g_pCamera->Yaw - YAW, FULL_ROTATION_DEGREES);
		const float pitch = g_pCamera->Pitch;

		std::cout << "\r"
			<< (bOrthographicProjection ? "O" : "P")
			<< " | speed: "
			<< std::fixed << std::setprecision(1)
			<< g_pCamera->MovementSpeed
			<< " | yaw: "
			<< (yawOffset > 0.0f ? "Right " :
				yawOffset < 0.0f ? "Left " : "Center ")
			<< std::fabs(yawOffset)
			<< " | pitch: "
			<< (pitch > 0.0f ? "Up " :
				pitch < 0.0f ? "Down " : "Level ")
			<< std::fabs(pitch)
			<< " | xyz: "
			<< g_pCamera->Position.x << ","
			<< g_pCamera->Position.y << ","
			<< g_pCamera->Position.z << "         "
			<< std::flush;
	}

	// Use the same camera view for both projections
	view = g_pCamera->GetViewMatrix();

	if (bOrthographicProjection)
	{
		// Objects keep the same size at different distances
		projection = glm::ortho(
			-ORTHOGRAPHIC_HALF_WIDTH, ORTHOGRAPHIC_HALF_WIDTH,
			-ORTHOGRAPHIC_HALF_HEIGHT, ORTHOGRAPHIC_HALF_HEIGHT,
			NEAR_CLIP_DISTANCE, FAR_CLIP_DISTANCE);
	}
	else
	{
		// Objects farther away appear smaller
		projection = glm::perspective(
			glm::radians(g_pCamera->Zoom),
			(GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT,
			NEAR_CLIP_DISTANCE,
			FAR_CLIP_DISTANCE);
	}

	// send viewing info to shader
	if (NULL != m_pShaderManager)
	{
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ViewName, view);
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ProjectionName, projection);
		// set the view position of the camera into the shader for proper rendering
		m_pShaderManager->setVec3Value("viewPosition", g_pCamera->Position);
	}
}