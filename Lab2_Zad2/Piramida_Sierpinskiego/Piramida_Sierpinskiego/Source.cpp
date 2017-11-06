#include <windows.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>	

using namespace std;


// Prototypy funkcji
void rysuj_piramide(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, GLfloat *e);
void podziel_piramide(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, GLfloat *e, int iteraciones);
void RenderScene();
void MyInint();

//punkty dla piramidy
GLfloat piramida[5][3] =
{ { 1.0f, -1.0f, 1.0f }, 
{ -1.0f,-1.0f, 1.0f },						
{ 0.0f, 1.0f, 0.0f },							
{ -1.0f,-1.0f,-1.0f },								
{ 1.0f, -1.0f, -1.0f }};								

int iteracje = 0;


void rysuj_piramide(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, GLfloat *e) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	// wyznaczenie 4 trojkątów dla stworzenia ostrosłupa 
	glBegin(GL_TRIANGLES);	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(a);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3fv(c);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3fv(e);
	glEnd();


	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(b);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3fv(c);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3fv(d);
	glEnd();

	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(c);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3fv(e);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3fv(d);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3fv(a);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3fv(b);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3fv(c);
	glEnd();
}

void podziel_piramide(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, GLfloat *e, int iteracja) {
	GLfloat wierzcholek[9][3];
	int j;
	if (iteracja > 0) {
		//znajdz punkty środkowe każdej krawędzi 
		//podział krawędzi wokół podstawy figury 
		for (j = 0; j < 3; j++) {
			wierzcholek[0][j] = (a[j] + b[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[1][j] = (b[j] + d[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[2][j] = (d[j] + e[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[3][j] = (e[j] + a[j]) / 2;
		}

		// podział krawędzi bocznych
		for (j = 0; j < 3; j++) {
			wierzcholek[4][j] = (c[j] + a[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[5][j] = (c[j] + b[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[6][j] = (c[j] + d[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[7][j] = (c[j] + e[j]) / 2;
		}
		for (j = 0; j < 3; j++) {
			wierzcholek[8][j] = (wierzcholek[3][j] + wierzcholek[1][j]) / 2;
		}

		//dla każdego trójkąta, który wchodzi, tworzone są 5 mniejsze trójkąty i rekurencyjnie są one podzielone po kolei 
		// od wierzchołka lewego dolnego w kierunku odwrotnym do wskazówek zegara, a na samym koncu górny trójkąt
		podziel_piramide(a, wierzcholek[0], wierzcholek[4], wierzcholek[8],wierzcholek[3], iteracja - 1);
		podziel_piramide(wierzcholek[0], b, wierzcholek[5], wierzcholek[1], wierzcholek[8], iteracja - 1);
		podziel_piramide(wierzcholek[8], wierzcholek[1], wierzcholek[6], d, wierzcholek[2], iteracja - 1);
		podziel_piramide(wierzcholek[3], wierzcholek[8], wierzcholek[7], wierzcholek[2], e, iteracja - 1);
		podziel_piramide(wierzcholek[4], wierzcholek[5], c, wierzcholek[6], wierzcholek[7], iteracja - 1);


	}
	else {
		//narysuj piramide gdy iteracja 0
		rysuj_piramide(a, b, c, d, e);
	}
}

void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();	
	glRotated(15, 0.0, 1.0, 0.0);  // Obrót o 15 stopni
	//random(true);
	//podczas wywoływania funkcji divideTriangle, czwarty parametr to liczba potrzebnych iteracji podpodziałów
	podziel_piramide(piramida[0], piramida[1], piramida[2], piramida[3],piramida[4], iteracje);

	glPopMatrix();
	glFlush();

	
}


void MyInint(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);		// czyszczenie coloru
	glColor3f(0.0, 0.0, 0.0);		//ustawienie koloru
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
	glEnable(GL_DEPTH_TEST);
}



int main(int argc, char** argv) {
	srand(time(NULL));

	// podanie ilosci iteracji
	cout << "Podaj liczbe interacji: ";
	cin >> iteracje;
	if (iteracje != 0)
		{
		iteracje--;
		}


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyświetlania
	// GLUT_RGBA - model kolorów RGB

	glutInitWindowSize(600, 600);
	glutCreateWindow("Piramida Sierpinskiego");
	// Utworzenie okna i określenie treści napisu w nagłówku okna
	glutDisplayFunc(RenderScene);

	MyInint();
	// Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczneprzed przystąpieniem do renderowania
	//glutDisplayFunc(RenderScene);
	
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}