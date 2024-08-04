#pragma region INCLUDE_NEEDED_OPENGL
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>

#include <imgui/imgui.h>
#pragma endregion

#include <imgui/imgui.h>
// TODO : Initialize Imgui

// own include
#include "Shader.h"
#include "UI.h"

// Constant Value
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// Camera Vector
glm::vec3 g_CameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 g_CameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 g_CameraUp(0.0f, 1.0f, 0.0f);

// Delta Time
float lastFrame = 0.0f;
float deltaTime = 0.0f;

// Functions
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double x, double y);

// functions
void updateDeltaTime();
void loadTexture(const char *path);

// mouse camera rotation variable
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float yaw = -90.0f;
float pitch = 0.0f;

bool mouse_Rotation = false;

int main()
{
    try
    {
        if (!glfwInit())
        {
            throw std::string("Failed to Initialize GLFW ");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Modern OpneGL ", 0, 0);
        if (!window)
        {
            throw std::string("Failed to Create Window ");
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);

        // enable v-sync
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc(glfwGetProcAddress))))
        {
            throw std::string("Failed to Initialize Glad Modern OpenGL");
        }

        // fix something don't change
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouse_Rotation = true;
        // checking if the Glad has load the modern OpenGL
        std::cout << glGetString(GL_VERSION) << std::endl;
        glEnable(GL_DEPTH_TEST);
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
            };

        Shader shader("Shader/vert.glsl", "Shader/frag.glsl");
        shader.use();
        // VAO
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        loadTexture("textures/img.jpg");

        // MAtrix uniform lcoation
        int UniMatLoc = glGetUniformLocation(shader.ID, "u_PVM");

        // UI
        UI::Init();
        UI::glInit(window, "#version 330");
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        bool show_demo_window = true;
        bool show_another_window = false;

        // ImGui Varible
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glm::vec3 PositionVec3(1.0f,0.0f,0.0f);
        glm::vec3 ScaleVec3(0.5f,0.5f,0.5f);

        // Render Loop
        while (!glfwWindowShouldClose(window))
        {
            UI::newFrame();
            updateDeltaTime();
            processInput(window);
            glClearColor(clear_color.x,clear_color.y,clear_color.z,clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,PositionVec3);
            model = glm::scale(model,ScaleVec3);
            //camera 
            glm::mat4 view = glm::lookAt(g_CameraPosition, g_CameraPosition + g_CameraFront, g_CameraUp);
            
            // projection 
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
            
            // PVM
            glm::mat4 PVM = projection * view * model;
            glUniformMatrix4fv(UniMatLoc, 1, GL_FALSE, glm::value_ptr(PVM));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            ImGui::Begin("Editor");
            ImGui::SliderFloat3("Position ", &PositionVec3.x,0.0f, 3.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::InputFloat3("Scale ", &ScaleVec3.x); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("Clear Color", (float *)&clear_color);             // Edit 3 floats representing a color
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        
        // Rendering
        UI::render();

            glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
catch (std::string error)
{
    std::cout << "[ERROR] : " << error << " !" << std::endl;
    glfwTerminate();
    return -1;
}
glfwTerminate();
return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    float CameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        g_CameraPosition += g_CameraFront * CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        g_CameraPosition -= g_CameraFront * CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        g_CameraPosition += glm::normalize(glm::cross(g_CameraPosition, g_CameraUp)) * CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        g_CameraPosition -= glm::normalize(glm::cross(g_CameraPosition, g_CameraUp)) * CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        g_CameraPosition += g_CameraUp * CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        g_CameraPosition -= g_CameraUp * CameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (mouse_Rotation)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
           // glfwSetCursorPosCallback(window, mouse_callback);
            mouse_Rotation = false;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
           // glfwSetCursorPosCallback(window, mouse_callback);
            mouse_Rotation = true;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double x, double y)
{
    if(mouse_Rotation){return ;}
    float PosX = static_cast<float>(x);
    float PosY = static_cast<float>(y);
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float offsetX = PosX - lastX;
    float offsetY = lastY - PosY;

    lastX = x;
    lastY = y;

    float Sensitivity = 0.1f;
    offsetX *= Sensitivity;
    offsetY *= Sensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 Front;
    Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front.y = sin(glm::radians(pitch));
    Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    g_CameraFront = glm::normalize(Front);
}

void updateDeltaTime()
{
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;
}

void loadTexture(const char *path)
{
    int width, height, nrChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannel, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (!data)
    {
        throw std::string("FILE::TEXTURE : FAILED_IMG_LOADING \n[Path] : " + std::string(path));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
