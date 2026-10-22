
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH, SCR_HEIGHT, "Computer Graphics: Lab09.1-Cube", NULL, NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("vShader.vs", "fShader.glsl");

    float vertices[] = {
        // Position              // Texture coordinates
        -0.5f,-0.5f,-0.5f,       0.0f,0.0f,
         0.5f,-0.5f,-0.5f,       1.0f,0.0f,
         0.5f, 0.5f,-0.5f,       1.0f,1.0f,
         0.5f, 0.5f,-0.5f,       1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,       0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,       0.0f,0.0f,

        -0.5f,-0.5f, 0.5f,       0.0f,0.0f,
         0.5f,-0.5f, 0.5f,       1.0f,0.0f,
         0.5f, 0.5f, 0.5f,       1.0f,1.0f,
         0.5f, 0.5f, 0.5f,       1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,       0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,       0.0f,0.0f,

        -0.5f, 0.5f, 0.5f,       1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,       1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,       0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,       0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,       0.0f,0.0f,
        -0.5f, 0.5f, 0.5f,       1.0f,0.0f,

         0.5f, 0.5f, 0.5f,       1.0f,0.0f,
         0.5f, 0.5f,-0.5f,       1.0f,1.0f,
         0.5f,-0.5f,-0.5f,       0.0f,1.0f,
         0.5f,-0.5f,-0.5f,       0.0f,1.0f,
         0.5f,-0.5f, 0.5f,       0.0f,0.0f,
         0.5f, 0.5f, 0.5f,       1.0f,0.0f,

        -0.5f,-0.5f,-0.5f,       0.0f,1.0f,
         0.5f,-0.5f,-0.5f,       1.0f,1.0f,
         0.5f,-0.5f, 0.5f,       1.0f,0.0f,
         0.5f,-0.5f, 0.5f,       1.0f,0.0f,
        -0.5f,-0.5f, 0.5f,       0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,       0.0f,1.0f,

        -0.5f, 0.5f,-0.5f,       0.0f,1.0f,
         0.5f, 0.5f,-0.5f,       1.0f,1.0f,
         0.5f, 0.5f, 0.5f,       1.0f,0.0f,
         0.5f, 0.5f, 0.5f,       1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,       0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,       0.0f,1.0f
    };

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1 = loadTexture("wall3.jpg");

    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.08f, 0.03f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        ourShader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        // หมุนลูกบาศก์รอบแกน X
        model = glm::rotate(
            model,
            (float)glfwGetTime(),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        view = glm::translate(
            view,
            glm::vec3(0.0f, 0.0f, -3.0f)
        );

        projection = glm::perspective(
            glm::radians(60.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f
        );

        GLint modelLoc = glGetUniformLocation(ourShader.ID, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.ID, "view");
        GLint projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(
        path, &width, &height, &nrChannels, 0
    );

    if (data)
    {
        GLenum format = GL_RGB;

        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(
            GL_TEXTURE_2D, 0, format,
            width, height, 0,
            format, GL_UNSIGNED_BYTE, data
        );

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Loaded texture: " << path << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        return 0;
    }

    return textureID;
}
