//v0.0.0

#include <gl.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <windows.h>

#include "stb_image.h"

#include "CamFPS.h"
#include "Materials.h"
#include "Light.h"
#include "Object.h"
#include "Shader.h"
#include "Timer.h"
#include "Model.h"


#define SCR_WIDTH 1000
#define SCR_HEIGHT 800

#define HK_POSTPROCESS false

void framebuffer_size_callback(GLFWwindow*, int, int);
void cursor_pos_callback(GLFWwindow*, double, double);
void key_callback(GLFWwindow*, int, int, int, int);
void processInput(GLFWwindow*);

unsigned int loadTex(const char*, GLenum);
unsigned int loadCubemap(std::string, std::vector<std::string>);

void shadersLogs(const std::vector<Shader*>&);

using std::cout;


/////////////////////////////////////////
// Camera

CamFPS cam(glm::vec3(0.0f, 1.0f, 7.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//delta time
float lastFrame = 0.0f;
float deltaTime = 0.0f;

//mouse view
float xLast = SCR_WIDTH / 2;
float yLast = SCR_HEIGHT / 2;
float yaw = -90.0f;
float pitch = 0.f;

bool firstCursor = true;

/////////////////////////////////////////
// Lights

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

//flashlight
bool flActive = true;
bool flFlicker = false;
float flTimer = 0.f;

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

	//cube
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

	//cube no normal
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

	//plane
	const float planeVertices[] = {
		-8.f, -0.502f, 8.f, 0.f, 1.f, 0.f,   0.f, 0.f,
		8.f, -.502f, 8.f, 0.f, 1.f, 0.f,     16.f, 0.f,
		-8.f, -.502f, -8.f, 0.f, 1.f, 0.f,   0.f, 16.f,

		8.f, -.502f, 8.f, 0.f, 1.f, 0.f,     16.f, 0.f,
		8.f, -.502f, -8.f, 0.f, 1.f, 0.f,    16.f, 16.f,
		-8.f, -.502f, -8.f, 0.f, 1.f, 0.f,   0.f, 16.f
	};

	//grass
	const float grassVertices[] = {
		-0.5f, 1.f, 0.f,   0.0f, 0.0f,
		-0.5f, 0.f, 0.f,  0.0f, 1.0f,
		0.5f, 0.f, 0.f,   1.0f, 1.0f,

		-0.5f, 1.f, 0.f,   0.0f, 0.0f,
		0.5f, 0.f, 0.f,   1.0f, 1.0f,
		0.5f, 1.f, 0.f,    1.0f, 0.0f,
	};

	vector<Object> grass;
	vector<Mesh> grassMeshes;
	vector<Vertex> grassVertexVec = vertexDataToVertexVector(grassVertices, sizeof(grassVertices) / sizeof(float), VERTEX_P_T);
	
	vector<Texture> grassTexVec;
	Mesh grassMesh(grassVertexVec, grassTexVec);
	grassMesh.loadMeshTexture("..\\img\\grass.png", GL_CLAMP_TO_EDGE, TEXTURE_DIFFUSE);
	grassMeshes.push_back(grassMesh);

	srand(time(NULL));
	for (int i = 0; i < 256; i++)
	{	
		float scaleRand = 1.f / (rand() % 9 + 1) + 0.5f;
		glm::vec3 positionRand(static_cast<float>(rand() % 15 - 7.f) + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)),
							   -.502,
							   static_cast<float>(rand() % 15 - 7.f) + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)));

		grass.push_back(Object(grassMeshes, positionRand));
		grass[i].rotate(static_cast<float>(rand() % 360), glm::vec3(0.f, 1.f, 0.f));
		grass[i].scale(glm::vec3(scaleRand, scaleRand, .0f));
	}

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

	//picture 
	float pictureVertices[] =
	{
		-1.f, 2.f, 0.f,		0.f, 0.f,
		-1.f, 0.f, 0.f,		0.f, 1.f,
		 1.f, 0.f, 0.f,		1.f, 1.f,

		-1.f, 2.f, 0.f,		0.f, 0.f,
		 1.f, 2.f, 0.f,		1.f, 0.f,
		 1.f, 0.f, 0.f,		1.f, 1.f
	};

	unsigned int VAOpicture;
	glGenVertexArrays(1, &VAOpicture);
	glBindVertexArray(VAOpicture);

	unsigned int VBOpicture;
	glGenBuffers(1, &VBOpicture);
	glBindBuffer(GL_ARRAY_BUFFER, VBOpicture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pictureVertices), pictureVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glBindVertexArray(0);

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)12);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)24);

	//grass VAO
	unsigned int VBOgrass, VAOgrass;
	glGenVertexArrays(1, &VAOgrass);
	glBindVertexArray(VAOgrass);
	glGenBuffers(1, &VBOgrass);
	glBindBuffer(GL_ARRAY_BUFFER, VBOgrass);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

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

	//unsigned int FBO0;
	//glGenFramebuffers(1, &FBO0);
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO0);
	//unsigned int FBOtex;
	//glGenTextures(1, &FBOtex);
	//glBindTexture(GL_TEXTURE_2D, FBOtex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtex, 0);
	////or?
	//glTexImage2D(
	//	GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0,
	//	GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	//);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, FBOtex, 0);*/

	////renderbuffers
	//unsigned int RBO;
	//glGenRenderbuffers(1, &RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	////check
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	cout << "ERROR: Framebuffer status NOT_COMPLETE\n";
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	DLight.multDiffuse(2.f);

	////////////////////////////////////////
	// Textures

	//unsigned int tex0 = loadTex("..\\img\\container1.png");
	//unsigned int tex0Spec = loadTex("..\\img\\container1_spec.png");
	//unsigned int tex0Emiss = loadTex("..\\img\\container1_emiss.png");
	//unsigned int flCookie = loadTex("..\\img\\cookie.png");

	unsigned int texContainerDiff = loadTex("..\\img\\container1.png", GL_REPEAT);
	unsigned int texContainerSpec = loadTex("..\\img\\container1_spec.png", GL_REPEAT);
	unsigned int texTile = loadTex("..\\img\\tile.jpg", GL_REPEAT);
	unsigned int texSkybox = loadCubemap("..\\img\\sky0", skyboxFaces);

	////////////////////////////////////////
	// Assets

	//Model tree0("..\\assets\\backpack\\Backpack.obj", false);

	////////////////////////////////////////
	// Shaders

	//default shaders
	Shader shader0("..\\shaders\\Lights.vs", "..\\shaders\\Lights.fs");
	Shader shader0Stencil("..\\shaders\\SingleColor.vs", "..\\shaders\\SingleColor.fs");
	Shader shaderTransp("..\\shaders\\Default.vs", "..\\shaders\\Transparency.fs");
	Shader shaderPostproc("..\\shaders\\Postprocess.vs", "..\\shaders\\Postprocess.fs");
	Shader shaderSkybox("..\\shaders\\Skybox.vs", "..\\shaders\\Skybox.fs");
	Shader shaderModel0("..\\shaders\\Lights.vs", "..\\shaders\\Lights.fs");

	std::vector<Shader*> shaders;
	shaders.push_back(&shader0);
	shaders.push_back(&shader0Stencil);
	shaders.push_back(&shaderTransp);
	shaders.push_back(&shaderPostproc);
	shaders.push_back(&shaderSkybox);
	shaders.push_back(&shaderModel0);

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
		100.0f
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

	//shaderTransp
	//-----------------------------------
	shaderTransp.use();

	//shaderSkybox
	//-----------------------------------
	shaderSkybox.use();
	shaderSkybox.setInt("skyboxTex", 0);

	////////////////////////////////////////
	// Pre-Render
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//depth
	//-----------------------------------
	glEnable(GL_DEPTH_TEST);
	
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
	glEnable(GL_MULTISAMPLE);

	////////////////////////////////////////////
	// RENDER

	glfwSetTime(0);

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
		cam.setDeltaTime(deltaTime);
		
		////////////////////////////////////
		// Camera

		glm::vec3 camPos = cam.getPos();
		glm::vec3 camDir = cam.getDir();
		glm::mat4 camView = cam.getView();

		////////////////////////////////////
		// Shader blocks

		glBindBuffer(GL_UNIFORM_BUFFER, UBOmat);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(projection), sizeof(camView), &camView[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		////////////////////////////////////
		// Framebuffers

		glEnable(GL_DEPTH_TEST);

		glClearColor(1.f, 1.f, 1.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////////////////////////////////////
		// Drawing
		
		glm::mat4 model = glm::mat4(1.f);

		//plane
		shader0.use();
		
		model = glm::mat4(1.f);
		//model = glm::scale(model, glm::vec3(0.25f, 0.f, 0.25f));
		shader0.setMat4("vuModel", model);
		shader0.setVec3("fuViewPos", camPos);

		glBindVertexArray(VAOplane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texTile);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//cubes
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-1.f, 0.f, -1.f));
		shader0.setMat4("vuModel", model);

		glBindVertexArray(VAOcube);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainerDiff);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texContainerSpec);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(1.f, -0.2f, 0.5f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, glm::radians(30.f), glm::vec3(0.f, 1.f, 0.f));

		shader0.setMat4("vuModel", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, 0);

		//grass
		
		for (auto& x : grass)
		{
			x.draw(shaderTransp);
		}
		
		
		//skybox
		glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_FALSE);

		shaderSkybox.use();

		glBindVertexArray(VAOskybox);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texSkybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		//postprocess
		//--------------------------
		
		

		////////////////////////////////////
		glBindVertexArray(0);

		////////////////////////////////////
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shadersLogs(shaders);

	////////////////////////////////////////
	// Memory dealloc

	glDeleteBuffers(1, &VBOcube);
	glDeleteBuffers(1, &VBOplane);
	glDeleteBuffers(1, &VBOgrass);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &VBOskybox);
	glDeleteVertexArrays(1, &VAOcube);
	glDeleteVertexArrays(1, &VAOplane);
	glDeleteVertexArrays(1, &VAOgrass);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &VAOskybox);

	glDeleteFramebuffers(1, &FBO);
	//glDeleteRenderbuffers(1, &RBO);

	glDeleteBuffers(1, &UBOmat);

	shader0.suicide();
	shader0Stencil.suicide();
	shaderModel0.suicide();

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
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

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