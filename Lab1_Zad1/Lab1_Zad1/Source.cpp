
/*************************************************************************************/
// Trochę bardziej skomplikowany program wykorzystujący funkcje biblioteki OpenGL
/*************************************************************************************/
#include "Header.h"
/*************************************************************************************/




//void RenderScene(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	// Czyszczenie okna aktualnym kolorem czyszczącym
//	glColor3f(0.0f, 1.0f, 0.0f);
//	// Ustawienie aktualnego koloru rysowania na zielony
//
//	glBegin(GL_TRIANGLES);       // Narysowanie zielonego trójkąta
//	glVertex2f(0.0f, 0.0f);
//	glVertex2f(0.0f, 50.0f);
//	glVertex2f(50.0f, 0.0f);
//	glEnd();
//	glColor3f(1.0f, 0.0f, 0.0f);
//	// Ustawienie aktualnego koloru rysowania na czerwony
//	glBegin(GL_TRIANGLES); // Narysowanie czerwonego trójkąta
//	glVertex2f(0.0f, 0.0f);
//	glVertex2f(0.0f, 50.0f);
//	glVertex2f(-50.0f, 0.0f);
//	glEnd();
//
//	glFlush();
//	// Przekazanie poleceń rysujących do wykonania
//}

//void RenderScene(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_TRIANGLES);
//	glColor3f(1.0f, 0.0f, 0.0f); // wierzchołek czerwony
//	glVertex2f(-50.0f, 0.0f);
//	glColor3f(0.0f, 1.0f, 0.0f); // wierzchołek zielony
//	glVertex2f(0.0f, 50.0f);
//	glColor3f(0.0f, 0.0f, 1.0f); // wierzchołek niebieski
//	glVertex2f(50.0f, 0.0f);
//	glEnd();
//	glFlush();
//}

