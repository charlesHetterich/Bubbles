#include "mathFunctions.h"

//////////////////////
///////VECTOR2F///////
//////////////////////

vector2f::vector2f(float x, float y)
{
	this->x = x;
	this->y = y;
}

float vector2f::length()
{
	return sqrt(x * x + y * y);
}
float vector2f::dotProduct(vector2f r)
{
	return x * r.getX() + y * r.getY();
}

vector2f vector2f::normalize()
{
	float length = this->length();

	x /= length;
	y /= length;

	return *this;
}

vector2f vector2f::rotate(float angle)
{
	double rad = (double)(M_PI / 180) * angle;
	double Cos = cos(rad);
	double Sin = sin(rad);

	return vector2f((float)(x * Cos - y * Sin), (float)(x * Sin + y * Cos));
}

vector2f vector2f::add(vector2f r)
{
	return vector2f(x + r.getX(), y + r.getY());
}

vector2f vector2f::add(float r)
{
	return vector2f(x + r, y + r);
}

vector2f vector2f::subtract(vector2f r)
{
	return vector2f(x - r.getX(), y - r.getY());
}

vector2f vector2f::subtract(float r)
{
	return vector2f(x - r, y - r);
}

vector2f vector2f::multiply(vector2f r)
{
	return vector2f(x * r.getX(), y * r.getY());
}

vector2f vector2f::multiply(float r)
{
	return vector2f(x * r, y * r);
}

vector2f vector2f::divide(vector2f r)
{
	return vector2f(x / r.getX(), y / r.getY());
}

vector2f vector2f::divide(float r)
{
	return vector2f(x / r, y / r);
}

std::string vector2f::toString()
{
	std::string string = "(";
	string += std::to_string(x);
	string += ", ";
	string += std::to_string(y);
	string += ")";

	return string;
}

float vector2f::getX()
{
	return x;
}

void vector2f::setX(float x)
{
	this->x = x;
}

float vector2f::getY()
{
	return y;
}

void vector2f::setY(float y)
{
	this->y = y;
}

//////////////////////
///////VECTOR3F///////
//////////////////////

vector3f::vector3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float vector3f::length()
{
	return sqrt(x * x + y * y + z * z);
}

float vector3f::dotProduct(vector3f r)
{
	return x * r.getX() + y * r.getY() + z * r.getZ();
}

vector3f vector3f::crossProduct(vector3f r)
{
	float x_ = y * r.getZ() - z * r.getY();
	float y_ = z * r.getX() - x * r.getZ();
	float z_ = x * r.getY() - z * r.getX();

	return vector3f(x_, y_, z_);
}

vector3f vector3f::normalize()
{
	float length = this->length();

	x /= length;
	y /= length;
	z /= length;

	return *this;
}

vector3f vector3f::rotate(float angle)
{
	return vector3f(NULL, NULL, NULL);
}

vector3f vector3f::add(vector3f r)
{
	return vector3f(x + r.getX(), y + r.getY(), z + r.getZ());
}

vector3f vector3f::add(float r)
{
	return vector3f(x + r, y + r, z + r);
}

vector3f vector3f::subtract(vector3f r)
{
	return vector3f(x - r.getX(), y - r.getY(), z - r.getZ());
}

vector3f vector3f::subtract(float r)
{
	return vector3f(x - r, y - r, z - r);
}

vector3f vector3f::multiply(vector3f r)
{
	return vector3f(x * r.getX(), y * r.getY(), z * r.getZ());
}

vector3f vector3f::multiply(float r)
{
	return vector3f(x * r, y * r, z * r);
}

vector3f vector3f::divide(vector3f r)
{
	return vector3f(x / r.getX(), y / r.getY(), z / r.getZ());
}

vector3f vector3f::divide(float r)
{
	return vector3f(x / r, y / r, z / r);
}

float vector3f::getX()
{
	return x;
}

void vector3f::setX(float x)
{
	this->x = x;
}

float vector3f::getY()
{
	return y;
}

void vector3f::setY(float y)
{
	this->y = y;
}

float vector3f::getZ()
{
	return z;
}

void vector3f::setZ(float z)
{
	this->z = z;
}

//////////////////////
///////MATRIX4F///////
//////////////////////

matrix4f::matrix4f()
{
}

matrix4f matrix4f::initIdentity()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

	return *this;
}

matrix4f matrix4f::multiply(matrix4f r)
{
	matrix4f res = matrix4f();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.set(i, j, m[i][0] * r.get(0, j)
						+ m[i][1] * r.get(1, j)
						+ m[i][2] * r.get(2, j)
						+ m[i][3] * r.get(3, j));
		}
	}

	return res;
}

glm::mat4 matrix4f::getM()
{
	return m;
}

void matrix4f::setM(glm::mat4 m)
{
	this->m = m;
}

float matrix4f::get(int x, int y)
{
	return m[x][y];
}

void matrix4f::set(int x, int y, float value)
{
	m[x][y] = value;
}

//////////////////////
//////QUATERNION//////
//////////////////////

quaternion::quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float quaternion::length()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

quaternion quaternion::normalize()
{
	float length = this->length();

	x /= length;
	y /= length;
	z /= length;
	w /= length;

	return *this;
}

quaternion quaternion::conjugate()
{
	return quaternion(-x, -y, -z, -w);
}

quaternion quaternion::multiply(quaternion r)
{
	float w_ = w * r.getW() - x * r.getX() - y * r.getY() - z * r.getZ();
	float x_ = x * r.getW() + w * r.getX() + y * r.getZ() - z * r.getY();
	float y_ = y * r.getW() + w * r.getY() + z * r.getX() - x * r.getZ();
	float z_ = z * r.getW() + w * r.getZ() + x * r.getY() - y * r.getX();

	return quaternion(x_, y_, z_, w_);
}

quaternion quaternion::multiply(vector3f r)
{
	float w_ = -x * r.getX() - y * r.getY() - z * r.getZ();
	float x_ = w * r.getX() + y * r.getZ() - z * r.getY();
	float y_ = w * r.getY() + z * r.getX() - x * r.getZ();
	float z_ = w * r.getZ() + x * r.getY() - y * r.getX();

	return quaternion(x_, y_, z_, w_);
}

float quaternion::getX()
{
	return x;
}

void quaternion::setX(float x)
{
	this->x = x;
}

float quaternion::getY()
{
	return y;
}

void quaternion::setY(float y)
{
	this->y = y;
}

float quaternion::getZ()
{
	return z;
}

void quaternion::setZ(float z)
{
	this->z = z;
}

float quaternion::getW()
{
	return w;
}

void quaternion::setW(float w)
{
	this->w = w;
}