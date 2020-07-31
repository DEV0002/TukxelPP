#pragma once
#ifndef MESH
#define MESH

#include <vector>
#include <string>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    const char* type;
    const char* path;
};

struct Mesh {
public:
    Vertex* vertices;
    unsigned int* indicies;
    unsigned int length;
    std::vector<Texture> textures;
	unsigned int VAO;

private:
    unsigned int VBO, EBO;

public:
	
	Mesh(Vertex vertices[], unsigned int indicies[], std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indicies = indicies;
        this->textures = textures;
        length = sizeof(this->vertices) / sizeof(Vertex);

        SetupMesh();
	}

    Mesh(Vertex vertices[], std::vector<Texture> textures) {
        this->vertices = vertices;
        length = sizeof(this->vertices) / sizeof(Vertex);
        indicies = new unsigned int[length];
        for (int i = 0; i < length; i++)
            indicies[i] = i;
        this->textures = textures;

        SetupMesh();
    }

    Mesh(Vertex vertices[], unsigned int indicies[], std::vector<const char*> textures) {
        this->vertices = vertices;
        this->indicies = indicies;
        length = sizeof(this->vertices) / sizeof(Vertex);
        for (int i = 0; i < textures.size(); i++) {
            Texture text = Texture();
            text.path = textures[i];
            text.type = "texture";
            bindTexture(text.id, text.path);
            this->textures.push_back(text);
        }

        SetupMesh();
    }

    Mesh(Vertex vertices[], std::vector<const char*> textures) {
        this->vertices = vertices;
        length = sizeof(this->vertices) / sizeof(Vertex);
        indicies = new unsigned int[length];
        for (int i = 0; i < length; i++)
            indicies[i] = i;
        for (int i = 0; i < textures.size(); i++) {
            Texture text;
            text.path = textures[i];
            text.type = "texture";
            bindTexture(text.id, text.path);
            this->textures.push_back(text);
        }

        SetupMesh();
    }

    void Draw(Shader& shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number = std::to_string(1);
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

    void Dispose() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        delete[] vertices;
        delete[] indicies;
    }

private:

    int bindTexture(unsigned int& ID, const char* location) {
        //Init Texture
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        //Load Texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load(location, &width, &height,
            &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load texture" << std::endl;
            return -1;
        }
        if (!(width && (!(width & (width - 1)))) || !(height && (!(height & (height - 1))))) {
            std::cout << "Texture at \"" << location << "\" is not power of 2" << std::endl;
            return -1;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return 0;
    }

    void SetupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), &indicies[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoords));

        glBindVertexArray(0);
    }
};

#endif