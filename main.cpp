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
#include <common/text2D.hpp>

GLuint programID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;

//legal limit for blood pingpong content
float paddleSpeed = 0.1f;

const float WIN_HEIGHT = 700.0f;
const float WIN_WIDTH = 1200.0f;

// for bounding box
GLuint box_vertexbuffer;
GLuint box_colorbuffer;
float box_width = 9.0;
float box_height = 5.0;
float box_depth = 7.0;

//paddle proj and viewmatrices
glm::mat4 pMatrix = glm::perspective(45.0f, WIN_WIDTH / WIN_HEIGHT, 0.1f, 40.0f);
glm::mat4 vMatrix = glm::lookAt(glm::vec3(box_width / 2.0, box_height - 1.0, box_depth / 2.0 * 4.0), glm::vec3(box_width / 2.0, box_height / 2.0, box_depth / 2.0), glm::vec3(0, 1, 0));
//debugmatrix
//glm::mat4 vMatrix = glm::lookAt(glm::vec3(-box_width, box_height/2.0, box_depth / 2.0), glm::vec3(box_width / 2.0, box_height / 2.0, box_depth / 2.0), glm::vec3(0, 1, 0));

// for paddles
GLuint p1_vertexBuffer;
GLuint p1_uvBuffer;
glm::vec3 p1Pos = glm::vec3(0.0, box_height / 2.0, box_depth / 2.0);
std::vector<glm::vec3> p1_vertices;
std::vector<glm::vec2> p1_uvs;
std::vector<glm::vec3> p1_normals; // Won't be used at the moment.

GLuint p2_vertexBuffer;
GLuint p2_uvBuffer;
glm::vec3 p2Pos = glm::vec3(box_width, box_height / 2.0, box_depth / 2.0);
std::vector<glm::vec3> p2_vertices;
std::vector<glm::vec2> p2_uvs;
std::vector<glm::vec3> p2_normals; // Won't be used at the moment.


int p1Score = 0;
int p2Score = 0;

// for THE BALL
GLuint ball_vertexBuffer;
GLuint ball_uvBuffer;
glm::vec3 ballPos = glm::vec3(box_width / 2.0, box_height / 2.0, box_depth / 2.0); // start in middle
glm::vec3 ballVel = glm::vec3(0.04, 0.01, 0.01);
std::vector<glm::vec3> ball_vertices;
std::vector<glm::vec2> ball_uvs;
std::vector<glm::vec3> ball_normals; // Won't be used at the moment.


GLuint ballTexture;
GLuint paddleTexture;

char winText[256] = "";

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
}

void init_paddles()
{
	bool res = loadOBJ("paddle.obj", p1_vertices, p1_uvs, p1_normals);

	glGenBuffers(1, &p1_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p1_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, p1_vertices.size() * sizeof(glm::vec3), &p1_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &p1_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p1_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, p1_uvs.size() * sizeof(glm::vec2), &p1_uvs[0], GL_STATIC_DRAW);

	res = loadOBJ("paddle.obj", p2_vertices, p2_uvs, p2_normals);
	glGenBuffers(1, &p2_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p2_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, p2_vertices.size() * sizeof(glm::vec3), &p2_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &p2_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p2_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, p2_uvs.size() * sizeof(glm::vec2), &p2_uvs[0], GL_STATIC_DRAW);
}

