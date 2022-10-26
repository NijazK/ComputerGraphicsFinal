#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL.glew.h> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>

using namespace std;

/*Shader Program*/
#ifndef GLSL
#define GLSLfiVersion, Source) "#version "#version "core\n"#Source
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// user-defined graphics rendering
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440, layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1
out vec4 vertexColor; // variable to transfer color data to the fragment shader

out vec2 vertexTextureCoordinate;

void mainf()
{
	  gl_Position = vec4(position.x, position.y, position.z, 1.0);    
	  vertexTextureCoordinate = textureCoordinate;
}
};

/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSLfi440, 
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main(
{    
	fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering
}
);

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{    
	for (int j = 0; j < height / 2; ++j)    
	{        
		int index1 = j * width * channels;        
		int index2 = (height - 1 - j) * width * channels;        
		
		for (int i = width * channels; i > 0; --i)        
		{            
			unsigned char tmp = image[index1];            
			image[index1] = image[index2];            
			image[index2] = tmp;            
			++index1;            
			++index2;        
			}    
		}
}

// Load texture (relative to project's directory)    
const char* texFilename = "../../resources/textures/brick.png";    
if (!UCreateTexture(texFilename, gTextureId))
 {        
	 cout << "Failed to load texture " << texFilename << endl;        
	 return EXIT_FAILURE;    
	}

// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)    
glUseProgram(gProgramId);    
// We set the texture as texture unit 0    
glUniform1i(glGetUniformLocation(gProgramId, "uTexture"), 0);

//Calculate Ambient lighting*/    
float ambientStrength = 0.1f; // Set ambient or global lighting strength    
vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

//Calculate Diffuse lighting*/    
vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit    
vec3 lightDirection = normalize(lightPos - vertexFragmentPos); 
float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light    
vec3 diffuse = impact * lightColor; // Generate diffuse light color

//Calculate Specular lighting*/    
float specularIntensity = 0.8f; // Set specular light strength    
float highlightSize = 16.0f; // Set specular highlight size    
vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction    
vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector    
//Calculate specular component    
float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);    
vec3 specular = specularIntensity * specularComponent * lightColor

// Calculate phong result    
vec3 phong = (ambient + diffuse + specular) * objectColor;    
fragmentColor = vec4(phong, 1.0f); // Send lighting results to GPU});

// Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Subject position and scale    
glm::vec3 gCubePosition(0.0f, 0.0f, 0.0f);
glm::vec3 gCubeScale(2.0f);

// Cube light color
glm::vec3 gObjectColor(0.6f, 0.5f, 0.75f);    
glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);    
glm::vec3 gLightColor(0.5f, 4.0f, 1.0f);

// Light position and scale
glm::vec3 gLightPosition(2.5f, -1.0f, 5.0f);    
glm::vec3 gLightScale(0.3f);

// Stores the GL data relative to a given mesh    
struct GLMesh    
{        
	GLuint vao;         // Handle for the vertex array object        
	GLuint vbo;         // Handle for the vertex buffer object        
	GLuint nVertices;    // Number of indices of the mesh    
};

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Applying brick textures to Pyramid", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create the mesh
	UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	void UProcessInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// Functioned called to render a frame
void URender()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);
    
    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. Scales the object by 2
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
    // 2. Rotates shape by 15 degrees in the x axis
    glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(1.0, 1.0f, 1.0f));
    // 3. Place object at the origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Transforms the camera: move the camera back (z axis)
    glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f));

    // Creates a orthographic projection
    glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Implements the UCreateMesh function
