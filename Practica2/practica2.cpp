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

// Tamano inicial de la ventana
const int W_WIDTH = 250;
const int W_HEIGHT = 500;

// Tamaño del Window
const int W_WINDOW = 2;
const int H_WINDOW = 2;

GLfloat fAngulo; // Variable que indica el angulo de rotacion de los ejes. 

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

    // Funcion de redimension de la ventana
    glutReshapeFunc(Reshape);

    // Borramos la escena
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    // Rotamos las proximas primitivas
    glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
    // Creamos a continuacion dibujamos los tres poligonos
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.5f, 0.866f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.5f, -0.866f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f(-0.5f, 0.866f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.5f, -0.866f);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void Idle() {
    // Incrementamos el angulo
    fAngulo += 0.3f;
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

    //Reshape al iniciar
    glutReshapeFunc(Reshape);

    // Indicamos cuales son las funciones de redibujado e idle
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);

    // El color de fondo sera el negro (RGBA, RGB + Alpha channel)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //gluOrtho2D(0 - (wMon * (aspectRatioV / aspecRatioW)) / 2, 0 + (wMon * (aspectRatioV / aspecRatioW)) / 2, -hMon / 2, hMon / 2);

    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

    // Comienza la ejecucion del core de GLUT
    glutMainLoop();

    return 0;
}