#include<iostream>
#include<fstream>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<Shader.h>

#include<Texture.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
//Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


//Vertecies in Normalized Device Cooridnates
float vertecies_left[]
{
	// left quad
	// vert positions		// vert colors
	-0.5f, 0.5f, 0.0f,		1.0f,0.0f,0.0f, // 0 top left  
	-0.1f, 0.5f, 0.0f,		0.0f,1.0f,0.0f, // 1 top right
	-0.1f, 0.0f, 0.0f,		0.0f,0.0f,1.0f, // 2 bot right
	-0.5f, 0.0f, 0.0f,		1.0f,1.0f,1.0f  // 3 bot left	 

};
unsigned int indices_left[]
{
	0,1,3,	// left triangle
	1,2,3  // Right triangle
};



float vertecies_right[]
{
	 // Right polygon
	// vert position	// vert colors   // texture coords
	 0.5f, 0.5f, 0.0f,	1.0f,0.0f,0.0f,	 0.5f,1.0f,	// 0 top 
	 0.9f, 0.0f, 0.0f,	0.0f,1.0f,0.0f,	 1.0f,0.5f,	// 1 right
	 0.5f,-0.5f, 0.0f,	0.0f,0.0f,1.0f,  0.5f,0.0f, // 2 bottom
	 0.1f, 0.0f, 0.0f,	1.0f,1.0f,1.0f,  0.0f,0.5f	// 3 left
};


unsigned int indices_right[]
{
	0,2,3,	// left triangle
	0,2,1  // Right triangle
};

float TexCords_right[]
{
	0.5f,1.0f,
	1.0f,0.5f,
	0.5f,0.0f,
	0.0f,0.5f
};



int main() {
	// initialize glfw with opengl versions;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// create GLFW window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Warning!: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return-1;
	}
	glfwMakeContextCurrent(window);
	// register GLFW window resize callback to adjust openGl viewport on window resizes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// initialize glad "We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific"
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Warning!: Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set openGl rendering viewport at position and width/height
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	
	// generate a texture
	


	Texture texture("textures/UVGrid.png", true, true);
	Texture textureTwo("textures/Wallpaper.jpg", true, true);
	
	Shader unlitShader("shaders/simpleVertex.vert", "shaders/unlit.frag");
	Shader unlitBlackShader("shaders/simpleVertex.vert", "shaders/unlitBlack.frag");
	Shader unlitTextureShader("shaders/textureVertex.vert", "shaders/unlitTexture.frag");

	//Shader fallbackShader("shaders/simpleVertex.glslVert", "shaders/fallback.glslFrag");

	// 1. Bind vertex array object first
	// 2. copy vertecies array in openGl vertex buffer buffer (bind and set vertex buffers)
	// 3. specifying and binding the inecies for our polygons to the element buffer to draw triangles using indecies
	// 4. Configure vertex attributes 
	// 5. repeat for other VBO'S and VAO's
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	// 
	
	unsigned int vertexBufferObject_LeftPoly;
	unsigned int vertexArrayObject_LeftPoly;
	unsigned int elementBufferObject_LeftPoly;
	glGenVertexArrays(1, &vertexArrayObject_LeftPoly);
	glGenBuffers(1, &vertexBufferObject_LeftPoly);
	glGenBuffers(1, &elementBufferObject_LeftPoly);


	unsigned int vertexBufferObject_RightPoly;
	unsigned int vertexArrayObject_RightPoly;
	unsigned int elementBufferObject_RightPoly;

	glGenVertexArrays(1, &vertexArrayObject_RightPoly);
	glGenBuffers(1, &vertexBufferObject_RightPoly);
	glGenBuffers(1, &elementBufferObject_RightPoly);


	// LEFT	
	glBindVertexArray(vertexArrayObject_LeftPoly);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_LeftPoly);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies_left), vertecies_left, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject_LeftPoly);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_left), indices_left, GL_STATIC_DRAW);
	// attribute pointers for position data in vertexbuffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// attribute pointers for vertex color data in vertexbuffer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // unbind VAO // not neccecarry here because we bind a different one in the next line but hey 

	
	// RIGHT
	glBindVertexArray(vertexArrayObject_RightPoly);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_RightPoly);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies_right), vertecies_right, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject_RightPoly);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_right), indices_right, GL_STATIC_DRAW);
	// attribute pointers for vertex position data in vertexbuffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// attribute pointers for vertex color data in vertexbuffer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// attribute pointers for texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0); // unbind VAO
	
	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//setting up the correct textures to texture samplers in shader
	unlitTextureShader.use();
	unlitTextureShader.setUniformInt("colorMap", 0);
	unlitTextureShader.setUniformInt("textureMask", 1);
	float mixValue = 0.5f;
	// RenderLoop
	while (!glfwWindowShouldClose(window)) 
	{
		// check input
		processInput(window);

		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		{
			mixValue = mixValue + 0.001f;
			//std::cout << "new Mix Value:  " << mixValue << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			mixValue = mixValue - 0.001f;
			//std::cout << "new Mix Value:  " << mixValue << std::endl;
		}
			

		

		float time = glfwGetTime();
		float sinTime = 0.5f * sin(time*2) +0.5; 
		

		// rendering commands
		glClearColor(0.02f, 0.08f,0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// draw Polygons opaque
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		unlitShader.use(); // glslUseProgram(id)
		unlitShader.setUniformFloat("_sinTime", sinTime);
		
		// first quad opaque
		glBindVertexArray(vertexArrayObject_LeftPoly);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// second quad opaque
		unlitTextureShader.use();
		texture.BindToLocation(0);
		textureTwo.BindToLocation(1);
		
		unlitTextureShader.setUniformFloat("mixValue", mixValue);
		glBindVertexArray(vertexArrayObject_RightPoly);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6); // use if we dont have indecies

		//Draw again in wireframe mode using second shader programm
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		unlitBlackShader.use();
		// first quad wireframe
		glBindVertexArray(vertexArrayObject_LeftPoly);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// second quad wirefram
		glBindVertexArray(vertexArrayObject_RightPoly);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// swap buffers and display new frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// delete all GLFW's recourses
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	// match new window size upon resizing to openGl render area
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
