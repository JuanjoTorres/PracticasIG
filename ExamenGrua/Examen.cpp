
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
#include <string>
#include <iostream>
#include <stdio.h>

const int W_WIDTH = 600;
const int W_HEIGHT = 600;

const int W_WINDOW = 2;
const int H_WINDOW = 2;

const float MAX_TOR_SIZE = 0.3f;
const float MIN_TOR_SIZE = 0.1f;

GLfloat fAnguloFig1;
GLfloat fAnguloFig2;
GLfloat fAnguloFig3;
GLfloat fAnguloFig4;

GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPosition0[] = {0.0f, 0.9f, 0.0f};
GLfloat lightPosition1[] = {0.0f, 0.9f, 0.0f};
GLfloat spot_direction[] = {0.0f, -1.0f, 0.0f};

GLfloat especularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat difuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};

void drawAxisXYZ();

void reshape(int width, int height) {

    if (height == 0)
        height = 1;

    float aspectViewport = float(width) / float(height);
    float aspectWindow = float(W_WINDOW) / float(H_WINDOW);

    if (aspectViewport > aspectWindow) {
        //the aspect of the viewport is greater than the aspect of your region, so it is wider.
        // In that case, you should map the full height and increase the horizontal range
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

// Función que visualiza la escena OpenGL
void render(void) {
    glEnable(GL_DEPTH_TEST);
    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.1, 0.1, 0.1, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0);

    //Dibujar ejes 3D
    drawAxisXYZ();

    //Base de la grua
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, 0.0f, -0.5f);
    glVertex3f(-0.5f, 0.0f, 0.5f);
    glVertex3f(0.5f, 0.0f, 0.5f);
    glVertex3f(0.5f, 0.0f, -0.5f);
    glEnd();
    glPopMatrix();

    //Rectangulo rojo
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(0.1f, 0.0f, 0.5f);
    glVertex3f(0.1f, 0.0f, -0.5f);
    glVertex3f(-0.1f, 0.0f, -0.5f);
    glVertex3f(-0.1f, 0.0f, 0.5f);
    glEnd();
    glPopMatrix();

    //Cubo blanco
    glPushMatrix();
    glTranslatef(0.4f, 0.1f, 0.25f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(0.1);
    glPopMatrix();

    //Triangulo.
    glPushMatrix();
    glColor3f(0.0f, 0.4f, 0.1f);
    glBegin(GL_POLYGON);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, especularLight);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

    glVertex3f(0.0f, 0.9f, 0.0f);
    glVertex3f(0.2f, 0.4f, 0.0f);
    glVertex3f(-0.2f, 0.4f, 0.0f);
    glPopMatrix();
    glEnd();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // Intercambiar buffers
    glutSwapBuffers();
}

void drawAxisXYZ() {
    //Eje X
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.95f, 0.01f, 0.0f);
    glVertex3f(0.95f, -0.01f, 0.0f);
    glEnd();

    //Eje Y
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.01f, 0.95f, -0.01f);
    glVertex3f(-0.01f, 0.95f, 0.01f);
    glEnd();

    //Eje Z
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.01f, 0.95f);
    glVertex3f(0.0f, -0.01f, 0.95f);
    glEnd();
}

void idle(void) {

    // Repintar pantalla
    glutPostRedisplay();
}

// Activar luces
void enableLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

int main(int argc, char **argv) {
    // Init GLUT
    glutInit(&argc, argv);

    // Configuración de la ventana
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);

    // Habilitar doblebuffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // Crer ventana
    glutCreateWindow("Examen");

    // Funciones de renderizado, cálculo y reshape
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    // El color de fondo será el negro (RGBA, RGB + Alpha channel)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Inicializar iluminación
    enableLighting();

    // Ejecutar bucle principal
    glutMainLoop();

    return 0;
}