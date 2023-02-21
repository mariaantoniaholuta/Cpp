//
//  main.cpp
//  OpenGL_Shader_Example_step1
//
//  Created by CGIS on 02/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC

#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "skybox.hpp"
#define TINYOBJLOADER_IMPLEMENTATION

#include "Model3D.hpp"
#include "Mesh.hpp"

int glWindowWidth = 840;
int glWindowHeight = 580;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

glm::mat4 model;
GLint modelLoc;

glm::mat4 view;
GLint viewLoc;

glm::mat4 view1;
GLint viewLoc1;

glm::vec3 camPosition = glm::vec3(20.0f, 18.0f, 80.0f);
glm::vec3 camTarget = glm::vec3(20.0f, 15.0f, 30.0f);
glm::vec3 camFront = glm::vec3(20.0f, 9.0f, 0.0f);

//gps::Camera myCamera(glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 2.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
gps::Camera myCamera(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));

const float cameraSpeed = 0.05f;
const float cameraSpeed1 = 0.007f;
const float cameraSpeedFast = 0.1f;

bool pressedKeys[1024];
float angle = 0.0f;

gps::Model3D myModel;
gps::Shader myCustomShader;
gps::Shader myCustomShaderLightv1;
gps::Shader myCustomShaderGolem;
gps::Shader myCustomShader_simple;
gps::Model3D myModel_iceIsland;
gps::Model3D myModel_wand;
gps::Model3D myModel_galaxyIsland;
gps::Model3D myModel_laser;
gps::Model3D myModel_moon;
gps::Model3D myModel_sun;
gps::Model3D myModel_alien;
gps::Model3D myModel_tea;
gps::Model3D myModel_ball;
gps::Model3D myModel_ball_ice;
gps::Model3D myModel_ball_fire;
gps::Model3D myModel_ball_galaxy;
gps::Model3D myModel_ball_wand;
gps::Model3D myModel_golem;
gps::Model3D myModel_moon_far;
gps::Model3D myModel_lightning;
gps::Model3D myModel_lightning_left;
gps::Model3D myModel_lightning_right;

gps::Model3D myModel_ball_1;
gps::Model3D myModel_snow_s;
gps::Model3D myModel_snow_b;


glm::mat4 projection;
GLuint projectionLoc;
GLuint projectionLoc1;


glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
GLuint normalMatrixLoc2;

glm::mat3 normalMatrixGolem;
GLuint normalMatrixLocGolem;

glm::vec3 lightDir;
GLuint lightDirLoc;

glm::vec3 lightDirGolem;
GLuint lightDirLocGolem;

glm::vec3 lightColor;
GLuint lightColorLoc;

glm::vec3 lightColorGolem;
GLuint lightColorLocGolem;

glm::vec3 ambient;
GLuint ambientLoc;

glm::vec3 baseColor_galaxy(0.1f, 0.2f, 1.0f); //blue-violet
glm::vec3 baseColor_fire(0.3f, 0.0f, 0.1f); //bloody red
glm::vec3 baseColor_blue_white(0.2f, 1.0f, 1.0f); //blue-white
glm::vec3 baseColor_ice(0.4f, 0.6f, 0.7f); //blue-ice
glm::vec3 baseColor_high(0.3f, 0.0f, 0.3f); //blue-red
glm::vec3 baseColor_white(1.0f, 1.0f, 1.0f); //white
//GLfloat ambientStrength = 0.9f;
//GLfloat ambientStrengthLoc;

GLuint baseColorLoc;
GLuint baseColorLoc2;
GLuint lightDirLoc2;
GLuint projectionLoc2;

GLuint modelLoc2;
GLuint viewLoc2;
GLuint viewLocGolem;
GLuint lightColorLoc2;
GLuint projectionLocGolem;
GLuint modelLocGolem;

GLfloat lightAngle;
glm::mat4 lightRotation;

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

int startPresentation = 0;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;
GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;
gps::Shader depthMapShader;
gps::Shader screenQuadShader;

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	myCustomShader.useShaderProgram();

	//set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	myCustomShaderLightv1.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glViewport(0, 0, retina_width, retina_height);

}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

float lastX = 400, lastY = 300;
int xpos_copy, ypos_copy;
int xpos, ypos;

