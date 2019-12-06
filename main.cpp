// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>

GLuint programID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;

// for bounding box
GLuint box_vertexbuffer;
GLuint box_colorbuffer;
float box_width = 9.0;
float box_height = 5.0;
float box_depth = 7.0;

// for paddles
GLuint p1_vertexbuffer;
GLuint p2_vertexbuffer;
glm::vec3 p1;
glm::vec3 p2;


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT))
		printf("p key pressed");
}

//----------------------------------------------------------------------------

// corner "origin" is at (0, 0, 0) -- must translate to center

void init_box()
{
	// box geometry with corner at origin

	static const GLfloat box_vertex_buffer_data[] = {
		0.0f,
		0.0f,
		0.0f, // X axis
		box_width,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f, // Y axis
		0.0f,
		box_height,
		0.0f,
		0.0f,
		0.0f,
		0.0f, // Z axis
		0.0f,
		0.0f,
		box_depth,
		box_width,
		box_height,
		box_depth, // other edges
		0.0f,
		box_height,
		box_depth,
		box_width,
		box_height,
		0.0f,
		0.0f,
		box_height,
		0.0f,
		box_width,
		0.0f,
		box_depth,
		0.0f,
		0.0f,
		box_depth,
		box_width,
		box_height,
		box_depth,
		box_width,
		0.0f,
		box_depth,
		0.0f,
		box_height,
		box_depth,
		0.0f,
		0.0f,
		box_depth,
		box_width,
		box_height,
		0.0f,
		box_width,
		0.0f,
		0.0f,
		box_width,
		box_height,
		box_depth,
		box_width,
		box_height,
		0.0f,
		0.0f,
		box_height,
		box_depth,
		0.0f,
		box_height,
		0.0f,
		box_width,
		0.0f,
		box_depth,
		box_width,
		0.0f,
		0.0f,
	};

	glGenBuffers(1, &box_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, box_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertex_buffer_data), box_vertex_buffer_data, GL_STATIC_DRAW);

	// "axis" edges are colored, rest are gray

	static const GLfloat box_color_buffer_data[] = {
		1.0f,
		0.0f,
		0.0f, // X axis is red
		1.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f,
		0.0f, // Y axis is green
		0.0f,
		1.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f, // Z axis is blue
		0.0f,
		0.0f,
		1.0f,
		0.5f,
		0.5f,
		0.5f, // all other edges gray
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
		0.5f,
	};

	glGenBuffers(1, &box_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, box_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_color_buffer_data), box_color_buffer_data, GL_STATIC_DRAW);
}

void init_paddles()
{
	// box geometry with corner at origin

	static const GLfloat p1_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
	};

	glGenBuffers(1, &p1_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p1_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p1_vertex_buffer_data), p1_vertex_buffer_data, GL_STATIC_DRAW);

	// p2

		// box geometry with corner at origin

	static const GLfloat p2_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
	};

	glGenBuffers(1, &p2_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p2_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p2_vertex_buffer_data), p2_vertex_buffer_data, GL_STATIC_DRAW);
}

void draw_box(glm::mat4 MVP)
{
	// make this transform available to shaders

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1st attribute buffer : vertices

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, box_vertexbuffer);
	glVertexAttribPointer(0,		// attribute. 0 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	// 2nd attribute buffer : colors

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, box_colorbuffer);
	glVertexAttribPointer(1,		// attribute. 1 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	// Draw the box!

	glDrawArrays(GL_LINES, 0, 24);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void draw_p1(glm::mat4 MVP)
{
		// make this transform available to shaders

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1st attribute buffer : vertices

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, p1_vertexbuffer);
	glVertexAttribPointer(0,		// attribute. 0 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
}

void draw_p2(glm::mat4 MVP)
{
			// make this transform available to shaders

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1st attribute buffer : vertices

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, p2_vertexbuffer);
	glVertexAttribPointer(0,		// attribute. 0 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 07 - Model Loading", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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
	// Hide the mouse and enable unlimited mouvement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, key_callback);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	//glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(15, -15, 0);

	// Load the texture
	GLuint Texture = loadDDS("uvmap.dds");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("hydroflask.obj", vertices, uvs, normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glm::mat4 ModelMatrix = glm::mat4(1.0);

	//glm::mat4 ProjectionMatrix = glm::perspective(50.0f, (float) 1024 / 768, 0.1f, 35.0f);
	glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 40.0f);
	init_box();
	init_paddles();
	p1 = glm::vec3(1.0);
	p2 = glm::vec3(3.0);
	do
	{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs();
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//glm::mat4 ViewMatrix = getViewMatrix();

		glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(-6.0, 6.0, 8.0), glm::vec3(box_width / 2.0, box_height / 2.0, box_depth / 2.0), glm::vec3(0, 1, 0));

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		draw_box(MVP);
		draw_p1(MVP);
		draw_p2(MVP);

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,		  // attribute
			3,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,		  // attribute
			2,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
