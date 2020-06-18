
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION
#define ABSOLUTE_PATH "/Users/antoni/CLionProjects/PracticasIG/Practica7/"

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "Camera.h"
#include "SOIL/SOIL.h"

#else
#define ABSOLUTE_PATH ""

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"

#include "GL/SOIL.h"

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
    float *vertex;
    float *normal;
    float *uv;
    int numVertex;
    int textureID;
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
Camera **cameras = new Camera *[4];
GLint counter;
GLint selectedCamera;
GLfloat const SPEED = 0.2;

//Models & Textures
//Model models[5];
int const NMODELS = 12;
int const NTEXTURES = 8;
Model models[NMODELS];
GLuint textures[NTEXTURES];

//Estado de las luces
GLboolean statusMenu0 = true;
GLboolean statusMenu1 = true;
GLboolean statusMenu2 = true;
GLboolean statusMenu3 = true;
GLboolean statusMenu4 = true;

//POsición y color del foco del faro
//GLfloat lightHousePosition[] = {8.7, 10.45, -4.15, 1.0};  //Buena
GLfloat lightHousePosition[] = {8.7, 10.45, -4.15, 1.0};
GLfloat lightHouseDirection[] = {0.0, -0.5, 0.0};

//POsición y color de las luces
GLfloat lightPosition[] = {8.7, 10.45, -4.15, 1.0};

// Variable que indica el angulo de rotacion de los ejes del faro
GLfloat rotateAngleHorizontal;
GLfloat rotateAngleVertical;
GLfloat rotateAngleLight;

//Color blanco
GLfloat dayLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat moonLight[] = {0.05f, 0.05f, 0.05f, 0.05f};
GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};

GLuint loadTexture(const char *pathname) {

    GLuint texture = SOIL_load_OGL_texture
            (
                    pathname,
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
            );

    if (0 == texture)
        cout << "SOIL loading error: " << &SOIL_last_result << endl;

    return texture;
}

Model importModel(string pathname, int textureID) {

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(pathname, aiProcessPreset_TargetRealtime_Quality);

    aiMesh *mesh = scene->mMeshes[0];

    int numVerts0 = mesh->mNumFaces * 3;

    float *vertexArray = new float[mesh->mNumFaces * 3 * 3];
    float *normalArray = new float[mesh->mNumFaces * 3 * 3];
    float *uvArray = new float[mesh->mNumFaces * 3 * 2];

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];

        for (int j = 0; j < 3; j++) {
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

    uvArray -= mesh->mNumFaces * 3 * 2;
    normalArray -= mesh->mNumFaces * 3 * 3;
    vertexArray -= mesh->mNumFaces * 3 * 3;

    Model a = {vertexArray, normalArray, uvArray, numVerts0, textureID};
    return a;
}

