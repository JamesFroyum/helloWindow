#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum KeyboardInput {
	SPRINT,
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ZOOM,
	UNZOOM,
	MOUSELOCK
};

class Camera {
	public: 
		void setFirstMouse();
		void setWindow(GLFWwindow*);
		void setSize(unsigned int, unsigned int);
		void newFrame();

		bool getMouseToggle();
		
		glm::mat4 getMatrix();

		void processKeyboard(KeyboardInput, float);
		void processMouse(double, double);
		void processScroll(double);

		Camera(unsigned int, unsigned int);

	private:
		bool firstMouse;
		bool mouseToggle;
		bool mouselockPress;
		bool lastMouselockPress;
		bool keyPress;

		unsigned int winWidth;
		unsigned int winHeight;

		const float pi          = 3.14159265359f;
		const float fovInitial  = 1.5f;
		const float fovMax      = 3.0f;
		const float zoomSpeed   = 1.0f;
		const float scrollSpeed = 0.05f;
		const float moveSpeed   = 5.0f;
		const float camNear     = 0.1f;
		const float camFar      = 100.0f; 
		const float lookSens    = 0.003f; 
		const float lookEps     = 0.03f;

		float fov;
		float yaw;
		float pitch;
		float lastMouseX;
		float lastMouseY;
		float keyPressSens;
		float sprintMult;

		const glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraRight;
		glm::vec3 cameraUp;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 cameraMatrix;

		GLFWwindow* window;

		void updateMatrices();
};

Camera::Camera(unsigned int _winWidth, unsigned int _winHeight) {
	winWidth  = _winWidth;
	winHeight = _winHeight;

	window = 0;

	firstMouse  = true;
	mouseToggle = true;
	mouselockPress = false;
	lastMouselockPress = false;

	fov   = fovInitial;
	yaw   = 0.0f;
	pitch = 0.0f;

	cameraPos   = glm::vec3(0.0f,  0.0f,  0.0f);
	cameraFront = glm::vec3(1.0f,  0.0f,  0.0f);
	cameraRight = glm::vec3(0.0f, -1.0f,  0.0f);
	cameraUp    = glm::vec3(0.0f,  0.0f,  1.0f);

	updateMatrices();
}

void Camera::setFirstMouse() {
	firstMouse = true;
}

void Camera::setWindow(GLFWwindow* _window) {
	window = _window;
}

void Camera::setSize(unsigned int _winWidth, unsigned int _winHeight) {
	winWidth = _winWidth;
	winHeight = _winHeight;
}

void Camera::newFrame() {
	keyPress = false;
	sprintMult = 1.0f;
	lastMouselockPress = mouselockPress;
	mouselockPress = false;
}

bool Camera::getMouseToggle() {
	return mouseToggle;
}

glm::mat4 Camera::getMatrix() {
	updateMatrices(); 
	return cameraMatrix;
}

void Camera::updateMatrices() {
	cameraFront = glm::vec3(cos(yaw) * cos(pitch), sin(yaw) * cos(pitch), sin(pitch));
	cameraRight = glm::normalize(glm::cross(cameraFront, up));
	cameraUp    = glm::cross(cameraRight, cameraFront);

	viewMatrix       = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
	projectionMatrix = glm::perspective(fov, (float)winWidth / (float)winHeight, camNear, camFar);
	cameraMatrix     = projectionMatrix * viewMatrix;
}

void Camera::processKeyboard(KeyboardInput input, float deltaTime) {
	keyPress = true;
	if (input == SPRINT)  { sprintMult = 3.0f; }
	if (input == FORWARD) { cameraPos += cameraFront * moveSpeed * sprintMult * deltaTime; }
	if (input == BACK)    { cameraPos -= cameraFront * moveSpeed * sprintMult * deltaTime; }
	if (input == RIGHT)   { cameraPos += cameraRight * moveSpeed * sprintMult * deltaTime; }
	if (input == LEFT)    { cameraPos -= cameraRight * moveSpeed * sprintMult * deltaTime; }
	if (input == UP)      { cameraPos += cameraUp    * moveSpeed * sprintMult * deltaTime; }
	if (input == DOWN)    { cameraPos -= cameraUp    * moveSpeed * sprintMult * deltaTime; }

	if (input == ZOOM)   { fov /= (1 + deltaTime * zoomSpeed); }
	if (input == UNZOOM) { fov *= (1 + deltaTime * zoomSpeed); }
	if (fov > fovMax) { fov = fovMax; }

	//only toggle the mouse on the first frame the toggle key is pressed
	if (input == MOUSELOCK) { 
		mouselockPress = true;
		if (!lastMouselockPress) {
			mouseToggle = !mouseToggle;
		}
	}

	if (mouseToggle) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstMouse = true;
	}
}

void Camera::processMouse(double x, double y) {
	//only do mouse controls when cursor is on window
	if (!mouseToggle) {
		x = winWidth - x;
		y = winHeight - y;

		if (firstMouse) {
			lastMouseX = (float)x;
			lastMouseY = (float)y;
			firstMouse = false;
		}

		float deltaX = (float)x - lastMouseX;
		float deltaY = (float)y - lastMouseY;

		lastMouseX = (float)x;
		lastMouseY = (float)y;

		//for some reason, the sensitivity drops when a key is being pressed so need it increase it again to compensate
		if (keyPress) { keyPressSens = 2.0f; }
		else          { keyPressSens = 1.0f; }

		//fov terms reduce sensitivity when zoomed in to make control easier
		yaw   += deltaX * lookSens * keyPressSens * (fov / fovInitial);
		pitch += deltaY * lookSens * keyPressSens * (fov / fovInitial);
		
		if      (pitch > (( pi / 2.0f) - lookEps)) { pitch = ( pi / 2.0f) - lookEps; }
		else if (pitch < ((-pi / 2.0f) + lookEps)) { pitch = (-pi / 2.0f) + lookEps; }
	}
}

void Camera::processScroll(double y) {
	fov *= 1 + (float)y * scrollSpeed * zoomSpeed;
	if (fov > fovMax) { fov = fovMax; }
}

#endif