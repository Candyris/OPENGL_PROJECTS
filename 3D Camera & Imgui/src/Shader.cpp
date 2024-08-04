#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <string>
#include <sstream>

enum ERROR {VERTEX,FRAGMENT,PROGRAM};

std::string readFile(const char* filePath)
{
    std::stringstream ss;
    std::fstream stream(filePath);
    if (!stream.is_open())
    {
        throw std::string("failed to Open the File \n[File_Path] : "+ std::string(filePath));
    }
    ss << stream.rdbuf();
    stream.close();
    return ss.str();
}

void ShaderError(unsigned int ID, ERROR errorType)
{
    int result;
    char infoLog[512];

    switch(errorType)
    {
    case ERROR::VERTEX :
        glGetShaderiv(ID,GL_COMPILE_STATUS,&result);
        if (!result)
        {
            glGetShaderInfoLog(ID,512,nullptr,infoLog);
            throw std::string("SHADER::VERTEX : COMPILE_FAILED : \n[SHADER ERROR] : "+ std::string(infoLog));
        }
    break;
    case ERROR::FRAGMENT :
         glGetShaderiv(ID,GL_COMPILE_STATUS,&result);
        if (!result)
        {
            glGetShaderInfoLog(ID,512,nullptr,infoLog);
            throw std::string("SHADER::FRAGMENT : COMPILE_FAILED : \n[SHADER ERROR] : "+ std::string(infoLog));
        }
    break;
    case ERROR::PROGRAM :
         glGetProgramiv(ID,GL_LINK_STATUS,&result);
        if (!result)
        {
            glGetProgramInfoLog(ID,512,nullptr,infoLog);
            throw std::string("SHADER::PROGRAM : LINKING_FAILED : \n[PROGRAM ERROR] : "+ std::string(infoLog));
        }
    break;
    }
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vString = readFile(vertexPath);
    std::string fString = readFile(fragmentPath);    

    const char* vRaw = vString.c_str();
    const char* fRaw = fString.c_str();

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs,1,&vRaw,nullptr);
    glShaderSource(fs,1,&fRaw,nullptr);

    glCompileShader(vs);
    glCompileShader(fs);

    ShaderError(vs,ERROR::VERTEX);
    ShaderError(fs,ERROR::FRAGMENT);

    ID = glCreateProgram();
    glAttachShader(ID,vs);
    glAttachShader(ID,fs);
    glLinkProgram(ID);

    ShaderError(ID,ERROR::PROGRAM);
    glDeleteShader(vs);
    glDeleteShader(fs);

}

void Shader::use()
{
    glUseProgram(ID);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}
