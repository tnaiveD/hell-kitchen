//v0.0.0

#include <gl.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <windows.h>

#include "stb_image.h"

#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Shader.h"
#include "Timer.h"
#include "Model.h"


#define SCR_WIDTH 1366
#define SCR_HEIGHT 768

#define HK_POSTPROCESS false


void framebuffer_size_callback(GLFWwindow*, int, int);
void cursor_pos_callback(GLFWwindow*, double, double);
void key_callback(GLFWwindow*, int, int, int, int);
void processInput(GLFWwindow*);

void shadersLogs(const std::vector<Shader*>&);

using std::cout;

/////////////////////////////////////////
// Speed

//delta time
float lastFrame = 0.0f;
float deltaTime = 0.0f;

/////////////////////////////////////////
// Camera

float xLast = SCR_WIDTH / 2;
float yLast = SCR_HEIGHT / 2;
float yaw = -90.0f;
float pitch = -10.f;

bool firstCursor = true;

Camera camera0(glm::vec3(0.f, 8.f, 40.f), glm::vec3(0.f, 1.f, 0.f), yaw, pitch);

/////////////////////////////////////////
// Lights

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

/////////////////////////////////////////
// MAIN

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(
		SCR_WIDTH, SCR_HEIGHT,
		"...",
		NULL, NULL
	);
	if (!window) {
		cout << "Error: Window creation FAILED\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		cout << "Error: GLAD loadGL FAILED\n";
		glfwTerminate();
		return -1;
	};

	auto sysWidth = GetSystemMetrics(SM_CXSCREEN);
	auto sysHeight = GetSystemMetrics(SM_CYSCREEN);
	glfwSetWindowMonitor(window, NULL, (sysWidth - SCR_WIDTH) / 2, (sysHeight - SCR_HEIGHT) / 2, SCR_WIDTH, SCR_HEIGHT, GLFW_DONT_CARE);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	////////////////////////////////////////
	// Data configuring
	
	//data
	//------------------------------

	//quad (screen)
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	//skybox
	std::vector<std::string> skyboxFacesJpg{
		std::string("right.jpg"),
		std::string("left.jpg"),
		std::string("top.jpg"),
		std::string("bottom.jpg"),
		std::string("front.jpg"),
		std::string("back.jpg"),
	};

	std::vector<std::string> skyboxFacesPng{
		std::string("right.png"),
		std::string("left.png"),
		std::string("top.png"),
		std::string("bottom.png"),
		std::string("front.png"),
		std::string("back.png"),
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	//Space Scene
	//-----------------------------------



	//data init
	//--------------------------------

	//screen VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//skybox VAO
	unsigned int VAOskybox, VBOskybox;
	glGenVertexArrays(1, &VAOskybox);
	glBindVertexArray(VAOskybox);
	glGenBuffers(1, &VBOskybox);
	glBindBuffer(GL_ARRAY_BUFFER, VBOskybox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindVertexArray(0);

	//frame, renderbuffer
	//--------------------------------------

	//framebuffer
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//screen texture
	unsigned int texBuffer;
	glGenTextures(1, &texBuffer);
	glBindTexture(GL_TEXTURE_2D, texBuffer);

	glTexImage2D(GL_TEXTURE, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBuffer, 0);

	//renderbuffer
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);

	//renderbuffer as attachment (linking)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Warning: Framebuffer not complete.\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	////////////////////////////////////////
	// Lights

	DirectionalLight DLight;
	DLight.multDiffuse(4.f);

	////////////////////////////////////////
	// Textures

	//unsigned int texSkybox = loadCubemap("..\\img\\stars0", skyboxFacesJpg);

	////////////////////////////////////////
	// Assets

	Model planet0("..\\assets\\planet\\planet.obj", false);
	
	//rock (Space theme)
	//-----------------------------------------------
	Model rock0("..\\assets\\rock\\rock.obj", false);

	//rings 
	unsigned int amount = 5000;
	float radius = 50.f;
	float offset = 15.f;

	glm::mat4* models = new glm::mat4[amount];

	srand(time(NULL));
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model(1.0f);

		// 1. translate
		float angle = static_cast<float>(i) / static_cast<float>(amount) * 360.f;
		float displacement = (rand() % static_cast<int>(offset * 2 * 100)) / 100.f - offset;
		float x = radius * sin(glm::radians(angle)) + displacement;

		displacement = (rand() % static_cast<int>(offset * 2 * 100)) / 100.f - offset;
		float y = displacement * 0.5f;

		displacement = (rand() % static_cast<int>(offset * 2 * 100)) / 100.f - offset;
		float z = radius * cos(glm::radians(angle)) + displacement;

		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale
		float r_scale = static_cast<float>(rand() % 25 + 10) / 100.f;
		model = glm::scale(model, glm::vec3(r_scale, r_scale, r_scale));

		// 3. rotate
		model = glm::rotate(model, glm::radians(static_cast<float>(rand() % 360)), glm::vec3(0.4f, .6f, .8f));


		models[i] = model;
	}

	//VBO models (rock)
	unsigned int VBOmodels;
	glGenBuffers(1, &VBOmodels);
	glBindBuffer(GL_ARRAY_BUFFER, VBOmodels);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &models[0], GL_STATIC_DRAW);

	int meshes_size = rock0.meshes.size();
	for (int i = 0; i < meshes_size; i++)
	{
		glBindVertexArray(rock0.meshes[i].VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(sizeof(glm::vec4) * 2));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(sizeof(glm::vec4) * 3));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}

	glBindVertexArray(0);

	////////////////////////////////////////
	// Shaders

	//default shaders
	Shader shader0("..\\shaders\\Lights.vs", "..\\shaders\\Lights.fs");
	Shader shader0Stencil("..\\shaders\\SingleColor.vs", "..\\shaders\\SingleColor.fs");
	Shader shaderPostproc("..\\shaders\\Postprocess.vs", "..\\shaders\\Postprocess.fs");
	Shader shaderSkybox("..\\shaders\\Skybox.vs", "..\\shaders\\Skybox.fs");
	Shader shaderInstances("..\\shaders\\Instances.vs", "..\\shaders\\Lights.fs");
	Shader shaderExplode("..\\shaders\\Explode.vs", "..\\shaders\\Explode.fs", "..\\shaders\\Explode.gs");
	
	std::vector<Shader*> shaders;
	shaders.push_back(&shader0);
	shaders.push_back(&shader0Stencil);
	shaders.push_back(&shaderPostproc);
	shaders.push_back(&shaderSkybox);
	shaders.push_back(&shaderInstances);
	shaders.push_back(&shaderExplode);
	
	//interface blocks
	//--------------------------------------
	unsigned int UBOmat;
	glGenBuffers(1, &UBOmat);

	glBindBuffer(GL_UNIFORM_BUFFER, UBOmat);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOmat, 0, sizeof(glm::mat4) * 2);

	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
		0.1f,
		200.0f
	);

	//inteface blocks data init
	//-----------------------------------
	glBindBuffer(GL_UNIFORM_BUFFER, UBOmat);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(projection), &projection[0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//shader0 (Default)
	//-----------------------------------
	shader0.use();

	shader0.setInt("fuMaterial.tex_diffuse0", 0);
	shader0.setInt("fuMaterial.tex_specular0", 1);
	shader0.setFloat("fuMaterial.shininess", 256.f);

	shader0.setInt("SLnum", 0);
	shader0.setInt("PLnum", 0);

	shader0.setDLight(DLight);
	shader0.setBool("DLActive", true);

	//shader0Stencil
	//-----------------------------------
	shader0Stencil.use();

	//shaderSkybox
	//-----------------------------------
	shaderSkybox.use();
	shaderSkybox.setInt("skyboxTex", 0);

	//shaderInstances
	//-----------------------------------
	shaderInstances.use();

	shaderInstances.setInt("fuMaterial.tex_diffuse0", 0);
	shaderInstances.setInt("fuMaterial.tex_specular0", 1);
	shaderInstances.setFloat("fuMaterial.shininess", 256.f);

	shaderInstances.setInt("SLnum", 0);
	shaderInstances.setInt("PLnum", 0);

	shaderInstances.setDLight(DLight);
	shaderInstances.setBool("DLActive", true);

	//shaderExplode
	//-----------------------------------
	shaderExplode.use();

	shaderExplode.setInt("fuMaterial.tex_diffuse0", 0);
	shaderExplode.setInt("fuMaterial.tex_specular0", 1);
	shaderExplode.setFloat("fuMaterial.shininess", 256.f);

	shaderExplode.setInt("SLnum", 0);
	shaderExplode.setInt("PLnum", 0);

	shaderExplode.setDLight(DLight);
	shaderExplode.setBool("DLActive", true);

	////////////////////////////////////////
	// Pre-Render

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//depth
	//-----------------------------------
	
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GEQUAL);
	
	//stencil
	//-----------------------------------
	
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	//blend
	//-----------------------------------
	
	//glEnable(GL_BLEND);

	//gamma correction
	//-----------------------------------

	//multisampling
	//-----------------------------------
	//glEnable(GL_MULTISAMPLE);

	////////////////////////////////////////////
	// RENDER

	glfwSetTime(0);
	srand(time(0));

	camera0.setMovementSpeed(camera0.getMovementSpeed() * 4.f);

	//render cycle
	//-----------------------------------
	while (!glfwWindowShouldClose(window)) {
		processInput(window);


		////////////////////////////////////
		// Render data

		float time = glfwGetTime();
		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		////////////////////////////////////
		// Camera

		glm::vec3 camPos = camera0.getPos();
		glm::mat4 camView = camera0.getView();

		////////////////////////////////////
		// Shader blocks

		glBindBuffer(GL_UNIFORM_BUFFER, UBOmat);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(projection), sizeof(camView), &camView[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		////////////////////////////////////
		// Framebuffers

		glClearColor(BLACK, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////////////////////////////////////
		// Drawing

		glm::mat4 model = glm::mat4(1.f);

		//planet
		//--------------------------

		shaderExplode.use();
		shaderExplode.setFloat("time", time);
		
		model = glm::scale(model, glm::vec3(5.f, 5.f, 5.f));
		//model = glm::rotate(model, glm::radians(75.f), glm::vec3(0.f, 0.f, 1.f));
		model = glm::rotate(model, glm::radians(time * 1.25f), glm::vec3(0.5f, 1.f, 0.f));

		shaderExplode.setMat4("vuModel", model);
		//shader0.use();
		//shader0.setMat4("vuModel", model);
		planet0.draw(shaderExplode);

		//rocks
		//--------------------------

		shaderInstances.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rock0.meshes[0].textures[0].id);

		for (unsigned int i = 0; i < rock0.meshes.size(); i++)
		{
			glBindVertexArray(rock0.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, rock0.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			
		}

		glBindVertexArray(0);

		
		//postprocess
		//--------------------------


		glBindVertexArray(0);

		////////////////////////////////////
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shadersLogs(shaders);

	////////////////////////////////////////
	// Memory dealloc

	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &VBOskybox);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &VAOskybox);

	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);

	glDeleteBuffers(1, &UBOmat);

	shader0.suicide();
	shader0Stencil.suicide();
	shaderInstances.suicide();
	shaderPostproc.suicide();
	shaderExplode.suicide();
	
	glfwTerminate();
	system("pause");
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int x, int y) {
	glViewport(0, 0, x, y);
}

void cursor_pos_callback(GLFWwindow* window, double xIn, double yIn)
{
	float _xIn = static_cast<float>(xIn);
	float _yIn = static_cast<float>(yIn);

	if (firstCursor) {
		xLast = _xIn;
		yLast = _yIn;
		firstCursor = false;
	}

	float xOffset = _xIn - xLast;
	float yOffset = yLast - _yIn;

	xLast = _xIn;
	yLast = _yIn;

	camera0.processMouseMovement(xOffset, yOffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int modes) {

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera0.move(Camera::Direction::FRONT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera0.move(Camera::Direction::BACK, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera0.move(Camera::Direction::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera0.move(Camera::Direction::RIGHT, deltaTime);
	}

}

void shadersLogs(const std::vector<Shader*>& shaders)
{
	for (const auto& x : shaders)
	{
		cout << '\n' << x->getInfoLog();
	}
}