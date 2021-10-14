#pragma once
#include <math.h>
#include <iostream>
using namespace std;
//https://www.youtube.com/watch?v=TtRn3HsOm1s
//https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

///A struct for vectors with 4 elements.
///
///Vectors can be initiated with 0, 3 or 4 arguments, in each case the vector will still have 4 elements but the ones excluded will be 0.

struct Vector
{
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float arr[4];
	};
	inline Vector() = default;
	inline Vector(float x, float y, float z);
	inline Vector(float x, float y, float z, float w);
	inline Vector operator+(Vector b) {

		Vector c(x, y, z);
		c.x = x + b.x;
		c.y = y + b.y;
		c.z = z + b.z;
		return c;
	}
	inline Vector operator-(Vector b) {

		Vector c(x, y, z);
		c.x = x - b.x;
		c.y = y - b.y;
		c.z = z - b.z;
		return c;
	}
	inline Vector operator*(float scaleFactor) {

		Vector c(x, y, z);
		c.x = x * scaleFactor;
		c.y = y * scaleFactor;
		c.z = z * scaleFactor;
		return c;
	}
	inline float operator[](int index) const {
		return arr[index];
	}
	inline float& operator[](int index) {
		return arr[index];
	}
	/// <summary>
	/// Returns vector length.
	/// </summary>
	/// <returns></returns>
	inline float Length();
	/// <summary>
	/// Makes input vector normalized.
	/// </summary>
	inline void Normalize();
	/// <summary>
	/// Prints vector values by cout.
	/// </summary>
	inline void Print();
};

//Vector::Vector() {
//	x = y = z = w = 0;
//}

Vector::Vector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1;
}

Vector::Vector(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float Vector::Length() {
	return(sqrtf(x * x + y * y + z * z));
}

void Vector::Normalize() {
	float temp = Length();
	x /= temp;
	y /= temp;
	z /= temp;
	w /= temp;
}
/// <summary>
/// Dot multiplication between 2 vectors, a and b.
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>the resulting vector.</returns>
inline float Dot(Vector a, Vector b) {
	return a.x* b.x + a.y * b.y + a.z * b.z;
}
inline float Dot4(Vector a, Vector b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/// <summary>
/// Cross multiplication between 2 vectors, a and b.
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>the resulting vector.</returns>
inline Vector Cross(Vector a, Vector b) {
	Vector temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

void Vector::Print() {
	for (int i = 0; i < 4; i++) {
		std::cout << (*this)[i] << endl;
	}
}

/// <summary>
/// A struct for 4x4 matrices.
/// </summary>
struct Matrix 
{
	union {
		Vector data[4];
		float data2[16];
	};
	/// <summary>
	/// Default constructor gives the matrix the value 0 in every slot.
	/// </summary>
	/// <returns></returns>
	inline Matrix();
	/// <summary>
	/// Send in a vector array to turn it into a matrix.
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	inline Matrix(Vector a[4]);
	inline Matrix operator*(Matrix m2) {
		Matrix m3 = Matrix();
		/*for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					m3[i][j] += (*this)[i][k] * m2[k][j];
				}
			}
		}*/
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m3[i][j] = Dot4((*this)[i], Vector(m2[0][j], m2[1][j], m2[2][j], m2[3][j]));
			}
		}
		return m3;
	}
	inline Vector operator*(Vector v) {
		Vector temp = Vector();
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				temp[j] += (*this)[j][k] * v[k];
			}
		}
		return temp;
	}
	inline Vector operator[](int index) const {
		return data[index];
	}
	inline Vector& operator[](int index) {
		return data[index];
	}
	/// <summary>
	/// Transpose returns the transposition of the matrix.
	/// </summary>
	/// <returns>Transposed matrix</returns>
	inline Matrix Transpose();
	/// <summary>
	/// Inverse returns the inverse of the matrix.
	/// </summary>
	/// <returns>Inverse matrix, if unable to inverse: returns matrix filled with 0</returns>
	inline Matrix Inverse();
	/// <summary>
	/// Prints out all elements by cout.
	/// </summary>
	inline void Print();
};

Matrix::Matrix() {
	/*for (int i = 0; i < 4; i++) {
		data[i] = Vector();
	}*/
	
	data[0] = Vector(1, 0, 0, 0);
	data[1] = Vector(0, 1, 0, 0);
	data[2] = Vector(0, 0, 1, 0);
	data[3] = Vector(0, 0, 0, 1);
	
}

