
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

const int W_WIDTH = 500;
const int W_HEIGHT = 500;

const int W_WINDOW = 2;
const int H_WINDOW = 2;

GLfloat fAngulo;
GLfloat fScale;

float in_radius = 0.2;
float out_radius = 0.2;
float mass_smallBall = 10;
float mass_bigBall = 10;
float in_angle = 0;
float out_angle = 0;

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
    }
    else {
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

    float positionX_smallBall = in_radius * sin(in_angle);
    float positionY_smallBall = in_radius * cos(in_angle);

    float positionX_bigBall = positionX_smallBall + out_radius * sin(out_radius);
    float positionY_bigBall = positionY_smallBall + out_radius * cos(out_radius);

    // Cuerda interna
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2d(0.0, 0.9);
    glVertex2d(positionX_smallBall, positionY_smallBall);
    glEnd();

    // Bola interna
    glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2d(-0.05 + positionX_smallBall,  0.05 + positionY_smallBall);
        glVertex2d( 0.05 + positionX_smallBall,  0.05 + positionY_smallBall);
        glVertex2d( 0.05 + positionX_smallBall, -0.05 + positionY_smallBall);
        glVertex2d(-0.05 + positionX_smallBall, -0.05 + positionY_smallBall);
    glEnd();

    // Cuerda externa
    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2d(positionX_smallBall, positionY_smallBall);
        glVertex2d(positionX_bigBall, positionY_bigBall);
    glEnd();

    // Bola externa
    /*glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
        glVertex2d(-0.05 + positionX_bigBall,  0.05 + positionY_bigBall);
        glVertex2d( 0.05 + positionX_bigBall,  0.05 + positionY_bigBall);
        glVertex2d( 0.05 + positionX_bigBall, -0.05 + positionY_bigBall);
        glVertex2d(-0.05 + positionX_bigBall, -0.05 + positionY_bigBall);
    glEnd();*/
    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {


    glutPostRedisplay();
}

// Funcion principal
int main(int argc, char** argv) {
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