static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << xpos << " : " << ypos << std::endl;
	xpos_copy = xpos;
	ypos_copy = ypos;

}

void cursorEnterCallback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		std::cout << "Entered Window" << std::endl;
	}
	else
	{
		std::cout << "Left window" << std::endl;
	}
}



void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << xpos << " : " << ypos << std::endl;
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "mouse button press" << std::endl;

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		std::cout << "mouse button release" << std::endl;
	}
}

float fov = 45.0;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << xoffset << " : " << yoffset << std::endl;
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	if (yoffset > 0)
		camPosition += (camTarget - camPosition) * cameraSpeed;
	else
		camPosition -= (camTarget - camPosition) * cameraSpeed;
}

glm::vec3 camPos(0.0f, 5.0f, 15.0f);
glm::vec3 camTar(0.0f, 2.0f, -10.0f);
//gps::Camera myCamera(camPos, camTar, glm::vec3(0.0f, 1.0f, 0.0f));
float angleR = 0, angleT = 0;
glm::vec3 camUp(0.0f, 1.0f, 0.0f);


void processMovement()
{
	const float cameraSpeed = 0.05f;

	if (pressedKeys[GLFW_KEY_Q]) {
		angle -= 0.05;

	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle += 0.05;

	}

	if (pressedKeys[GLFW_KEY_W]) {
		//myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		// 
		camPosition.z -= 0.5;
		camTarget.z -= 0.5;
		// 
		//const float cameraSpeed = 0.05f;
		//camPosition += (camTarget - camPosition) * cameraSpeed1;

	}

	if (pressedKeys[GLFW_KEY_S]) {
		//myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
		//camPosition.z += 0.7;
		//camTarget.z += 0.7;

		camPosition -= (camTarget - camPosition) * cameraSpeed1;
	}

	if (pressedKeys[GLFW_KEY_A]) {
		//myCamera.move(gps::MOVE_LEFT, cameraSpeed);
		//camPosition.x -= 0.4;
		//camTarget.x -= 0.4;

		camPosition -= glm::normalize(glm::cross((camTarget - camPosition), camUp)) * cameraSpeedFast;
	}

	if (pressedKeys[GLFW_KEY_D]) {
		//myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
		//camPosition.x += 0.4;
		//camTarget.x += 0.4;

		camPosition += glm::normalize(glm::cross((camTarget - camPosition), camUp)) * cameraSpeedFast;
	}

	if (pressedKeys[GLFW_KEY_UP]) {
		camPosition.y += 0.4;
		camTarget.y += 0.4;
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {
		camPosition.y -= 0.4;
		camTarget.y -= 0.4;
	}
	if (pressedKeys[GLFW_KEY_RIGHT]) {
		camPosition.x += 0.4;
		camTarget.x += 0.4;
	}
	if (pressedKeys[GLFW_KEY_LEFT]) {
		camPosition.x -= 0.4;
		camTarget.x -= 0.4;
	}


	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 2.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 2.0f;
	}

	if (pressedKeys[GLFW_KEY_B]) {
		startPresentation = 1;
	}
	if (pressedKeys[GLFW_KEY_V]) {
		startPresentation = 0;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_P)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_F)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//smooth
	if (glfwGetKey(glWindow, GLFW_KEY_M)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_SMOOTH);
		//myCustomShader.useShaderProgram();
		//shader.setBool(UNIFORM_POLYGONAL_VIEW, false);

	}
	if (glfwGetKey(glWindow, GLFW_KEY_N)) {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_POLYGON_SMOOTH);
		//myCustomShader.useShaderProgram();
		//shader.setBool(UNIFORM_POLYGONAL_VIEW, false);

	}
	if (glfwGetKey(glWindow, GLFW_KEY_X)) {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		//myCustomShader.useShaderProgram();
		//shader.setBool(UNIFORM_POLYGONAL_VIEW, false);

	}


}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

GLuint ReadTextureFromFile(const char* file_name) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}

	int width_in_bytes = x * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return textureID;
}

GLuint verticesVBO;
GLuint verticesEBO;
GLuint objectVAO;
GLint texture;


float delta = 0;
float deltaRight = 0;
float deltaUp = 0;
float deltaDown = 0;
float movementSpeed = 2; // units per second
float delta1 = 0;
int direction = 1;
float deltaAlien = 0.0;
float deltaD = 0.0;

