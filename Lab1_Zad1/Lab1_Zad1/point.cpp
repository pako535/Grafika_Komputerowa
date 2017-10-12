#include "point.h"



point::point()
{
	x = 0;
	y = 0;

}

point::point(GLfloat a, GLfloat b)
{
	x = a;
	y = b;

}


point::~point()
{
}

GLfloat point:: getX()
{
	return x;
}

GLfloat point::getY()
{
	return y;
}

void point::setPoint( GLfloat a, GLfloat b)
{
	x = a;
	y = b;
}