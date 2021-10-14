#define _USE_MATH_DEFINES
#include "core/mathLib.h"
#include <iostream>
#include <cmath>
using namespace std;
int main() {
	/*Vector a = Vector(1,2,3);
	Vector b = Vector(4, 5, 6);
	Vector c = Cross(a, b);
	std::cout << c.x << " " << c.y << " " << c.z << endl;
	Vector add = a + b;
	Vector sub = a - b;
	std::cout << a.x << " " << a.y << " " << a.z << endl;
	std::cout << add.x << " " << add.y << " " << add.z << endl;
	std::cout << sub.x << " " << sub.y << " " << sub.z << endl;
	Vector d = a * 2;
	std::cout << d.x << " " << d.y << " " << d.z << endl;
	std::cout << b.Length() << endl;
	a.Normalize();
	std::cout << a.Length() << endl;*/
	Matrix m = Matrix(new Vector[4]{ Vector(5, 2, 6, 1),
									 Vector(0, 6, 2, 0),
									 Vector(3, 8, 1, 4),
									 Vector(1 ,8, 5, 6) });
	m.Print();
	Matrix m2 = Matrix(new Vector[4]{ Vector(7, 5, 8, 0),
									  Vector(1, 8, 2, 6),
									  Vector(9, 4, 3, 8),
									  Vector(5, 3, 7, 9) });
	m2.Print();
	Matrix m3 = m * m2;
	std::cout << endl;
	m3.Print();

	Vector matrixmult = Vector(1, 2, 3);
	matrixmult.Print();

	std::cout << endl;

	Vector product = Vector();
	product = m * matrixmult;
	std::cout << endl;
	product.Print();

	Matrix mt = m.Transpose();
	mt.Print();

	Matrix inv = m.Inverse();
	inv.Print();

	Matrix identity = inv * m;
	identity.Print();

	Matrix rot = Rotation(M_PI/2, (Vector(1,0,0)));
	rot.Print();

	cout << endl;

	Matrix rotY = RotationY(M_PI / 2);
	rotY.Print();
}