float deltaUD = 0.0;

float delta_thunder = 0.0;
int direction_l = 1;
int direction_s = 1;
float deltaDown_snow = 0.0;
float deltaRight_snow = 0.0;
float deltaLeft_snow = 0.0;

void updateDeltaLeft(double elapsedSeconds) {
	delta = delta - movementSpeed * elapsedSeconds;
}

void updateDeltaRight(double elapsedSeconds) {
	deltaRight = deltaRight + movementSpeed * elapsedSeconds;
}

void updateDeltaUp(double elapsedSeconds) {
	deltaUp = deltaUp + movementSpeed * elapsedSeconds;
}

void updateDeltaDown(double elapsedSeconds) {
	deltaDown = deltaDown - movementSpeed * elapsedSeconds;
}

void updateDeltaUpDown(double elapsedSeconds, int direction) {

	if (direction == 1 && deltaUD < 5.1f)
	{
		//std::cout << deltaUD << " -up " << std::endl;
		deltaUD += movementSpeed * elapsedSeconds;
		if (deltaUD > 5.0f)
		{
			direction = 0;
		}
	}
	if (direction == 0 && deltaUD > -5.1f)
	{
		//std::cout << deltaUD << " -DOWN " << std::endl;
		deltaUD -= movementSpeed * elapsedSeconds;
		if (deltaUD < -5.0f)
		{
			direction = 1;
		}
	}
}

double lastTimeStamp = glfwGetTime();
double lastTimeStamp1 = glfwGetTime();
double lastTimeStamp2 = glfwGetTime();
double lastTimeStamp3 = glfwGetTime();
double lastTimeStamp4 = glfwGetTime();
double lastTimeStamp5 = glfwGetTime();
double lastTimeStamp6 = glfwGetTime();

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	const GLfloat near_plane = 1.0f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
	glm::mat4 lightView = glm::lookAt(lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

	return lightSpaceTrMatrix;
}

