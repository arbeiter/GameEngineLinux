#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    ALCdevice *device;
    device = alcOpenDevice(NULL);
    if(!device) {
      std::cout << "FAILURE";
      return -1;
    }
    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXIT");
    if(enumeration == AL_FALSE) {
      std::cout << "ENUMERATION FALSE";
    } else {
      std::cout << "ENUMERATION TRUE";
    }
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
