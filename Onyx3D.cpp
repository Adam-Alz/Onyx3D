#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include "Triangle.h"
#include "Square.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShader(const std::string& filename);
float xOffset = 0.0f;
float yOffset = 0.0f;
float mixValue = 0.5f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Onyx3D", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported : " << nrAttributes << std::endl;

	Shader shader("vertexShader.vert", "fragmentShader1.frag");
	Shader shader1("vertexShader.vert", "fragmentShader.frag");
	Shader simpleShader("simpleShader.vert", "simpleFragShader.frag");
	Shader textureShader("textureShader.vert", "textureFragShader.frag");


	float topTriangle[] = {
		0.0f, 0.99f, 0.0f,	1.0f, 0.0f, 0.0,// TOP
		-0.5f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,// LEFT  // HAUT
		0.5f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,// RIGHT
	};
	
	float bottomTriangle[] = {
		-0.5f, 0.0f, 0.0f,
		-1.0f, -0.99f, 0.0f, // GAUCHE
		0.0f, -0.99f, 0.0f,

		0.5f, 0.0f, 0.0f,
		1.0f, -0.99f, 0.0f, // DROITE
		0.0f, -0.99f, 0.0f
	};


	std::vector<float> texturedTriangleVertices = {
		0.0f, -0.98f, 0.0f,		1.0f, 0.0f, 0.0f,		0.5f, 1.0f,
		-0.49f, -0.01f, 0.0f,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		0.49f, -0.01f, 0.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f
	};


	float texturedSquare[] =
	{
		-1.0f, 1.0f, 0.0f, /* HAUT GAUCHE */	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.50f, 1.0f, 0.0f, /* HAUT DROITE */	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
	    -1.0f, 0.25f, 0.0f, /* BAS GAUCHE */	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.50, 0.25f, 0.0f, /* BAS DROITE*/		1.0f, 1.0f, 0.0f,	1.0f, 0.0f

	};

	unsigned int texturedSquareIndice[] =
	{
		0, 1, 2,
		1, 2, 3
	};

	//-0.79f, 0.9f, 0.0f,
	//-0.95f, 0.5f, 0.0f, // GAUCHE
	//-0.6f, 0.5f, 0.0f,

	std::vector<float> triangleTestClass = {
		1.0f, 1.0f, 0.0f,	0.5f, 1.0f,
		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.51f, 0.0f, 0.0f,	0.0f, 0.0f,
	};

	std::vector<float> SquareTestClass = {
		-1.0f, -0.5f, 0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,		1.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,		1.0f, 0.0f
	};

	std::vector<unsigned int> indicesSquareTestClass = {
		0, 1, 2,
		1, 2, 3
	};

	float triforce[] =
	{
		-0.5f, 0.0f, 0.0f,
		-1.0f, -0.99f, 0.0f, // 1er triangle
		0.0f, -0.99f, 0.0f,

		0.5f, 0.0f, 0.0f,
		1.0f, -0.99f, 0.0f, // 2ieme triangle
		//0.0f, -0.99f, 0.0f,


		0.0f, 0.99f, 0.0f, // TOP
		//-0.5f, 0.0f, 0.0f, // LEFT  3ieme triangle
		//0.5f, 0.0f, 0.0f, // RIGHT
	};

	unsigned int triforceIndices[] = {
		0, 1, 2,
		3, 4, 2,
		5, 0, 3
	};





	unsigned int VBO, VAO, EBO, VBO1, VAO1, TexturedSquareVBO, TexturedSquareVAO, TexturedTriangleVBO, TexturedTriangleVAO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO1);
	glGenVertexArrays(1, &TexturedSquareVAO);
	glGenVertexArrays(1, &TexturedTriangleVAO);


	
	
	

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &TexturedSquareVBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &TexturedTriangleVBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottomTriangle), bottomTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	



	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topTriangle), topTriangle, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triforceIndices), triforceIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	glBindVertexArray(TexturedSquareVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TexturedSquareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedSquare), texturedSquare, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texturedSquareIndice), texturedSquareIndice, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("ressources/Textures/wall.jpg", &width, &height, &nrChannels, 0);
	
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

	

	//float border_color[] = { 1.0f, 0.0f, 0.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);



	Triangle texturedTriangle(texturedTriangleVertices, textureShader);
	texturedTriangle.InitColorTexture();
	texturedTriangle.SetTexture("ressources/Textures/chokbar.jpg");
	
	


	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("ressources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//float border_color[] = { 1.0f, 0.0f, 0.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);


	Triangle testTriangle(triangleTestClass, textureShader);
	testTriangle.InitTexture();
	testTriangle.SetTexture("ressources/Textures/wall.jpg");

	Square testSquare(SquareTestClass, indicesSquareTestClass, simpleShader);
	//testSquare.Init();
	testSquare.InitTexture();
	testSquare.SetTexture("ressources/Textures/container.jpg");
	//testTriangle.SetTexture("ressources/Textures/awesomeface.jpg");

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	

	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	//trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.5));
	
	//testTriangle.Scale(0.5f, 0.5f, 0.5f);
	//testTriangle.Rotate(180, 0.0f, 1.0f, 0.0f);
	//testTriangle.Translate(0.5, 0.0, 0.0);
	
	//testSquare.Translate(0.2f, 0.5f, 0.0f);

	shader1.use();
	shader1.setInt("texture", 0);
	shader1.setInt("texture2", 1);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Rendering command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

		shader.use();
		shader.setBool("useTransform", true);
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		shader.setTrans("transform", testTriangle.trans);
		
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//std::cout << "green value : " << greenValue << std::endl;
		//int vertexColorLocation = shader.getUniformLocation("ourColor");//glGetUniformLocation(shaderProgram, "ourColor");
		shader.setFloat4("ourColor", 0.5f, greenValue, 0.0f, 1.0f);
		shader.setFloat("xOffset", xOffset);
		shader.setFloat("yOffset", yOffset);
		//glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.0f, 1.0f);


		glBindVertexArray(VAO); // Methode affichage 1 
		glDrawArrays(GL_TRIANGLES, 0, 6);

		
		textureShader.use();
		//textureShader.setInt("texture1", 0);
		testSquare.BindTexture(0);
		//simpleShader.setBool("useTransform", true);
		//simpleShader.setTrans("transform", testSquare.trans);
		//testSquare.SetColor("color", 255, 0, 0, 0);
		testSquare.Draw();

		//shader.setBool("useTransform", false);
		//testTriangle.SetColor("ourColor", 114.0f, 35.f, 217.0f, 255.0f);
		textureShader.use();
		testTriangle.BindTexture(0);
		//testTriangle.SetColor("color", 0, 255, 0, 0);
		testTriangle.Draw();

		//shader.setFloat4("ourColor", 1.0f, greenValue, 0.2f, 1.0f);

		

		//glBindVertexArray(VAO1); // Methode affichage 2
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		

		shader1.use();

		unsigned int transoformLoc1 = glGetUniformLocation(shader1.ID, "transform");
		//glUniformMatrix4fv(transoformLoc1, 1, GL_FALSE, glm::value_ptr(trans));

		shader1.setBool("useTexture", false);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3); //Methode affichage 1
		//glDrawArrays(GL_TRIANGLES, 2, 3);



		shader1.use();
		shader1.setBool("useTexture", true);
		shader1.setBool("mixTexture", true);
		shader1.setFloat("mixValue", mixValue);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		

		glBindVertexArray(TexturedSquareVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shader1.use();
		shader1.setBool("useTexture", true);
		shader1.setBool("mixTexture", false);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glBindVertexArray(TexturedTriangleVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		
		texturedTriangle.BindTexture(0);
		texturedTriangle.Draw();

		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6*sizeof(unsigned int))); // Methode affichage 2


		glBindVertexArray(0);



		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	testTriangle.~Triangle();
	testSquare.~Square();
	//glDeleteProgram(shaderProgram);
	//glDeleteProgram(shaderProgram1);
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xOffset += 0.001f;
		if (xOffset >= 1.0f) { xOffset = 1.0f; }
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xOffset -= 0.001f;
		if (xOffset <= 0.0f) { xOffset = 0.0f; }
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		yOffset += 0.001f;
		if (yOffset >= 1.0f) { yOffset = 1.0f; }
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		yOffset -= 0.001f;
		if (yOffset <= 0.0f) { yOffset = 0.0f; }
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if (mixValue >= 1.0f) { mixValue = 1.0f; }
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if (mixValue <= 0.0f) { mixValue = 0.0f; }
	}
}

std::string loadShader(const std::string& filename)
{
	std::string result = "";
	std::string line = "";
	std::ifstream f(filename.c_str());

	if (f.is_open())
	{
		while (std::getline(f, line))
		{
			result += line + '\n';
		}
		f.close();
	}
	else
	{
		std::cout << "ERROR : LOADER_SHADER::OPENING_FILE_FAILED" << std::endl;
	}
	return result;
}