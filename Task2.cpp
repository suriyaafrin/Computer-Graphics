#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>


void hueToRGB(float hue, float &r, float &g, float &b) {
    float c = 1.0f;
    float x = 1.0f - fabs(fmod(hue / 60.0f, 2) - 1.0f);
    float m = 0.0f;

    if (hue < 60)      { r = c; g = x; b = 0; }
    else if (hue < 120){ r = x; g = c; b = 0; }
    else if (hue < 180){ r = 0; g = c; b = x; }
    else if (hue < 240){ r = 0; g = x; b = c; }
    else if (hue < 300){ r = x; g = 0; b = c; }
    else               { r = c; g = 0; b = x; }


}


void drawSymmetricPoints(int cx, int cy, int x, int y) {
    glVertex2i(cx + x, cy + y);
    glVertex2i(cx - x, cy + y);
    glVertex2i(cx + x, cy - y);
    glVertex2i(cx - x, cy - y);
    glVertex2i(cx + y, cy + x);
    glVertex2i(cx - y, cy + x);
    glVertex2i(cx + y, cy - x);
    glVertex2i(cx - y, cy - x);
}

// -----------------------------------------------------
void plotCircle(int cx, int cy, int radius) {
    int x = 0, y = radius;
    int decision = 1 - radius;
    while (x <= y) {
        drawSymmetricPoints(cx, cy, x, y);
        if (decision < 0)
            decision += 2 * x + 3;
        else {
            decision += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}


void drawRing(int cx, int cy, int radius, int thickness) {
    int inner = radius - thickness / 2;
    int outer = radius + thickness / 2;

    for (int r = inner; r <= outer; ++r)
        plotCircle(cx, cy, r);
}


void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    int centerX = 0, centerY = 0;
    int totalCircles = 10;
    int startRadius = 30;
    int gap = 15;
    int thickness = 5;

    for (int i = 0; i < totalCircles; ++i) {
        // Hue from 0° to 360° — gives rainbow effect
        float hue = (360.0f / totalCircles) * i;
        float r, g, b;
        hueToRGB(hue, r, g, b);
        glColor3f(r, g, b);

        drawRing(centerX, centerY, startRadius + i * gap, thickness);
    }

    glEnd();
    glutSwapBuffers();
}

// -----------------------------------------------------
void resizeWindow(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// -----------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Concentric Circles - Distinct Colors");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resizeWindow);
    glutMainLoop();
    return 0;
}
