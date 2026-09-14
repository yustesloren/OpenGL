#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Función para mantener la relación de aspecto fija al redimensionar la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Mantiene la proporción 1:1 para evitar que los cuadrados se distorsionen
    int length = width < height ? width : height;
    glViewport((width - length) / 2, (height - length) / 2, length, length);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0 ) in vec3 aPos;\n"
"void main ()\n"
"{\n"
"   gl_Position= vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Grilla de Cuadrados", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Registrar el callback de cambio de tamaño
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gladLoadGL();

    // GENERACIÓN AUTOMÁTICA DE LOS CUADRADOS
    
    std::vector<GLfloat> vertices;
    int filas = 3;
    int columnas = 3;

    float inicioX = -0.75f;
    float inicioY = -0.75f;
    float tamanoCuadro = 0.5f; // Tamaño de cada celda en espacio NDC

    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas; c++)
        {
            float x1 = inicioX + c * tamanoCuadro;
            float y1 = inicioY + f * tamanoCuadro;
            float x2 = x1 + tamanoCuadro;
            float y2 = y1 + tamanoCuadro;

            // Segmento 1: Abajo
            vertices.insert(vertices.end(), { x1, y1, 0.0f,  x2, y1, 0.0f });
            // Segmento 2: Derecha
            vertices.insert(vertices.end(), { x2, y1, 0.0f,  x2, y2, 0.0f });
            // Segmento 3: Arriba
            vertices.insert(vertices.end(), { x2, y2, 0.0f,  x1, y2, 0.0f });
            // Segmento 4: Izquierda
            vertices.insert(vertices.end(), { x1, y2, 0.0f,  x1, y1, 0.0f });
        }
    }
    // -------------------------------------------------------------

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Dibuja dinámicamente el número de vértices generados
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size() / 3));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
	// UNA CLASE OARA EL CUADRADO,Y UN ARRAY EN FILAS 
