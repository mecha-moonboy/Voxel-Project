// Includes the corresponding header file to access the Shader class declaration
#include "Shader.h"

// Includes standard I/O for printing error messages to the console
#include <iostream>

#include <glm/glm.hpp> // GLM for matrix operations
#include <glm/gtc/type_ptr.hpp> // GLM for matrix transformations

/**
 * Constructor: Compiles the vertex and fragment shaders, links them into a shader program, 
 * and cleans up temporary shader objects.
 * 
 * @param vertexSource   A string containing the GLSL code for the vertex shader.
 * @param fragmentSource A string containing the GLSL code for the fragment shader.
 */
Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    // --- Compile Vertex Shader ---
    
    // Create a new vertex shader object in OpenGL
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Convert the C++ string to a C-style string (char pointer) for OpenGL
    const char* vertexSrc = vertexSource.c_str();
    
    // Attach the source code to the shader object and compile it
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    // Check for compilation errors
    checkShaderCompileErrors(vertexShader, "VERTEX");

    // --- Compile Fragment Shader ---
    
    // Create a new fragment shader object in OpenGL
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Convert the C++ string to a C-style string
    const char* fragmentSrc = fragmentSource.c_str();
    
    // Attach the source code to the shader object and compile it
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    // Check for compilation errors
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    // --- Link the Shader Program ---
    
    // Create a new shader program object
    programID = glCreateProgram();

    // Attach the compiled vertex and fragment shaders to the program
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    // Link the program to create an executable shader pipeline
    glLinkProgram(programID);

    // Check for linking errors
    checkProgramLinkErrors(programID);

    // --- Cleanup Temporary Shader Objects ---
    
    // The shaders are now linked into the program, so we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

/**
 * Destructor: Cleans up the shader program when the object is destroyed.
 */
Shader::~Shader() {
    // Deletes the shader program from OpenGL memory
    glDeleteProgram(programID);
}

/**
 * Activates the shader program, making it the currently used shader in OpenGL.
 * This must be called before rendering objects that use this shader.
 */
void Shader::use() const {
    glUseProgram(programID);
}

/**
 * Sets a float uniform variable in the shader program.
 * 
 * @param name  The name of the uniform variable in the shader code.
 * @param value The float value to be assigned to the uniform variable.
 */
void Shader::setFloat(const std::string& name, float value) const {
    // Gets the location of the uniform variable in the shader program
    GLint location = glGetUniformLocation(programID, name.c_str());

    // Assigns the provided float value to the uniform variable
    glUniform1f(location, value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

/**
 * Checks for shader compilation errors and prints an error message if compilation fails.
 * 
 * @param shader The OpenGL ID of the shader being checked.
 * @param type   The type of shader ("VERTEX" or "FRAGMENT") for error messages.
 */
void Shader::checkShaderCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    
    // Query the shader object to check if it compiled successfully
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    // If compilation failed, retrieve and print the error log
    if (!success) {
        GLchar infoLog[512];  // Buffer for error messages
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

/**
 * Checks for shader program linking errors and prints an error message if linking fails.
 * 
 * @param program The OpenGL ID of the shader program being checked.
 */
void Shader::checkProgramLinkErrors(GLuint program) {
    GLint success;

    // Query the shader program to check if linking was successful
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    // If linking failed, retrieve and print the error log
    if (!success) {
        GLchar infoLog[512];  // Buffer for error messages
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}
