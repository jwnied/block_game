#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>

glm::vec2 triangleOffset = glm::vec2(0.0f, 0.0f);

// Shader compilation function
GLuint compileShader(const char* path, GLenum type) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    const char* shaderCode = code.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float moveSpeed = 0.05f;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        triangleOffset.y += moveSpeed;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        triangleOffset.y -= moveSpeed;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        triangleOffset.x -= moveSpeed;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        triangleOffset.x += moveSpeed;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Register the Callback with GLFW
    glfwSetKeyCallback(window, key_callback);

    // Load OpenGL using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set viewport size and callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // ==============================
    // Compile Shaders
    // ==============================
    GLuint vertexShader = compileShader("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader("shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ==============================
    // Define Triangle Data (Position + Texture)
    // ==============================
    float vertices[] = {
        // First triangle - positions      // texture coords
         0.0f,  0.5f, 0.0f,   0.5f, 1.0f,  // Top center
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  // Bottom left
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  // Bottom right
    
        // Second triangle - positions     // texture coords
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  // Top left
        -1.0f, -0.5f, 0.0f,   0.0f, 0.0f,  // Bottom left
         0.0f, -0.5f, 0.0f,   1.0f, 0.0f   // Bottom right
    };
    

    GLuint VAO1, VBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint VAO2, VBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(15 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(18 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // ==============================
    // Load Texture
    // ==============================
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Texture wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/brick.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture 1\n";
    }
    stbi_image_free(data);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load("textures/metal.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture 2\n";
    }
    stbi_image_free(data);

    // After linking shader
    GLuint overlayColorLoc = glGetUniformLocation(shaderProgram, "overlayColor");
    if (overlayColorLoc == -1) {
        std::cerr << "Failed to find uniform: overlayColor" << std::endl;
    }

    //Update Triangle Position
    GLuint offsetLoc = glGetUniformLocation(shaderProgram, "offset");

    // ==============================
    // Render Loop
    // ==============================
    while (!glfwWindowShouldClose(window)) {

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // First triangle (moving one)
        glUseProgram(shaderProgram);
        glUniform4f(overlayColorLoc, 1.0f, 0.0f, 0.0f, 0.5f);
        glUniform2f(offsetLoc, triangleOffset.x, triangleOffset.y);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Second triangle (stationary)
        glUseProgram(shaderProgram);
        glUniform4f(overlayColorLoc, 0.0f, 0.0f, 1.0f, 0.75f);
        glUniform2f(offsetLoc, 0.0f, 0.0f); // Keep this one fixed
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ==============================
    // Cleanup
    // ==============================
    glDeleteVertexArrays(1, &VAO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}