void rain(float x, float y, float z) {

	//deltaDown_snow -= 0.6;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));
	model = glm::translate(model, glm::vec3(deltaRight_snow, deltaDown_snow, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	myModel_snow_s.Draw(myCustomShader);
	model = glm::translate(model, glm::vec3(-x, -y, -z));
}


void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.7, 0.8, 2.8, 1.0);
	glViewport(0, 0, retina_width, retina_height);


	depthMapShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));


	//myCustomShader.useShaderProgram();
	//initialize the model matrix

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	myCustomShaderLightv1.useShaderProgram();


	//lightning thunder animation

	if (direction_l == 1)
	{
		delta_thunder += 10;
		if (delta_thunder > 500)
		{
			direction_l = 0;
		}
	}
	if (direction_l == 0)
	{
		delta_thunder -= 10;
		if (delta_thunder < 10)
		{
			direction_l = 1;
		}
	}

	if (delta_thunder == 200) {
		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_white));
		myModel_ball.Draw(myCustomShaderLightv1);
		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_white));
		myModel_lightning.Draw(myCustomShaderLightv1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		myModel_ball_ice.Draw(myCustomShaderLightv1);

		myModel_ball_fire.Draw(myCustomShaderLightv1);

		myModel_ball_galaxy.Draw(myCustomShaderLightv1);

		myModel_ball_wand.Draw(myCustomShaderLightv1);



	}
	else if (delta_thunder == 220) {
		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_white));
		myModel_ball.Draw(myCustomShaderLightv1);
		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_white));
		myModel_lightning_right.Draw(myCustomShaderLightv1);
		myModel_lightning_left.Draw(myCustomShaderLightv1);

		myModel_ball_ice.Draw(myCustomShaderLightv1);

		myModel_ball_fire.Draw(myCustomShaderLightv1);

		myModel_ball_galaxy.Draw(myCustomShaderLightv1);

		myModel_ball_wand.Draw(myCustomShaderLightv1);
	}
	else if (delta_thunder == 420) {
		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_white));
		myModel_ball.Draw(myCustomShaderLightv1);
		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_white));
		myModel_lightning_left.Draw(myCustomShaderLightv1);

		myModel_ball_ice.Draw(myCustomShaderLightv1);

		myModel_ball_fire.Draw(myCustomShaderLightv1);

		myModel_ball_galaxy.Draw(myCustomShaderLightv1);

		myModel_ball_wand.Draw(myCustomShaderLightv1);
	}
	else {

		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_high));
		myModel_ball.Draw(myCustomShaderLightv1);

		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_ice));
		myModel_ball_ice.Draw(myCustomShaderLightv1);

		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_fire));
		myModel_ball_fire.Draw(myCustomShaderLightv1);

		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_galaxy));
		myModel_ball_galaxy.Draw(myCustomShaderLightv1);

		baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
		glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_blue_white));
		myModel_ball_wand.Draw(myCustomShaderLightv1);
	}




	glDisable(GL_BLEND);
	//myCustomShaderGolem.useShaderProgram();
	myModel_golem.Draw(myCustomShader);

	//myCustomShader.useShaderProgram();
	myModel_wand.Draw(myCustomShader);
	myModel.Draw(myCustomShader);
	//myModel_tea.Draw(myCustomShaderLightv1);

	//ANIMATIONS

	//ice island animation
	// get current time
	double currentTimeStamp = glfwGetTime();
	//fprintf(stdout, "timew  %d\n",  currentTimeStamp);
	updateDeltaLeft(currentTimeStamp - lastTimeStamp);
	lastTimeStamp = currentTimeStamp;
	model = glm::translate(model, glm::vec3(delta, delta1, 0));


	//send matrix data to vertex shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	for (int i = 0; i < 1000; i++)
		myModel_iceIsland.Draw(myCustomShader);


	//galaxy island animation

	model = glm::mat4(1.0f);

	double currentTimeStamp1 = glfwGetTime();
	updateDeltaRight(currentTimeStamp1 - lastTimeStamp1);
	lastTimeStamp1 = currentTimeStamp1;

	model = glm::translate(model, glm::vec3(deltaRight, 0, 0));

	//model = glm::rotate(model, deltaRight, glm::vec3(1, 0, 0));
	//model = glm::translate(model, glm::vec3(-deltaRight, 0, 0));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	for (int i = 0; i < 500; i++)
		myModel_galaxyIsland.Draw(myCustomShader);

	//laser animation
	model = glm::mat4(1.0f);
	//double currentTimeStamp2 = glfwGetTime();
	//updateDeltaUp(currentTimeStamp2 - lastTimeStamp2);
	//lastTimeStamp2 = currentTimeStamp2;
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//myModel_laser.Draw(myCustomShader);


	//alien animation

	model = glm::mat4(1.0f);

	if (direction == 1)
	{
		//std::cout << deltaAlien << " -up " << std::endl;
		deltaAlien += 0.09;
		model = glm::translate(model, glm::vec3(0, deltaAlien, 0));
		if (deltaAlien > 2.0f)
		{
			direction = 0;
		}
	}
	if (direction == 0)
	{
		//std::cout << deltaAlien << " -DOWN " << std::endl;
		deltaAlien -= 0.09;
		model = glm::translate(model, glm::vec3(0, deltaAlien, 0));
		if (deltaAlien < -1.0f)
		{
			direction = 1;
		}
	}
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	myModel_alien.Draw(myCustomShader);



	//moon and sun animation
	model = glm::mat4(1.0f);
	delta1 += 0.009;
	model = glm::rotate(model, delta1, glm::vec3(0, 0.1, 1));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//myCustomShader_simple.useShaderProgram();
	lightAngle -= 2.0f;

	myModel_moon.Draw(myCustomShader);
	myModel_sun.Draw(myCustomShader);


	//LIGHT
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



	//rain
	if (direction_s == 1)
	{
		deltaRight_snow += 0.2;
		if (deltaRight_snow > 1.75)
		{
			direction_s = 0;
		}
	}
	if (direction_s == 0)
	{
		deltaRight_snow -= 0.2;
		if (deltaRight_snow < 0.5)
		{
			direction_s = 1;
		}
	}



	deltaDown_snow -= 0.2;

	for (int i = 0; i <= 50; i++) {
		float x = 1 + (rand() % 100);
		float y = 1 + (rand() % 100);
		float z = 1 + (rand() % 30);
		rain(x, y, -z);
		rain(x, 4 * y, -z);
		rain(-x, y, -z);
		rain(-x, 4 * y, -z);
	}
	


	mySkyBox.Draw(skyboxShader, view, projection);

}



