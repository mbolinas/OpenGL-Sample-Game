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

GLuint programID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;



int play = 1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        if(play == 1){
            play = 0;
        }
        else{
            play = 1;
        }
    }
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 07 - Model Loading", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
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
    programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    ViewMatrixID = glGetUniformLocation(programID, "V");
    ModelMatrixID = glGetUniformLocation(programID, "M");
    
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    glm::vec3 lightPos = glm::vec3(15, -15, 0);

    
    
	// Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

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

    glm::mat4 ProjectionMatrix = glm::perspective(50.0f, (float) 1024 / 768, 0.1f, 35.0f);
    
    glm::vec3 keyframes[] = {
        glm::vec3(17, 7, 17),
        glm::vec3(15, 5, 15),
        glm::vec3(12, 3, 12),
        glm::vec3(7, 2, 7),
        glm::vec3(3, 2, 3),
        glm::vec3(2, 2, 2),
        glm::vec3(15, 2, 0),
        glm::vec3(15, 2, -15),
        glm::vec3(-15, 2, -15),
        glm::vec3(0, 4, -15),
        glm::vec3(0, 4, 2),
        glm::vec3(0, 4, -15),
        glm::vec3(-6, -4, 1),
        glm::vec3(-5, -2, -3),
        glm::vec3(-4, 2, -4),
        glm::vec3(-3, 5, -3),
        glm::vec3(-2, 6, -3),
        glm::vec3(0, 5, -4),
        glm::vec3(1, 3, -5),
        glm::vec3(2, 3, -6),
        glm::vec3(3, 1, -5),
        glm::vec3(5, 3, -4),
        glm::vec3(7, 0, 7),
        glm::vec3(7, 8, 7),
        glm::vec3(7, -8, 7),
        glm::vec3(7, -20, 7),
        glm::vec3(0, -20, 0),
        glm::vec3(15, -20, 15)
    };
    
    int steps = 50;    //50 discrete interpolation points between each keyframe
    
    std::vector<glm::vec3> path;
    
    // equation derived from: http://web.eecs.utk.edu/~smarz1/splines.html
    // direct image link: http://web.eecs.utk.edu/~smarz1/speqns/CatmullRomExpanded.png
    // it's easier, although less legible, than matrix form
    
    for(int i = 1; i < 25 - 2; i++){
        for(int j = 0; j < steps; j++){
            float u = ((float) j) / ((float) steps);
            glm::vec3 interpPoint;
            glm::vec3 P0 = keyframes[i-1];
            glm::vec3 P1 = keyframes[i];
            glm::vec3 P2 = keyframes[i+1];
            glm::vec3 P3 = keyframes[i+2];
            interpPoint = 2.0f * keyframes[i];
            interpPoint += u * (((-1.0f) * P0) + P2);
            interpPoint += (u * u) * ((2.0f * P0) - (5.0f * P1) + (4.0f * P2) - P3);
            interpPoint += (u * u * u) * (((-1.0f) * P0) + (3.0f * P1) - (3.0f * P2) + P3);
            interpPoint = interpPoint / 2.0f;
            
            path.push_back(interpPoint);
        }
    }
    
    int currentPath = 0;
    
    std::cout << path.size() << '\n';
    //std::cout << path[1500].x << ',' << path[1500].y << ',' << path[1500].z << '\n';
    
    int frameSkip = 0;
    
	do{
        

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		//computeMatricesFromInputs();
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		//glm::mat4 ViewMatrix = getViewMatrix();
        
        glm::mat4 ViewMatrix = glm::lookAt(path[currentPath], glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
        
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        
        

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
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
        
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        if(play == 1){
            if(frameSkip == 0){
                //std::cout << currentPath << '\n';
                //std::cout << path[currentPath].x << ',' << path[currentPath].y << ',' << path[currentPath].z << '\n';
                if(currentPath >= path.size() - 1){
                    currentPath = 0;
                }
                else{
                    currentPath++;
                }
            }
            //frameSkip no longer used, keeping it just in case
            //frameSkip = (frameSkip % 5) + 1;
            frameSkip = 0;
        }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

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

