#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

using namespace std;

class point
{
private:

	GLfloat x, y;
public:
	point();
	point(GLfloat x, GLfloat y);
	~point();
	

	GLfloat getX();
	GLfloat getY();

	void setPoint(GLfloat a, GLfloat b);

};

