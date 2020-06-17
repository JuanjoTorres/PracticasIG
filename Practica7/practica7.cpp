
#if defined(__APPLE__)

#define GL_SILENCE_DEPRECATION
#define ABSOLUTE_PATH "/Users/antoni/CLionProjects/PracticasIG/Practica7/"

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "Camera.h"

#else
#define ABSOLUTE_PATH ""

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

#include "GL/SOIL.h"
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

GLuint loadTexture(const char* pathname) {

    GLuint texture = SOIL_load_OGL_texture
    (
        pathname,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (0 == texture) {
        cout << "SOIL loading error: " << SOIL_last_result << endl;
    }

//Estado de las luces
GLboolean statusLight0 = true;
GLboolean statusLight1 = false;
GLboolean statusLight2 = true;
GLboolean statusLight3 = true;

//POsición y color del foco del faro
//GLfloat lightHousePosition[] = {8.7, 10.45, -4.15, 1.0};  //Buena
GLfloat lightHousePosition[] = {8.7, 10.45, -4.15, 1.0};
GLfloat lightHouseDirection[] = {0.0, -0.5, 0.0};

//POsición y color de las luces
GLfloat lightPosition[] = {8.7, 10.45, -4.15, 1.0};

// Variable que indica el angulo de rotacion de los ejes del faro
GLfloat rotateAngle;
GLfloat rotateAngleLight;

//Color blanco
GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};


Model importModel(string pathname) {

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
    cameras[FRONT_VIEW] = new Camera({0.0f, 0.0f, 20.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f},
                                     FRONT_VIEW); // Vista de arriba
    cameras[SIDE_VIEW] = new Camera({-20.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                    SIDE_VIEW); // Vista de lado izquierdo
    cameras[FREE_VIEW] = new Camera({12.9f, 11.7f, 1.0f}, {-0.80f, -0.25f, -0.53f}, {0.0f, 1.0f, 0.0f},
                                    FREE_VIEW); // Camara movil
    cameras[SPHERICAL_VIEW] = new Camera({0.0f, 5.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f},
                                         SPHERICAL_VIEW); // Camara esferica

    selectedCamera = FREE_VIEW;
    cameras[selectedCamera]->setThetaAngle(-M_PI / 3.0f);
    cameras[selectedCamera]->setPhiAngle(M_PI / 2.8f);
    cameras[selectedCamera]->updateOrientation();

    textures[0] = loadTexture("Media/Textures/green_field.png");
    textures[1] = loadTexture("Media/Textures/blue_sea.png");
    textures[2] = loadTexture("Media/Textures/brown_door.png");
    textures[3] = loadTexture("Media/Textures/brown_mountain.png");
    textures[4] = loadTexture("Media/Textures/green_forest.png");
    textures[5] = loadTexture("Media/Textures/grey.png");
    textures[6] = loadTexture("Media/Textures/red.png");
    textures[7] = loadTexture("Media/Textures/white.png");

    models[0] = importModel("Media/Terrain/Terrain_Field.obj", 0);
    models[1] = importModel("Media/Terrain/Mountains.obj", 3);
    models[2] = importModel("Media/Terrain/Terrain_Sea.obj", 1);
    models[3] = importModel("Media/Trees/Tree_Sheets.obj", 4);
    models[4] = importModel("Media/Trees/Tree_Logs.obj", 2);
    models[5] = importModel("Media/Rocks/Rocks.obj", 3);
    models[6] = importModel("Media/Lighthouse/Lighthouse_DOOR.obj", 2);
    models[7] = importModel("Media/Lighthouse/Lighthouse_BASE.obj", 3);
    models[8] = importModel("Media/Lighthouse/Lighthouse_WHITEPART.obj", 7);
    models[9] = importModel("Media/Lighthouse/Lighthouse_REDPART.obj", 6);
    models[10] = importModel("Media/Lighthouse/Lighthouse_LIGHT.obj", 5);
    models[11] = importModel("Media/Lighthouse/Lighthouse_ROTATIONPIECE.obj", 5);

}

void reshape(GLsizei width, GLsizei height) {

    GLfloat aspect = (GLfloat) width / (GLfloat) height;

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
        if (i == 0) {
            //Rotación sobre el eje vertical del faro
            glTranslatef(lightHousePosition[0], lightHousePosition[1], lightHousePosition[2]);
            glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
            glTranslatef(-lightHousePosition[0], -lightHousePosition[1], -lightHousePosition[2]);
        } else if (i == 1) {
            //Rotación sobre el eje vertical y horizontal del foco del faro
            glTranslatef(lightHousePosition[0], lightHousePosition[1], lightHousePosition[2]);
            glRotatef(rotateAngle, 1.0f, 1.0f, 0.0f);
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

    //Posicion del cubo
    glTranslatef(lightHousePosition[0], lightHousePosition[1], lightHousePosition[2] + 1.0f);
    glutSolidCube(0.1f);

    glutSwapBuffers();
}

// Funcion que se ejecuta cuando el sistema no esta ocupado
void idle() {

    if (statusLight3) {
        // Incrementamos el angulo y la escala
        rotateAngle += 2.5f;
        rotateAngleLight += 0.05f;
    }

    // Mod 360º
    rotateAngle = fmod(rotateAngle, 360.0f);
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
            cameras[selectedCamera]->moveRight(SPEED);
            break;
        case GLUT_KEY_UP:
            cameras[selectedCamera]->moveForward(SPEED);
            break;
        case GLUT_KEY_DOWN:
            cameras[selectedCamera]->moveBackward(SPEED);
            break;
        case GLUT_KEY_RIGHT:

            break;
    }
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 'a':
        case 'A':
            lightHousePosition[0] = lightHousePosition[0] + 0.005;
            break;
        case 'z':
        case 'Z':
            lightHousePosition[0] = lightHousePosition[0] - 0.005;
            break;
        case 's':
        case 'S':
            lightHousePosition[1] = lightHousePosition[1] + 0.005;
            break;
        case 'x':
        case 'X':
            lightHousePosition[1] = lightHousePosition[1] - 0.005;
            break;
        case 'd':
        case 'D':
            lightHousePosition[2] = lightHousePosition[2] + 0.005;
            break;
        case 'c':
        case 'C':
            lightHousePosition[2] = lightHousePosition[2] - 0.005;
            break;
    }

    cout << lightHousePosition[0] << ' ' << lightHousePosition[1] << ' ' << lightHousePosition[2] << '\n';
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
            statusLight0 = !statusLight0;
            if (statusLight0) {
                glEnable(GL_LIGHT0);
                glutChangeToMenuEntry(item + 1, "Disable diffuse light", item);
            } else {
                glDisable(GL_LIGHT0);
                glutChangeToMenuEntry(item + 1, "Enable diffuse light", item);
            }
            break;
        case 1:
            statusLight1 = !statusLight1;
            if (statusLight1) {
                glEnable(GL_LIGHT1);
                glutChangeToMenuEntry(item + 1, "Disable ambient light", item);
            } else {
                glDisable(GL_LIGHT1);
                glutChangeToMenuEntry(item + 1, "Enable ambient light", item);
            }
            break;
        case 2:
            statusLight2 = !statusLight2;
            if (statusLight2) {
                glEnable(GL_LIGHT2);
                glutChangeToMenuEntry(item + 1, "Disable lighthouse", item);
            } else {
                glDisable(GL_LIGHT2);
                glutChangeToMenuEntry(item + 1, "Enable lighthouse", item);
            }
            break;
        case 3:
            statusLight3 = !statusLight3;
            if (statusLight3) {
                glutChangeToMenuEntry(item + 1, "Disable automatic lighthouse", item);
            } else {
                glutChangeToMenuEntry(item + 1, "Enable automatic lighthouse", item);
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
    glutAddMenuEntry("Enable Ambient light", 1);
    glutAddMenuEntry("Disable lighthouse", 2);
    glutAddMenuEntry("Disable automatic lighthouse", 3);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Configurar luces
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT1, GL_AMBIENT, white);
    //glEnable(GL_LIGHT1);

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