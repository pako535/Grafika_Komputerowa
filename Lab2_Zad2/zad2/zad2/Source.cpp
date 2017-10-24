/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzednych
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846
using namespace std;

typedef float point3[3];
int model = 1;  // 1- punkty, 2- siatka, 3 - wype³nione trójk¹ty
/*************************************************************************************/


// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz¹tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}
/*************************************************************************************/
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba 
// przerysowaæ scenê)


float x(int i, int j,float n)
{	

	float u = i / (n-1);
	float v = j / (n-1);
	float xx = ((-90 * pow( u, 5)) + (225 * pow(u , 4)) - (270* pow(u , 3)) + (180 * pow( u , 2)) - 45*u)*cos(M_PI * v);

	return xx;
}


float y(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float yy = (160 * pow( u , 4)) - (320 * pow( u , 3)) + (160 * pow(u , 2)) - 5 ;
	
	return yy;
}

float z(int i, int j, float n)
{
	float u = i / (n - 1);
	float v = j / (n - 1);
	float zz = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45*u)*sin(M_PI * v);

	return zz;
}

void Jajko()
{
	int N = 100;

	point3 ** tab = new point3 *[N];

	for (int i = 0; i < N; i++)
	{
		tab[i] = new point3[N];
	}


	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			tab[i][j][0] = x(i, j, N);
			tab[i][j][1] = y(i, j, N);
			tab[i][j][2] = z(i, j, N);

		}
	}
	

	
	/*glBegin(GL_POINTS);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			glVertex3fv(tab[i][j]);
		}
	}
	glEnd();*/


	/*glBegin(GL_LINES);
	for (int i = 0; i < N-1; i++)
	{
		for (int j = 0; j < N-1; j++)
		{
			glVertex3fv(tab[i][j]);
			glVertex3fv(tab[i+1][j+1]);
		}
	}
	glEnd();*/


// Wype³nic trojkatami nie liniami

}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy¿ej
	Jajko();
	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na bia³y
	//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	//glRotated(60.0, 1.0, 1.0, 1.0);  // Obrót o 60 stopni

	//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
	//
}



void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}

/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny
}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
	// przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                   
	glLoadIdentity();
	// Czyszcenie macierzy bie¿¹cej
}
/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void main(void)
{

	cout << sin(M_PI);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");
	glutKeyboardFunc(keys);
	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/
