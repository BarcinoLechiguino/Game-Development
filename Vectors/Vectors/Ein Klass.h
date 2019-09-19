#pragma once
#include <iostream>

//Useful commands: Ctrl + f + f (Searches the whole project for whatever you ask it to), Ctrl + r + r (Changes any text in the whole solution/item, like the name  of variables and the such).

using namespace std;

//Standarizes the variables and they can be changed whenever in the main() without having to change them all.
template <class Vec> class Vec3
{
private:
	Vec x, y, z;

public:
	//Constructors are functions that are run when the class is called.
	Vec3(Vec posX, Vec posY, Vec posZ) //Sets the introduced values as the coordinates.
	{
		x = posX;
		y = posY;
		z = posZ;
	}

	Vec3() //In case no values are introduced the coordinates will be all set to 0.
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3(Vec posS)
	{

	}

	//Destructors are functions that are run when the class stops being called.
	~Vec3() {}

	//Get() function that returns a coordinate when called. It's const because it only returns a number, it does not modify it.
	Vec getx() const { return x; }
	Vec gety() const { return y; }
	Vec getz() const { return z; }

	//Operator functions that calculate the addition and substraction of vectors as well as equaling and comparing them.
	Vec operator + (Vec3 &v)
	{
		x = x + v.x;
		y = y + v.y;
		z = z + v.z;
		return x, y, z;
	}

	Vec operator - (Vec3 &v)
	{
		x = x - v.x;
		y = y - v.y;
		z = z - v.z;
		return x, y, z;
	}

	Vec operator += (Vec3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return x, y, z;
	}

	Vec operator -= (Vec3 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return x, y, z;
	}

	Vec operator = (Vec3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return x, y, z;
	}

	void operator == (Vec3 &v)
	{
		if (x == v.x && y == v.y && z == v.z) { cout << "Both vectors are equal." << endl; }
		else { cout << "Both vectors are not equal" << endl; }
		//if (x != v.x || y != v.y || z != v.z) { cout << "Both vectors are not equal" << endl; }
	}

	Vec normalize()
	{
		Vec Module = sqrt(x*x + y*y + z*z);

		x = x / Module;
		y = y / Module;
		z = z / Module;

		return x, y, z;
	}

	void setToZero() { x = 0; y = 0; z = 0; }

	void getVector() { cout << "X is " << x << ", Y is " << y << " and Z is " << z << ". The vector will be V = ( " << x << ", " << y << ", " << z << " )" << endl; }

	void isZero() //This function checks which coordinates of a given vector are 0.
	{
		bool allZero = false;

		if (x == 0 && y == 0 && z == 0) { cout << "All the coordinates are equal to 0" << endl; allZero = true; }
		if (x != 0 && y != 0 && z != 0) { cout << "None of the coordinates are equal to 0" << endl; allZero = false; }

		if (allZero == false)
		{
			if (x == 0 && y != 0 && z != 0) { cout << "Only X is equal to 0" << endl; }
			if (y == 0 && x != 0 && z != 0) { cout << "Only Y is equal to 0" << endl; }
			if (z == 0 && x != 0 && y != 0) { cout << "Only Z is equal to 0" << endl; }
			if (x == 0 && y == 0 && z != 0) { cout << "Only X and Y are equal to 0" << endl; }
			if (x == 0 && y != 0 && z == 0) { cout << "Only X and Z are equal to 0" << endl; }
			if (x != 0 && y == 0 && z == 0) { cout << "Only Y and Z are equal to 0" << endl; }
		}

		allZero = false;
	}

	Vec distanceTo(Vec3 &v) 
	{
		Vec powerOfX = (v.x - x) * (v.x - x);
		Vec powerOfY = (v.y - y) * (v.y - y);
		Vec powerOfZ = (v.z - z) * (v.z - z);
		Vec distance = sqrt(powerOfX + powerOfY + powerOfZ);

		cout << "The distance between the two given points is " << distance << endl;
		return distance;
	}
};