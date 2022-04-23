#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stub_image.h"

float mixLevel = 0.2f;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

float recVerts[] = {
	//position			 //colors			//texture coords
	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  2.0f, 2.0f,  //top right
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,  //bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  //bottom left
	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 2.0f   //top left
};

//float recVerts[] = {
//	//position			 //colors			//texture coords
//	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.6f, 0.6f,  //top right
//	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.6f, 0.4f,  //bottom right
//	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.4f, 0.4f,  //bottom left
//	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.4f, 0.6f   //top left
//};

float twoTris[] = {
	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.1f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,

	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.1f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.9f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixLevel += 0.001f;
		if (mixLevel > 1.0f)
			mixLevel = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixLevel -= 0.001f;
		if (mixLevel < 0.0f)
			mixLevel = 0.0f;
	}
}

int main()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	// need to read a little more information in regards to glfw and how it handles windows, this code is just going based on the open gl tutorials
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(recVerts), recVerts, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(twoTris), twoTris, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recVerts), recVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader shaderProg1("Passthrough.vs", "texture.fs");
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture1;
	glGenTextures(1, &texture1);

	unsigned int texture2;
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture2);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	shaderProg1.use();
	shaderProg1.setInt("texture1", 0);
	shaderProg1.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//render code
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProg1.use();
		shaderProg1.setFloat("mixLevel", mixLevel);
		trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProg1.setMatrix4("transform", trans);
		//glUniformMatrix4fv(glGetUniformLocation(shaderProg1.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), 0.0f));
		shaderProg1.setMatrix4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(VAO2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap and event check
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}