#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <cmath>
using namespace std;

// Input coordinates and settings
int startX, startY, endX, endY, lineWidth;
int drawMode = 1; // 1 = Normal line, 2 = Bold line

// Plot a pixel on screen
void drawPixel(int x, int y) {
    glVertex2i(x, y);
}

// Standard Bresenham algorithm
void drawBasicLine(int xStart, int yStart, int xEnd, int yEnd) {
    bool isSteep = abs(yEnd - yStart) > abs(xEnd - xStart);
    if (isSteep) swap(xStart, yStart), swap(xEnd, yEnd);
    if (xStart > xEnd) swap(xStart, xEnd), swap(yStart, yEnd);

    int dx = xEnd - xStart;
    int dy = abs(yEnd - yStart);
    int error = dx / 2;
    int yStep = (yStart < yEnd) ? 1 : -1;
    int y = yStart;

    for (int x = xStart; x <= xEnd; ++x) {
        if (isSteep)
            drawPixel(y, x);
        else
            drawPixel(x, y);

        error -= dy;
        if (error < 0) {
            y += yStep;
            error += dx;
        }
    }
}

// Bresenham for bold (thick) line
void drawBoldLine(int xStart, int yStart, int xEnd, int yEnd, int width) {
    bool isSteep = abs(yEnd - yStart) > abs(xEnd - xStart);
    if (isSteep) swap(xStart, yStart), swap(xEnd, yEnd);
    if (xStart > xEnd) swap(xStart, xEnd), swap(yStart, yEnd);

    int dx = xEnd - xStart;
    int dy = abs(yEnd - yStart);
    int error = dx / 2;
    int yStep = (yStart < yEnd) ? 1 : -1;
    int y = yStart;

    for (int x = xStart; x <= xEnd; ++x) {
        for (int i = -width / 2; i <= width / 2; ++i) {
            if (isSteep)
                drawPixel(y + i, x);
            else
                drawPixel(x, y + i);
        }

        error -= dy;
        if (error < 0) {
            y += yStep;
            error += dx;
        }
    }
}

// Display function
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glColor3f(0, 0, 0);

    if (drawMode == 1)
        drawBasicLine(startX, startY, endX, endY);
    else
        drawBoldLine(startX, startY, endX, endY, lineWidth);

    glEnd();
    glFlush();
}

// Initialize OpenGL environment
void initializeGL() {
    glClearColor(1, 1, 1, 1);  // White background
    glColor3f(0, 0, 0);        // Black color for drawing
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -300, 300);
}

// Main function
int main(int argc, char** argv) {
    cout << "Enter startX startY endX endY: ";
    cin >> startX >> startY >> endX >> endY;

    cout << "Select mode (1 = Normal, 2 = Bold): ";
    cin >> drawMode;

    if (drawMode == 2) {
        cout << "Enter line thickness: ";
        cin >> lineWidth;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Custom Bresenham Line Renderer");
    initializeGL();
    glutDisplayFunc(renderScene);
    glutMainLoop();
    return 0;
}
