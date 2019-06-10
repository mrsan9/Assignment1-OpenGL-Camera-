#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <camera.h>
#include <iostream>
#include <list> 
#include <iterator> 
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void UIRender();

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 960;

typedef glm::vec4 vec4;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;



enum displayMode {
	point = 0,
	wireframe,
	shaded
};

enum view {
	perspective=0,
	orthographic
};

//Camera movement values


float randVal;
//delta time calc values
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//rotaiton with mouse values
bool firstMouse = true;
Camera camera(glm::vec3(0.5f, 3.0f, 9.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;


vec3 objPos,objRot,objScale;
string currCamera = "Perspective";
string currDisMode = "Shaded";
ImVec4 clear_color;
displayMode currDMode=shaded;
view currView=perspective;
int main()
{
	objPos = objRot = vec3(0.0,2.0f,0.0f) ; objScale = vec3(1.0);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Camera Opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window,mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew initioalization failed!" << std::endl;
		return -1;
	}


	//888888 IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//8888888888

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	
	Shader myShader("vShader.sh","fShader.sh");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
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
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f   // top left 
	};
	
	/*glm::vec3 cubePositions[] = {
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
	};*/
	
	
	/*glm::vec3 cubePositions[4][4];
	for(int i =0;i<4 ;i++)
		for (int j = 0; j < 4; ++j)
		{
			cubePositions[i][j] = vec3((float)i*1.5f, (float)j*1.5f,3.0f);
		}*/

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	int tex1 = myShader.loadTexture("vista.jpg",false);
	int tex2 = myShader.loadTexture("dexter.jpg",true);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	myShader.use();
	myShader.setInt("Texture1", 0);
	myShader.setInt("Texture2",1);


	// uncomment this call to draw in wireframe polygons.
	
	
	
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	

     clear_color = ImVec4(0.329412f, 0.329412f, 0.329412f, 1.0f);
	// render loop
	// -----------


	while (!glfwWindowShouldClose(window))
	{
		float currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;
		// input
		// -----
		if(currDMode == shaded)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else if(currDMode == wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		
			
		processInput(window);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// 1. Show a simple window.
	   // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		UIRender();
		//glClearColor(0.0f,1.0f,0.5f,1.0f);
		// render
		// ------
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			float timeValue = glfwGetTime();

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex1);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, tex2);
			glEnable(GL_DEPTH_TEST);

			//glm::mat4 model = glm::mat4(1.0f);

			//glm::mat4 projection = glm::ortho(-6.0f, 6.0f, 6.0f, -6.0f, 0.1f, 100.0f);
			glm::mat4 projection;
			if (currView == perspective)
				 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			else
				 projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, 0.1f, 100.0f);

			glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			view = camera.GetViewMatrix();

			//float radius = 10.0f;
			//float camX = sin(glfwGetTime()) * radius;
			//float camZ = cos(glfwGetTime()) * radius;
			//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			myShader.setMat4("view", view);

			myShader.setMat4("projection", projection);

			// draw our first triangle
			myShader.use();

			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model,objScale);
			model = glm::translate(model,objPos);
			// angle = 20.0f * i + 10;
			model = glm::rotate(model, timeValue*glm::radians(objRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, timeValue*glm::radians(objRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, timeValue*glm::radians(objRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			myShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			/*for (GLint i = 0; i < 4; ++i)
				for (GLint j = 0; j < 4; ++j)
				{

					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, cubePositions[i][j]);
					float angle = 20.0f * i + 10;
					//model = glm::rotate(model, timeValue*glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.5f));
					myShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}*/
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		// glBindVertexArray(0); // no need to unbind it every time 
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
void UIRender()
{
	{
		ImGui::Begin("Inspector:");

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::PushID("Transform");


			// Create multiple items in a same cell before switching to next column
			//if (ImGui::TreeNode("Mixed items"))
			{				
				
				if (ImGui::TreeNode("Position")) {
										
					static float xPos = 1.0f;
					ImGui::Columns(3, "Position");
					ImGui::Separator();
					ImGui::InputFloat("  X ", &xPos, 0.09f, 0, "%.3f");
					
					ImGui::NextColumn();
					static float yPos = 1.0f;
					ImGui::InputFloat("  Y ", &yPos, 0.05f, 0, "%.3f");
					
					ImGui::NextColumn();
					static float zPos = 1.0f;
					ImGui::InputFloat("  Z ", &zPos, 0.05f, 0, "%.3f");
					objPos = vec3(xPos,yPos,zPos);
					ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::TreePop();
				}											
				
				if (ImGui::TreeNode("Rotation")) {

					static float xPos = 1.0f;
					ImGui::Columns(3, "Rotation");
					ImGui::Separator();
					ImGui::InputFloat("  X ", &xPos, 0.09f, 0, "%.3f");

					ImGui::NextColumn();
					static float yPos = 1.0f;
					ImGui::InputFloat("  Y ", &yPos, 0.05f, 0, "%.3f");

					ImGui::NextColumn();
					static float zPos = 1.0f;
					ImGui::InputFloat("  Z ", &zPos, 0.05f, 0, "%.3f");
					objRot = vec3(xPos, yPos, zPos);
					ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Scale")) {

					static float xPos = 1.0f;
					ImGui::Columns(3, "Scale");
					ImGui::Separator();
					ImGui::InputFloat("  X ", &xPos, 0.09f, 0, "%.3f");

					ImGui::NextColumn();
					static float yPos = 1.0f;
					ImGui::InputFloat("  Y ", &yPos, 0.05f, 0, "%.3f");

					ImGui::NextColumn();
					static float zPos = 1.0f;
					ImGui::InputFloat("  Z ", &zPos, 0.05f, 0, "%.3f");
					objScale = vec3(xPos, yPos, zPos);
					ImGui::NextColumn();
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::TreePop();
				}
				
			}

			// Word wrapping

			ImGui::PopID();
		}

		ImGui::Separator();
		if (ImGui::CollapsingHeader("View"))
		{
			ImGui::PushID("View");


			// Create multiple items in a same cell before switching to next column
			//if (ImGui::TreeNode("Mixed items"))
			{
				
				ImGui::Text("Set camera view:");
				if (ImGui::Button("Perspective")) {
					currCamera = "Perspective";
					currView = perspective;
				}
				ImGui::Spacing();
				if (ImGui::Button("Orthographic")) {
					currCamera = "Orthographic";
					currView = orthographic;
				}				
				ImGui::Spacing();
				ImGui::Spacing();
				string viewCam = "Current View: ";
				viewCam += currCamera;
				ImGui::Text(&viewCam[0]);
				
				ImGui::Separator();
				//ImGui::TreePop();
			}

			// Word wrapping

			ImGui::PopID();
		}

		ImGui::Separator();
		if (ImGui::CollapsingHeader("Display Mode"))
		{
			ImGui::PushID("Display Mode");


			// Create multiple items in a same cell before switching to next column
			//if (ImGui::TreeNode("Mixed items"))
			{
				ImGui::Text("Set display mode:");
				if (ImGui::Button("Shaded")) {
					currDisMode = "Shaded";
					currDMode = shaded;
				}
				ImGui::Spacing();
				if (ImGui::Button("Wireframe")) {
					currDisMode = "Wireframe";
					currDMode = wireframe;
				}
				ImGui::Spacing();
				if (ImGui::Button("Point")) {
					currDisMode = "Point";
					currDMode = point;
				}
				ImGui::Spacing();
				ImGui::Spacing();
				string disStr = "Current Display Mode: ";
				disStr += currDisMode;
				ImGui::Text(&disStr[0]);

				ImGui::Separator();
				//ImGui::TreePop();
			}

			// Word wrapping

			ImGui::PopID();
		}

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Background color:");
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Reset Camera")) {
			camera.camReset();
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("* W A S D to move");
		ImGui::Text("* Mouse to rotate");
		ImGui::Text("* Use UI to move object");

		ImGui::End();
	}


	
	{
		//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		//ImGui::ShowDemoWindow();
	}
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	float cameraSpeed = 3.5 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.camReset();
	}	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

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

	camera.ProcessMouseMovement(xoffset,yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);	
}

