
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include <iostream>
#include <cmath>

const int W_WIDTH = 500;
const int W_HEIGHT = 500;

const int W_WINDOW = 2;
const int H_WINDOW = 2;

const float CENTER_X = 0.0f;
const float CENTER_Y = 0.75f;

float innerRadius = 0.2f;
float outerRadius = 0.2f;
float massSmallBall = 10;
float massBigBall = 10;
float innerAngle = 0.3f;
float outterAngle = 0.3f;

float posXInner;
float posYInner;

float posXOutter;
float posYOutter;


void drawCircle(GLfloat x, GLfloat y, GLfloat xcenter, GLfloat ycenter) {
    int i;
    //Número de triangulos usados para dibujar el círculo
    int triangleAmount = 50;

    GLfloat twicePi = 2.0f * 3.1415926f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                x + ((xcenter + 1) * cos(i * twicePi / triangleAmount)),
                y + ((ycenter - 1) * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void Reshape(int width, int height) {

    if (height == 0)
        height = 1;

    float aspectViewport = float(width) / float(height);
    float aspectWindow = float(W_WINDOW) / float(H_WINDOW);

    if (aspectViewport > aspectWindow) {
        //the aspect of the viewport is greater than the aspect of your region, so it is wider.
        // In that case, you should map the full height and increase the horitonal range
        // by a factor of (aspect_viewport/aspect_region)
        glLoadIdentity();
        gluOrtho2D(0 - (W_WINDOW * (aspectViewport / aspectWindow)) / 2,
                   0 + (W_WINDOW * (aspectViewport / aspectWindow)) / 2,
                   float(-H_WINDOW) / 2,
                   float(H_WINDOW) / 2);
    } else {
        //Otherwise, the aspect of the window is lower than aspect of your region,
        // so you should use the full width and scale up the vertical range by (aspect_region/aspect_viewport)
        glLoadIdentity();
        gluOrtho2D(float(-W_WINDOW) / 2,
                   float(W_WINDOW) / 2,
                   0 - (H_WINDOW * (aspectWindow / aspectViewport)) / 2,
                   0 + (H_WINDOW * (aspectWindow / aspectViewport)) / 2);
    }

    glViewport(0, 0, width, height);
}

void drawInnerLine(float x, float y) {
    glBegin(GL_LINES);
    glVertex2d(CENTER_X, CENTER_Y);
    glVertex2d(x, y);
    glEnd();
}

void drawOutterLine(float initX, float initY, float endX, float endY) {
    glBegin(GL_LINES);
    glVertex2d(initX, initY);
    glVertex2d(endX, endY);
    glEnd();
}

// Funcion que visualiza la escena OpenGL
void Display() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    //Dibujar punto de anclaje del péndulo
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(CENTER_X, CENTER_Y, -0.98f, 0.98f);

    posXInner = innerRadius * sin(innerAngle);
    posYInner = innerRadius * cos(innerAngle);

    posXOutter = posXInner + outerRadius * sin(outterAngle);
    posYOutter = posYInner + outerRadius * cos(outterAngle);

    drawInnerLine(posXInner, posYInner);
    drawCircle(posXInner, posYInner, -0.98f, 0.98f);

    drawOutterLine(posXInner, posYInner, posXOutter, posYOutter);
    drawCircle(posXOutter, posYOutter, -0.98f, 0.98f);

    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {


    //Repintar la pantalla
    glutPostRedisplay();
}

// Funcion principal
int main(int argc, char **argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos como ha de ser la nueva ventana
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // Creamos la nueva ventana
    glutCreateWindow("Péndulo Doble");

    // Indicamos cuales son las funciones de redibujado, idle y reshape
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutReshapeFunc(Reshape);

    // El color de fondo sera el negro (RGBA, RGB + Alpha channel)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    gluOrtho2D(0 - float(W_WINDOW) / 2, float(W_WINDOW) / 2, 0 - float(H_WINDOW) / 2, float(W_WINDOW) / 2);

    // Comienza la ejecucion del core de GLUT
    glutMainLoop();

    return 0;
}