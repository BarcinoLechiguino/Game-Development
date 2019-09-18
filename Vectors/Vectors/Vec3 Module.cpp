#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

template <class Vec> class Vec3 //Standarizes the variables
{
private:
	Vec x, y, z;

public:
	Vec3(Vec posx, Vec posy, Vec posz) //Constructors are processes that are processed when the class is called.
	{
		x = posx; 
		y = posy;
		z = posz;
	}

	Vec3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3()
	{
		
	}

	Vec getx() const { return x; }
	Vec gety() const { return y; }
	Vec getz() const { return z; }

	Vec operator + (Vec3 &v) { x = x + v.x; y = y + v.y; z = z + v.z; return x, y, z; }
	Vec operator - (Vec3 &v) { x = x - v.x; y = y - v.y; z = z - v.z; return x, y, z; }
	Vec operator += (Vec3 &v) { x += v.x; y += v.y; z += v.z; return x, y, z; }
	Vec operator -= (Vec3 &v) { x -= v.x; y -= v.y; z -= v.z; return x, y, z; }
	Vec operator = (Vec3 &v) { x = v.x; y = v.y; z = v.z; return x, y, z; }
	Vec operator == (Vec &v) {}

	Vec normalize() {}
	Vec setToZero() { x = 0; y = 0; z = 0; }
	Vec getVector() { cout << "X is " << x << " Y is " << y << " Z is " << z << "So the vector will be V = ( "<< x y z << " )"}
	Vec isZero() 
	{
		if (x == 0 && y == 0 && z == 0) { cout << "x, y and z are all equal to 0" << endl; }
		if (x == 0) { cout << "X is equal to 0" << endl; }
		if (y == 0) { cout << "Y is equal to 0" << endl; }
		if (z == 0) { cout << "Z is equal to 0" << endl; }
		if (x == 0 && y == 0) { cout << "X and Y are equal to 0" << endl; }
		if (x == 0 && z == 0) { cout << "X and Z are equal to 0" << endl; }
		if (y == 0 && z == 0) { cout << "Y and Z are equal to 0" << endl; }
		else (x != 0 && y != 0 && z != 0) { cout << "None of the coordinates are equal to 0" << endl; }
	}
	Vec distanceTo() {}
};

int main()
{
	Vec3 <int> Vector();

	Vector.getx();

	system("pause");
	return 0;
}