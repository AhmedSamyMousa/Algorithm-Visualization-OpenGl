#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
std::vector<int> array(150);
bool sorting = true;

bool initOpenGL(GLFWwindow*& window) {
    if (!glfwInit()) return false;
    window = glfwCreateWindow(WIDTH, HEIGHT, "Sort Visualization", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

void drawBars(const std::vector<int>& arr, int highlightIdx1 = -1, int highlightIdx2 = -1) {
    glClear(GL_COLOR_BUFFER_BIT);
    float barWidth = 2.0f / arr.size();

    for (size_t i = 0; i < arr.size(); ++i) {
        if (i == highlightIdx1 || i == highlightIdx2) glColor3f(0.0f, 1.0f, 1.0f);
        else glColor3f(0.0f, 0.9f, 0.0f);

        float x = -1.0f + i * barWidth;
        float height = arr[i] / 500.0f;

        glBegin(GL_QUADS);
        glVertex2f(x, -1.0f);
        glVertex2f(x + barWidth, -1.0f);
        glVertex2f(x + barWidth, -1.0f + height);
        glVertex2f(x, -1.0f + height);
        glEnd();
    }
    glfwSwapBuffers(glfwGetCurrentContext());
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
            drawBars(arr, i, j);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    std::swap(arr[i + 1], arr[high]);
    drawBars(arr, i + 1, high);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void renderLoop(GLFWwindow* window) {
    std::srand(static_cast<unsigned>(std::time(0)));

    for (int k = 0; k < 5; ++k) {
        array.clear();
        for (int i = 0; i < 150; ++i) {
            array.push_back(std::rand() % 500 + 1);
        }

        drawBars(array);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        quickSort(array, 0, array.size() - 1);
        sorting = false;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

int main() {
    GLFWwindow* window;
    if (!initOpenGL(window)) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return -1;
    }

    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    renderLoop(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