void init(void) {

    cameras[TOP_VIEW] = new Camera({0.0f, 20.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f},
                                   TOP_VIEW); // Vista de enfrente
    cameras[FRONT_VIEW] = new Camera({0.0f, 10.0f, 20.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f},
                                     FRONT_VIEW); // Vista de arriba
    cameras[SIDE_VIEW] = new Camera({-20.0f, 10.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                    SIDE_VIEW); // Vista de lado izquierdo
    cameras[FREE_VIEW] = new Camera({12.9f, 11.7f, 1.0f}, {-0.80f, -0.25f, -0.53f}, {0.0f, 1.0f, 0.0f},
                                    FREE_VIEW); // Camara movil
    cameras[SPHERICAL_VIEW] = new Camera({0.0f, 5.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f},
                                         SPHERICAL_VIEW); // Camara esferica

    selectedCamera = FREE_VIEW;
    cameras[selectedCamera]->setThetaAngle(-M_PI / 3.0f);
    cameras[selectedCamera]->setPhiAngle(M_PI / 2.8f);
    cameras[selectedCamera]->updateOrientation();

    textures[0] = loadTexture(ABSOLUTE_PATH "Media/Textures/green_field.png");
    textures[1] = loadTexture(ABSOLUTE_PATH "Media/Textures/blue_sea.png");
    textures[2] = loadTexture(ABSOLUTE_PATH "Media/Textures/brown_door.png");
    textures[3] = loadTexture(ABSOLUTE_PATH "Media/Textures/brown_mountain.png");
    textures[4] = loadTexture(ABSOLUTE_PATH "Media/Textures/green_forest.png");
    textures[5] = loadTexture(ABSOLUTE_PATH "Media/Textures/grey.png");
    textures[6] = loadTexture(ABSOLUTE_PATH "Media/Textures/red.png");
    textures[7] = loadTexture(ABSOLUTE_PATH "Media/Textures/white.png");

    models[0] = importModel(ABSOLUTE_PATH "Media/Terrain/Terrain_Field.obj", 0);
    models[1] = importModel(ABSOLUTE_PATH "Media/Terrain/Mountains.obj", 3);
    models[2] = importModel(ABSOLUTE_PATH "Media/Terrain/Terrain_Sea.obj", 1);
    models[3] = importModel(ABSOLUTE_PATH "Media/Trees/Tree_Sheets.obj", 4);
    models[4] = importModel(ABSOLUTE_PATH "Media/Trees/Tree_Logs.obj", 2);
    models[5] = importModel(ABSOLUTE_PATH "Media/Rocks/Rocks.obj", 3);
    models[6] = importModel(ABSOLUTE_PATH "Media/Lighthouse/Lighthouse_DOOR.obj", 2);
    models[7] = importModel(ABSOLUTE_PATH "Media/Lighthouse/Lighthouse_BASE.obj", 3);
    models[8] = importModel(ABSOLUTE_PATH "Media/Lighthouse/Lighthouse_WHITEPART.obj", 7);
    models[9] = importModel(ABSOLUTE_PATH "Media/Lighthouse/Lighthouse_REDPART.obj", 6);
    models[10] = importModel(ABSOLUTE_PATH "Media/Lighthouse/Lighthouse_LIGHT.obj", 5);
    models[11] = importModel(ABSOLUTE_PATH "Media/Lighthouse/Lighthouse_ROTATIONPIECE.obj", 5);

}

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);                    // Select Projection matrix
    glLoadIdentity();                               // Reset the Projection matrix
    gluPerspective(FOV, aspect, NEARFACE, FARFACE);

    glMatrixMode(GL_MODELVIEW);
}

// Funcion que renderiza la escena OpenGL
void render() {
    glMatrixMode(GL_MODELVIEW);

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
        glPushMatrix();
        if (i == 11) {
            //Rotación sobre el eje vertical del faro
            glTranslatef(lightHousePosition[0], lightHousePosition[1], lightHousePosition[2]);
            glRotatef(rotateAngleHorizontal, 0.0f, 1.0f, 0.0f);
            glTranslatef(-lightHousePosition[0], -lightHousePosition[1], -lightHousePosition[2]);
        } else if (i == 10) {
            //Rotación sobre el eje vertical y horizontal del foco del faro
            glTranslatef(lightHousePosition[0], lightHousePosition[1], lightHousePosition[2]);
            glRotatef(rotateAngleHorizontal, 0.0f, 1.0f, 0.0f);
            glRotatef(rotateAngleVertical, 1.0f, 0.0f, 0.0f);
            glTranslatef(-lightHousePosition[0], -lightHousePosition[1], -lightHousePosition[2]);
        }

        GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
        GLfloat mat_diffuse[] = {0.8, 0.8, 0.8, 1.0};
        GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat mat_emission[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat mat_shininess = 64.0;

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

        glVertexPointer(3, GL_FLOAT, 0, models[i].vertex);
        glNormalPointer(GL_FLOAT, 0, models[i].normal);

        glBindTexture(GL_TEXTURE_2D, textures[models[i].textureID]);

        glTexCoordPointer(2, GL_FLOAT, 0, models[i].uv);
        glDrawArrays(GL_TRIANGLES, 0, models[i].numVertex);
        glPopMatrix();
    }
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPushMatrix(); // push under the stack the current modeview matrix
    glLightfv(GL_LIGHT2, GL_POSITION, lightHousePosition);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightHouseDirection);
    glPopMatrix(); // pop the last pushed modelview matrix to restore it as the current

    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void idle() {

    if (!statusMenu3) {
        // Incrementamos el angulo y la escala
        rotateAngleHorizontal += 2.5f;
        rotateAngleLight += 0.05f;
    }

    if (rotateAngleHorizontal < 306)
        rotateAngleHorizontal += 360;


    // Mod 360º
    rotateAngleHorizontal = fmod(rotateAngleHorizontal, 360.0f);
    rotateAngleLight = fmod(rotateAngleLight, 360.0f);

    lightHouseDirection[0] = sin(rotateAngleLight);
    lightHouseDirection[2] = cos(rotateAngleLight);

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
        case GLUT_KEY_LEFT:
            cameras[selectedCamera]->moveLeft(SPEED);
            break;
        case GLUT_KEY_UP:
            cameras[selectedCamera]->moveForward(SPEED);
            break;
        case GLUT_KEY_DOWN:
            cameras[selectedCamera]->moveBackward(SPEED);
            break;
        case GLUT_KEY_RIGHT:
            cameras[selectedCamera]->moveRight(SPEED);
            break;
    }
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case VK_SPACE:
            cameras[selectedCamera]->moveUpward(SPEED);
            break;
        case 'z':
        case 'Z':
            cameras[selectedCamera]->moveDownward(SPEED);
            break;
        case 'a':
        case 'A':
            if (statusMenu3) {
                rotateAngleHorizontal += 2.5f;
                rotateAngleLight += 0.05f;
            }
            break;
        case 'd':
        case 'D':
            if (statusMenu3) {
                rotateAngleHorizontal -= 2.5f;
                rotateAngleLight -= 0.05f;
            }
            break;
        case 'w':
        case 'W':
            rotateAngleVertical -= 2.5f;
            break;
        case 's':
        case 'S':
            rotateAngleVertical += 2.5f;
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

