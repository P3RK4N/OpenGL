#include "PerkanTools.h"

int main()
{	
	GLfloat a = 1.0f;
	GLfloat vertices[] =
	{ //	POSITION				COLOR			UV
		-a/2, -a/2, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-a/2, a/2, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		a/2, -a/2, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		a/2, a/2, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f
	};
	GLuint indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
									//name,version,dimensions
	GLFWwindow* window = initialize((char *)"PerkanGL", 4, 6, 800, 800);

	//Load glad so it configures OpenGL
	gladLoadGL();

	//Specify viewport of openGL in window
	//x = [0,800] y = [0,800]
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();
	VAO vao;
	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));

	vao.Bind();
	//Linking position
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (void *)0);
	//Linking color
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	//Linking uv data
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	GLfloat background[] =
	{
		0.4f, 0.7f, 0.1f, 1.0f
	};


	//Texture
	Texture t1("gigachad.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_LINEAR, GL_REPEAT);

	//Uniform values
	GLuint timeID = glGetUniformLocation(shaderProgram.ID, "time");
	GLuint tex0ID = glGetUniformLocation(shaderProgram.ID, "tex0");
	glUniform1i(tex0ID, 0);

	//This was in while loop
	//shaderProgram.Activate();
	vao.Bind();
	ebo.Bind();

	//Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//Background color
		background[0] = 0.4f + 0.2 * sin(glfwGetTime());
		glClearColor(background[0],background[1],background[2],background[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		//SHADER PROGRAM
		glUniform1f(timeID, glfwGetTime());

		//VAO; EBO
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		//GLFW
		//Take care of all glfw events
		glfwSwapBuffers(window); 
		glfwPollEvents();
	}


	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	shaderProgram.Delete();
	t1.Delete();


	//Close window, terminate process
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}