// Includes the corresponding header file to access the Mesh class declaration
#include "Mesh.h"

/**
 * Constructor: Initializes the mesh by setting up the Vertex Array Object (VAO),
 * Vertex Buffer Object (VBO), and Element Buffer Object (EBO).
 * 
 * @param vertices A vector of floating-point values representing vertex positions.
 * @param indices  A vector of unsigned integers representing the order of vertices in drawing.
 */
Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Calls a helper function to generate and bind buffers, and configure vertex attributes
    setupMesh(vertices, indices);
}

/**
 * Destructor: Cleans up allocated OpenGL objects when the mesh is destroyed.
 */
Mesh::~Mesh() {
    // Deletes the VAO (Vertex Array Object)
    glDeleteVertexArrays(1, &VAO);

    // Deletes the VBO (Vertex Buffer Object)
    glDeleteBuffers(1, &VBO);

    // Deletes the EBO (Element Buffer Object)
    glDeleteBuffers(1, &EBO);
}

/**
 * Draws the mesh by binding its VAO and calling OpenGL’s draw function.
 */
void Mesh::draw() const {
    // Bind the VAO (which contains all vertex and index data)
    glBindVertexArray(VAO);

    // Draws the mesh using indexed drawing (GL_TRIANGLES mode means each 3 indices form a triangle)
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // Unbind the VAO after drawing (optional, but good practice)
    glBindVertexArray(0);
}

/**
 * Sets up the mesh data by creating buffers and defining how vertex data is interpreted.
 * 
 * @param vertices A vector of floating-point values representing vertex positions.
 * @param indices  A vector of unsigned integers representing the order of vertices in drawing.
 */
void Mesh::setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Store the number of indices for later use in drawing
    indexCount = indices.size();

    // Generate OpenGL objects: a VAO, a VBO, and an EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // --- Configure VAO ---
    glBindVertexArray(VAO);

    // --- Upload Vertex Data to VBO ---
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertex data into the buffer (GL_STATIC_DRAW suggests the data won't change frequently)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // --- Upload Index Data to EBO ---
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Copy index data into the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // --- Define Vertex Attribute Layout ---
    
    // Configure how OpenGL should interpret the vertex data:
    // Attribute index 0 -> 3 floats per vertex (x, y, z), no normalization, tightly packed
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Enable the attribute so OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Unbind the VBO (optional, but a good practice)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO to prevent accidental modification
    glBindVertexArray(0);
}
