#include "App.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <sstream>

App::App() {
    init();
}

App::~App() {
    cleanup();
}

void App::init() {
    if (!glfwInit()) {
        std::cerr << "Error iniciando GLFW\n";
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "Triángulo con índices", NULL, NULL);
    if (!window) {
        std::cerr << "Error creando ventana\n";
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error iniciando GLAD\n";
        exit(-1);
    }

    glViewport(0, 0, 800, 800);

    shaderProgramSquare = initShaders("../shaders/texture.fs", "../shaders/basic.vs");
    shaderProgramTriangle = initShaders("../shaders/basic.fs", "../shaders/basic.vs");
    initTexture();
    initTriangle();
    initSquare();
}

std::string App::loadShaderSource(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el shader: " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int App::initShaders(std::string fileFragment, std::string fileVertex) {
    std::string vertexCode = loadShaderSource(fileVertex);
    std::string fragmentCode = loadShaderSource(fileFragment);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Error compilando Vertex Shader:\n" << infoLog << "\n";
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Error compilando Fragment Shader:\n" << infoLog << "\n";
    }

    unsigned int result = glCreateProgram();
    glAttachShader(result, vertexShader);
    glAttachShader(result, fragmentShader);
    glLinkProgram(result);

    glGetProgramiv(result, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(result, 512, nullptr, infoLog);
        std::cerr << "Error linkeando Shader Program:\n" << infoLog << "\n";
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return result;
}

void App::initSquare() {
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAOSquare);
    glGenBuffers(1, &VBOSquare);
    glGenBuffers(1, &EBOSquare);

    glBindVertexArray(VAOSquare);

    glBindBuffer(GL_ARRAY_BUFFER, VBOSquare);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOSquare);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
void App::initTriangle() {
    std::cout << "Inicializacion triangle " << std::endl;
    float vertices[] = {
        // positions          // colors           // texture coords
         -0.5f,  -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 
         0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
    };

    glGenVertexArrays(1, &VAOTriangle);
    glGenBuffers(1, &VBOTriangle);
    glGenBuffers(1, &EBOTriangle);

    glBindVertexArray(VAOTriangle);

    glBindBuffer(GL_ARRAY_BUFFER, VBOTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTriangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
void App::initTexture()
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../assets/textures/grassBlock.jpg", &width, &height, &nrChannels, 0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(textureID, "texture1"), 0);
}

void App::run() {
    mainLoop();
}

void App::mainLoop() {
    std::cout << "Main Loop" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramTriangle);

        int offsetLoc = glGetUniformLocation(shaderProgramTriangle, "offset");
        glUniform2f(offsetLoc, 0.5f, 0.5f);
        glBindVertexArray(VAOTriangle);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgramTriangle);
        offsetLoc = glGetUniformLocation(shaderProgramTriangle, "offset");
        glUniform2f(offsetLoc, -0.5f, -0.5f);
        glBindVertexArray(VAOTriangle);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgramTriangle);
        offsetLoc = glGetUniformLocation(shaderProgramTriangle, "offset");
        glUniform2f(offsetLoc, -0.5f, 0.5f);
        glBindVertexArray(VAOTriangle);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        glUseProgram(shaderProgramSquare);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glUniform1i(glGetUniformLocation(shaderProgramSquare, "texture1"), 0);
        offsetLoc = glGetUniformLocation(shaderProgramSquare, "offset");
        glUniform2f(offsetLoc, 0.5f, -0.5f);
        glBindVertexArray(VAOSquare);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::cleanup() {
    glDeleteVertexArrays(1, &VAOSquare);
    glDeleteBuffers(1, &VBOSquare);
    glDeleteBuffers(1, &EBOSquare);
    glDeleteProgram(shaderProgramSquare);

    glfwDestroyWindow(window);
    glfwTerminate();
}
