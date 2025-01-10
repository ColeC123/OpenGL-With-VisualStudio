#include "glad/glad.h"
#include "Shaders.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

char* shaderFileToString(char* path) {
    //Have to open file in binary mode because fseek's behavior in text mode doesn't work properly
    FILE* fptr = fopen(path, "rb");
    if (fptr == NULL) {
        printf("Failed to open file\n");
        exit(-1);
    }

    fseek(fptr, 0L, SEEK_END);
    int fileSize = (int)ftell(fptr) / sizeof(char);
    rewind(fptr);

    char* shaderSource = (char*)malloc((fileSize + 1) * sizeof(char));
    if (shaderSource == NULL) {
        printf("Failed to allocate memory for string\n");
        exit(-1);
    }
    fread(shaderSource, sizeof(char), fileSize, fptr);
    shaderSource[fileSize] = '\0';

    fclose(fptr);

    return shaderSource;
}

int shaderProgramCreate(Shader* program, char* vertexPath, char* fragmentPath) {
    int success;
    char infoLog[512];

    *program = glCreateProgram();

    char* shaderSource = shaderFileToString(vertexPath);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderSource, NULL);
    glCompileShader(vertexShader);
    free(shaderSource);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        exit(-1);
    }

    shaderSource = shaderFileToString(fragmentPath);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSource, NULL);
    glCompileShader(fragmentShader);
    free(shaderSource);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        exit(-1);
    }

    glAttachShader(*program, vertexShader);
    glAttachShader(*program, fragmentShader);
    glLinkProgram(*program);

    glGetProgramiv(*program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*program, 512, NULL, infoLog);
        printf("ERROR::SHADER_PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}

