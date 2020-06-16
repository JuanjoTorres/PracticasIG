
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "Camera.h"

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"

#endif

#define _USE_MATH_DEFINES

#define VK_SHIFT 0x10
#define VK_SPACE ' '

#include <cmath>
#include <string>
#include <iostream>

using namespace std;

const GLint W_WIDTH = 640;
const GLint W_HEIGHT = 480;

const GLint W_WINDOW = 2;
const GLint H_WINDOW = 2;

// Control de dispositivos
GLint leftMouseButton;
GLint rightMouseButton;
GLint mouseX = -1;
GLint mouseY = -1;

// Control de la proyeccion
GLfloat NEARFACE = 1.0f;
GLfloat FARFACE = 750.0f;
GLfloat FOV = 45.0f;

// Control de la camara
Camera** cameras = new Camera * [4];
GLint counter;
GLint selectedCamera;
GLfloat const SPEED = 0.2;

void init(void) {

    cameras[TOP_VIEW] = new Camera({ 0.0f, 20.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, TOP_VIEW); // Vista de enfrente
    cameras[FRONT_VIEW] = new Camera({ 0.0f, 0.0f, 20.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, FRONT_VIEW); // Vista de arriba
    cameras[SIDE_VIEW] = new Camera({ -20.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, SIDE_VIEW); // Vista de lado izquierdo
    cameras[FREE_VIEW] = new Camera({ 30.0f, 20.0f, 15.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, FREE_VIEW); // Camara movil
    cameras[SPHERICAL_VIEW] = new Camera({ 0.0f, 5.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, SPHERICAL_VIEW); // Camara esferica

    selectedCamera = FREE_VIEW;
    cameras[selectedCamera]->setThetaAngle(-M_PI / 3.0f);
    cameras[selectedCamera]->setPhiAngle(M_PI / 2.8f);
    cameras[selectedCamera]->updateOrientation();
}

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);                    // Select Projection matrix
    glLoadIdentity();                               // Reset the Projection matrix
    gluPerspective(FOV, aspect, NEARFACE, FARFACE);

    glMatrixMode(GL_MODELVIEW);
}

void paintGrid() {
    for (int i = 0; i < 40; i++) {

        glPushMatrix();

        if (i < 20)
            glTranslatef(0, 0, i);
        if (i >= 20) {
            glTranslatef(i - 20, 0, 0);
            glRotatef(-90, 0, 1, 0);
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(0.0f, -0.1f, 0.0f);
        glVertex3f(19.0f, -0.1f, 0.0f);
        glEnd();
        glPopMatrix();
    }
}

// Funcion que renderiza la escena OpenGL
void render() {

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // Resetear transformaciones
    vector<float> cameraPosition = cameras[selectedCamera]->getPosition();
    vector<float> lookDirection = cameras[selectedCamera]->getDirection();
    vector<float> cameraRotation = cameras[selectedCamera]->getRotation();

    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],  // Posicion de la camara
        cameraPosition[0] + lookDirection[0],                     // Dirección donde apunta la camara
        cameraPosition[1] + lookDirection[1],
        cameraPosition[2] + lookDirection[2],
        cameraRotation[0], cameraRotation[1], cameraRotation[2]); // Vector de rotacion

// Pintar suelo
    glColor3f(1.0f, 1.0f, 1.0f);
    paintGrid();
    //Pintar las figuras
    glColor3f(0.6f, 0.6f, 0.6f);
    glutSolidCube(2.0f);


    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void idle() {

    //Repintar la pantalla
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        selectedCamera = FREE_VIEW;
        break;
    case GLUT_KEY_F2:
        selectedCamera = TOP_VIEW;
        break;
    case GLUT_KEY_F3:
        selectedCamera = FRONT_VIEW;
        break;
    case GLUT_KEY_F4:
        selectedCamera = SIDE_VIEW;
        break;
    case GLUT_KEY_F5:
        selectedCamera = SPHERICAL_VIEW;
        break;
    }
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case VK_SPACE:
        cameras[selectedCamera]->moveUpward(SPEED);
        break;
    case 'a':
    case 'A':
        cameras[selectedCamera]->moveLeft(SPEED);
        break;
    case 'w':
    case 'W':
        cameras[selectedCamera]->moveForward(SPEED);
        break;
    case 's':
    case 'S':
        cameras[selectedCamera]->moveBackward(SPEED);
        break;
    case 'd':
    case 'D':
        cameras[selectedCamera]->moveRight(SPEED);
        break;
    case 'e':
    case 'E':
        counter++;
        if (counter % 5 == 0) {
            cameras[SPHERICAL_VIEW]->setPosition({ 0.0f, 5.0f, 0.0f }); // vista cenital
            cameras[SPHERICAL_VIEW]->setDirection({ 0.0f, -1.0f, 0.0f });
            cameras[SPHERICAL_VIEW]->setRotation({ 0.0f, 0.0f, -1.0f });
        }
        else if (counter % 5 == 1) {
            cameras[SPHERICAL_VIEW]->setPosition({ 0.0f, 5.0f, 5.0f }); // vista picado
            cameras[SPHERICAL_VIEW]->setDirection({ 0.0f, -1.0f, -1.0f });
            cameras[SPHERICAL_VIEW]->setRotation({ 0.0f, 1.0f, 0.0f });
        }
        else if (counter % 5 == 2) {
            cameras[SPHERICAL_VIEW]->setPosition({ 0.0f, 0.0f, 5.0f }); // vista normal
            cameras[SPHERICAL_VIEW]->setDirection({ 0.0f, 0.0f, -1.0f });
            cameras[SPHERICAL_VIEW]->setRotation({ 0.0f, 1.0f, 0.0f });
        }
        else if (counter % 5 == 3) {
            cameras[SPHERICAL_VIEW]->setPosition({ 0.0f, -5.0f, 5.0f }); // vista contrapicado
            cameras[SPHERICAL_VIEW]->setDirection({ 0.0f, 1.0f, -1.0f });
            cameras[SPHERICAL_VIEW]->setRotation({ 0.0f, 0.0f, 1.0f });
        }
        else {
            cameras[SPHERICAL_VIEW]->setPosition({ 0.0f, -5.0f, 0.0f }); // vista nadir
            cameras[SPHERICAL_VIEW]->setDirection({ 0.0f, 1.0f, 0.0f });
            cameras[SPHERICAL_VIEW]->setRotation({ 0.0f, 0.0f, 1.0f });
        }
        break;
    case 'z':
    case 'Z':
        cameras[selectedCamera]->moveDownward(SPEED);
        break;
    }
}

// Acciones del mouse [PULSAR]
void mouseCallback(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    else if (button == GLUT_RIGHT_BUTTON)
        rightMouseButton = state;

    mouseX = x;
    mouseY = y;
}

// Acciones del mouse [MOVER]
void mouseMotion(int x, int y) {

    // Solo se ejecutara cuando el boton este pulsado.
    if (leftMouseButton == GLUT_DOWN) {

        float thetaAngle = cameras[selectedCamera]->getThetaAngle();
        float phiAngle = cameras[selectedCamera]->getPhiAngle();

        thetaAngle += (y - mouseY) * 0.002f;
        phiAngle += (mouseX - x) * 0.002f;

        cameras[selectedCamera]->setThetaAngle(thetaAngle);
        cameras[selectedCamera]->setPhiAngle(phiAngle);

        cameras[selectedCamera]->updateOrientation();
    }

    mouseX = x;
    mouseY = y;
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

    //Configurar luces
    GLfloat lightPosition[] = { 10.0, 10.0, 10.0, 1.0 };
    GLfloat lightColor[] = { 1.0, 1.0, 1.0, 0.0 };

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glEnable(GL_LIGHT0);

    // El color de fondo sera el negro (RGBA, RGB + Alpha channel)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    init();

    // Indicamos cuales son las funciones de redibujado, idle y reshape
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);

    // Habilitar
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}