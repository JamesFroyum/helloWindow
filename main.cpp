#include <GLFW/glfw3.h>
#include <glad/glad.h> 
#include <iostream>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION // idk this is some weird shit i gotta do to make the image loader work ignore this bs
#include <stb/stb_image.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Shader shader);

//this is prolly bad, but i dont know of any other way to get the data from the call back function
unsigned int winWidth = 1600;
unsigned int winHeight = 1200;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float PI = 3.14159265359f;

Camera camera(winWidth, winHeight);

int main() {


	// -----------------------------------------------


	//  ## INIT STUFF ## //

	//initialize glfw, which makes the window for us
	glfwInit();
	//set opengl to version 3.3 and use core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//make window
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Hello Window", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	camera.setWindow(window);

	//initiallize glad, which handles os specific implimentation of OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ----------------------------------------------------------------


	// ## DEFINE DATA ## //

	//defining data to render
	//texture coords start with top left at 0,0, opposite y of vertex coords
	float vertices[] = {
	//positions          //colors                //tex coords
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};


	// ----------------------------------------------------------------------


	// ## SETUP RENDERING ## //

	glEnable(GL_DEPTH_TEST);

	Shader shader("C:/Users/james/source/repos/helloWindow/transform.vs", "C:/Users/james/source/repos/helloWindow/basic.fs");

	int texWidth;
	int texHeight;
	int nrChannels;
	unsigned char* data = stbi_load("C:/Users/james/source/repos/helloWindow/georgia.jpg", &texWidth, &texHeight, &nrChannels, 0);

	//activate the texture unit so we can use multiple textures if we feel like it
	glActiveTexture(GL_TEXTURE0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //cant use a mipmap filter bc it is large than our largest mipmap

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//free the image memory
	stbi_image_free(data);

	//texture 2 baby
	data = stbi_load("C:/Users/james/source/repos/helloWindow/awesomeface.png", &texWidth, &texHeight, &nrChannels, 0);

	glActiveTexture(GL_TEXTURE1);
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //cant use a mipmap filter bc it is large than our largest mipmap

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//free the image memory
	stbi_image_free(data);

	/*
	//create ebo to store the indices of our vertices, so we can reuse the vertices and save memory
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	*/

	//vertex buffer objects store the vertex data in gpu memory
	//vertex array objects stores things like our rendering program, and tells opengl how to use our vertex data and vbos
	//we initialize both here
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	//bind ebo and set data, we do this after binding the vao so that the ebo is stored in the vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/
	
	//instruct opengl how to read the buffer
	//position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color data
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//tex data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind the vbo, vao, and ebo, this is not necessary but is good practice
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//only needs to be done once, so can happen before render loop
	shader.use();
	shader.setInt("texture0", 0);
	shader.setInt("texture1", 1);
	shader.setFloat("MixValue", 0.5f);


	// ---------------------------------------------------------------------------------------


	// ## COORD TRANSFORMS ## //

	//positiions
	glm::vec3 positions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// ---------------------------------------------------------------------------------------


	// ## SETUP WINDOW ## //
	
	//pass the window parameters to opengl
	glViewport(0, 0, winWidth, winHeight);

	//"registering a callback function," these are functions called when something happens, like when the window is resized in this case
	//registering call back functions is done after creating the window, but before the render loop
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);
	
	//quick little declaration
	float currentFrame;

	// ---------------------------------------------------------------------------------------------

	// ## RENDER LOOP ## //
	
	//creating the render loop
	//rendering must be done in a while loop, otherwise only a single frame would be drawn before the window would be closed
	while (!glfwWindowShouldClose(window)) {
		
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//tell the camera class a new frame has started
		camera.newFrame();

		//process the current input
		processInput(window, shader);

		//actual rendering
		glClearColor(0.07f, 0.15f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//give our shader the camera matrix
		unsigned int cameraLoc = glGetUniformLocation(shader.ID, "camera");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera.getMatrix()));


		//draw our triangles :)
		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);

		for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);

			float angle = 20.0f * i + (40.0f + 4 * i) * (float)glfwGetTime();
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);

		//rendering works with two buffers, this swaps which one is being drawn to and which is being displayed
		glfwSwapBuffers(window);

		//checks if anything important has happened and will call the corresponding call back function
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

//runs when the window is resized, tells opengl the new parameters of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	winWidth = width;
	winHeight = height;
}

//process the input given during rendering
void processInput(GLFWwindow* window, Shader shader) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {glfwSetWindowShouldClose(window, true);}

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		shader.use(); 
		shader.setFloat("mixValue", shader.getFloat("mixValue") - 0.01f);
	}

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		shader.use();
		shader.setFloat("mixValue", shader.getFloat("mixValue") + 0.01f);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { camera.processKeyboard(SPRINT,    deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_W)          == GLFW_PRESS) { camera.processKeyboard(FORWARD,   deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_D)          == GLFW_PRESS) { camera.processKeyboard(RIGHT,     deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_S)          == GLFW_PRESS) { camera.processKeyboard(BACK,      deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_A)          == GLFW_PRESS) { camera.processKeyboard(LEFT,      deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_SPACE)      == GLFW_PRESS) { camera.processKeyboard(UP,        deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)   == GLFW_PRESS) { camera.processKeyboard(DOWN,      deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_UP)         == GLFW_PRESS) { camera.processKeyboard(ZOOM,      deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_DOWN)       == GLFW_PRESS) { camera.processKeyboard(UNZOOM,    deltaTime); }
	if (glfwGetKey(window, GLFW_KEY_TAB)        == GLFW_PRESS) { camera.processKeyboard(MOUSELOCK, deltaTime); }
}

void mouse_callback(GLFWwindow* window, double x, double y) {
	camera.processMouse(x, y);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processScroll(yoffset);
}