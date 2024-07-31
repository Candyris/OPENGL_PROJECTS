// TODO : 12 -> enable GL_depth
#include <iostream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
// glm math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

// Constant Value 
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//typedef
typedef unsigned int u32;


void framebuffer_size_callback(GLFWwindow* window,int width, int height);
void loadTexture(const char* imgPath);

int main()
{
    try 
    {
        if (!glfwInit())
        {
            throw std::string("Failed to Initialize GLFW ");
        }    
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"3D Cube OpenGL",0,0);
        if (!window) // same as window == nullptr
        {
            throw std::string("Failed to Create Window");
        }

        glfwMakeContextCurrent(window);
        // Enable v-sync mine 60 fps native 😅
        glfwSwapInterval(1);
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
        // Glad Init
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::string("Failed to Initialize Glad ");
        }
        
        // Print OpenGL Version we are using on Application / game / app 
        std::cout<<glGetString(GL_VERSION)<<std::endl;
        glEnable(GL_DEPTH_TEST);
        float vertices[] = 
        {
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
        // I think that I have it ;
        Shader shader("Shader/vertex.glsl","Shader/fragment.glsl");
        // <VAO> VERTEX ATTRIBUTE OBJECT
        u32 VAO;
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);

        // <VBO> VERTEX BUFFER OBJECT
        u32 VBO;
        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(const void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(const void*)12);
        glEnableVertexAttribArray(1);

        // texture 
        u32 texture1;
        glGenTextures(1,&texture1);
        glBindTexture(GL_TEXTURE_2D,texture1);
        loadTexture("textures/img.png");
        shader.use();

        int uniTransLoc = glGetUniformLocation(shader.ID,"transform");


        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINES);
        while(!glfwWindowShouldClose(window))
        {
           // glClearColor(std::sin(glfwGetTime()),0.5f,0.3f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model = glm::mat4(1.0f); // identity matrix
            model = glm::rotate(model,(float)glfwGetTime(),glm::vec3(1.0f,0.5f,0.0f));
            glm::mat4 view =glm::mat4(1.0f);
            view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f); 
            glm::mat4 PVM = projection * view * model; 
            glUniformMatrix4fv(uniTransLoc,1,GL_FALSE,glm::value_ptr(PVM));

            // Draw triangle function
            glDrawArrays(GL_TRIANGLES,0,6 * 6);

            glfwPollEvents();
            glfwSwapBuffers(window);
        }

    }
    catch(std::string error)
    {
        std::cout<<"[ERROR] : "<<error<<"!"<<std::endl;
    }

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width, int height)
{
    glViewport(0,0,width,height);
}

void loadTexture(const char *imgPath)
{
    int width, height, nrChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(imgPath,&width,&height,&nrChannel,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    if (!data)
    {
        throw std::string("Failed to Load the Texture : "+ std::string(imgPath));
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