void UCreateMesh(GLMesh &mesh)
{
     // Position and Color data
	 GLfloat verts[] = {        
		    
			// First side of pyramid
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,        
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,        
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,        
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,        
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			// Top layer slopes
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,        
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,        
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f

			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,        
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,        
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,        
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

			// Second side of pyramid
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,        
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,        
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,        
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			// Middle layer slope vertices
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 

			 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,        
			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,        
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f,        
			 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			 // Third side of pyramid
			 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,        
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,        
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,        
			 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			 // Bottom layer vertice slopes
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f,        
			 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,           

			 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			 -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f

		};

		// textureing for the base layer
		float basetexcoords[] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f

		}

		// texturing for the sides
		float sidetexcoords[] = {
			// Top layer texturing
			0.5f, 0.5f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Middle layer texturing
			0.5f, 0.5f,
			1.0f, 0.0f,
			0.0f, 0.0f,

			// Bottom layer texturing
			0.5f, 0.5f,
			1.0f, 0.0f,
			0.0f, 0.0f
		}

		const GLuint floatsPerVertex = 7;    
		const GLuint floatsPerColor = 2;    
		const GLuint floatsPerUV = 2;




	// positions of all pyramid vertices
	glm::vec3 pyramidPositions[] = {
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
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.2f,  2.0f),
		glm::vec3(2.2f, -3.6f, -4.2f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	
	// first, configure the Pyramid's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(mesh.vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	//unsigned int diffuseMap = loadTexture("container2.png");
	//unsigned int specularMap = loadTexture("container2_specular.png");

	void UDestroyMesh(GLMesh &mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// Added Directional lighting to enhance graphics
		lightingShader.setVec3("dirLight.direction", -0.1f, 1.0f, -0.3f);
		lightingShader.setVec3("dirLight.direction", -0.1f, 1.0f, -0.3f);
		lightingShader.setVec3("dirLight.direction", -0.1f, 1.0f, -0.3f);
		
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// Added specular lighting from the left direction
		lightingShader.setVec3("pointLights[0].specular", -2.0f, 1.0f, 1.0f);
		lightingShader.setVec3("pointLights[0].specular", -2.0f, 1.0f, 1.0f);
		
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// Added Diffuse lighting and ambient lighting
		lightingShader.setVec3("pointLights[1].diffuse", 1.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].diffuse", 1.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].diffuse", 1.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].ambient", 2.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].ambient", 2.05f, 0.05f, 0.05f);
		
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// Point light 3 has added matrices to enhance the lighting direction
		lightingShader.setFloat("pointLights[2].linear", 0.13);
		lightingShader.setFloat("pointLights[2].constant", -2.0f);
		lightingShader.setVec3("pointLights[1].ambient", 1.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 1.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", -2.0f, 1.0f, 1.0f);
		
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		
		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// render containers
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object(s)
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{    
	switch (button)    
	{
		case GLFW_MOUSE_BUTTON_LEFT:    
		{        
			if (action == GLFW_PRESS)            
			cout << "Left mouse button pressed" << endl;        
			else            
			cout << "Left mouse button released" << endl;    
		}    
		break;    
		
		case GLFW_MOUSE_BUTTON_MIDDLE:    
		{        
			if (action == GLFW_PRESS)            
			cout << "Middle mouse button pressed" << endl;        
			else            
			cout << "Middle mouse button released" << endl;    
		}    
		break;    
		
		case GLFW_MOUSE_BUTTON_RIGHT:    
		{        
			if (action == GLFW_PRESS)
			cout << "Right mouse button pressed" << endl;        
			else            
			cout << "Right mouse button released" << endl;    
		}    
		break;    
		
		default:        
		cout << "Unhandled mouse button event" << endl;
		break;    
		}
	}

	// mouse scroll
	void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{    
		gCamera.ProcessMouseScroll(yoffset);
	}
	
	// Mouse button handle
	void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{    
		switch (button)    
		{    
			case GLFW_MOUSE_BUTTON_LEFT:    
		{        
			if (action == GLFW_PRESS)            
			    cout << "Left mouse button pressed" << endl;        
			else            
				cout << "Left mouse button released" << endl;
			}    
			break;    
			
			case GLFW_MOUSE_BUTTON_MIDDLE:    
			{        
				if (action == GLFW_PRESS)            
				 cout << "Middle mouse button pressed" << endl;        
				else            
				 cout << "Middle mouse button released" << endl;    
				 }    
				 break;    
				 
				 case GLFW_MOUSE_BUTTON_RIGHT:    
				 {        
					 if (action == GLFW_PRESS)            
					 cout << "Right mouse button pressed" << endl;        
					 else            
					 cout << "Right mouse button released" << endl;    
				 }    
				 break;    
				 
				 default:        
				 cout << "Unhandled mouse button event" << endl;        
				 break;    
				 }
				}

	// Functioned called to render a frame
	void URender()
	{    
		// Enable z-depth    
		glEnable(GL_DEPTH_TEST);    
		
		// Clear the frame and z buffers    
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);    
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
		
		// Activate the cube VAO (used by cube and lamp)    
		glBindVertexArray(gMesh.vao);    
		
		// CUBE: draw cube    
		//----------------    
		// Set the shader to be used    
		glUseProgram(gCubeProgramId);    
		
		// Model matrix: transformations are applied right-to-left order    
		glm::mat4 model = glm::translate(gCubePosition) * glm::scale(gCubeScale);    
		
		// camera/view transformation    
		glm::mat4 view = gCamera.GetViewMatrix();

		// Creates a perspective projection    
		glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);    
		// Retrieves and passes transform matrices to the Shader program    
		GLint modelLoc = glGetUniformLocation(gCubeProgramId, "model");    
		GLint viewLoc = glGetUniformLocation(gCubeProgramId, "view");    
		GLint projLoc = glGetUniformLocation(gCubeProgramId, "projection");    
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));    
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));    
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));    
		
		// Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position    
		GLint objectColorLoc = glGetUniformLocation(gCubeProgramId, "objectColor");    
		GLint lightColorLoc = glGetUniformLocation(gCubeProgramId, "lightColor");    
		GLint lightPositionLoc = glGetUniformLocation(gCubeProgramId, "lightPos");    
		GLint viewPositionLoc = glGetUniformLocation(gCubeProgramId, "viewPosition");    
		
		// Pass color, light, and camera data to the Cube Shader program's corresponding uniforms    
		glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);    
		glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);    
		glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);    
		const glm::vec3 cameraPosition = gCamera.Position;    
		glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);    
		
		// Draws the triangles    
		glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

		// LAMP: draw lamp    
		//----------------    
		glUseProgram(gLampProgramId);    
		
		//Transform the smaller cube used as a visual que for the light source    
		model = glm::translate(gLightPosition) * glm::scale(gLightScale);    
		
		// Reference matrix uniforms from the Lamp Shader program    
		modelLoc = glGetUniformLocation(gLampProgramId, "model");    
		viewLoc = glGetUniformLocation(gLampProgramId, "view");    
		projLoc = glGetUniformLocation(gLampProgramId, "projection");    
		
		// Pass matrix data to the Lamp Shader program's matrix uniforms    
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));    
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));    
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));    
		glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);    
		
		// Deactivate the Vertex Array Object and shader program    
		glBindVertexArray(0);    
		glUseProgram(0);    
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)    
		glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
	}

	const GLuint floatsPerVertex = 3;    
	const GLuint floatsPerNormal = 3;
	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal));

	glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time    
	glBindVertexArray(mesh.vao);

	 // Create 2 buffers: first one for the vertex data; second one for the indices    
	 glGenBuffers(1, &mesh.vbo);    
	 glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer    
	 glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinatedata to the GPU

	 // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.    
	 GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal);// The number of floats before each    
	 
	 // Create Vertex Attribute Pointers    
	 glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);    
	 glEnableVertexAttribArray(0);    
	 
	 glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));    
	 glEnableVertexAttribArray(1);
	 }
	 
	 void UDestroyMesh(GLMesh& mesh)
	 {    
		 glDeleteVertexArrays(1, &mesh.vao);    
		 glDeleteBuffers(1, &mesh.vbo);
		 
		}

	/*Generate and load the texture*/
	bool UCreateTexture(const char* filename, GLuint& textureId)
	{    
		int width, height, channels;    
		unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);    
		if (image)    
		{        
			flipImageVertically(image, width, height, channels);        
			
			glGenTextures(1, &textureId);        
			glBindTexture(GL_TEXTURE_2D, textureId);       
			
			// set the texture wrapping parameters        
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        
			
			// set texture filtering parameters        
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);        
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
			
			if (channels == 3)            
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);        
			
			else if (channels == 4)            
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);        
			
			else        
			{
				cout << "Not implemented to handle image with " << channels << " channels" << endl;            
				return false;        
			}        
			
			glGenerateMipmap(GL_TEXTURE_2D);        
			
			stbi_image_free(image);        
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture        
			return true;    
			}    
			
			// Error loading the image    
			return false;
			}
			
			void UDestroyTexture(GLuint textureId)
			{    
				glGenTextures(1, &textureId);
			}
			
			// Implements the UCreateShaders function
			bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
			{    
				// Compilation and linkage error reporting    
				int success = 0
				char infoLog[512];    
				
				// Create a Shader program object.    
				programId = glCreateProgram();    
				
				// Create the vertex and fragment shader objects    
				GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);    
				GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);    
				
				// Retrive the shader source    
				glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);    
				glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);    
				
				// Compile the vertex shader, and print compilation errors (if any)    
				glCompileShader(vertexShaderId); // compile the vertex shader    
				// check for shader compile errors    
				glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);    
				if (!success)    
				{        
					glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);        
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;        
					
					return false;    
				}    
				
				glCompileShader(fragmentShaderId); // compile the fragment shader    
				// check for shader compile errors    
				glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);    
				if 
				{        
					glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);        
					std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;        
					
					return false;    
				}    
				
				// Attached compiled shaders to the shader program    
				glAttachShader(programId, vertexShaderId);    
				glAttachShader(programId, fragmentShaderId);    
				glLinkProgram(programId);   // links the shader program    
				
				// check for linking errors    
				glGetProgramiv(programId, GL_LINK_STATUS, &success);    
				if (!success)    
				{        
					glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);        
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;        
					return false;    
				}    
				
				glUseProgram(programId);    // Uses the shader program    
				return true;
				}
				
				void UDestroyShaderProgram(GLuint programId)(!success)
				{    
					glDeleteProgram(programId);
				}