Matrix::Matrix(Vector a[4]) {

	for (int i = 0; i < 4; i++) {
		data[i] = a[i];
	}

}
/// <summary>
/// Given an angle in radians and a vector describing the rotation axis a rotation matrix is created.
/// </summary>
/// <param name="angle">Angle in radians</param>
/// <param name="axis">A vector axis, e.g Vector(1,0,0) for rotation around the x axis.</param>
/// <returns>Rotation matrix</returns>
inline Matrix Rotation(float angle, Vector axis) {
	axis.Normalize();
	Matrix r = Matrix(new Vector[4]{ Vector(cosf(angle) + (1-cosf(angle)) * pow(axis.x, 2), (1-cosf(angle))*axis.x*axis.y-axis.z*sinf(angle), (1-cosf(angle))*axis.x*axis.z + axis.y*sinf(angle), 0),
									 Vector((1-cosf(angle))*axis.x*axis.y + axis.z*sinf(angle), cosf(angle) + (1-cosf(angle))*pow(axis.y, 2), (1-cosf(angle))*axis.y*axis.z - axis.x*sinf(angle), 0),
									 Vector((1-cosf(angle))*axis.x*axis.z - axis.y*sinf(angle), (1-cosf(angle))*axis.y*axis.z + axis.x*sinf(angle), cosf(angle) + (1-cosf(angle))*pow(axis.z, 2), 0),
									 Vector(0 ,0, 0, 1) });
	return r;
}
/// <summary>
/// Given an angle in radians a rotation matrix is created around the x axis.
/// </summary>
/// <param name="angle">Angle in radians</param>
/// <returns>Rotation matrix around x</returns>
inline Matrix RotationX(float angle) {
	return Rotation(angle, Vector(1, 0, 0));
}
/// <summary>
/// Given an angle in radians a rotation matrix is created around the y axis.
/// </summary>
/// <param name="angle">Angle in radians</param>
/// <returns>Rotation matrix around y</returns>
inline Matrix RotationY(float angle) {
	return Rotation(angle, Vector(0, 1, 0));
}
/// <summary>
/// Given an angle in radians a rotation matrix is created around the z axis.
/// </summary>
/// <param name="angle">Angle in radians</param>
/// <returns>Rotation matrix around z</returns>
inline Matrix RotationZ(float angle) {
	return Rotation(angle, Vector(0, 0, 1));
}

inline Matrix PositionMat(Vector xyz) {
	Matrix r = Matrix(new Vector[4]{ Vector(1, 0, 0, xyz.x),
									 Vector(0, 1, 0, xyz.y),
									 Vector(0, 0, 1, xyz.z),
									 Vector(0 ,0, 0, 1) });
	return r;
}

Matrix Matrix::Transpose() {
	Matrix temp = Matrix();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp[j][i] = (*this)[i][j];
		}
	}
	return temp;
}

