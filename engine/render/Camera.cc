#include "config.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

Camera::Camera(float FOV, float width, float height, float near, float far) {
	this->pos = Vector(0, 0, 0);
	this->rot = RotationZ(0);

	float scale = 1 / tan(FOV * 0.5 * M_PI / 180);
	this->P[0][0] = scale;
	this->P[1][1] = width/height / scale;
	this->P[2][2] = -far / (far - near);
	this->P[2][3] = -far * near / (far - near);
	this->P[3][2] = -1;
	this->P[3][3] = 0;
}

void Camera::SetPos(Vector newPos) {
	this->pos = newPos;
}
Vector Camera::GetPos(){
    return this->pos;
}

void Camera::AddPos(Vector addedPos) {
	this->pos.x = this->pos.x + addedPos.x;
	this->pos.y = this->pos.y + addedPos.y;
	this->pos.z = this->pos.z + addedPos.z;
}

void Camera::SetRot(Matrix newRot) {
	this->rot = newRot;
}

void Camera::AddRot(Matrix addedRot) {
	this->rot = this->rot * addedRot;
}

Matrix Camera::GetVPMatrix() {
	Matrix temp = PositionMat(Vector(0,0,-5)) * this->rot * PositionMat(this->pos);
	temp = this->P * temp;
	return temp;
}