void initUniforms()
{
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/simpleDepthMap.vert", "shaders/simpleDepthMap.frag");
	//SHADER ONE
	myCustomShader.loadShader("shaders/shaderStart1.vert", "shaders/shaderStart1.frag");
	myCustomShader.useShaderProgram();


	view = glm::lookAt(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//initialize the projection matrix -apropiere
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)glWindowWidth / (float)glWindowHeight, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	//set the light direction
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	baseColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "baseColor");
	glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_high));

	//ambientStrengthLoc = glGetUniformLocation(myCustomShader.shaderProgram, "ambientStrength");
	//glUniform1f(ambientStrengthLoc, ambientStrength);

	//bind the shadow map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));


	//SECOND SHADER FOR LIGHT
	myCustomShaderLightv1.loadShader("shaders/shaderStartLightv1.vert", "shaders/shaderStartLightv1.frag");
	myCustomShaderLightv1.useShaderProgram();
	//initUniforms();
	modelLoc2 = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));

	//view = glm::lookAt(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	viewLoc2 = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glm::mat4 projection2 = glm::perspective(glm::radians(fov), (float)glWindowWidth / (float)glWindowHeight, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc2 = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	baseColorLoc = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "baseColor");
	glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor_high));


	//SHADER GOLEM
	myCustomShaderGolem.loadShader("shaders/shaderGolem.vert", "shaders/shaderGolem.frag");
	myCustomShaderGolem.useShaderProgram();
	//initUniforms();
	modelLocGolem = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "model");
	glUniformMatrix4fv(modelLocGolem, 1, GL_FALSE, glm::value_ptr(model));

	//view = glm::lookAt(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	viewLocGolem = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "view");
	glUniformMatrix4fv(viewLocGolem, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrixGolem = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLocGolem = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "normalMatrixGolem");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrixGolem));

	glm::mat4 projectionGolem = glm::perspective(glm::radians(fov), (float)glWindowWidth / (float)glWindowHeight, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLocGolem = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "projection");
	glUniformMatrix4fv(projLocGolem, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction
	lightDirGolem = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLocGolem = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "lightDirGolem");
	glUniform3fv(lightDirLocGolem, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * lightDirGolem));

	//set light color
	lightColorGolem = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLocGolem = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "lightColorGolem");
	glUniform3fv(lightColorLocGolem, 1, glm::value_ptr(lightColorGolem));

	//baseColorLoc = glGetUniformLocation(myCustomShaderGolem.shaderProgram, "baseColor");
	//glUniform3fv(baseColorLoc, 1, glm::value_ptr(baseColor));
	//SIMPLE SHADER

}


void init_skybox()
{

	std::vector<const GLchar*> faces;

	faces.push_back("textures/skybox/cubemap/px.png");
	faces.push_back("textures/skybox/cubemap/nx.png");
	faces.push_back("textures/skybox/cubemap/py.png");
	faces.push_back("textures/skybox/cubemap/ny.png");
	faces.push_back("textures/skybox/cubemap/nz.png");
	faces.push_back("textures/skybox/cubemap/nz.png");



	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));


}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int cam_direction_right = 1;
int x_movement = 1;
int z_movement = 1;
int y_movement = 0;
int x_movement_1 = 0;

