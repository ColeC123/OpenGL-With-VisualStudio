#include "glad/glad.h"
#include "Shaders.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* shaderFileToString(char* path) {
    char tempFileBuff[150];
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        printf("Shader file does not exist or was incorrectly specified\n");
        exit(-1);
    }

    fseek(file, 0L, SEEK_END);
    int fileSize = (int)ftell(file);
    char* shaderSource = (char*)malloc(fileSize);
    if (shaderSource == NULL) {
        printf("Failed to allocate memory for shaderSource String in shaderFileToString function\n");
        exit(-1);
    }
    fseek(file, 0L, SEEK_SET);
    int fileCount = 0;
    while (fgets(tempFileBuff, 150, file)) {
        for (int i = 0; tempFileBuff[i] != '\0' && fileCount < fileSize - 1; i++) {
            shaderSource[fileCount] = tempFileBuff[i];
            fileCount++;
        }
    }
    shaderSource[fileCount] = '\0';
    fclose(file);

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