void switchLights(int item) {

    switch (item) {
        case 0:
            statusMenu0 = !statusMenu0;
            if (statusMenu0) {
                glEnable(GL_LIGHT0);
                glutChangeToMenuEntry(item + 1, "Disable diffuse light", item);
            } else {
                glDisable(GL_LIGHT0);
                glutChangeToMenuEntry(item + 1, "Enable diffuse light", item);
            }
            break;
        case 1:
            statusMenu1 = !statusMenu1;
            if (statusMenu1) {
                glEnable(GL_LIGHT1);
                glutChangeToMenuEntry(item + 1, "Disable ambient light", item);
            } else {
                glDisable(GL_LIGHT1);
                glutChangeToMenuEntry(item + 1, "Enable ambient light", item);
            }
            break;
        case 2:
            statusMenu2 = !statusMenu2;
            if (statusMenu2) {
                glEnable(GL_LIGHT2);
                glutChangeToMenuEntry(item + 1, "Disable lighthouse", item);
            } else {
                glDisable(GL_LIGHT2);
                glutChangeToMenuEntry(item + 1, "Enable lighthouse", item);
            }
            break;
        case 3:
            statusMenu3 = !statusMenu3;
            if (statusMenu3) {
                glutChangeToMenuEntry(item + 1, "Set automatic lighthouse", item);
            } else {
                glutChangeToMenuEntry(item + 1, "Set manual lighthouse", item);
            }
            break;
        case 4:
            statusMenu4 = !statusMenu4;
            if (statusMenu4) {
                glutChangeToMenuEntry(item + 1, "Change to DAY MODE", item);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, moonLight);
                glLightfv(GL_LIGHT1, GL_AMBIENT, moonLight);
            } else {
                glutChangeToMenuEntry(item + 1, "Change to NIGHT MODE", item);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, dayLight);
                glLightfv(GL_LIGHT1, GL_AMBIENT, dayLight);
            }
            break;
    }
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
    glutCreateWindow("Etapa final");

    //Configurar menu
    glutCreateMenu(switchLights);
    glutAddMenuEntry("Disable Diffuse light", 0);
    glutAddMenuEntry("Disable Ambient light", 1);
    glutAddMenuEntry("Disable lighthouse", 2);
    glutAddMenuEntry("Set automatic lighthouse", 3);
    glutAddMenuEntry("Change to DAY MODE", 4);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Configurar luces
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, moonLight);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, moonLight);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_POSITION, lightHousePosition);
    glLightfv(GL_LIGHT2, GL_SPECULAR, yellow);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightHouseDirection);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);
    //glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 4);
    glEnable(GL_LIGHT2);

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
    glEnable(GL_TEXTURE_2D);

    // Comienza la ejecucion del core de GLUT (RENDERING)
    glutMainLoop();
    return 0;
}