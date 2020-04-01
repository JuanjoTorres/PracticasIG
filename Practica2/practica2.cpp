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

#include <iostream>

// Tamano inicial de la ventana
const int W_WIDTH = 500;
const int W_HEIGHT = 500;

// Tamaño del Window
const int W_WINDOW = 2;
const int H_WINDOW = 2;

GLfloat fAngulo; // Variable que indica el angulo de rotacion de los ejes. 
GLfloat fScale;
bool goSmall = true;

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

// Funcion que visualiza la escena OpenGL
void Display() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(-0.5f, 0.5f, 0.0f);
    glScalef(fScale, fScale, fScale);
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
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.0f);
    glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
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

    glPopMatrix();

    glPushMatrix();

    glTranslatef(0.5f, -0.5f, 0.0f);
    glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
    glScalef(fScale, fScale, fScale);
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

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f); // Linea horizontal
    glVertex2f(1.0f, 0.0f);

    glVertex2f(0.0f, 1.0f); // Linea vertical
    glVertex2f(0.0f, -1.0f);
    glEnd();

    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {
    // Incrementamos el angulo y la escala
    fAngulo += 0.3f;

    if (fScale > 1.0f && !goSmall)
        goSmall = true;
    else if (fScale < 0.0f && goSmall)
        goSmall = false;

    if (goSmall)
        fScale -= 0.0005f;
    else
        fScale += 0.0005f;

    // Si es mayor que dos pi la decrementamos
    if (fAngulo > 360)
        fAngulo -= 360;
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