Matrix Matrix::Inverse() {
	float inv[16], det;
	Matrix invOut = Matrix();
	int i;

	inv[0] = data2[5] * data2[10] * data2[15] -
		data2[5] * data2[11] * data2[14] -
		data2[9] * data2[6] * data2[15] +
		data2[9] * data2[7] * data2[14] +
		data2[13] * data2[6] * data2[11] -
		data2[13] * data2[7] * data2[10];

	inv[4] = -data2[4] * data2[10] * data2[15] +
		data2[4] * data2[11] * data2[14] +
		data2[8] * data2[6] * data2[15] -
		data2[8] * data2[7] * data2[14] -
		data2[12] * data2[6] * data2[11] +
		data2[12] * data2[7] * data2[10];

	inv[8] = data2[4] * data2[9] * data2[15] -
		data2[4] * data2[11] * data2[13] -
		data2[8] * data2[5] * data2[15] +
		data2[8] * data2[7] * data2[13] +
		data2[12] * data2[5] * data2[11] -
		data2[12] * data2[7] * data2[9];

	inv[12] = -data2[4] * data2[9] * data2[14] +
		data2[4] * data2[10] * data2[13] +
		data2[8] * data2[5] * data2[14] -
		data2[8] * data2[6] * data2[13] -
		data2[12] * data2[5] * data2[10] +
		data2[12] * data2[6] * data2[9];

	inv[1] = -data2[1] * data2[10] * data2[15] +
		data2[1] * data2[11] * data2[14] +
		data2[9] * data2[2] * data2[15] -
		data2[9] * data2[3] * data2[14] -
		data2[13] * data2[2] * data2[11] +
		data2[13] * data2[3] * data2[10];

	inv[5] = data2[0] * data2[10] * data2[15] -
		data2[0] * data2[11] * data2[14] -
		data2[8] * data2[2] * data2[15] +
		data2[8] * data2[3] * data2[14] +
		data2[12] * data2[2] * data2[11] -
		data2[12] * data2[3] * data2[10];

	inv[9] = -data2[0] * data2[9] * data2[15] +
		data2[0] * data2[11] * data2[13] +
		data2[8] * data2[1] * data2[15] -
		data2[8] * data2[3] * data2[13] -
		data2[12] * data2[1] * data2[11] +
		data2[12] * data2[3] * data2[9];

	inv[13] = data2[0] * data2[9] * data2[14] -
		data2[0] * data2[10] * data2[13] -
		data2[8] * data2[1] * data2[14] +
		data2[8] * data2[2] * data2[13] +
		data2[12] * data2[1] * data2[10] -
		data2[12] * data2[2] * data2[9];

	inv[2] = data2[1] * data2[6] * data2[15] -
		data2[1] * data2[7] * data2[14] -
		data2[5] * data2[2] * data2[15] +
		data2[5] * data2[3] * data2[14] +
		data2[13] * data2[2] * data2[7] -
		data2[13] * data2[3] * data2[6];

	inv[6] = -data2[0] * data2[6] * data2[15] +
		data2[0] * data2[7] * data2[14] +
		data2[4] * data2[2] * data2[15] -
		data2[4] * data2[3] * data2[14] -
		data2[12] * data2[2] * data2[7] +
		data2[12] * data2[3] * data2[6];

	inv[10] = data2[0] * data2[5] * data2[15] -
		data2[0] * data2[7] * data2[13] -
		data2[4] * data2[1] * data2[15] +
		data2[4] * data2[3] * data2[13] +
		data2[12] * data2[1] * data2[7] -
		data2[12] * data2[3] * data2[5];

	inv[14] = -data2[0] * data2[5] * data2[14] +
		data2[0] * data2[6] * data2[13] +
		data2[4] * data2[1] * data2[14] -
		data2[4] * data2[2] * data2[13] -
		data2[12] * data2[1] * data2[6] +
		data2[12] * data2[2] * data2[5];

	inv[3] = -data2[1] * data2[6] * data2[11] +
		data2[1] * data2[7] * data2[10] +
		data2[5] * data2[2] * data2[11] -
		data2[5] * data2[3] * data2[10] -
		data2[9] * data2[2] * data2[7] +
		data2[9] * data2[3] * data2[6];

	inv[7] = data2[0] * data2[6] * data2[11] -
		data2[0] * data2[7] * data2[10] -
		data2[4] * data2[2] * data2[11] +
		data2[4] * data2[3] * data2[10] +
		data2[8] * data2[2] * data2[7] -
		data2[8] * data2[3] * data2[6];

	inv[11] = -data2[0] * data2[5] * data2[11] +
		data2[0] * data2[7] * data2[9] +
		data2[4] * data2[1] * data2[11] -
		data2[4] * data2[3] * data2[9] -
		data2[8] * data2[1] * data2[7] +
		data2[8] * data2[3] * data2[5];

	inv[15] = data2[0] * data2[5] * data2[10] -
		data2[0] * data2[6] * data2[9] -
		data2[4] * data2[1] * data2[10] +
		data2[4] * data2[2] * data2[9] +
		data2[8] * data2[1] * data2[6] -
		data2[8] * data2[2] * data2[5];

	det = data2[0] * inv[0] + data2[1] * inv[4] + data2[2] * inv[8] + data2[3] * inv[12];

	if (det == 0)
		return Matrix();

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut.data2[i] = inv[i] * det;

	return invOut;
}

void Matrix::Print() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << (*this)[i][j] << " ";
		}
		std::cout << endl;
	}
}
