#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>

struct Segment {
    float xStart, yStart, xEnd, yEnd;
    float cxStart, cyStart, cxEnd, cyEnd;
    bool isVisible;
};


float clipLeft = 150.0f;
float clipBottom = 120.0f;
float clipRight = 450.0f;
float clipTop = 350.0f;

std::vector<Segment> segments;


bool clipLiangBarsky(
    float x0, float y0, float x1, float y1,
    float left, float bottom, float right, float top,
    float &nx0, float &ny0, float &nx1, float &ny1)
{
    const float EPS = 1e-6f;
    float dx = x1 - x0, dy = y1 - y0;
    float p[4] = { -dx, dx, -dy, dy };
    float q[4] = { x0 - left, right - x0, y0 - bottom, top - y0 };

    float tEnter = 0.0f, tLeave = 1.0f;

    for (int i = 0; i < 4; ++i) {
        if (fabs(p[i]) < EPS) {
            if (q[i] < 0) return false; // Parallel & outside
            continue;
        }
        float t = q[i] / p[i];
        if (p[i] < 0) { // entering
            if (t > tEnter) tEnter = t;
        } else { // leaving
            if (t < tLeave) tLeave = t;
        }
        if (tEnter > tLeave) return false;
    }

    nx0 = x0 + tEnter * dx;
    ny0 = y0 + tEnter * dy;
    nx1 = x0 + tLeave * dx;
    ny1 = y0 + tLeave * dy;
    return true;
}

// ----------------------------------------------------
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Clipping window (blue)
    glColor3f(0.0f, 0.3f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(clipLeft, clipBottom);
        glVertex2f(clipRight, clipBottom);
        glVertex2f(clipRight, clipTop);
        glVertex2f(clipLeft, clipTop);
    glEnd();

    // Original lines (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    for (const auto &seg : segments) {
        glBegin(GL_LINES);
            glVertex2f(seg.xStart, seg.yStart);
            glVertex2f(seg.xEnd, seg.yEnd);
        glEnd();
    }

    // Clipped lines (green)
    glColor3f(0.0f, 0.8f, 0.2f);
    glLineWidth(3.0f);
    for (const auto &seg : segments) {
        if (seg.isVisible) {
            glBegin(GL_LINES);
                glVertex2f(seg.cxStart, seg.cyStart);
                glVertex2f(seg.cxEnd, seg.cyEnd);
            glEnd();
        }
    }

    glFlush();
    glutSwapBuffers();
}

// ----------------------------------------------------
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ----------------------------------------------------
int main(int argc, char** argv) {
    std::cout << "=== Liang–Barsky Line Clipping Visualization ===\n";
    std::cout << "Default Clipping Window: "
              << "Left=" << clipLeft
              << " Bottom=" << clipBottom
              << " Right=" << clipRight
              << " Top=" << clipTop << "\n\n";

    int n;
    std::cout << "Enter number of line segments: ";
    std::cin >> n;
    if (n <= 0) {
        std::cerr << "Invalid number. Exiting.\n";
        return 1;
    }

    segments.resize(n);
    std::cout << "Enter each line as: x1 y1 x2 y2\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "Line " << i + 1 << ": ";
        std::cin >> segments[i].xStart >> segments[i].yStart
                 >> segments[i].xEnd >> segments[i].yEnd;
    }

    char modify = 'n';
    std::cout << "Use default clipping window? (y/n): ";
    std::cin >> modify;
    if (modify == 'n' || modify == 'N') {
        std::cout << "Enter Left Bottom Right Top: ";
        std::cin >> clipLeft >> clipBottom >> clipRight >> clipTop;
        if (clipLeft > clipRight) std::swap(clipLeft, clipRight);
        if (clipBottom > clipTop) std::swap(clipBottom, clipTop);
    }

    // Perform clipping
    for (auto &seg : segments) {
        float nx0, ny0, nx1, ny1;
        if (clipLiangBarsky(seg.xStart, seg.yStart, seg.xEnd, seg.yEnd,
                            clipLeft, clipBottom, clipRight, clipTop,
                            nx0, ny0, nx1, ny1)) {
            seg.isVisible = true;
            seg.cxStart = nx0; seg.cyStart = ny0;
            seg.cxEnd = nx1; seg.cyEnd = ny1;
        } else {
            seg.isVisible = false;
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 450);
    glutCreateWindow("Liang–Barsky Line Clipping (Modified Values)");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(handleResize);
    glutMainLoop();

    return 0;
}
