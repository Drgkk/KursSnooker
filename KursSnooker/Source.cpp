#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "SnookerImport.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	Window* window = new Window(800, 600, "Snooker");
	GLADWrapper* gladWrapper = new GLADWrapper();
	gladWrapper->Initialize();
	GameEvents* gameEvents = new GameEvents();
	gameEvents->AddEvent(new GameEventWindowResized(window));

	Game snookerGame(window);

	Shader* vertexShader = new ShaderVertex(vertexShaderSource);
	vertexShader->Compile();
	Shader* fragmentShader = new ShaderFragment(fragmentShaderSource);
	fragmentShader->Compile();

	Shaders* programShaders = new Shaders();
	programShaders->AddShader(vertexShader);
	programShaders->AddShader(fragmentShader);
	ShaderProgram* shaderProgram = new ShaderProgram(programShaders);



	//float vertices[] = {
	//		0.5f, 0.5f, 0.0f,
	//		0.5f, -0.5f, 0.0f,
	//		-0.5f, -0.5f, 0.0f,
	//		-0.5f, 0.5f, 0.0f
	//};

	//unsigned int indices[] = {
	//	0, 1, 2,
	//	2, 3, 0
	//};

	//unsigned int VAO, VBO, EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);


	////LOOP
	//while (!glfwWindowShouldClose(window)) {
	//	processInput(window);

	//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT);



	//	glUseProgram(shaderProgram);
	//	glBindVertexArray(VAO);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

	//glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}