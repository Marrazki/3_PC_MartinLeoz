#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class App {
public:
    App();
    ~App();

    void run();

private:
    GLFWwindow* window;
    // App.h
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float velX = 0.5f; // velocidad en unidades NDC / seg
    float velY = 0.3f;
    float halfSize = 0.5f; // la mitad del tamaño del cuadrado
    double lastTime = 0.0;

    unsigned int VBOSquare, EBOSquare, VAOSquare, shaderProgramSquare;
    unsigned int VBOTriangle, EBOTriangle, VAOTriangle, shaderProgramTriangle;
    unsigned int textureID;
    void init();
    void mainLoop();
    void cleanup();
    void initTexture();



    void initSquare();
    void initTriangle();
    unsigned int initShaders(std::string fileFragment, std::string fileVertex);
    std::string loadShaderSource(const std::string& path);
};
