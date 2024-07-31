#pragma once

class Shader
{
    public:
    unsigned int ID;
    Shader(const char* vertexPath,const char* fragmentPath);
    ~Shader();
    void use();
public:
    // later Uniform

};