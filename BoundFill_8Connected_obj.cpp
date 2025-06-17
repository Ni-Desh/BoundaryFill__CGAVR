
/*
 100, 100
         200, 100
        200, 300

        300, 300
        300, 100

        200, 97
        200, 0
      100, 0
       100, 100



*/
#include <iostream>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

using namespace std;

const int NUM_POINTS = 9; // Number of points needed to define the object
int points[NUM_POINTS][2]; // Store (x, y) coordinates

void delay(float ms){
    clock_t goal = ms + clock();
    while(goal > clock());
}

void init(){
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 480);
}

void bound_it(int x, int y, float* fillColor, float* bc){
    float color[3];
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);

    if ((color[0] != bc[0] || color[1] != bc[1] || color[2] != bc[2]) &&
        (color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2])) {

        glColor3f(fillColor[0], fillColor[1], fillColor[2]);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();

        // 8-connected flood fill
        bound_it(x + 2, y, fillColor, bc);  // Right
        bound_it(x - 2, y, fillColor, bc);  // Left
        bound_it(x, y + 2, fillColor, bc);  // Up
        bound_it(x, y - 2, fillColor, bc);  // Down
//comment down these bottom 4 lines for 4 connected algoritm.
        bound_it(x + 2, y + 2, fillColor, bc);  // Diagonal (Top-Right)
        bound_it(x + 2, y - 2, fillColor, bc);  // Diagonal (Bottom-Right)
        bound_it(x - 2, y + 2, fillColor, bc);  // Diagonal (Top-Left)
        bound_it(x - 2, y - 2, fillColor, bc);  // Diagonal (Bottom-Left)

    }
}

void mouse(int btn, int state, int x, int y){
    y = 480 - y;  // Adjust for OpenGL coordinates
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float bCol[] = {0, 0, 1};  // Boundary color (Blue)
        float color[] = {1, 0, 1};  // Fill color (Purple)
        bound_it(x, y, color, bCol);
    }
}

void world(){
    glLineWidth(3);
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 1);  // Boundary color (Blue)

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_POINTS; i++) {
        glVertex2i(points[i][0], points[i][1]);
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv){
    cout << "Enter the coordinates (x, y) for the shape (9 points):\n";

    for (int i = 0; i < NUM_POINTS; i++) {
        cout << "Point " << i + 1 << ": ";
        cin >> points[i][0] >> points[i][1];
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("User-Defined Object with Boundary Fill");
    glutDisplayFunc(world);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
    return 0;
}


