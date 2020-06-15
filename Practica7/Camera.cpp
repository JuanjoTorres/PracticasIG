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

	if (type != FREE_VIEW && type != SPHERICAL_VIEW)
		cameraPosition[2] -= speed;
	else if (type == FREE_VIEW) {
		cameraPosition[0] += lookDirection[0] * speed;
		cameraPosition[1] += lookDirection[1] * speed;
		cameraPosition[2] += lookDirection[2] * speed;
	}
}

void Camera::moveBackward(float speed) {

	if (type != FREE_VIEW && type != SPHERICAL_VIEW)
		cameraPosition[2] += speed;
	else if (type == FREE_VIEW) {
		cameraPosition[0] -= lookDirection[0] * speed;
		cameraPosition[1] -= lookDirection[1] * speed;
		cameraPosition[2] -= lookDirection[2] * speed;
	}
}

void Camera::moveLeft(float speed) {

	if (type != FREE_VIEW && type != SPHERICAL_VIEW)
		cameraPosition[0] -= speed;
}

void Camera::moveRight(float speed) {

	if (type != FREE_VIEW && type != SPHERICAL_VIEW)
		cameraPosition[0] += speed;
}

void Camera::moveUpward(float speed) {

	if (type != FREE_VIEW && type != SPHERICAL_VIEW)
		cameraPosition[1] += speed;
}

void Camera::moveDownward(float speed) {

	if (type != FREE_VIEW && type != SPHERICAL_VIEW)
		cameraPosition[1] -= speed;
}