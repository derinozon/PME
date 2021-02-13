#pragma once

#include <fstream>
#include <stdio.h>

using namespace PME;

#if PORT == 0
const char* vpath = "res/shaders/web/vertex.glsl";
const char* fpath = "res/shaders/web/fragment.glsl";
#else
const char* vpath = "res/shaders/vertex.glsl";
const char* fpath = "res/shaders/fragment.glsl";
#endif

const char* ReadFile (const char* path) {
	std::FILE* file = fopen(path, "r");

	if (!file) {
		printf("file not found\n");
		return NULL;
	}

	fseek(file, 0L, SEEK_END);
	size_t numbytes = ftell(file);

	fseek(file, 0L, SEEK_SET);	

	char* buffer = (char*)calloc(numbytes, sizeof(char));	
	if (!buffer) {
		printf("buffer errors\n");
		return NULL;
	}
	fread(buffer, sizeof(char), numbytes, file);
	fclose(file);
	
	return buffer;
}

const char* vertexShaderSource = ReadFile(vpath);
const char* fragmentShaderSource = ReadFile(fpath);

int shaderProgram;
unsigned int VBO, VAO;

int CreateShader (GLenum shaderType, const char* shaderSource) {
	int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::string stype = (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
		std::string msg = stype + " shader compilation failed";
        Internal::LogError(msg);
		Internal::LogError(infoLog);
    }
	return shader;
}

int CompileShaders () {
	shaderProgram = glCreateProgram();

	int vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Internal::LogError("Shader linking failed");
		Internal::LogError(infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	return 0;
}

void glstart () {
	CompileShaders();
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
        0.5f, -0.5f, 0.0f, // right 
        0.0f,  0.5f, 0.0f  // top   
    }; 

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void glupdate () {
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}