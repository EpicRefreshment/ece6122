/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/7/2025
Date Last Modified: 10/17/2025

Description:

Lab 3

This is the main file for Lab 3. This is based on the tutorial09_ass_imp.cpp
originally found in the ogl tutorials repo. This file contains the code for initializing and 
drawing all objects (8 suzannas and 1 bright green rectangle). 
After determining initial position based on dimensions of suzanna,
it computes the orientation of all objects and camera by calling helper functions in controls.cpp.

*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

/*
This is the main function where all main OpenGL calls and object initialization and drawing takes place.
Closes application when program exit detected.

Arguments:
    N/A

Return Values:
    N/A
*/

int main( void )
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 09 - Loading with AssImp", NULL, NULL);
	if( window == NULL )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint suzannaVAO;
	glGenVertexArrays(1, &suzannaVAO);

	GLuint rectangleVAO;
	glGenVertexArrays(1, &rectangleVAO);

	// Create and compile our GLSL program from the shaders
	GLuint textureProgramID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(textureProgramID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(textureProgramID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(textureProgramID, "M");

	// Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(textureProgramID, "myTextureSampler");

	GLuint LightID = glGetUniformLocation(textureProgramID, "LightPosition_worldspace");
	GLuint lightSwitchID = glGetUniformLocation(textureProgramID, "lightSwitch");
	glUniform1i(lightSwitchID, 1); // initialize the light switch value to on in the shader

	// Create and compile our other shaders for simple colors for the rectangle.
	GLuint colorProgramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	GLuint colorMatrixID = glGetUniformLocation(colorProgramID, "MVP"); // Get the handle for this one too.

	// Read our .obj file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	bool res = loadAssImp("suzanne.obj", indices, indexed_vertices, indexed_uvs, indexed_normals);

	glBindVertexArray(suzannaVAO); // Bind suzanna VAO

	// Load it into a VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

	// Calculate dimensions of suzanna object so we can accurately place them.
	float minX = indexed_vertices[0].x;
	float maxX = indexed_vertices[0].x;
	float minY = indexed_vertices[0].y;
	float maxY = indexed_vertices[0].y;
	float minZ = indexed_vertices[0].z;
	float maxZ = indexed_vertices[0].z;
	for (const auto& vertex: indexed_vertices)
	{
		// get x boundaries
		if (vertex.x < minX)
		{
			minX = vertex.x;
		}
		else if (vertex.x > maxX)
		{
			maxX = vertex.x;
		}
		// get y boundaries
		if (vertex.y < minY)
		{
			minY = vertex.y;
		}
		else if (vertex.y > maxY)
		{
			maxY = vertex.y;
		}
		// get z boundaries
		if (vertex.z < minZ)
		{
			minZ = vertex.z;
		}
		else if (vertex.z > maxZ)
		{
			maxZ = vertex.z;
		}
	}
	float width = maxX - minX;
	float height = maxY - minY;
	float depth = maxZ - minZ;
	std::cout << "Width: " << width << " Height: " << height << " Depth: " << depth << std::endl;

	// initialize position variables based on octagonal dimensions
	const int numObjects = 8;
	float angleIncrement = 360.0f / numObjects;
	float posX = 0.0f;
	float posY = 0.0f;
	float radius = ((width/2.0f) / tan(radians(angleIncrement/2.0f))) - (width/8.0f); // radius so that objects fit in circle nicely

	// Create green rectangle
	// initialize array of 6 vertexes
	static const GLfloat rect_vert_buffer_data[] = 
	{ 
		-radius - (depth / 2.0f), -radius - (depth / 2.0f), 0.0f,
		 radius + (depth / 2.0f), -radius - (depth / 2.0f), 0.0f,
		 radius + (depth / 2.0f),  radius + (depth / 2.0f), 0.0f,
		-radius - (depth / 2.0f), -radius - (depth / 2.0f), 0.0f,
		 radius + (depth / 2.0f),  radius + (depth / 2.0f), 0.0f,
		-radius - (depth / 2.0f),  radius + (depth / 2.0f), 0.0f
	};
	// initialize array of colors for the 6 vertexes
	static const GLfloat rect_color_buffer_data[] = 
	{ 
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glBindVertexArray(rectangleVAO); // Bind the rectangle VAO
	// create vertex buffer for rectangle
	GLuint rect_vertexbuffer;
	glGenBuffers(1, &rect_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rect_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vert_buffer_data), rect_vert_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // attribute buffer: vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// create color buffer for rectangle
	GLuint rect_colorbuffer;
	glGenBuffers(1, &rect_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rect_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_color_buffer_data), rect_color_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // attribute buffer: colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// draw until program exit
	do{
		// ------------------- DO ONCE PER FRAME ---------------------
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) // If last printf() was more than 1sec ago
		{ 
			// printf and reset
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(textureProgramID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		// check if light switch toggled
		int lightSwitch = lightSwitchToggle();
		glUniform1i(lightSwitchID, lightSwitch);
		// Send our light position to the currently bound shader,
		glm::vec3 lightPos = glm::vec3(6.0f, 6.0f, 4.0f);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// ------------------- DRAW EACH SUSANNA OBJECT --------------------
		glBindVertexArray(suzannaVAO);
		for (int i = 0; i < numObjects; i++)
		{
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glm::mat4 ModelMatrix = glm::mat4(1.0);

			// Calculate position around octagon
			float angle = radians(angleIncrement * i);
			posX = radius * cos(angle + radians(90.0f));
			posY = radius * sin(angle + radians(90.0f));

			// translate current object to proper spot around the "octagon"
			// offset suzanna so the bottom barely grazes the z = 0 plane
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(posX, posY, ((height / 2.0f) + height/16.0f)));
			// rotate so that it is upright relative to z = 0 plane and faces outward
			ModelMatrix = glm::rotate(ModelMatrix, angle, glm::vec3(0.0f,0.0f,1.0f));
			ModelMatrix = glm::rotate(ModelMatrix, radians(90.0f), glm::vec3(1.0f,0.0f,0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));

			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

			// Draw the triangles !
			glDrawElements(
				GL_TRIANGLES,      // mode
				indices.size(),    // count
				GL_UNSIGNED_SHORT, // type
				(void*)0           // element array buffer offset
			);
		}

		// ------------------- DRAW RECTANGLE --------------------
		glDisable(GL_CULL_FACE); // Disable culling so we can see the rectangle from both sides

		// Use shader for solid color
		glUseProgram(colorProgramID);

		glBindVertexArray(rectangleVAO); // bind rectangle VAO

		// determine position
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(colorMatrixID, 1, GL_FALSE, &MVP[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6); // Draw!

		glEnable(GL_CULL_FACE); // Re-enable culling for next frame
		
		// ------------------- DO ONCE PER FRAME ---------------------

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(textureProgramID);
	glDeleteProgram(colorProgramID);
	glDeleteBuffers(1, &rect_vertexbuffer);
	glDeleteBuffers(1, &rect_colorbuffer);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &suzannaVAO);
	glDeleteVertexArrays(1, &rectangleVAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

