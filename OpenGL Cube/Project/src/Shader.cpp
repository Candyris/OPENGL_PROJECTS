#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

enum ERROR {VERTEX,FRAGMENT,PROGRAM};

std::string readFile(const char* Path)
{
    std::stringstream ss;
    std::fstream stream(Path);
    if (!stream.is_open())
    {
        throw std::string("Failed to Load File \nFILE::PATH : "+ std::string(Path));
    }
    ss << stream.rdbuf();
    stream.close();
    return ss.str();
}
void ShaderError(unsigned int ID, ERROR errorType)
{
    switch(errorType)
    {
        int result;
        char infoLog[512];
        case ERROR::VERTEX :
            glGetShaderiv(ID,GL_COMPILE_STATUS,&result);
            if (!result)
            {
                glGetShaderInfoLog(ID,512,nullptr,infoLog);
                throw std::string("SHADER::VERTEX::COMPILE : \n" + std::string(infoLog));
            }
        break;                
        case ERROR::FRAGMENT :
            glGetShaderiv(ID,GL_COMPILE_STATUS,&result);
            if (!result)
            {
                glGetShaderInfoLog(ID,512,nullptr,infoLog);
                throw std::string("SHADER::FRAGMENT::COMPILE : \n" + std::string(infoLog));
            }
        break;            
        case ERROR::PROGRAM :
            glGetProgramiv(ID,GL_LINK_STATUS,&result);
            if (!result)
            {
                glGetProgramInfoLog(ID,512,nullptr,infoLog);
                throw std::string("SHADER::PROGRAM::LINKING : \n" + std::string(infoLog));
            }
        break;            

    }
}


Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // File Handling
    std::string vString = readFile(vertexPath);
    std::string fString = readFile(fragmentPath);

    const char* vRaw = vString.c_str();
    const char* fRaw = fString.c_str();    

    // Shader Compile
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs,1,&vRaw,nullptr);
    glShaderSource(fs,1,&fRaw,nullptr);

    glCompileShader(vs);
    glCompileShader(fs);
    
    // error
    ShaderError(vs,VERTEX);
    ShaderError(fs,FRAGMENT);
    // Program Linking 
    ID = glCreateProgram();

    glAttachShader(ID,vs);
    glAttachShader(ID,fs);

    glLinkProgram(ID);
    ShaderError(ID,PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(fs);

}

Shader::~Shader()
{
    glDeleteShader(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}
