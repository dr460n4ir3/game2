#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 32;
const int GRID_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

// Define the grid and player position
std::vector<std::vector<char>> grid(GRID_HEIGHT, std::vector<char>(GRID_WIDTH, '.'));
int playerX = GRID_WIDTH / 2;
int playerY = GRID_HEIGHT / 2;

// Time tracking for player speed
double lastMoveTime = 0.0;
const double moveDelay = 0.2; // Delay in seconds between moves

void renderCharacter(float x, float y, char c) {
    glRasterPos2f(x, y);
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

void renderGrid() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            float xpos = x * TILE_SIZE / (float)SCREEN_WIDTH * 2.0f - 1.0f;
            float ypos = 1.0f - y * TILE_SIZE / (float)SCREEN_HEIGHT * 2.0f;

            // Set color based on tile type
            switch (grid[y][x]) {
            case '#': glColor3f(0.5f, 0.5f, 0.5f); break; // Wall: Gray
            case 'B': glColor3f(0.0f, 1.0f, 1.0f); break; // Building: Cyan
            case '.': glColor3f(0.0f, 0.7f, 0.0f); break; // Grass: Dark Green
            default: glColor3f(1.0f, 1.0f, 1.0f); break;  // Default: White
            }

            renderCharacter(xpos, ypos, grid[y][x]);
        }
    }

    // Render the player on top of the grid
    float playerXpos = playerX * TILE_SIZE / (float)SCREEN_WIDTH * 2.0f - 1.0f;
    float playerYpos = 1.0f - playerY * TILE_SIZE / (float)SCREEN_HEIGHT * 2.0f;
    glColor3f(1.0f, 0.5f, 0.0f); // Player: Orange
    renderCharacter(playerXpos, playerYpos, '@');

    glFlush();
}

void handleInput(GLFWwindow* window) {
    double currentTime = glfwGetTime();
    if (currentTime - lastMoveTime < moveDelay) {
        return; // Skip movement if delay has not passed
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && playerY > 0) {
        --playerY;
        lastMoveTime = currentTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && playerY < GRID_HEIGHT - 1) {
        ++playerY;
        lastMoveTime = currentTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && playerX > 0) {
        --playerX;
        lastMoveTime = currentTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && playerX < GRID_WIDTH - 1) {
        ++playerX;
        lastMoveTime = currentTime;
    }

    // Map editing keys
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        grid[playerY][playerX] = '.'; // Grass
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        grid[playerY][playerX] = '#'; // Wall
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        grid[playerY][playerX] = 'B'; // Building
    }
}

int main(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tile Map Editor", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Initialize GLUT for text rendering
    glutInit(&argc, argv);

    while (!glfwWindowShouldClose(window)) {
        handleInput(window);
        renderGrid();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
