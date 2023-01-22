// OpenGL_lesTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//https://learnopengl.com/Getting-started/Hello-Window

#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <fstream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void loadFromFile(const char* url, char** target) {
	std::ifstream stream(url, std::ios::binary);
	stream.seekg(0, stream.end);
	int total = stream.tellg();
	*target = new char[total + 1];
	stream.seekg(0, stream.beg);
	stream.read(*target, total);
	(*target)[total] = '\0';
	stream.close();
}

unsigned int loadTexture(std::string url) {

	//generate and bind ID's
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//instellen texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//inladen
	int width, height, channels;
	unsigned char* data;
	data = stbi_load(url.c_str(), &width, &height, &channels, 0);
	if (data == nullptr) {
		std::cout << "error loading file" << url.c_str() << std::endl;
	}
	else {
		//naar openGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL!", nullptr,nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}
	glViewport(0, 0, 800, 600);
	float vertices[] = {
		-1.0f, -1.0f, 0.5f,        1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.5f,         0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 0.5f,          0.0f, 0.0f, 1.0f,
		 -1.0f, 1.0f, 0.5f,         1.0f, 1.0f, 1.0f,
	};

	int indices[] = {
		0,1,2,
		0,2,3
	};
	/// <summary>
	/// SETUP TRIANGLES
	/// </summary>

	//vertex array
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//Element buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	int stride = sizeof(float) * 6;
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)
		* 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/// <summary>
	/// SETUP SHADERS
	/// </summary>
	char* vertexShader;
	loadFromFile("vertexShader.shader", &vertexShader);

	char* fragmentShader;
	loadFromFile("fragShader.shader", &fragmentShader);

	unsigned int vert, frag;
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertexShader, nullptr);
	glCompileShader(vert);
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragmentShader, nullptr);
	glCompileShader(frag);
	
	// DEBUG SHADER-------------
	int success;
	char infoLog[512];
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//----------------------

	unsigned int myProgram = glCreateProgram();
	glAttachShader(myProgram, vert);
	glAttachShader(myProgram, frag);
	glLinkProgram(myProgram);
	glDeleteShader(vert);
	glDeleteShader(frag);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(myProgram);
	glUniform2f(glGetUniformLocation(myProgram, "iResolution"), 800.0f, 600.0f);

	//update Loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(glGetUniformLocation(myProgram, "iTime"), (float)glfwGetTime());
		// draw stuff
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


