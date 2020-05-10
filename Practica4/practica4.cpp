
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

using namespace std;

// Window World and Screen Variables
const GLint W_WIDTH  = 600;
const GLint W_HEIGHT = 500;
const GLint W_WINDOW = 2;
const GLint H_WINDOW = 2;

// Frame Rate Variables
GLint lastTime;
GLint elapsedTime;
GLint frames;
GLint framesElapsed;
GLint fps;

// Projection Variables
GLfloat const NEARFACE = 5.0f;
GLfloat const FARFACE  = 100.0f;
GLfloat const FOV      = 45.0f;

GLint projectionMode = 1;

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);   // Seleccionar la matriz proyeccion
    glLoadIdentity();              // Introducir dentro de la pila la matriz identidad

    cout << "---- INFORMATION ----" << endl;
    cout << "Aspect: "    << aspect << endl;

    if (projectionMode == 0) {

        if (width <= height)
            glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, NEARFACE, FARFACE);  // aspect <= 1
        else
            glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, NEARFACE, FARFACE);  // aspect > 1

    } else if (projectionMode == 1) {

        GLfloat top    = (GLfloat)tan(FOV * 0.5) * NEARFACE;
        GLfloat bottom = -top;
        GLfloat left   = aspect * bottom;
        GLfloat right  = aspect * top;

        cout << "Top:    " << top    << endl;
        cout << "Bottom: " << bottom << endl;
        cout << "Left: "   << left   << endl;
        cout << "Right: "  << right  << endl;

        glFrustum(left, right, bottom, top, NEARFACE, FARFACE);
    }
    else
        gluPerspective(FOV, aspect, NEARFACE, FARFACE);

    glMatrixMode(GL_PROJECTION); // Volvemos a la pila de transformaciones
}


void printFPS() {

    string textFrames = to_string(fps) + " FPS";

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos3f(W_WINDOW, -H_WINDOW, 0.0f);

    for (int i = 0; i < textFrames.length(); ++i)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, textFrames[i]);
}


// FIGURA CERCANA (TORUS)
void paintTorus() {

    glPushMatrix();                   // Matriz de escalado
    glTranslatef(-0.4f, 0.0f, -1.0f); // Mover a posicion cercana
    glColor3f(0.0f, 1.0f, .0f);       // Color
    glutSolidTorus(0.3, 0.5, 32, 32); // Dibujar toro
    glPopMatrix();                    // Aplicar matrix
}

// FIGURA LEJANA (TEAPOT)
void paintTeapot() {
    
    glPushMatrix();                  // Matriz de escalado
    glTranslatef(0.5f, 0.0f, -3.0f); // Mover a posicion lejana
    glColor3f(1.0f, 0.0f, 0.0f);     // Color
    glutSolidTeapot(1.0);            // Dibujar taza
    glPopMatrix();                   // Aplicar matrices
}

// Funcion que renderiza la escena OpenGL
void render() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0); // Posicionar la cámara

    paintTorus();                         // Pintar las figuras cercana y lejana
    paintTeapot();
    printFPS();                           // Pintar texto

    glutSwapBuffers();
}

// That function only executes in dead cycles of the processor
void idle() {

    frames++;
    framesElapsed++;

    elapsedTime = glutGet(GLUT_ELAPSED_TIME);

    if (elapsedTime - lastTime > 1000) {
        fps = framesElapsed * 1000 / (elapsedTime - lastTime);
        lastTime = elapsedTime;
        framesElapsed = 0;
    }

    glutPostRedisplay(); // Repaint screen
}

void keyboard(unsigned char c, int x, int y) {
    if (c == 27) {
        exit(0);
    }
}

void switchProyection(int value) {
    projectionMode = value;
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void init(void) {

    // Configurar menu
    glutCreateMenu(switchProyection);
    glutAddMenuEntry("GL_ORTHO", 0);
    glutAddMenuEntry("GL_FRUSTUM", 1);
    glutAddMenuEntry("GL_PERSPECTIVE", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //Configurar luces
    GLfloat lightPosition[] = {10.0, 10.0, 10.0, 1.0};
    GLfloat lightColor[] = {1.0, 1.0, 0.0, 0.0};

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glEnable(GL_LIGHT0);

    // El color de fondo sera el negro (RGBA, RGB + Alpha channel)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Habilitar
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

// Main function
int main(int argc, char **argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos posición y tamaño de la ventana
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - W_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - W_HEIGHT) / 2);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Creamos la nueva ventana
    glutCreateWindow("Etapa 3");
    init();

    // Indicamos cuales son las funciones de redibujado, idle y reshape
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}