#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "main.hpp"
#include "object.hpp"
#include "shader.hpp"

GLFWwindow* window;

using namespace std;

glm::vec3 cameraPos = glm::vec3(4,3,-3);
glm::vec3 lightPos = glm::vec3(0,0,3);
float rotateDeg = 0.0;
glm::vec3 rotateAxis(0,1,0);

bool running = true;

const string testString (
	"v 1 2 3\n"
	"v 3 2 1\n"
	"v 2 1 3\n");

int main( void )
{

	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	bool res = loadOBJFile("assets/models/monkey.obj", vertices, normals);
	cout << "Vertices: " << vertices.size() << endl;

	// Initialise GLFW
	if( !glfwInit() )
	{
		cout << "Failed to initialize GLFW" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 640, 480, "Test", NULL, NULL);
	if( window == NULL ){
		cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	GLuint programID = LoadShaders( "assets/shaders/SimpleVertexShader.vert", "assets/shaders/SimpleFragmentShader.frag" );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint vertexbuffer;
	glGenBuffers(1,&vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(glm::vec3),&vertices[0],GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1,&normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalbuffer);
	glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(glm::vec3),&normals[0],GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint mvpID = glGetUniformLocation(programID,"mvp");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)640/(float)480,0.1f,100.0f);
	glm::mat4 view = glm::lookAt(
		cameraPos,	// Camera location.
		glm::vec3(0,0,0),	// Looking at origin.
		glm::vec3(0,1,0)	// Up vector.
	);
	glm::mat4 mvp = projection * view;

	GLuint viewID = glGetUniformLocation(programID,"v");
	GLuint modelID = glGetUniformLocation(programID,"m");

	GLuint lightPosID = glGetUniformLocation(programID,"lightPos_worldspace");
	glUniformMatrix4fv(lightPosID, 1, GL_FALSE, &lightPos[0]);

	//GLuint programID = LoadShaders( "SimpleVertexShader.vert","SimpleFragmentShader.frag");
	cout << "test..." << endl;
	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram(programID);

		glm::mat4 view = glm::lookAt(
			cameraPos,	// Camera location.
			glm::vec3(0,0,0),	// Looking at origin.
			glm::vec3(0,1,0)	// Up vector.
		);
		glm::mat4 model = glm::rotate(rotateDeg,rotateAxis);
		glm::mat4 mvp = projection * view * model;

		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glDrawArrays(GL_TRIANGLES,0,vertices.size());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( running && !glfwWindowShouldClose(window));

	glDeleteBuffers(1,&vertexbuffer);
	glDeleteVertexArrays(1,&vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		if (key == GLFW_KEY_Q) {
			cameraPos[1] += moveAmt;
		}
		else if (key == GLFW_KEY_W) {
			cameraPos[2] += moveAmt;
		}
		else if (key == GLFW_KEY_A) {
			cameraPos[0] += moveAmt;
		}
		else if (key == GLFW_KEY_S) {
			cameraPos[2] -= moveAmt;
		}
		else if (key == GLFW_KEY_E) {
			cameraPos[1] -= moveAmt;
		}
		else if (key == GLFW_KEY_D) {
			cameraPos[0] -= moveAmt;
		}
		else if (key == GLFW_KEY_LEFT) {
			rotateDeg -= rotAmt;
		}
		else if (key == GLFW_KEY_RIGHT) {
			rotateDeg += rotAmt;
		}
		else if (key == GLFW_KEY_ESCAPE) {
			running = false;
		}
	}
}
