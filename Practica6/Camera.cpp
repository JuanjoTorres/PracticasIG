#include "Camera.h"

Camera::Camera(vector <float> cameraPosition, vector <float> lookDirection, vector <float> cameraRotation, CameraType type) {
	this->cameraPosition.assign(cameraPosition.begin(), cameraPosition.end());
	this->lookDirection.assign(lookDirection.begin(), lookDirection.end());
	this->cameraRotation.assign(cameraRotation.begin(), cameraRotation.end());
	this->type = type;
}

vector<float> Camera::updateOrientation() {

	if (type != FREE_VIEW)
		return lookDirection;

	lookDirection[0] = sinf(thetaAngle) * sinf(phiAngle);
	lookDirection[1] = -cosf(thetaAngle) * sinf(phiAngle);
	lookDirection[2] = -cosf(phiAngle);

	float magnitude = sqrt(lookDirection[0] * lookDirection[0]
		+ lookDirection[1] * lookDirection[1]
		+ lookDirection[2] * lookDirection[2]);

	// normalizar vector
	lookDirection[0] /= magnitude;
	lookDirection[1] /= magnitude;
	lookDirection[2] /= magnitude;

	return lookDirection;
}

void Camera::setThetaAngle(float angle) {
	thetaAngle = angle;
}

void Camera::setPhiAngle(float angle) {

	phiAngle = angle;
}

float Camera::getThetaAngle(void) {
	return thetaAngle;
}

float Camera::getPhiAngle(void) {
	return phiAngle;
}

vector<float> Camera::getPosition() {
	return cameraPosition;
}

vector<float> Camera::getDirection() {
	return lookDirection;
}

vector<float> Camera::getRotation() {
	return cameraRotation;
}

void Camera::setPosition(vector<float> cameraPosition) {
	this->cameraPosition.assign(cameraPosition.begin(), cameraPosition.end());
}
void Camera::setDirection(vector<float> lookDirection) {
	this->lookDirection.assign(lookDirection.begin(), lookDirection.end());
}
void Camera::setRotation(vector<float> cameraRotation) {
	this->cameraRotation.assign(cameraRotation.begin(), cameraRotation.end());
}

void Camera::moveForward(float speed) {
	cameraPosition[0] += lookDirection[0] * speed;
	cameraPosition[1] += lookDirection[1] * speed;
	cameraPosition[2] += lookDirection[2] * speed;
}

void Camera::moveBackward(float speed) {
	cameraPosition[0] -= lookDirection[0] * speed;
	cameraPosition[1] -= lookDirection[1] * speed;
	cameraPosition[2] -= lookDirection[2] * speed;
}

void Camera::moveRight(float speed) {
	float viewX = lookDirection[0] - cameraPosition[0];
	float viewY = lookDirection[1] - cameraPosition[1];
	float viewZ = lookDirection[2] - cameraPosition[2];

	std::cout << "x: " << viewX << endl;
	std::cout << "y: " << viewY << endl;
	std::cout << "z: " << viewZ << endl;

	float x = ((viewY * cameraRotation[2]) - (viewZ * cameraRotation[1]));
	float y = -((viewX * cameraRotation[2]) - (viewZ * cameraRotation[0]));
	float z = ((viewX * cameraRotation[1]) - (viewY * cameraRotation[0]));

	float magnitude = sqrt((x * x) + (y * y) + (z * z));

	x /= magnitude;
	y /= magnitude;
	z /= magnitude;

	cameraPosition[0] -= x * speed;
	cameraPosition[1] -= y * speed;
	cameraPosition[2] -= z * speed;

}