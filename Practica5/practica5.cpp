
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

const GLint W_WIDTH = 640;
const GLint W_HEIGHT = 480;

const GLint W_WINDOW = 2;
const GLint H_WINDOW = 2;

double rx = 0, ry = 0, rz = 1, px = 0, py = 0, pz = 0, nx = 0, ny = 1, nz = 0;

GLint lastTime;
GLint elapsedTime;
GLint frames;
GLint framesElapsed;
GLint fps;

GLint projectionMode = 1;

GLfloat nearFace = 0.1f;
GLfloat farFace = 100.0f;
GLfloat fov = 45.0f;

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    cout << "Aspect: " << aspect << endl;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);   // Select Projection matrix
    glLoadIdentity();              // Reset the Projection matrix

    if (projectionMode == 0) {

        if (width <= height) {
            glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, nearFace, farFace);  // aspect <= 1
        }
        else {
            glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, nearFace, farFace);  // aspect > 1
        }

    }
    else if (projectionMode == 1) {

        GLfloat top = (GLfloat)tan(fov * 0.5) * nearFace;
        GLfloat bottom = -top;
        GLfloat left = aspect * bottom;
        GLfloat right = aspect * top;

        cout << "Top: " << top << endl;
        cout << "Bottom: " << bottom << endl;
        cout << "Left: " << left << endl;
        cout << "Right: " << right << endl;

        glFrustum(left, right, bottom, top, nearFace, farFace);
    }
    else {
        gluPerspective(fov, aspect, nearFace, farFace);
    }

    // Posicionar la cámara
    gluLookAt(rx, ry, rz, px, py, pz, nx, ny, nz);

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


// FIGURA CERCANA (TORUS)
void paintTorus() {
    // Matriz de escalado
    glPushMatrix();

    // Mover a posicion cercana
    glTranslatef(-0.4f, 0.0f, -1.0f);

    // Color
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidTorus(0.3, 0.5, 32, 32);

    // Aplicar matrix
    glPopMatrix();
}

// FIGURA LEJANA (TEAPOT)
void paintTeapot() {
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
    paintTorus();
    paintTeapot();

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

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'w':
        printf("w");
        rz = rz - 0.1;
        gluLookAt(rx, ry, rz, px, py, pz, nx, ny, nz);
        printf("rz = %.7lf\n", rz);
        break;
    case 's':
        printf("s");
        rz = rz + 0.1;
        gluLookAt(rx, ry, rz, px, py, pz, nx, ny, nz);
        printf("rz = %.7lf\n", rz);
        break;
    }
}

void switchProyection(int value) {
    projectionMode = value;
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// Funcion principal
int main(int argc, char** argv) {
    // Inicializamos la libreria GLUT
    glutInit(&argc, argv);

    // Indicamos posición y tamaño de la ventana
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - W_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - W_HEIGHT) / 2);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Creamos la nueva ventana
    glutCreateWindow("Etapa 4");

    //Configurar menu
    glutCreateMenu(switchProyection);
    glutAddMenuEntry("GL_ORTHO", 0);
    glutAddMenuEntry("GL_FRUSTUM", 1);
    glutAddMenuEntry("GL_PERSPECTIVE", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //Configurar luces
    GLfloat lightPosition[] = { 10.0, 10.0, 10.0, 1.0 };
    GLfloat lightColor[] = { 1.0, 1.0, 0.0, 0.0 };

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
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}