
/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/7/2025
Date Last Modified: 10/17/2025

Description:

Lab 3

This is a modified version of the controls.cpp file found from the ogl tutorials repo.
This file contains the code for computing the camera view and projection matrices
based on user input. The camera orbits around the origin where all objects are located and maintains 
its position such that it is always aimed at the origin. This all implements the light switch toggle feature

*/

// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f); 
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units/second

// flag for turning some lighting components on or off
int lightSwitch = 1; // 1 on, 0 off
int lKeyState = GLFW_RELEASE; // the user must press and release the L key to toggle the light switch

/*
This function computes the camera view and projection matrices which are used by main to
compute the orientation of all displayed objects. It handles rotation in each cardinal direction
as well as zooming in and out. The camera always points toward the origin where all objects are located.

Arguments:
    N/A

Return Values:
    N/A
*/
void computeMatricesFromInputs()
{

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Rotate right/left around the relative Y-axis of the camera viewpoint
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
        // Create a rotation matrix
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deltaTime * (speed / 2.0f), up);
        // Apply the rotation to the camera's position
        position = glm::vec3(rotation * glm::vec4(position, 1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -deltaTime * (speed / 2.0f), up);
        position = glm::vec3(rotation * glm::vec4(position, 1.0f));
    }

    glm::vec3 direction = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - position); // Direction from camera to origin
    glm::vec3 right = glm::normalize(glm::cross(direction, up)); // Direction to the right of the camera

	// Rotate up/down around the relative X-axis of the camera viewpoint
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -deltaTime * (speed / 2.0f), right);
        position = glm::vec3(rotation * glm::vec4(position, 1.0f));
		up = glm::vec3(rotation * glm::vec4(up, 0.0f)); // Rotate the up vector as well
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deltaTime * (speed / 2.0f), right);
        position = glm::vec3(rotation * glm::vec4(position, 1.0f));
		up = glm::vec3(rotation * glm::vec4(up, 0.0f)); // Rotate the up vector as well
    }

	// Move forward / zoom in
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}
	// Move backward / zoom out
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= direction * deltaTime * speed;
	}

	float FoV = initialFoV;

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,                    // Camera is here
								glm::vec3(0.0f, 0.0f, 0.0f), // Always looks at the origin
								up  // set camera relative to up vector
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

/*
This function implements the light switch toggle feature.
When the user presses and releases the L key, the light switch value is toggled between 0 and 1.

Arguments:
    N/A

Return Values:
    lightSwitch - the current state of the light switch (1 for on, 0 for off)
*/
int lightSwitchToggle()
{
	// Toggle the light switch value between 0 and 1
	int updateKeyState = glfwGetKey(window, GLFW_KEY_L);
	if (updateKeyState == GLFW_RELEASE && lKeyState == GLFW_PRESS)
	{
		// key was just released, toggle the light switch
		lightSwitch = 1 - lightSwitch;
	}

	lKeyState = updateKeyState; // update the key state for the next iteration

	return lightSwitch;
}