void init_ball()
{
	bool res = loadOBJ("apple.obj", ball_vertices, ball_uvs, ball_normals);

	glGenBuffers(1, &ball_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ball_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, ball_vertices.size() * sizeof(glm::vec3), &ball_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ball_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ball_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, ball_uvs.size() * sizeof(glm::vec2), &ball_uvs[0], GL_STATIC_DRAW);
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

void draw_p1()
{
	glm::mat4 p1MVP = pMatrix * vMatrix * glm::scale(glm::translate(glm::mat4(1.0), p1Pos), glm::vec3(10.0));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &p1MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, p1_vertexBuffer);
	glVertexAttribPointer(0,		// attribute. 0 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, p1_vertices.size());

	glDisableVertexAttribArray(0);
}

void draw_p2()
{
	glm::mat4 p2MVP = pMatrix * vMatrix * glm::scale(glm::translate(glm::mat4(1.0), p2Pos), glm::vec3(10.0));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &p2MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, p2_vertexBuffer);
	glVertexAttribPointer(0,		// attribute. 0 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, p2_vertices.size());

	glDisableVertexAttribArray(0);
}

void draw_ball()
{
	glm::mat4 ballMVP = pMatrix * vMatrix * glm::scale(glm::translate(glm::mat4(1.0), ballPos), glm::vec3(1.0));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ballMVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ball_vertexBuffer);
	glVertexAttribPointer(0,		// attribute. 0 to match the layout in the shader.
						  3,		// size
						  GL_FLOAT, // type
						  GL_FALSE, // normalized?
						  0,		// stride
						  (void *)0 // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ball_uvBuffer);
	glVertexAttribPointer(
		1,		  // attribute
		2,		  // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0,		  // stride
		(void *)0 // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, ball_vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//clamps paddle positions.
void clampPositions()
{
	//p2
	if (p2Pos.y > box_height - 1.2)
		p2Pos.y = box_height - 1.2;
	if (p2Pos.y < 0.0)
		p2Pos.y = 0.0;
	if (p2Pos.z > box_depth - .5)
		p2Pos.z = box_depth - .5;
	if (p2Pos.z < .5)
		p2Pos.z = .5;
	//p1
	if (p1Pos.y > box_height - 1.2)
		p1Pos.y = box_height - 1.2;
	if (p1Pos.y < 0.0)
		p1Pos.y = 0.0;
	if (p1Pos.z > box_depth - .5)
		p1Pos.z = box_depth - .5;
	if (p1Pos.z < .5)
		p1Pos.z = .5;
}

void update_ball()
{
	//ball
	if (ballPos.x > box_width)
	{
		ballVel.x *= -1.05;
		//p2 check
		if (p2Pos.z > ballPos.z + 0.64 || p2Pos.z < ballPos.z - 0.64 || p2Pos.y > ballPos.y + 0.2 || p2Pos.y < ballPos.y - 1.4)
		{
			p1Score++;
		}
	}
	if (ballPos.x < 0)
	{
		ballVel.x *= -1.05;
		//p1check
		if (p1Pos.z > ballPos.z + 0.64 || p1Pos.z < ballPos.z - 0.64 || p1Pos.y > ballPos.y + 0.2 || p1Pos.y < ballPos.y - 1.4)
		{
			p2Score++;
		}
	}
	if (ballPos.y > box_height)
	{
		ballVel.y *= -1.0;
	}
	if (ballPos.y < 0)
	{
		ballVel.y *= -1.0;
	}
	if (ballPos.z > box_depth)
	{
		ballVel.z *= -1.0;
	}
	if (ballPos.z < 0)
	{
		ballVel.z *= -1.0;
	}
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
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "3D Pong Simulator 2019", NULL, NULL);
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

	// Set the mouse at the center of the screen
	glfwPollEvents();
	//glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
	ballTexture = loadDDS("apple.dds");
	paddleTexture = loadDDS("paddle.dds");

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

	init_box();
	init_paddles();
	init_ball();
	initText2D("font.dds");
	do
	{
		char text[256];
		sprintf(text, "  P1 score: %d             P2 Score: %d", p1Score, p2Score);

		if (p1Score >= 5)
		{
			sprintf(winText, "Player 1 won! Play another round!");
			p1Score = 0;
			p2Score = 0;
			ballVel = glm::vec3(0.04, 0.01, 0.01);
			ballPos = glm::vec3(box_width/2.0, box_height/2.0, box_depth/2.0);
		}
		else if (p2Score >= 5)
		{
			sprintf(winText, "Player 2 won! Play another round!");
			p1Score = 0;
			p2Score = 0;
			ballVel = glm::vec3(0.04, 0.01, 0.01);
			ballPos = glm::vec3(box_width/2.0, box_height/2.0, box_depth/2.0);
		}

		/*****************************************/
		/*****************************************/
		/*****************************************/
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			p2Pos.y += paddleSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			p2Pos.z -= paddleSpeed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			p2Pos.y -= paddleSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			p2Pos.z += paddleSpeed;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			p1Pos.y += paddleSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			p1Pos.z -= paddleSpeed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			p1Pos.y -= paddleSpeed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			p1Pos.z += paddleSpeed;

		//Clamp those bad boys
		clampPositions();

		/*****************************************/
		/*****************************************/
		/*****************************************/

		ballPos += ballVel;

		/*****************************************/
		/*****************************************/
		/*****************************************/

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		printText2D(text, 10, 30, 20);
		printText2D(winText, 50, 500, 20);
		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs();
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ProjectionMatrix = pMatrix;

		glm::mat4 ViewMatrix = vMatrix;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		draw_box(MVP);

		draw_p1();
		draw_p2();
		draw_ball();
		update_ball();

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
