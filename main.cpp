// Include necessary headers
#include <SDL_main.h>               // SDL main entry point (needed on some platforms)
#include <SDL.h>                    // SDL for window and event handling
#include <GL/glew.h>                // GLEW for OpenGL function loading
#include <iostream>                 // Standard I/O for debugging and messages
#include <vector>                   // Vector container for storing mesh data
#include <glm/glm.hpp>                  // GLM for matrix operations
#include <glm/gtc/type_ptr.hpp>         // GLM for matrix transformations
#include <glm/gtc/matrix_transform.hpp> // GLM for matrix transformations
#include "Shader.h"      // Custom Shader class for handling GLSL shaders
#include "Mesh.h"        // Custom Mesh class for handling OpenGL mesh rendering

// Jolt physics headers
#include "Jolt/Jolt.h"
#include "Jolt/RegisterTypes.h"
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

// using namespace Jolt;

/**
 * Entry point of the application.
 * Initializes SDL, creates an OpenGL context, sets up shaders and a rotating 2D quad.
 */
int main(int argc, char* argv[]) {
    // --- Initialize SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Initialize only the video subsystem
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // --- Set OpenGL Attributes ---
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);  // Use OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Use core profile

    // --- Create SDL Window ---
    SDL_Window* window = SDL_CreateWindow(
        "Voxel Engine",               // Window title
        SDL_WINDOWPOS_CENTERED,       // Centered X position
        SDL_WINDOWPOS_CENTERED,       // Centered Y position
        800, 600,                     // Width & Height
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // Enable OpenGL rendering
    );

    if (!window) { // Error handling if window creation fails
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // --- Create OpenGL Context ---
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) { // Error handling if context creation fails
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // --- Initialize GLEW ---
    glewExperimental = GL_TRUE; // Allow modern OpenGL extensions
    if (glewInit() != GLEW_OK) { // Check for errors
        std::cout << "GLEW could not initialize!" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // --- Define GLSL Shader Sources ---
    std::string vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos; // Vertex position input

        uniform mat4 mvp; // Rotation angle uniform

        void main() {
            gl_Position = mvp * vec4(aPos, 1.0); // Apply transformation
        }
    )";

    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor; // Output fragment color

        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Set constant color (orange)
        }
    )";

    // --- Compile and Link Shaders ---
    Shader shader(vertexShaderSource, fragmentShaderSource);

    // --- Define 2D Quad Geometry (Square) ---
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f, // 0: Bottom-left-front
         0.5f, -0.5f,  0.5f, // 1: Bottom-right-front
         0.5f,  0.5f,  0.5f, // 2: Top-right-front
        -0.5f,  0.5f,  0.5f, // 3: Top-left-front
        // Back face
        -0.5f, -0.5f, -0.5f, // 4: Bottom-left-back
         0.5f, -0.5f, -0.5f, // 5: Bottom-right-back
         0.5f,  0.5f, -0.5f, // 6: Top-right-back
        -0.5f,  0.5f, -0.5f  // 7: Top-left-back
    };

    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2,  2, 3, 0,
        // Right
        1, 5, 6,  6, 2, 1,
        // Back
        5, 4, 7,  7, 6, 5,
        // Left
        4, 0, 3,  3, 7, 4,
        // Top
        3, 2, 6,  6, 7, 3,
        // Bottom
        0, 1, 5,  5, 4, 0
    };

    // --- Create Mesh Object ---
    Mesh cube(vertices, indices);

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.01f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 2.0f), // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // Look at origin
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );
    glm::mat4 model = glm::mat4(1.0f);

    // Camera setup
    float cameraX = 0.0f, cameraY = 0.0f, cameraZ = -5.0f; // Initial position
    float moveSpeed = 0.01f; // Movement speed per frame

    // --- Main Rendering Loop ---
    bool running = true;
    SDL_Event event;
    float angle = 0.0f; // Angle for rotation animation
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    while (running) {
        // Handle events (polling input events)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { // If user closes the window
                running = false;
            }
        }

        // Update camera position based on keyboard input
        if (keyboardState[SDL_SCANCODE_W])      cameraZ += moveSpeed; // Up arrow
        if (keyboardState[SDL_SCANCODE_S])      cameraZ -= moveSpeed; // Down arrow
        if (keyboardState[SDL_SCANCODE_D])      cameraX += moveSpeed; // Right arrow
        if (keyboardState[SDL_SCANCODE_A])      cameraX -= moveSpeed; // Left arrow
        if (keyboardState[SDL_SCANCODE_SPACE])  cameraY += moveSpeed; // Space (move forward)
        if (keyboardState[SDL_SCANCODE_LSHIFT]) cameraY -= moveSpeed; // Left Shift (move back)

        model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 mvp = projection * view * model;

        // --- Render Frame ---
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color (dark teal)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        // Activate shader program
        shader.use();

        // Pass rotation angle to shader
        shader.setMat4("mvp", mvp);

        // Draw the cube (quad)
        cube.draw();

        // Swap buffers to display the rendered frame
        SDL_GL_SwapWindow(window);

        // Increment angle for animation (rotates over time)
        angle += 0.0025f;
    }

    // --- Cleanup OpenGL and SDL Resources ---
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