//void SierpinskiCarpet(GLfloat x, GLfloat y, GLfloat width, GLint level)
//{
//
//	if (level > 0)
//	{
//		width = width / 3;
//		SierpinskiCarpet(x, y, width, level - 1);
//		SierpinskiCarpet(x - width, y, width, level - 1);
//		SierpinskiCarpet(x - 2 * width, y, width, level - 1);
//		SierpinskiCarpet(x, y - width, width, level - 1);
//		SierpinskiCarpet(x - 2 * width, y - width, width, level - 1);
//		SierpinskiCarpet(x, y - 2 * width, width, level - 1);
//		SierpinskiCarpet(x - width, y - 2 * width, width, level - 1);
//		SierpinskiCarpet(x - 2 * width, y - 2 * width, width, level - 1);
//	}
//
//	else
//	{
//		GLfloat def = (rand() % 20)*st_deformacji / (10 * width);
//		glBegin(GL_POLYGON);
//
//		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
//		glVertex2f(x + def, y + def);
//
//		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
//		glVertex2f(x + def, y - width + def);
//
//		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
//		glVertex2f(x - width + def, y - width + def);
//
//		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
//		glVertex2f(x - width + def, y + def);
//
//		glEnd();
//	}
//}
//
//
////void SierpinskiTriangle(point a, point b, point c, GLint level)
////{
////		glClear(GL_COLOR_BUFFER_BIT);
////		glBegin(GL_TRIANGLES);
////		glColor3f(1.0f, 0.0f, 0.0f); // wierzchołek czerwony
////		glVertex2f(a.getX, a.getY);
////		glVertex2f(b.getX, b.getY); 
////		glVertex2f(c.getX, c.getY);
////		
////		glEnd();
////		glFlush();
////}
//
//void RenderScene(void)
//{
//
//	glClear(GL_COLOR_BUFFER_BIT);
//	// Czyszczenie okna aktualnym kolorem czyszczacym
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	// Ustawienie aktualnego koloru rysowania na zielony
//
//	SierpinskiCarpet(50, 50, szerokosc, poziom);
//	//point a = new point(-50, 0);
//		
//	//cout << a.getX << " " << a.getY;
//	//SierpinskiTriangle(a,b,c,poziom);
//
//	glFlush();
//	// przekazanie polecen do wykonania 
//}
///*************************************************************************************/
//// Funkcja ustalająca stan renderowania
//
//void MyInit(void)
//{
//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//	// Kolor okna wnętrza okna - ustawiono na szary
//}
///*************************************************************************************/
//// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
//// niezależnie od rozmiarów okna graficznego
//
//
//void ChangeSize(GLsizei horizontal, GLsizei vertical)
//// Parametry horizontal i vertical (szerokość i wysokość okna) są
//// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna
//{
//	GLfloat AspectRatio;
//	// Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna
//
//	if (vertical == 0)
//		// Zabezpieczenie pzred dzieleniem przez 0
//		vertical = 1;
//
//	glViewport(0, 0, horizontal, vertical);
//	// Ustawienie wielkościokna okna urządzenia (Viewport)
//	// W tym przypadku od (0,0) do (horizontal, vertical)
//
//	glMatrixMode(GL_PROJECTION);
//	// Określenie układu współrzędnych obserwatora
//
//	glLoadIdentity();
//	// Określenie przestrzeni ograniczającej
//
//	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
//	// Wyznaczenie współczynnika proporcji okna
//	// Gdy okno na ekranie nie jest kwadratem wymagane jest 
//	// określenie okna obserwatora. 
//	// Pozwala to zachować właściwe proporcje rysowanego obiektu
//	// Do określenia okna obserwatora służy funkcja glOrtho(...)
//
//	if (horizontal <= vertical)
//		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
//	else
//		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);
//	glMatrixMode(GL_MODELVIEW);
//	// Określenie układu współrzędnych    
//	glLoadIdentity();
//}
///*************************************************************************************/
//// Główny punkt wejścia programu. Program działa w trybie konsoli
//
//
//
////!!! FUNKCJA main musi wygladac tak dla -> NUPEN_GL - NuGet
//void main(int argc, char* argv[])
//{
//	cout << "Jaki algorytm chcesz realizowac\n 1->Dywan Sierpinskiego\n 2->Trojkat Sierpinskiego :";
//	int  k = 0;
//	cin >> k;
//	if (k == 1)
//	{
//		cout << "Podaj poziom rekurencji: ";
//		cin >> poziom;
//		cout << "Podaj stopien deformacji: ";
//		cin >> st_deformacji;
//	}
//	else if (k == 2)
//	{
//		cout << "Podaj poziom rekurencji: ";
//		cin >> poziom;
//		cout << "Podaj stopien deformacji: ";
//		cin >> st_deformacji;
//	}
//	
//	
//	
//
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
//	// Ustawienie trybu wyświetlania
//	// GLUT_SINGLE - pojedynczy bufor wyświetlania
//	// GLUT_RGBA - model kolorów RGB
//	
//	//!!! FUNKCJA DLA NUPEN_GL - NuGet
//	glutInit( &argc,  argv);
//
//	glutCreateWindow(" Window title");
//	// Utworzenie okna i określenie treści napisu w nagłówku okna
//	
//
//	
//
//	glutDisplayFunc(RenderScene);
//	// Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
//	// Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
//	// trzeba będzie przerysować okno
//
//	
//	glutReshapeFunc(ChangeSize);
//	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
//	// zmiany rozmiaru okna
//	
//	MyInit();
//	// Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie  
//	// inicjalizacje konieczneprzed przystąpieniem do renderowania
//	
//	glutMainLoop();
//	// Funkcja uruchamia szkielet biblioteki GLUT
//
//
//
//	
//}
///*************************************************************************************/


void myinit()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);/* white background */
	glColor3f(1.0, 0.0, 0.0); /* red paint */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 50.0, 0.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
}

// Funkcaja określająca, co ma być rysowane 
// (zawsze wywoływana, gdy trzeba przerysować scenę)
//void RenderScene(void)
//{
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	// Ustawienie aktualnego koloru rysowania na zielony
//
//	glRectf(-50.0f, 50.0f, 50.0f, -50.0f);
//	// Narysowanie prostokąta
//
//	glFlush();
//	// Przekazanie poleceń rysujących do wykonania
//}
void RenderScene()
{
	// współrzędne trójkąta
	GLfloat vertices[3][2] = { { 0.0, 0.0 },{ 25.0, 50.0 },{ 50.0, 0.0 } };

	int pointsNum, midPoint;
	//int rand();
	// triangle inside point
	GLfloat p[2] = { 7.5, 5.0 };
	glClear(GL_COLOR_BUFFER_BIT); 
	// Czyszczenie okna aktualnym kolorem czyszczącym
	glBegin(GL_POINTS);
	
	for (pointsNum = 0; pointsNum < 50000; pointsNum++)
	{
		midPoint = rand() % 3;
		// middle
		p[0] = (p[0] + vertices[midPoint][0]) / 2.0;
		p[1] = (p[1] + vertices[midPoint][1]) / 2.0;
		glVertex2fv(p);
	}
	glEnd();


	glFlush();
	// Przekazanie poleceń rysujących do wykonania
}

int main(int argc, char *argv[])
{
	// initialize 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sierpinski Triangle");
	glutDisplayFunc(RenderScene);
	myinit();
	glutMainLoop();

}