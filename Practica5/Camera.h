#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

enum CameraType { TOP_VIEW, FRONT_VIEW, SIDE_VIEW, FREE_VIEW };

class Camera {
private:
	vector <float> cameraPosition; // posicion de la camara
	vector <float>  lookDirection; // direccion donde mira la camara
	vector <float> cameraRotation; // inclinacion de la camara

	float   phiAngle; // angulo fi en coordenadas cartesianas
	float thetaAngle; // angulo teta en coordenadas cartesianas
	float  rollAngle; // angulo de rotacion sobre el eje Z

	CameraType type; // tipo de camara
public:
	Camera(vector <float> cameraPosition, vector <float> lookDirection, vector <float> cameraRotation, CameraType type);

	vector<float> updateOrientation();

	vector<float> getPosition(void);
	vector<float> getDirection(void);
	vector<float> getRotation(void);

	void setThetaAngle(float angle);
	void setPhiAngle(float angle);

	float getThetaAngle(void);
	float getPhiAngle(void);

	void setPosition(vector<float> cameraPosition);
	void setDirection(vector<float> lookDirection);
	void setRotation(vector<float> cameraRotation);

	void moveForward (float speed);
	void moveBackward(float speed);
	void moveRight   (float speed);
	void moverLeft   (float speed);
};
