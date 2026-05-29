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
#include "Circle.h"
#include "Camera.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h> 

// Test Git
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShader(const std::string& filename);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float xOffset = 0.0f;
float yOffset = 0.0f;
float mixValue = 0.5f;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool cameraMode = true;
float vitesse = 25.0f;



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Onyx3D", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported : " << nrAttributes << std::endl;

	Shader shader("vertexShader.vert", "fragmentShader1.frag");
	Shader shader1("vertexShader.vert", "fragmentShader.frag");
	Shader simpleShader("simpleShader.vert", "simpleFragShader.frag");
	Shader textureShader("textureShader.vert", "textureFragShader.frag");

	glEnable(GL_DEPTH_TEST);

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


	std::vector<float> squarePreuve = {
		0.75f, 0.25f, 0.0f,		//0.0f, 1.0f,
		1.0f, 0.25f, 0.0f,		//1.0f, 1.0f,
		0.75f, -0.25f, 0.0f,	//0.0f, 0.0f,
		1.0f, -0.25f, 0.0f,		//1.0f, 0.0f
	};

	std::vector<unsigned int> indiceSquarePreuve = {
		0, 1, 2,
		1, 2, 3

	};

	
	std::vector<float> verticesCoursSquare = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};


	std::vector<unsigned int> indiceCoursSquare = {
		0, 1, 2,
		1, 2, 3
	};

	std::vector<float> vertices = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	std::vector<float> verticesPrism = {

		// ===== BASE (carré → 2 triangles) =====

		// triangle 1
		-0.5f, 0.0f, -0.5f,   //0.0f, 1.0f,
		 0.5f, 0.0f, -0.5f,   //0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,   //1.0f, 1.0f,

		 // triangle 2
		 -0.5f, 0.0f, -0.5f,   //0.0f, 0.0f,
		  0.5f, 0.0f,  0.5f,   //0.0f, -1.0f,
		 -0.5f, 0.0f,  0.5f,   //1.0f, -1.0f,


		 // ===== CÔTÉS (4 triangles) =====

		 // face 1
		 -0.5f, 0.0f, -0.5f,   //0.0f, 1.0f, // BAS DROITE
		  0.5f, 0.0f, -0.5f,   //0.0f, 0.0f, // BAS GAUCHE	Visage 
		  0.0f, 1.0f,  0.0f,   //1.0f, 1.0f, // HAUT

		  // face 2
		   0.5f, 0.0f, -0.5f,   //0.0f, 1.0f,
		   0.5f, 0.0f,  0.5f,   //0.0f, 0.0f,
		   0.0f, 1.0f,  0.0f,   //0.5f, 0.0f,

		   // face 3
			0.5f, 0.0f,  0.5f,   //1.0f, 0.0f,
		   -0.5f, 0.0f,  0.5f,  //0.0f, 0.0f,
			0.0f, 1.0f,  0.0f,   //0.165f, 0.5f,

			// face 4
			-0.5f, 0.0f,  0.5f,   //1.0f, 0.5f, // BAS GAUCHE
			-0.5f, 0.0f, -0.5f,	  //1.0f, 1.0f, // BAS DROITE
			 0.0f, 1.0f,  0.0f,   //0.165f, 0.5f, // HAUT
	};

	// face 4
	//-0.5f, 0.0f, 0.5f, 1.0f, 0.5f, // BAS GAUCHE
	//	-0.5f, 0.0f, -0.5f, 1.0f, 1.0f, // BAS DROITE
	//	0.0f, 1.0f, 0.0f, 0.165f, 0.5f, // HAUT

	
	Triangle prism(verticesPrism, simpleShader);
	prism.Init();


	
	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

	Texture theRockTex("ressources/Textures/patronTheRock.jpg");
	Texture memeBeastTex("ressources/Textures/obamaCrop.png", true);
	Square coursSquare(vertices, simpleShader);
	coursSquare.InitTexture();


	Circle circle(simpleShader, 50, 0.2f);
	circle.Init();


	Texture testTex("ressources/Textures/hacker.jpg");
	Square preuvreSquare(squarePreuve, indiceSquarePreuve, shader);
	preuvreSquare.Init();
	


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


	Texture tex("ressources/Textures/paff.png", true);

	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	//trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.5));
	
	//testTriangle.Scale(0.5f, 0.5f, 0.5f);
	//testTriangle.Rotate(45, 0.0f, 1.0f, 0.0f);
	//testTriangle.Translate(0.5, 0.0, 0.0);
	
	//testSquare.Translate(0.2f, 0.5f, 0.0f);


	shader1.use();
	shader1.setInt("texture", 0);
	shader1.setInt("texture2", 1);


	


	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Rendering command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		
		//simpleShader.use();
		//simpleShader.setBool("useTransform", true);
		//prism.SetColor("color", 255.0f, 0.0f, 0.0f, 255.0f);
		



		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		textureShader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		
		

		textureShader.use();
		textureShader.setBool("useTransform", true);
		textureShader.setMat4("projection", projection);
		textureShader.setMat4("view", view);
		//textureShader.setMat4("model", model);

		

		testTex.ActiveAndBind(0);



		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = (float)glfwGetTime() * 25.0f;
			//float angle = (float)glfwGetTime() * vitesse;

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			textureShader.setMat4("model", model);

			//glDrawArrays(GL_TRIANGLES, 0, 36);
			coursSquare.DrawCube();
		}
		
		

		/*
		prism.Draw(18);

		textureShader.use();

		textureShader.setMat4("projection", projection);
		textureShader.setMat4("view", view);
		textureShader.setMat4("model", model);


		textureShader.setBool("useTransform", true);
		//textureShader.setInt("texture1", 0);
		memeBeastTex.ActiveAndBind(0);
		coursSquare.DrawCube();
		*/

		//textureShader.setBool("useTransform", true);
		//coursSquare.SetColor("color", 255.0f, 0.0f, 0.0f, 255.0f);
		//coursSquare.DrawCube();


		/*

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




		simpleShader.use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(simpleShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(simpleShader.ID, "view");
		unsigned int projectionloc = glGetUniformLocation(simpleShader.ID, "projection");

		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionloc, 1, GL_FALSE, glm::value_ptr(projection));



		simpleShader.setBool("transform", true);
		preuvreSquare.SetColor("color", 255.0f, 0.0f, 0.0f, 255.0f);

		preuvreSquare.Draw();


		glBindVertexArray(0);





		simpleShader.use();
		simpleShader.setBool("transform", true);
		simpleShader.setTrans("model", model);
		simpleShader.setTrans("view", view);
		simpleShader.setTrans("projection", projection);
		circle.Draw();
		*/


		//IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove; //| ImGuiWindowFlags_NoCollapse;

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		ImVec2 uv_min = ImVec2(0.0f, 0.0f); // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right

		ImGui::Begin("Scene", 0, windowFlags);
		//ImGui::ImageWithBg(tex.ID, ImVec2(256, 256), uv_min, uv_max, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		//ImGui::SetWindowSize(ImVec2(width / 6, ImGui::GetWindowHeight()));
		ImGui::SetWindowPos(ImVec2(width - ImGui::GetWindowWidth(), 0));
		ImGui::Text("FPS : %.1f", io.Framerate);
		ImGui::Text("Vitesse : %.1f", vitesse);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	// Check and call events and swap the buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		vitesse += 10000000.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		vitesse -= 0.5f;
	}


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

	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		cameraMode = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float cameraSpeed = static_cast<float>(2.5 * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		cameraMode = false;
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (cameraMode)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}
}