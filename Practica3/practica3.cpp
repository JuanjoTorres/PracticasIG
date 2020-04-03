
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif

//#include <iostream>
#include <cmath>

using namespace std;

const int W_WIDTH = 500;
const int W_HEIGHT = 500;

const int W_WINDOW = 2;
const int H_WINDOW = 2;

const int PATH_LENGTH = 1000;

const float PI = 3.1415926f;
const float GRAV = -0.001;

const float CENTER_X = 0.0f;
const float CENTER_Y = 0.5f;

float innerRadius = 0.5f;
float outterRadius = 0.5f;

float massInnerBall = 40;
float massOutterBall = 40;

float innerAngle = PI / 2;
float outterAngle = PI / 2;

float innerAccel = 0.0f;
float outterAccel = 0.0f;

float innerSpeed = 0.0f;
float outterSpeed = 0.0f;

float posXInner;
float posYInner;

float posXOutter;
float posYOutter;

float pathXVector[PATH_LENGTH];
float pathYVector[PATH_LENGTH];

int frames;


void drawCircle(GLfloat x, GLfloat y, GLfloat xcenter, GLfloat ycenter) {
    int i;
    //Número de triangulos usados para dibujar el círculo
    int triangleAmount = 50;

    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                x + ((xcenter + 1) * cos(float(i) * twicePi / float(triangleAmount))),
                y + ((ycenter - 1) * sin(float(i) * twicePi / float(triangleAmount)))
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

void drawPath() {
    for (size_t i = 0; i < PATH_LENGTH; i++) {
        if (pathXVector[i] != 0.0) {
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(pathXVector[i], pathYVector[i], 0.0f);
            glEnd();
        }
    }
}

// Funcion que visualiza la escena OpenGL
void Display() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    //Dibujar punto de anclaje del péndulo
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(CENTER_X, CENTER_Y, -0.99f, 0.99f);

    drawInnerLine(posXInner, posYInner);
    drawCircle(posXInner, posYInner, -0.98f, 0.98f);

    drawOutterLine(posXInner, posYInner, posXOutter, posYOutter);
    drawCircle(posXOutter, posYOutter, -0.97f, 0.97f);

    drawPath();

    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {

    float num1 = -GRAV * (2 * massInnerBall + massOutterBall) * sin(innerAngle);
    float num2 = -massOutterBall * GRAV * sin(innerAngle - 2 * outterAngle);
    float num3 = -2 * sin(innerAngle - outterAngle) * massOutterBall;
    float num4 = outterSpeed * outterSpeed * outterRadius +
                 innerSpeed * innerSpeed * innerRadius * cos(innerAngle - outterAngle);

    float denom = innerRadius * (2 * massInnerBall + massOutterBall -
                                 massOutterBall * cos(2 * innerAngle - 2 * outterAngle));

    innerAccel = (num1 + num2 + num3 * num4) / denom;

    num1 = 2 * sin(innerAngle - outterAngle);
    num2 = (innerSpeed * innerSpeed * innerRadius * (massInnerBall + massOutterBall));
    num3 = GRAV * (massInnerBall + massOutterBall) * cos(innerAngle);
    num4 = outterSpeed * outterSpeed * outterRadius * massOutterBall * cos(innerAngle - outterAngle);

    denom = outterRadius * (2 * massInnerBall + massOutterBall -
                            massOutterBall * cos(2 * innerAngle - 2 * outterAngle));

    outterAccel = (num1 * (num2 + num3 + num4)) / denom;

    innerSpeed += innerAccel;
    outterSpeed += outterAccel;

    innerAngle += innerSpeed;
    outterAngle += outterSpeed;

    posXInner = innerRadius * sin(innerAngle);
    posYInner = innerRadius * cos(innerAngle) + CENTER_Y;

    posXOutter = posXInner + outterRadius * sin(outterAngle);
    posYOutter = posYInner + outterRadius * cos(outterAngle);

    pathXVector[frames % PATH_LENGTH] = posXOutter;
    pathYVector[frames % PATH_LENGTH] = posYOutter;

    frames++;

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
    glutInitDisplayMode(GLUT_DOUBLE);

    // Creamos la nueva ventana
    glutCreateWindow("Pendulo Doble");

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