#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include "stb/stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include "model.hpp"
#include "camera.hpp"

#define WIDTH 1600
#define HEIGHT 800


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true; 

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool isClosed = false;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // for using more texture in same mesh.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader modelShader("C:/Users/ASUS/Desktop/GraphicsProject/res/modelShader"); //change path
    Model ourModel("C:/Users/ASUS/Desktop/GraphicsProject/res/Skull/Skull.obj"); //change path
    
    
    Shader lampShader( "C:/Users/ASUS/Desktop/GraphicsProject/res/lampShader" ); //change path
    
    GLfloat vertices[] =
    {
        // Positions            // Normals              // Texture Coords
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
    };

    glm::vec3 lampPosition = glm::vec3(  2.5f,   0.0f,   0.0f);


    //lamp vbo and vao
    GLuint lampVBO, lampVAO;
    glGenVertexArrays( 1, &lampVAO );
    glGenBuffers( 1, &lampVBO );
    
    glBindBuffer( GL_ARRAY_BUFFER, lampVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    
    glBindVertexArray( lampVAO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )0 );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    glBindVertexArray( 0 );

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
    
    //track the direction of point light
    int light_move_direction = 1; 
    // render loop
    while (!isClosed)
    {

        // Changing the position of point light
        float  max = 2.5;
        float inc = 0.0015;
        if(light_move_direction == 1 && lampPosition.x > 0)
		{
            lampPosition.x -= inc;
            lampPosition.y -= inc;
            if(abs(lampPosition.x) - 0 < inc)
            {
                lampPosition.x = 0;
                lampPosition.y = -max;
                light_move_direction = 2;
            }
		}
        else if(light_move_direction == 2 && lampPosition.y < 0)
		{
            lampPosition.y += inc;
            lampPosition.z += inc;
            if(abs(lampPosition.y) - 0 < inc)
            {
                lampPosition.y = 0;
                lampPosition.z = max;
                light_move_direction = 3;
            }
		}
        else if(light_move_direction == 3 && lampPosition.z > 0)
		{
            lampPosition.x -= inc;
            lampPosition.z -= inc;
            if(abs(lampPosition.z) - 0 < inc)
            {
                lampPosition.x = -max;
                lampPosition.z = 0;
                light_move_direction = 4;
            }
		}
        else if(light_move_direction == 4 && lampPosition.x < 0)
		{
            lampPosition.x += inc;
            lampPosition.y += inc;
            if(abs(lampPosition.x) - 0 < inc)
            {
                lampPosition.x = 0;
                lampPosition.y = max;
                light_move_direction = 5;
            }
		}
        else if(light_move_direction == 5 && lampPosition.y > 0)
		{
            lampPosition.y -= inc;
            lampPosition.z -= inc;
            if(abs(lampPosition.y) - 0 < inc)
            {
                lampPosition.y = 0;
                lampPosition.z = -max;
                light_move_direction = 6;
            }
		}
        else if(light_move_direction == 6 && lampPosition.z < 0)
		{
            lampPosition.x += inc;
            lampPosition.z += inc;
            if(abs(lampPosition.z) - 0 < inc)
            {
                lampPosition.x = max;
                lampPosition.z = 0;
                light_move_direction = 1;
            }
		}

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();

        // lamp shader program
        lampShader.Bind();

        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1);
        glBindVertexArray( lampVAO );
        model = glm::translate(model, lampPosition);
        model = glm::scale(model, glm::vec3( 0.151f, 0.15f, 0.15f));
        modelShader.setMat4("model", model);
        // Draw lamp
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        
        
        // using the created program.
        modelShader.Bind();

        modelShader.getUniformLocation("light.position", lampPosition.x, lampPosition.y, lampPosition.z);
        modelShader.getUniformLocation("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

        modelShader.setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        modelShader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        modelShader.setVec1("light.constant", glm::vec1(0.5f));
        modelShader.setVec1("light.linear", glm::vec1(0.1));
        modelShader.setVec1("light.quadratic", glm::vec1(0.05));

        modelShader.setVec1("material.shininess", glm::vec1(200.0f));

        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        model = glm::translate(model, glm::vec3(0.0f, -20.0f, 0.0f));
        GLfloat angle = 80.0f;
        model = glm::rotate( model, angle, glm::vec3( 1.0f, 0.0f, 0.0f ) );
        modelShader.setMat4("model", model);
        // modelShader.Update(transform);
        ourModel.Draw(modelShader);
    

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &lampVBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwWindowShouldClose(window))
    {
        isClosed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        isClosed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}