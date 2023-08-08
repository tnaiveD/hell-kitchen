#include <gl.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <windows.h>

#include "stb_image.h"

#include "Shader.h"
#include "CamFPS.h"
#include "Materials.h"
#include "Light.h"
#include "Timer.h"
#include "Model.h"
#include "Object.h"

#define SCR_WIDTH 1100
#define SCR_HEIGHT 700

void framebuffer_size_callback(GLFWwindow*, int, int);
void cursor_pos_callback(GLFWwindow*, double, double);
void key_callback(GLFWwindow*, int, int, int, int);
void processInput(GLFWwindow*);

void drawScene(const Shader&);

//Debug funs
void shadersLogs(const std::vector<Shader*>&);

using std::cout;

/////////////////////////////////////////
// Camera

CamFPS cam(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//delta time
float lastFrame = 0.0f;
float deltaTime = 0.0f;

//mouse view
float xLast = 0.0f, yLast = 0.0f;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstCursor = true;

/////////////////////////////////////////
// Lights

bool flActive = false;

/////////////////////////////////////////
// MAIN

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	//cube V N T
	const float cube1vertices[] = {
		//vertex			  //normal			   //tex
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//cube V T
	const float cube2vertices[] = {
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

	//plane V N T
	const float planeVertices[] = {
		-15.f, -.0f, 15.f, 0.f, 1.f, 0.f,	 0.f, 0.f,
		15.f, -.0f, 15.f, 0.f, 1.f, 0.f,     30.f, 0.f,
		-15.f, -.0f, -15.f, 0.f, 1.f, 0.f,   0.f, 30.f,

		15.f, -.0f, 15.f, 0.f, 1.f, 0.f,     30.f, 0.f,
		15.f, -.0f, -15.f, 0.f, 1.f, 0.f,    30.f, 30.f,
		-15.f, -.0f, -15.f, 0.f, 1.f, 0.f,   0.f, 30.f
	};

	//quad (screen) V T
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
	std::vector<std::string> skyboxFaces{
		std::string("right.jpg"),
		std::string("left.jpg"),
		std::string("top.jpg"),
		std::string("bottom.jpg"),
		std::string("front.jpg"),
		std::string("back.jpg"),
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

	//data init
	//--------------------------------

	//cube VAO
	unsigned int VBOcube, VAOcube;
	glGenVertexArrays(1, &VAOcube);
	glBindVertexArray(VAOcube);
	glGenBuffers(1, &VBOcube);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube1vertices), cube1vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	//plane VAO
	unsigned int VBOplane, VAOplane;
	glGenVertexArrays(1, &VAOplane);
	glBindVertexArray(VAOplane);
	glGenBuffers(1, &VBOplane);
	glBindBuffer(GL_ARRAY_BUFFER, VBOplane);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

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

	//framebuffers (and renderbuffers)
	//---------------------------------------
	unsigned int FBO0;
	glGenFramebuffers(1, &FBO0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO0);

	unsigned int FBOtex;
	glGenTextures(1, &FBOtex);
	glBindTexture(GL_TEXTURE_2D, FBOtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtex, 0);

	//renderbuffers
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR: Framebuffer status NOT_COMPLETE\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	////////////////////////////////////////
	// Lights

	DirectionalLight dLight;
	dLight.setActive(true);
	dLight.multDiffuse(1.5f);

	PointLight pLight0(-5.f, 2.5f, 0.f);
	pLight0.multAmbient(0.05f);
	pLight0.multDiffuse(2.f);
	pLight0.setAttenuation(ATTENUATION20);

	SpotLight sLight0(glm::vec3(0.0f, 1.5f, 0.0f));
	SpotLight sLight1(glm::vec3(5.0f, 1.5f, 0.0f));
	SpotLight sLight2(-1.f, 0.5f, 4.5f, 0.25f, 0.f, -1.f);
	SpotLight sLight3(1.f, 0.5f, 4.5f, -0.25f, 0.f, -1.f);

	vector<SpotLight*> sLights
	{
		&sLight0, &sLight1, &sLight2, &sLight3
	};

	for (const auto& x : sLights)
	{
		x->setAttenuation(ATTENUATION32);
		x->multAmbient(0.05f);
		x->multDiffuse(2.f);
		x->setRadius(50.f);
		x->setActive(false);
	}

	sLight0.setActive(false);
	sLight2.setDiffuse(glm::vec3(0.6f, 0.1f, 0.2f));
	sLight3.setDiffuse(glm::vec3(0.1f, 0.2f, 0.7f));
	sLight2.setSpecular(glm::vec3(0.6f, 0.1f, 0.2f));
	sLight3.setSpecular(glm::vec3(0.1f, 0.2f, 0.7f));

	////////////////////////////////////////
	// Shadows

	unsigned int FBOdepthmap;
	glGenFramebuffers(1, &FBOdepthmap);

	unsigned int depthmap;
	const unsigned int c_shadowwidth = 1024, c_shadowheight = 1024;
	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, c_shadowwidth, c_shadowheight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, FBOdepthmap);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//light space
	CamFPS lightCam(glm::vec3(2.0f, 3.0f, 1.0f), dLight.getDir(), glm::vec3(0.0f, 1.0f, 0.0f));
	

	////////////////////////////////////////
	// Textures

	unsigned int texContainer0 = loadTex("..\\img\\container1.png", GL_CLAMP_TO_EDGE);
	unsigned int texContainer0Spec = loadTex("..\\img\\container1_spec.png", GL_REPEAT);
	unsigned int texParquet0 = loadTex("..\\img\\parquet0_diffuse.jpg", GL_REPEAT);
	unsigned int texParquet0Spec = loadTex("..\\img\\parquet0_spec.jpg", GL_REPEAT);
	//unsigned int texSkybox = loadCubemap("..\\img\\sky0", skyboxFaces);

	////////////////////////////////////////
	// Assets and Meshes
	
	// Horse
	Model horse0("..\\assets\\horse0\\Rocking Horse.obj", false);
	
	// Boxes
	vector<Texture> boxTextures
	{
		loadTexTex("..\\img\\container1.png", GL_CLAMP_TO_EDGE, TEXTURE_DIFFUSE),
		loadTexTex("..\\img\\container1_spec.png", GL_CLAMP_TO_EDGE, TEXTURE_SPECULAR),
	};
	
	Mesh box0Mesh(vertexDataToVertexVector(cube1vertices, sizeof(cube1vertices) / sizeof(float), VERTEX), boxTextures);
	Object box1(box0Mesh);
	Object box2(box0Mesh);

	// Plane
	vector<Texture> planeTextures
	{
		loadTexTex("..\\img\\parquet0_diffuse.jpg", GL_REPEAT, TEXTURE_DIFFUSE),
		loadTexTex("..\\img\\parquet0_spec.jpg", GL_REPEAT, TEXTURE_SPECULAR),
	};
	
	Mesh plane0Mesh(vertexDataToVertexVector(planeVertices, sizeof(planeVertices) / sizeof(float), VERTEX), planeTextures);
	Object plane0(plane0Mesh);

	// Light cube
	Mesh lightCubeMesh(vertexDataToVertexVector(cube2vertices, sizeof(cube1vertices) / sizeof(float), VERTEX_P_T));
	Object lightCube0(box0Mesh);

	////////////////////////////////////////
	// Shaders

	Shader shaderLights0("..\\shaders\\Lighting0.vs", "..\\shaders\\Lighting0.fs");
	Shader shaderSingleColor("..\\shaders\\SingleColor.vs", "..\\shaders\\SingleColor.fs");
	Shader shaderLightSpace("..\\shaders\\LightSpace.vs", "..\\shaders\\LightSpace.fs");
	Shader shaderScreenDepth("..\\shaders\\ScreenTexture.vs", "..\\shaders\\ScreenTexture.fs");

	std::vector<Shader*> shaders;
	shaders.push_back(&shaderLights0);
	shaders.push_back(&shaderSingleColor);
	shaders.push_back(&shaderLightSpace);
	shaders.push_back(&shaderScreenDepth);

	//interface blocks
	unsigned int UBOmat;
	glGenBuffers(1, &UBOmat);
	glBindBuffer(GL_UNIFORM_BUFFER, UBOmat);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOmat, 0, sizeof(glm::mat4) * 2);

	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
		0.1f,
		100.0f
	);
	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//shaderLights
	//-----------------------------------
	shaderLights0.use();
	
	shaderLights0.setInt("fuMaterial.tex_diffuse0", 0);
	shaderLights0.setInt("fuMaterial.tex_specular0", 1);

	shaderLights0.setDLight(dLight);
	shaderLights0.setSLights(sLights);
	shaderLights0.setPLights(pLight0);

	//shaderLightSpace
	//-----------------------------------


	//shaderScreenDepth
	//-----------------------------------
	
	shaderScreenDepth.use();
	shaderScreenDepth.setInt("screenTex", 0);

	////////////////////////////////////////
	// Static objects preparing

	plane0.translate(glm::vec3(0.f, -0.502f, 0.f));

	box1.translate(glm::vec3(-1.2f, 0.125f, 0.f));
	box1.scale(glm::vec3(1.25f, 1.25f, 1.25f));
	box1.rotateY(10);
	
	box2.translate(glm::vec3(1.2f, -0.25f, 0.f));
	box2.scale(glm::vec3(.5f, .5f, .5f));
	box2.rotateY(-30);

	lightCube0.translate(sLight0.getPos());
	lightCube0.rescale(0.05f);

	////////////////////////////////////////
	// Pre-Render

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	
	//glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_MULTISAMPLE);

	////////////////////////////////////////
	// RENDER
	
	bool firstFrame = true;
	glfwSetTime(0);

	//render cycle
	//-----------------------------------
	while (!glfwWindowShouldClose(window)) {
		processInput(window);


		////////////////////////////////////
		// Framebuffers
	
		
		////////////////////////////////////
		
		float time = glfwGetTime();
		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cam.setDeltaTime(deltaTime);

		////////////////////////////////////
		// Camera

		glm::vec3 camPos = cam.getPos();
		glm::vec3 camDir = cam.getDir();
		glm::mat4 camView = cam.getView();

		glBindBuffer(GL_UNIFORM_BUFFER, UBOmat);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &camView[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		////////////////////////////////////
		// Drawing

		glClearColor(BLACK, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shadows
		// ----------------------------------

		// Light Space

		float near_plane = 1.0f, far_plane = 7.5f;

		glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
		/*glm::mat4 lightView = glm::lookAt(glm::vec3(-2.f, 4.f, -1.f),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f));*/
		glm::mat4 lightView = lightCam.getView();
		glm::mat4 lightSpace = lightProjection * lightView;

		shaderLightSpace.use();
		shaderLightSpace.setMat4("vuLightSpace", lightSpace);

		glViewport(0, 0, c_shadowwidth, c_shadowheight);
		glBindFramebuffer(GL_FRAMEBUFFER, FBOdepthmap);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		
		// Writing to depthmap
		// ------------------------

		glm::mat4 model(1.f);

		//plane

		plane0.draw(shaderLightSpace);

		//cube 1
		/*model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-1.2f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(-20.f), glm::vec3(.0f, 1.f, .0f));
		shaderLightSpace.setMat4("vuModel", model);
		glBindVertexArray(VAOcube);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/
		box1.draw(shaderLightSpace);

		//cube2
		/*model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(1.0f, -0.25f, 0.f));
		model = glm::rotate(model, glm::radians(30.f), glm::vec3(.0f, 1.f, .0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		shaderLightSpace.setMat4("vuModel", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/
		box2.draw(shaderLightSpace);

		//horse
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-0.15f, -0.55f, -0.2f));
		model = glm::rotate(model, glm::radians(-40.f), glm::vec3(0.0f, 1.f, .0f));
		model = glm::rotate(model, glm::radians(sin(time)) * 5.f, glm::vec3(.0f, 0.f, 1.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));

		shaderLightSpace.setMat4("vuModel", model);

		horse0.draw(shaderLightSpace);

		//light cube
		model = glm::mat4(1.f);
		model = glm::translate(model, sLight0.getPos());
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		model = glm::rotate(model, glm::radians(sin(time * 1.75f) * 8.5f), glm::vec3(0.f, 0.f, 1.f));
		shaderLightSpace.setMat4("vuModel", model);
		glBindVertexArray(VAOcube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ending

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		// Main framebuffer
		// ----------------------------------------

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shaderLights0.use();
		shaderLights0.setVec3("fuViewPos", camPos);

		//shadow map
		shaderLights0.setMat4("vuLightSpace", lightSpace);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, depthmap);
		shaderLights0.setInt("shadowMap", 5);

		//plane
		shaderLights0.setFloat("fuMaterial.shininess", 256.f);
		plane0.draw(shaderLights0);

		//cubes
		shaderLights0.setFloat("fuMaterial.shininess", 256.f);

		//box1
		box1.draw(shaderLights0);
		
		//box2
		box2.draw(shaderLights0);

		//horse
		shaderLights0.setFloat("fuMaterial.shininess", 32.f);							// ÄÎÁÀÂÈÒÜ ÐÀÑÏÎÇÍÀÂÀÍÈÅ ÌÀÒÅÐÈÀËÎÂ
		
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-0.15f, -0.573f, -0.2f));
		model = glm::rotate(model, glm::radians(-40.f), glm::vec3(0.0f, 1.f, .0f));
		model = glm::rotate(model, glm::radians(sin(time)) * 5.f, glm::vec3(.0f, 0.f, 1.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		shaderLights0.setMat4("vuModel", model);
		
		horse0.draw(shaderLights0);

		//light cube
		
		lightCube0.draw(shaderSingleColor);


		/*shaderScreenDepth.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthmap);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		//postprocess
		//----------------------------------
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		glBindVertexArray(0);
		
		////////////////////////////////////
		// Events etc.
		//////////////////

		// LIGHTS data (shaderLights0)
		// -----------------------------------

		if (firstFrame)
		{
			glfwSetTime(0);
			firstFrame = false;
		}

		float xMovePos = cos(time * 1.75f) * deltaTime * 0.1f;
		float yMovePos = sin(time * 3.5f) * deltaTime * 0.05f;
		float xMoveDir = cos(time * 1.75f) * deltaTime * 0.1f;

		sLight0.movePos(glm::vec3(xMovePos, yMovePos, 0.f));
		sLight0.moveDir(glm::vec3(xMoveDir, 0.f, 0.f));

		//Sunset??
		//lightCam.moveDir(glm::vec3(0.f, sin(time * 0.45f) * deltaTime * 0.15f, 0.f));

		lightCube0.translate(sLight0.getPos());
		lightCube0.rotateZ(cos(time * 1.75f) * 0.1f);


		////////////////////////////////////
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//shader uniforms log
	shadersLogs(shaders);

	////////////////////////////////////////
	// Memory dealloc

	glDeleteBuffers(1, &VBOcube);
	glDeleteBuffers(1, &VBOplane);
	glDeleteVertexArrays(1, &VAOcube);
	glDeleteVertexArrays(1, &VAOplane);

	shaderLights0.suicide();
	shaderSingleColor.suicide();

	glfwTerminate();
	system("pause");
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int x, int y) {
	glViewport(0, 0, x, y);
}

void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos) {

	if (firstCursor) {
		xLast = xPos;
		yLast = yPos;
		firstCursor = false;
	}
	float xOffset = xPos - xLast;
	float yOffset = yLast - yPos;
	xLast = xPos;
	yLast = yPos;
	float sensitivity = 0.02f;
	yaw += xOffset * sensitivity;
	pitch += yOffset * sensitivity;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 dir;
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cam.setDir(glm::normalize(dir));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int modes) {

	//flashlight
	if ((key == GLFW_KEY_L) && (action == GLFW_PRESS)) {
		if (flActive)
			flActive = false;
		else
			flActive = true;
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.moveFront();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.moveBack();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.moveLeft();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.moveRight();
	} 
}

void shadersLogs(const std::vector<Shader*>& shaders)
{
	for (const auto& x : shaders)
	{
		cout << '\n' << x->getInfoLog();
	}
}

void drawScene(const Shader& shader)
{
	
}