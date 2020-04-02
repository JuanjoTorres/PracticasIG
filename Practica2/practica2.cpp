// Etapa1.cpp
// Fichero principal 
////////////////////////////////////////////////////
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include <cmath>

//Para debug
#include <string>
#include <iostream>

using namespace std;

// Tamano inicial de la ventana
const int W_WIDTH = 500;
const int W_HEIGHT = 500;

// Tamaño del Window
const int W_WINDOW = 2;
const int H_WINDOW = 2;

// Variable que indica el angulo de rotacion de los ejes del triangulo y el cuadrado
GLfloat rotateAngle;
//Variable de factor de escalado del pentagono
GLfloat scaleFactor;
//Variables de translación de círculo
GLfloat translateX;
GLfloat translateY;

//Variable para saber si el pentagono y el cuadrado están haciendo pequeños o grandes
bool isShrinking = true;

//Variable para saber el movimiento actual del circulo
bool movingLeft = true;
bool movingRight = false;
bool movingTop = false;
bool movingBottom = false;

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

/**
 * Pentágono con escalado
 */
void paintFigure1() {
    //Matriz de escalado
    glPushMatrix();

    //Mover a su cuadrante
    glTranslatef(-0.5f, 0.5f, 0.0f);

    //Escalado
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(-0.175, -0.25);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.25, 0.075);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f(0.0, 0.25);
    glVertex2f(0.25, 0.075);
    glColor3f(0.0f, 0.25f, 0.5f);
    glVertex2f(0.175, -0.25);
    glEnd();

    //Aplicar matrix
    glPopMatrix();
}

/**
 * Triángulo con rotación
 */
void paintFigure2() {
    //Matriz de escalado
    glPushMatrix();

    //Mover a su cuadrante
    glTranslatef(0.5f, 0.5f, 0.0f);

    //Rotación y translación
    glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);

    //Mover al centro
    glTranslatef(-0.5f, -0.5f, 0.0f);

    // Dibujamos el triangulo
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.5f, 0.2f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.2f, 0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.8f, 0.8f);
    glEnd();

    //Aplicar matrices
    glPopMatrix();
}

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

/**
 * Circulo con translación
 */
void paintFigure3() {

    glPushMatrix();

    glTranslatef(translateX, translateY, 0);

    // Dibujamos el pentagono
    glBegin(GL_POLYGON);
    glColor3f(0.5f, 1.0f, 0.5f);
    drawCircle(-0.5f, -0.5f, -0.75f, 0.75f);
    glEnd();

    glPopMatrix();
}

/**
 * Cuadrado con rotación y escalado
 */
void paintFigure4() {
    glPushMatrix();

    glTranslatef(0.5f, -0.5f, 0.0f);
    glRotatef(rotateAngle, 0.0f, 0.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glTranslatef(-0.5f, 0.5f, 0.0f);

    // Dibujamos el cuadrado
    glBegin(GL_QUADS);
    glColor3f(0.5895f, 0.234234f, 0.06546f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, -0.8f);
    glVertex2f(0.8f, -0.8f);
    glVertex2f(0.8f, -0.2f);
    glEnd();

    glPopMatrix();
}

void paintSectors() {
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f); // Linea horizontal
    glVertex2f(1.0f, 0.0f);

    glVertex2f(0.0f, 1.0f); // Linea vertical
    glVertex2f(0.0f, -1.0f);
    glEnd();
}

// Funcion que visualiza la escena OpenGL
void Display() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    //Pintar las figuras
    paintFigure1();
    paintFigure2();
    paintFigure3();
    paintFigure4();

    //Pintar los separadores de los 4 cuadrantes
    paintSectors();

    //Aplicar doble buffer
    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {
    // Incrementamos el angulo y la escala
    rotateAngle += 0.3f;

    //Si ha llegado al tamaño máximo, hacerlo pequeño
    if (scaleFactor > 1.0f && !isShrinking)
        isShrinking = true;
        //Si ha llegado al tamaño mínimo, hacerlo grande
    else if (scaleFactor < 0.1f && isShrinking)
        isShrinking = false;

    if (isShrinking)
        scaleFactor -= 0.005f;
    else
        scaleFactor += 0.005f;

    // Si es mayor que dos pi la decrementamos
    if (rotateAngle > 360)
        rotateAngle -= 360;

    if (movingLeft && translateX < -0.2f) {
        movingLeft = false;
        movingBottom = true;
    } else if (movingBottom && translateY < -0.2f) {
        movingBottom = false;
        movingRight = true;
    } else if (movingRight && translateX > 0.2f) {
        movingRight = false;
        movingTop = true;
    } else if (movingTop && translateY > 0.2f) {
        movingTop = false;
        movingLeft = true;
    }

    //Determinar movimiento actual del circulo
    if (movingLeft)
        translateX -= 0.005f;
    else if (movingBottom)
        translateY -= 0.005f;
    else if (movingRight)
        translateX += 0.005f;
    else if (movingTop)
        translateY += 0.005f;

    // Indicamos que es necesario repintar la pantalla
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
    glutCreateWindow("Mi segunda Ventana");

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