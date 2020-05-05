
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
#include <string>
#include <iostream>

using namespace std;

const int W_WIDTH = 640;
const int W_HEIGHT = 480;

const int W_WINDOW = 2;
const int H_WINDOW = 2;

const int PATH_LENGTH = 1000;

const float PI = 3.1415926f;
const float GRAV = -0.00001;

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

int lastTime;
int elapsedTime;
int frames;
int framesElapsed;
int fps;

int projectionMode = 1;

GLfloat near = 0.1f;
GLfloat far = 100.0f;
GLfloat fov = 45.0f;

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    cout << "Aspect: " << aspect << endl;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);   // Select Projection matrix
    glLoadIdentity();              // Reset the Projection matrix

    if (projectionMode == 0) {

        if (width <= height) {
            glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, near, far);  // aspect <= 1
        } else {
            glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, near, far);  // aspect > 1
        }

    } else if (projectionMode == 1) {

        GLfloat top = (GLfloat) tan(fov * 0.5) * near;
        GLfloat bottom = -top;
        GLfloat left = aspect * bottom;
        GLfloat right = aspect * top;

        cout << "Top: " << top << endl;
        cout << "Bottom: " << bottom << endl;
        cout << "Left: " << left << endl;
        cout << "Right: " << right << endl;

        glFrustum(left, right, bottom, top, near, far);
    } else {
        gluPerspective(fov, aspect, near, far);
    }

    //Posicionar la cámara
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
}


void printFPS() {
    string textFrames = to_string(fps) + " FPS";
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos3f(0.8f, -0.9f, -1.0f);

    //glRasterPos2i( 10, 1014 );  // move in 10 pixels from the left and bottom edges
    for (int i = 0; i < textFrames.length(); ++i)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, textFrames[i]);

}


/**
 * Figura cercana --> DONUTS
 */
void paintNearFigure() {
    //Matriz de escalado
    glPushMatrix();

    //Mover a posicion cercana
    glTranslatef(-0.4f, 0.0f, -1.0f);

    //Color
    glColor3f(0.0f, 0.0f, 0.0f);

    glutSolidTorus(0.3, 0.5, 32, 32);

    //Aplicar matrix
    glPopMatrix();
}

/**
 * Figura lejana --> TETERA
 */
void paintFarFigure() {
    //Matriz de escalado
    glPushMatrix();

    //Mover a posicion lejana
    glTranslatef(0.5f, 0.0f, -3.0f);

    //Color
    glColor3f(1.0f, 0.0f, 0.0f);

    //glutSolidTorus(0.3, 0.5, 32, 32);
    glutSolidTeapot(1.0);

    //Aplicar matrices
    glPopMatrix();
}

// Funcion que renderiza la escena OpenGL
void render() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Pintar las figuras cercana y lejana
    paintNearFigure();
    paintFarFigure();

    printFPS();
    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void idle() {

    frames++;
    framesElapsed++;

    elapsedTime = glutGet(GLUT_ELAPSED_TIME);

    if (elapsedTime - lastTime > 1000) {
        fps = framesElapsed * 1000 / (elapsedTime - lastTime);
        lastTime = elapsedTime;
        framesElapsed = 0;
    }

    //Repintar la pantalla
    glutPostRedisplay();
}

void keyboard(unsigned char c, int x, int y) {
    if (c == 27) {
        exit(0);
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {

    }
}

void switchProyection(int value) {
    projectionMode = value;
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// Funcion principal
int main(int argc, char **argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos posición y tamaño de la ventana
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - W_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - W_HEIGHT) / 2);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Creamos la nueva ventana
    glutCreateWindow("Etapa 3");

    //Configurar menu
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //Habilitar
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // Indicamos cuales son las funciones de redibujado, idle y reshape
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}