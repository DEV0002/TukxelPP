#include "shader.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>

Shader::Shader(std::string vertPath, std::string fragPath)
{
    unsigned      vertex, fragment;
    std::ifstream file;
    std::string   text;
    char          infoLog[512];
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    {    // Vertex Shader
        // Read file
        file.open(vertPath, std::ios::in | std::ios::binary | std::ios::ate);
        text.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&text[0], text.size());
        file.close();

        // Compile
        vertex            = glCreateShader(GL_VERTEX_SHADER);
        const char *c_str = text.c_str();
        glShaderSource(vertex, 1, &c_str, NULL);
        glCompileShader(vertex);

        // Error checking
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
            return;
        }
    }

    {    // Fragment Shader
        // Read file
        file.open(fragPath, std::ios::in | std::ios::binary | std::ios::ate);
        text.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&text[0], text.size());
        file.close();

        // Compile
        fragment          = glCreateShader(GL_FRAGMENT_SHADER);
        const char *c_str = text.c_str();
        glShaderSource(fragment, 1, &c_str, NULL);
        glCompileShader(fragment);

        // Error checking
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
            return;
        }
    }

    {    // Shader Program
        program = std::make_shared<unsigned>(glCreateProgram());

        // Link Vertex & Fragment Shaders
        glAttachShader(*program, vertex);
        glAttachShader(*program, fragment);
        glLinkProgram(*program);

        // Error checking
        glGetProgramiv(*program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(*program, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
            return;
        }
    }

    // Clean-up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    success = true;
}

Shader::~Shader()
{
    if (success && program.unique()) glDeleteProgram(*program);
}

void Shader::Use()
{
    glUseProgram(*program);
}