void startPres() {
	//std::cout <<"pos y: " << camPosition.y << std::endl;
			//std::cout << "pos x: "<< camPosition.x << std::endl;
			//std::cout << "pos z: " << camPosition.z << std::endl;
			//camPosition += (camTarget - camPosition) * cameraSpeed1;
	if (z_movement == 1) {
		camPosition.z -= 0.3;
		camTarget.z -= 0.3;
	}
	if (camPosition.z < 15 && camPosition.z > 7) {
		z_movement = 1;
		y_movement = 1;
	}
	if (camPosition.z < 7) {
		z_movement = 0;
		y_movement = 1;
	}
	if (y_movement == 1) {
		camPosition.y += 0.2;
		camTarget.y += 0.2;
	}
	if (camPosition.y >= 28) {
		y_movement = 0;
		x_movement_1 = 1;

	}
	if (x_movement_1 == 1) {
		camPosition.x -= 0.2;
		camTarget.x -= 0.2;
		camPosition -= glm::normalize(glm::cross((camTarget - camPosition), camUp)) * cameraSpeedFast;
		camPosition.z += 0.4;
		camTarget.z += 0.4;
	}
	if (camPosition.x < -20.26) {
		x_movement_1 = 0;
		x_movement = 1;
		y_movement = 0;
		z_movement = 1;

		camPosition = glm::vec3(20.0f, 18.0f, 80.0f);
		camTarget = glm::vec3(20.0f, 15.0f, 30.0f);
		camFront = glm::vec3(20.0f, 9.0f, 0.0f);
		startPresentation = 0;

	}
	if (cam_direction_right == 1 && x_movement == 1) {
		camPosition += glm::normalize(glm::cross((camTarget - camPosition), camUp)) * cameraSpeedFast;
	}
	else {
		if (x_movement == 1)
			camPosition -= glm::normalize(glm::cross((camTarget - camPosition), camUp)) * cameraSpeedFast;
	}
	if (camPosition.x > 27.66) {
		cam_direction_right = 0;
	}
	if (camPosition.x < 20) {
		cam_direction_right = 1;
		x_movement = 0;
	}
}

void loadModels() {
	myModel.LoadModel("objects\\all_without_golem.obj", "textures\\");

	myModel_ball.LoadModel("objects\\ballt4.obj", "textures\\");
	myModel_iceIsland.LoadModel("objects\\ice_island_moving_wizard.obj", "textures\\");
	myModel_wand.LoadModel("objects\\wand_golem_emit.obj", "textures\\");
	myModel_galaxyIsland.LoadModel("objects\\galaxy_island_moving_wizard.obj", "textures\\");

	myModel_moon.LoadModel("objects\\moon.obj", "textures\\");

	myModel_moon.LoadModel("objects\\sun.obj", "textures\\");

	myModel_alien.LoadModel("objects\\alien.obj", "textures\\");
	myModel_golem.LoadModel("objects\\golem_1.obj", "textures\\");

	myModel_ball_galaxy.LoadModel("objects\\ball_galaxy.obj", "textures\\");
	myModel_ball_ice.LoadModel("objects\\ball_ice.obj", "textures\\");
	myModel_ball_fire.LoadModel("objects\\ball_fire.obj", "textures\\");
	myModel_ball_wand.LoadModel("objects\\ball_wand.obj", "textures\\");
	myModel_lightning.LoadModel("objects\\lightning.obj", "textures\\");
	myModel_lightning_left.LoadModel("objects\\lightning_left.obj", "textures\\");
	myModel_lightning_right.LoadModel("objects\\lightning_right.obj", "textures\\");

	myModel_ball_1.LoadModel("objects\\ball_wand.obj", "textures\\");
	myModel_snow_s.LoadModel("objects\\snowflake_small_1.obj", "textures\\");

}


int main(int argc, const char* argv[]) {

	initOpenGLWindow();

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	//FOR TRANSP
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//glfwSetCursorEnterCallback(glWindow, cursorEnterCallback);
	glfwSetScrollCallback(glWindow, scrollCallback);
	glfwSetMouseButtonCallback(glWindow, mouseButtonCallBack);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);


	//argument 1 = obj file; argument 2 = textures folder
	//myModel.LoadModel("objects\\wizards_island_v2.obj", "textures\\");
	initFBO();
	loadModels();

	initUniforms();


	int screenWidth, screenHeight;
	glfwGetFramebufferSize(glWindow, &screenWidth, &screenHeight);

	if (!glWindow)
	{
		glfwTerminate();
		return -1;
	}

	init_skybox();


	while (!glfwWindowShouldClose(glWindow)) {
		

		model = glm::mat4(1.0f);
		myCustomShader.useShaderProgram();
		modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");

		if (startPresentation == 1) {
			startPres();
		}

		processMovement();
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
		view = glm::lookAt(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		GLint viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		model = glm::mat4(1.0f);
		myCustomShaderLightv1.useShaderProgram();

		modelLoc2 = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "model");

		

		processMovement();
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
		view = glm::lookAt(camPosition, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		GLint viewLoc2 = glGetUniformLocation(myCustomShaderLightv1.shaderProgram, "view");
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);

		glClear(GL_COLOR_BUFFER_BIT);


	}

	//close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
