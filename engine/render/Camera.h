#pragma once
#include "core/mathLib.h"

class Camera {
	Vector pos;
	Matrix rot;
	Matrix P;

public:
	Camera(float FOV, float width, float height, float near, float far);
	void SetPos(Vector newPos);
    Vector GetPos();
	void AddPos(Vector addedPos);
	void SetRot(Matrix newRot);
	void AddRot(Matrix addedRot);
	Matrix GetVPMatrix();
};
