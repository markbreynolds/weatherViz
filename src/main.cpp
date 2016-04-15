#include <iostream>
#include <vector>
#include <climits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "main.hpp"
#include "vboIndexer.hpp"
#include "object.hpp"
#include "assets/objLoader.hpp"
#include "assets/image.hpp"
#include "assets/material.hpp"
#include "shader.hpp"

GLFWwindow* window;

using namespace std;

glm::vec3 cameraPos = glm::vec3(0,1,5);
glm::vec3 lightPos = glm::vec3(3,4,3);
glm::vec3 rotation(0,0,0);

//For measuring speed.
double lastTime = glfwGetTime();
unsigned short numFrames = 0;

bool running = true;

const string testString (
	"v 1 2 3\n"
	"v 3 2 1\n"
	"v 2 1 3\n");

GLuint load();

int main( void )
{

	/*cout << "USHRT_MAX: " << USHRT_MAX << endl;
	cout << "UINT_MAX: " << UINT_MAX << endl; //*/

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
		cout << "Failed to open GLFW window. If you have an Intel GPU, it is not 3.3 compatible." << endl;
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

	vector<glm::vec3> verticesOrig;
	vector<glm::vec2> uvsOrig;
	vector<glm::vec3> normalsOrig;

	Material* material;

	bool res = loadOBJFile("assets/models/monkey2.obj", verticesOrig, uvsOrig, normalsOrig, material);
	cout << verticesOrig.size() << " vertices loaded." << endl;
	cout << uvsOrig.size() << " UVs loaded." << endl;
	cout << normalsOrig.size()  << " normals loaded." << endl << endl;

	vector<unsigned short> indices;
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	createVBOIndex(verticesOrig,uvsOrig,normalsOrig,indices,vertices,uvs,normals);
	cout << "Reduced to: " << vertices.size() << " vertices." << endl;

	vector<Object*> objects;
	objects.push_back(new Object(vertices,uvs,normals,indices,material));
	objects[0]->translate(glm::vec3(-3,1,-3));
	objects[0]->rotate(glm::vec3(0,0,3.14));
	objects.push_back(new Object(vertices,uvs,normals,indices,material));
	objects[1]->translate(glm::vec3(3,1,-3));
	objects[1]->rotate(glm::vec3(0,0,3.14));
	objects.push_back(new Object(vertices,uvs,normals,indices,material));

	GLuint solidProgramID = LoadShaders("assets/shaders/SolidShader.vert", "assets/shaders/SolidShader.frag");
	GLuint textureProgramID = LoadShaders("assets/shaders/TextureShader.vert", "assets/shaders/TextureShader.frag");

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)640/(float)480,0.1f,100.0f);

	do{
		showTimePerFrame();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::mat4 view = glm::lookAt(
			cameraPos,	// Camera location.
			glm::vec3(0,0,0),	// Looking at origin.
			glm::vec3(0,1,0)	// Up vector.
		);

		for (int i = 0; i < objects.size(); i++) {
			objects[i]->rotate(rotation);
			if (objects[i]->getShaderType() == TextureShader) {
				glUseProgram(textureProgramID);
				GLuint lightPosID = glGetUniformLocation(textureProgramID,"lightPos");
				glUniform3f(lightPosID,lightPos.x,lightPos.y,lightPos.z);
				objects[i]->draw(textureProgramID, view, projection);
			}
			else {
				glUseProgram(solidProgramID);
				GLuint lightPosID = glGetUniformLocation(solidProgramID,"lightPos");
				glUniform3f(lightPosID,lightPos.x,lightPos.y,lightPos.z);
				objects[i]->draw(solidProgramID, view, projection);
			}
		}
		rotation = glm::vec3(0,0,0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( running && !glfwWindowShouldClose(window));

	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	glDeleteVertexArrays(1,&vao);
	glDeleteProgram(solidProgramID);
	glDeleteProgram(textureProgramID);

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
			rotation += glm::vec3(0,-rotAmt,0);
		}
		else if (key == GLFW_KEY_RIGHT) {
			rotation += glm::vec3(0,rotAmt,0);
		}
		else if (key == GLFW_KEY_UP) {
			rotation += glm::vec3(-rotAmt,0,0);
		}
		else if (key == GLFW_KEY_DOWN) {
			rotation += glm::vec3(rotAmt,0,0);
		}
		else if (key == GLFW_KEY_ESCAPE) {
			running = false;
		}
	}
}

void showTimePerFrame(){
	double currTime = glfwGetTime();
	numFrames++;
	if (currTime - lastTime >= 5.0) {
		cout << 5000.0/double(numFrames) << " ms/frame" << endl;
		numFrames = 0;
		lastTime += 5.0;
	}
}
