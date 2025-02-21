// Prevents multiple inclusions of this header file, which can cause redefinition errors
#ifndef MESH_H  
#define MESH_H  

// Includes the OpenGL Extension Wrangler Library (GLEW), 
// which helps manage OpenGL extensions for rendering graphics
#include <GL/glew.h>

// Includes the vector container from the C++ Standard Library, 
// used to store dynamic arrays of data
#include <vector>

/**
 * The `Mesh` class represents a 3D mesh in OpenGL.
 * A mesh is a collection of vertices (points in 3D space) 
 * and indices (which define how these points connect to form triangles).
 */
class Mesh {
public:
    /**
     * Constructor: Initializes a mesh using given vertices and indices.
     * 
     * @param vertices A list of floating-point numbers representing 
     *                the positions, colors, or texture coordinates of the mesh's vertices.
     * @param indices  A list of unsigned integers representing 
     *                how the vertices should be connected to form triangles.
     */
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    /**
     * Destructor: Cleans up GPU resources when the mesh object is destroyed.
     */
    ~Mesh();

    /**
     * Renders the mesh on the screen using OpenGL.
     * This function binds the necessary buffers and issues a draw call.
     */
    void draw() const;

private:
    // OpenGL handles for storing mesh data in GPU memory

    /** Vertex Array Object (VAO) - Stores the structure of the mesh */
    GLuint VAO;

    /** Vertex Buffer Object (VBO) - Stores the actual vertex data (positions, colors, etc.) */
    GLuint VBO;

    /** Element Buffer Object (EBO) - Stores the indices that define how vertices form triangles */
    GLuint EBO;

    /** The number of indices used for rendering */
    unsigned int indexCount;

    /**
     * Sets up the mesh by sending vertex and index data to the GPU.
     * 
     * @param vertices The vertex data to be uploaded.
     * @param indices  The index data to be uploaded.
     */
    void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};

#endif  // Ends the conditional inclusion directive
