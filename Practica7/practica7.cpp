
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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>

using namespace std;

const GLint W_WIDTH = 640;
const GLint W_HEIGHT = 480;

const GLint W_WINDOW = 2;
const GLint H_WINDOW = 2;

struct Model {
    float* vertex;
    float* normal;
    float* uv;
    int numVertex;
    float position;
};

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

//Models
//Model models[5];
int const NMODELS = 12;
Model models[NMODELS];


Model importModel(string pathname) {

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pathname,
        aiProcessPreset_TargetRealtime_Quality);

    aiMesh* mesh = scene->mMeshes[0];

    int numVerts0 = mesh->mNumFaces * 3;

    float* vertexArray = new float[mesh->mNumFaces * 3 * 3];
    float* normalArray = new float[mesh->mNumFaces * 3 * 3];
    float* uvArray     = new float[mesh->mNumFaces * 3 * 2];

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];

        for (int j = 0; j < 3; j++)
        {
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            memcpy(uvArray, &uv, sizeof(float) * 2);
            uvArray += 2;

            aiVector3D normal = mesh->mNormals[face.mIndices[j]];
            memcpy(normalArray, &normal, sizeof(float) * 3);
            normalArray += 3;

            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            memcpy(vertexArray, &pos, sizeof(float) * 3);
            vertexArray += 3;
        }
    }

    uvArray     -= mesh->mNumFaces * 3 * 2;
    normalArray -= mesh->mNumFaces * 3 * 3;
    vertexArray -= mesh->mNumFaces * 3 * 3;

    Model a = { vertexArray, normalArray, uvArray, numVerts0, 0.0 };
    return a;
}

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

    models[0] = importModel("Media/LightHouse/Lighthouse_BASE.obj");
    models[1] = importModel("Media/LightHouse/Lighthouse_ROTATIONPIECE.obj");
    models[2] = importModel("Media/LightHouse/Lighthouse_LIGHT.obj");
    models[3] = importModel("Media/Rocks/BigRock_1.obj");
    models[4] = importModel("Media/Rocks/BigRock_2.obj");
    models[5] = importModel("Media/Rocks/MediumRock_1.obj");
    models[6] = importModel("Media/Rocks/SmallRock_1.obj");
    models[7] = importModel("Media/Rocks/SmallRock_2.obj");
    models[8] = importModel("Media/Rocks/SmallRock_3.obj");
    models[9] = importModel("Media/Trees/Tree1.obj");
    models[10] = importModel("Media/Trees/Tree2.obj");
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

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glPushMatrix();

    for (int i = 0; i < NMODELS; i++) {
        glVertexPointer(3, GL_FLOAT, 0, models[i].vertex);
        glNormalPointer(GL_FLOAT, 0, models[i].normal);

        // glClientActiveTexture(GL_TEXTURE0_ARB);
        glTexCoordPointer(2, GL_FLOAT, 0, models[i].uv);
        glDrawArrays(GL_TRIANGLES, 0, models[i].numVertex);
    }

    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

// Pintar suelo
    glColor3f(1.0f, 1.0f, 1.0f);
    paintGrid();


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