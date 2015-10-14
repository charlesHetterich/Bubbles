#pragma once
#include <string>
#include "SDL/SDL.h"
#include <glm\glm.hpp>
//#include <glm\gtx\transform.hpp>
class vector2f
{
public:
	vector2f(float x, float y);

	//operations
	float length();
	float dotProduct(vector2f r);
	vector2f normalize();
	vector2f rotate(float angle);

	//basic operation
	vector2f add(vector2f r);
	vector2f add(float r);
	vector2f subtract(vector2f r);
	vector2f subtract(float r);
	vector2f multiply(vector2f r);
	vector2f multiply(float r);
	vector2f divide(vector2f r);
	vector2f divide(float r);

	std::string toString();

	//getters & setters
	float getX();
	void setX(float x);
	float getY();
	void setY(float y);

private:

	float x;
	float y;
};

class vector3f
{
public:
	vector3f(float x, float y, float z);

	//operations
	float length();
	float dotProduct(vector3f r);
	vector3f crossProduct(vector3f r);
	vector3f normalize();
	vector3f rotate(float angle);

	//basic operations
	vector3f add(vector3f r);
	vector3f add(float r);
	vector3f subtract(vector3f r);
	vector3f subtract(float r);
	vector3f multiply(vector3f r);
	vector3f multiply(float r);
	vector3f divide(vector3f r);
	vector3f divide(float r);

	//getters & setters
	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
	float getZ();
	void setZ(float z);

private:

	float x;
	float y;
	float z;
};

class matrix4f
{
public:
	matrix4f();

	matrix4f initIdentity();
	matrix4f multiply(matrix4f r);

	//getters & setters
	glm::mat4 getM();
	void setM(glm::mat4 m);
	float get(int x, int y);
	void set(int x, int y, float value);
	

private:

	glm::mat4 m;
};

class quaternion
{
public:
	quaternion(float x, float y, float z, float w);

	//operations
	float length();
	quaternion normalize();
	quaternion conjugate();
	quaternion multiply(quaternion r);
	quaternion multiply(vector3f r);

	//getters & setters
	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
	float getZ();
	void setZ(float z);
	float getW();
	void setW(float w);

private:
	
	float x;
	float y;
	float z;
	float w;
};