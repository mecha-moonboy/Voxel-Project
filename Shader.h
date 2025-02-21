// Prevents multiple inclusions of this header file, which can cause redefinition errors
#ifndef SHADER_H  
#define SHADER_H  

// Includes the OpenGL Extension Wrangler Library (GLEW), 
// which helps manage OpenGL extensions for rendering graphics
#include <GL/glew.h>

// Includes the C++ Standard Library string class, 
// used to handle shader source code as text
#include <string>

#include <glm/glm.hpp> // GLM for matrix operations
#include <glm/gtc/type_ptr.hpp> // GLM for matrix transformations

/**
 * The `Shader` class manages OpenGL shaders.
 * Shaders are small programs that run on the GPU, 
 * processing how objects are drawn and colored on the screen.
 * 
 * This class handles:
 * - Compiling shader source code
 * - Linking vertex and fragment shaders into a shader program
 * - Activating the shader program for rendering
 * - Setting uniform variables (such as colors, transformations, etc.)
 */
class Shader {
public:
    /**
     * Constructor: Compiles and links a shader program using 
     * provided vertex and fragment shader source code.
     * 
     * @param vertexSource   A string containing the GLSL code for the vertex shader.
     * @param fragmentSource A string containing the GLSL code for the fragment shader.
     */
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    /**
     * Destructor: Cleans up the shader program when the object is destroyed.
     */
    ~Shader();

    /**
     * Activates the shader program, making it the currently used shader in OpenGL.
     * This must be called before rendering objects that use this shader.
     */
    void use() const;

    /**
     * Sets a float uniform variable in the shader program.
     * 
     * @param name  The name of the uniform variable in the shader code.
     * @param value The float value to be assigned to the uniform variable.
     */
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    /** The OpenGL ID of the compiled and linked shader program */
    GLuint programID;

    /**
     * Checks for compilation errors in a shader.
     * If there are errors, it prints debug information to help fix them.
     * 
     * @param shader The OpenGL ID of the shader being checked.
     * @param type   The type of shader ("VERTEX" or "FRAGMENT") for error messages.
     */
    void checkShaderCompileErrors(GLuint shader, const std::string& type);

    /**
     * Checks for linking errors in the shader program.
     * If there are errors, it prints debug information to help fix them.
     * 
     * @param program The OpenGL ID of the shader program being checked.
     */
    void checkProgramLinkErrors(GLuint program);
};

#endif  // Ends the conditional inclusion directive
