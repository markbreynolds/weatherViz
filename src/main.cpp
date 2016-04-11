#include <iostream>
#include <vector>
#include <climits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "main.hpp"
#include "assets/objLoader.hpp"
#include "assets/image.hpp"
#include "assets/material.hpp"
#include "shader.hpp"

GLFWwindow* window;

using namespace std;

glm::vec3 cameraPos = glm::vec3(0,1,5);
glm::vec3 lightPos = glm::vec3(3,4,3);
float rotateDeg = 0.0;
glm::vec3 rotateAxis(0,1,0);

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

	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;

	//Is there a better way to do this?
	Material* materialPtr = nullptr;
	Material** materialPtrPtr = &materialPtr;

	bool res = loadOBJFile("assets/models/cube.obj", vertices, uvs, normals, materialPtrPtr);
	cout << vertices.size() << " vertices loaded." << endl;
	cout << uvs.size() << " UVs loaded." << endl;
	cout << normals.size()  << " normals loaded." << endl << endl;

	Material* material = *materialPtrPtr;

	GLuint solidProgramID = LoadShaders("assets/shaders/SolidShader.vert", "assets/shaders/SolidShader.frag");
	GLuint textureProgramID = LoadShaders("assets/shaders/TextureShader.vert", "assets/shaders/TextureShader.frag");

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint vertexBuffer;
	glGenBuffers(1,&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(glm::vec3),&vertices[0],GL_STATIC_DRAW);

	GLuint uvBuffer;
	glGenBuffers(1,&uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glBufferData(GL_ARRAY_BUFFER,uvs.size() * sizeof(glm::vec3),&uvs[0],GL_STATIC_DRAW);

	GLuint normalBuffer;
	glGenBuffers(1,&normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(glm::vec3),&normals[0],GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	//GLuint Texture = pngTexture("assets/textures/testUV.png");
	//GLuint* texturePtr = &Texture;
	//GLuint texture = load();

	do{
		showTimePerFrame();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		GLuint programID;
		if (material->getShaderType() == TextureShader) {
			programID = textureProgramID;
			glUseProgram(programID);

			GLuint textureID = glGetUniformLocation(programID, "textureSampler");
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->getDiffuseTexture());
			glUniform1i(textureID, 0);
		}
		else {
			programID = solidProgramID;
			glUseProgram(programID);

			GLuint matColorID = glGetUniformLocation(programID, "materialColor");
			glm::vec3 matColor = material->getDiffuseColor();
			glUniform3f(matColorID,matColor.r,matColor.g,matColor.b);
		}	 //*/

		GLuint mvpID = glGetUniformLocation(programID,"mvp");
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)640/(float)480,0.1f,100.0f);

		GLuint modelID = glGetUniformLocation(programID,"m");
		GLuint viewID = glGetUniformLocation(programID,"v");

		GLuint lightPosID = glGetUniformLocation(programID,"lightPos");		//*/

		glm::mat4 view = glm::lookAt(
			cameraPos,	// Camera location.
			glm::vec3(0,0,0),	// Looking at origin.
			glm::vec3(0,1,0)	// Up vector.
		);
		glm::mat4 model = glm::rotate(rotateDeg,rotateAxis);
		glm::mat4 mvp = projection * view * model;

		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
		glUniform3f(lightPosID,lightPos.x,lightPos.y,lightPos.z);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
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

	glDeleteBuffers(1,&vertexBuffer);
	glDeleteBuffers(1,&normalBuffer);
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

void showTimePerFrame(){
	double currTime = glfwGetTime();
	numFrames++;
	if (currTime - lastTime >= 5.0) {
		cout << 5000.0/double(numFrames) << " ms/frame" << endl;
		numFrames = 0;
		lastTime += 5.0;
	}
}
