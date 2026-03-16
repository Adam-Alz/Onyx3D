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
#include "Texture.h"

// Test Git

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


	std::vector<float> topTriangle = {
		0.0f, 0.99f, 0.0f,	/*1.0f, 0.0f, 0.0,*/	0.5f, 1.0f,// TOP
		-0.5f, 0.0f, 0.0f,	/*0.0f, 1.0f, 0.0f,*/	0.0f, 0.0f,// LEFT  // HAUT
		0.5f, 0.0f, 0.0f,	/*0.0f, 0.0f, 1.0f,*/	1.0f, 0.0f// RIGHT
	};

	std::vector<float> bottomTriangle = {
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


	std::vector<float> texturedSquare = {
		-1.0f, 1.0f, 0.0f, /* HAUT GAUCHE */	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.50f, 1.0f, 0.0f, /* HAUT DROITE */	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
	    -1.0f, 0.25f, 0.0f, /* BAS GAUCHE */	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.50, 0.25f, 0.0f, /* BAS DROITE*/		1.0f, 1.0f, 0.0f,	1.0f, 0.0f

	};

	std::vector<unsigned int> texturedSquareIndice =
	{
		0, 1, 2,
		1, 2, 3
	};


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


	Triangle bottomTriangleClass(bottomTriangle, shader);
	bottomTriangleClass.Init();


	Texture obamaTexture("ressources/Textures/obama2.jpg");
	Triangle topTriangleClass(topTriangle, shader1);
	topTriangleClass.InitTexture();


	Texture textureTriangleTexture("ressources/Textures/container.jpg");
	Triangle texturedTriangle(texturedTriangleVertices, textureShader);
	texturedTriangle.InitColorTexture();
	

	Texture textureTriangle("ressources/Textures/wall.jpg");
	Triangle testTriangle(triangleTestClass, textureShader);
	testTriangle.InitTexture();


	Texture texture("ressources/Textures/milesMorales.jpg");
	Texture texture1("ressources/Textures/hacker.jpg");
	Square testSquare(texturedSquare, texturedSquareIndice, shader);
	testSquare.InitColorTexture();


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

		bottomTriangleClass.Draw(6);;
		

		//shader.setBool("useTransform", false);
		//testTriangle.SetColor("ourColor", 114.0f, 35.f, 217.0f, 255.0f);
		textureShader.use();
		//testTriangle.BindTexture(testTriangleTexture, 0);
		//testTriangle.SetColor("color", 0, 255, 0, 0);
		//textureTriangle.ActiveAndBind(0, testTriangleTexture);
		textureTriangle.ActiveAndBind(0, textureTriangle.ID);
		testTriangle.Draw(3);

		

		shader1.use();

		unsigned int transoformLoc1 = glGetUniformLocation(shader1.ID, "transform");
		//glUniformMatrix4fv(transoformLoc1, 1, GL_FALSE, glm::value_ptr(trans));

		//shader1.setBool("useTexture", true);
		textureShader.use();
		//obamaTexture.ActiveAndBind(0, obamaTex);
		obamaTexture.ActiveAndBind(0, obamaTexture.ID);
		topTriangleClass.Draw(3);


		shader1.use();
		shader1.setBool("useTexture", true);
		shader1.setBool("mixTexture", true);
		shader1.setFloat("mixValue", mixValue);

		//testSquare.BindTexture(testSquareTexture, 0);
		//testSquare.BindTexture(testSquareTexture1, 1);
		//texture.ActiveAndBind(0, testSquareTexture);
		//texture.ActiveAndBind(1, testSquareTexture1);
		texture.ActiveAndBind(0, texture.ID);
		texture1.ActiveAndBind(1, texture1.ID);
		testSquare.Draw();


		shader1.use();
		shader1.setBool("useTexture", true);
		shader1.setBool("mixTexture", false);
		
		
		//textureTriangleTexture.ActiveAndBind(0, texturedTriangleM);
		textureTriangleTexture.ActiveAndBind(0, textureTriangleTexture.ID);
		//texturedTriangle.BindTexture(texturedTriangleTexture, 0);
		texturedTriangle.Draw(3);


		glBindVertexArray(0);



		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	testTriangle.~Triangle();
	testSquare.~Square();
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