#ifndef SHADER_H
#define SHADER_H    

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "transform.hpp"

class Shader
{
public:
    Shader(const std::string& fileName);

    void Bind();
    void Update(const Transform& transform);

    virtual ~Shader();

    // Utility uniform functions.
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec1(const std::string &name, const glm::vec1 &vec) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void getUniformLocation(const std::string &name, float x, float y, float z);


private:
    static const unsigned int NUM_SHADERS = 2;

    enum {
        TRANSFORM_U,

        NUM_UNIFORMS
    };

    std::string LoadShader(const std::string& fileName);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    GLuint CreateShader(const std::string& text, unsigned int type);

    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];

};

#endif