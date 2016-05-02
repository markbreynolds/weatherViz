#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <queue>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "main.hpp"
#include "vboIndexer.hpp"
#include "objects/object.hpp"
#include "objects/windmill.hpp"
#include "assets/objLoader.hpp"
#include "assets/image.hpp"
#include "assets/material.hpp"
#include "shader.hpp"
#include "console.hpp"

GLFWwindow* window;

using namespace std;

glm::vec3 cameraPos = glm::vec3(0,1,5);
glm::vec3 lookAtDir = glm::vec3(0,0,1);
glm::vec3 lightPos = glm::vec3(0,10,0);
glm::vec3 rotation(0,0,0);

glm::vec2* dragLast = nullptr;

//For measuring speed.
double lastTime = glfwGetTime();
unsigned short numFrames = 0;

unsigned short wind = 0;		// Windspeed in miles per hour

map<const string, float> displayText;

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
	window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Test", NULL, NULL);
	if( window == NULL ){
		cout << "Failed to open GLFW window. If you have an Intel GPU, it is not 3.3 compatible." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	cout << "Loading Sky..." << endl;
	Object* sky;
	{
		vector<glm::vec3> verticesOrig;
		vector<glm::vec2> uvsOrig;
		vector<glm::vec3> normalsOrig;
		queue<unsigned short> groupIndex;

		Material* material;

		bool res = loadOBJFile("assets/models/Sky.obj", verticesOrig, uvsOrig, normalsOrig, material, groupIndex);

		vector<unsigned short> indices;
		vector<glm::vec3> vertices;
		vector<glm::vec2> uvs;
		vector<glm::vec3> normals;
		createVBOIndex(verticesOrig,uvsOrig,normalsOrig,indices,vertices,uvs,normals);

		material->setShaderType(SkyShader);
		sky = new Object(vertices,uvs,normals,indices,material);
	}

	cout << "Loading Ground..." << endl;
	Object* ground;
	{
		vector<glm::vec3> verticesOrig;
		vector<glm::vec2> uvsOrig;
		vector<glm::vec3> normalsOrig;
		queue<unsigned short> groupIndex;

		Material* material;

		bool res = loadOBJFile("assets/models/Ground.obj", verticesOrig, uvsOrig, normalsOrig, material, groupIndex);

		vector<unsigned short> indices;
		vector<glm::vec3> vertices;
		vector<glm::vec2> uvs;
		vector<glm::vec3> normals;
		createVBOIndex(verticesOrig,uvsOrig,normalsOrig,indices,vertices,uvs,normals);

		ground = new Object(vertices,uvs,normals,indices,material);
	}

	//vector<Object*> objects;
	Windmill* windmill;

	cout << "Loading Windmill..." << endl;
	{
		vector<glm::vec3> verticesOrig;
		vector<glm::vec2> uvsOrig;
		vector<glm::vec3> normalsOrig;
		queue<unsigned short> groupIndex;

		Material* material;
		bool res = loadOBJFile("assets/models/Windmill.obj", verticesOrig, uvsOrig, normalsOrig, material, groupIndex);

		vector<unsigned short> indices;
		vector<glm::vec3> vertices;
		vector<glm::vec2> uvs;
		vector<glm::vec3> normals;
		vector<unsigned short> indexedGroup;
		createVBOIndex(verticesOrig,uvsOrig,normalsOrig,groupIndex,indices,indexedGroup,vertices,uvs,normals);

		windmill = new Windmill(vertices,uvs,normals,indices,material,indexedGroup);
		windmill->translate(glm::vec3(6,0,6));
		windmill->rotate(glm::vec3(0,2,0));
	}

	cout << "Loading Shaders..." << endl;
	GLuint solidProgramID = LoadShaders("assets/shaders/SolidShader.vert", "assets/shaders/SolidShader.frag");
	GLuint textureProgramID = LoadShaders("assets/shaders/TextureShader.vert", "assets/shaders/TextureShader.frag");
	GLuint skyProgramID = LoadShaders("assets/shaders/SkyShader.vert", "assets/shaders/SkyShader.frag");

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	glm::mat4 projection = glm::perspective(glm::radians(FOV),(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,100.0f);

	cout << "Done loading!" << endl << endl << endl;

	do{
		displayText["Wind Speed (m.p.h.): "] = wind;
		if (wind < 10) {
			windmill->setSpinSpeed(0);
		}
		else if (wind > 60) {
			windmill->setSpinSpeed(.16);
		}
		else {
			float speed = (wind-10.0)/50.0 * .16;
			cout << speed;
			cout.flush();
			windmill->setSpinSpeed(speed);
		}

		showTimePerFrame();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::mat4 view = glm::lookAt(
			cameraPos,	// Camera location.
			cameraPos+lookAtDir,	// Looking direction
			glm::vec3(0,1,0)	// Up vector.
		);

		//Sky->setPosition(cameraPos);
		glUseProgram(skyProgramID);
		sky->draw(skyProgramID, view, projection);

		glUseProgram(solidProgramID);
		GLuint lightPosID = glGetUniformLocation(solidProgramID,"lightPos");
		glUniform3f(lightPosID,lightPos.x,lightPos.y,lightPos.z);
		ground->draw(solidProgramID, view, projection);

		glUseProgram(textureProgramID);
		glUniform3f(lightPosID,lightPos.x,lightPos.y,lightPos.z);
		windmill->update(0.0);
		windmill->draw(textureProgramID, view, projection);

		/*for (int i = 0; i < objects.size(); i++) {
			objects[i]->update(0.0);
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
		}*/

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		printLineVals(displayText);

	} // Check if the ESC key was pressed or the window was closed
	while( running && !glfwWindowShouldClose(window));

	delete sky;
	delete ground;
	delete windmill;
	/*for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}*/
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
			cameraPos.x += lookAtDir.x;
			cameraPos.z += lookAtDir.z;
		}
		else if (key == GLFW_KEY_A) {
			cameraPos.x += lookAtDir.z;
			cameraPos.z -= lookAtDir.x;
		}
		else if (key == GLFW_KEY_S) {
			cameraPos.x -= lookAtDir.x;
			cameraPos.z -= lookAtDir.z;
		}
		else if (key == GLFW_KEY_E) {
			cameraPos[1] -= moveAmt;
		}
		else if (key == GLFW_KEY_D) {
			cameraPos.x -= lookAtDir.z;
			cameraPos.z += lookAtDir.x;
		}
		else if (key == GLFW_KEY_O) {
			wind += 1;
		}
		else if (key == GLFW_KEY_L) {
			wind -= 1;
		}
		/*else if (key == GLFW_KEY_LEFT) {
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
		}*/
		else if (key == GLFW_KEY_ESCAPE) {
			running = false;
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_RELEASE) {
		delete dragLast;
		dragLast = nullptr;
		return;
	}
	else {
		if (dragLast == nullptr){
			dragLast = new glm::vec2(xpos,ypos);
		}
		else {
			double xdist = xpos - dragLast->x;
			double ydist = dragLast->y - ypos;

			displayText["xDist: "] = xdist;
			displayText["yDist: "] = ydist;

			glm::quat rot = glm::quat(glm::vec3(glm::radians(ydist*(FOV/SCREEN_WIDTH))*2,glm::radians(xdist*(FOV/SCREEN_WIDTH))*2,0));
			lookAtDir = glm::vec3(glm::mat4_cast(rot) * glm::vec4(lookAtDir,1));

			displayText["lookX: "] = lookAtDir.x;
			displayText["lookY: "] = lookAtDir.y;
			displayText["lookZ: "] = lookAtDir.z;

			dragLast = new glm::vec2(xpos,ypos);
		}
	}
}

void showTimePerFrame(){
	double currTime = glfwGetTime();
	numFrames++;
	if (currTime - lastTime >= 1.0) {
		displayText["ms/frame = "] = 1000.0/double(numFrames);
		numFrames = 0;
		lastTime += 1.0;
	}
}
