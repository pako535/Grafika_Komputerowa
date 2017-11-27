/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rzednych
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846
using namespace std;

typedef float point3[3];
int model = 1;  // 1- punkty, 2- siatka, 3 - wype�nione tr�jk�ty
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k�ty obrotu
											/*************************************************************************************/


											// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz�tek i koniec obrazu osi y
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
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba 
// przerysowa� scen�)

// **************************************************************
// Przeliczenie wspo�rz�dnych dwu wymiarowych na wspolrzedne trzy wymiarowe dla x, y, z
//***************************************************************
float x(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float xx = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*cos(M_PI * v);

	return xx;
}


float y(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float yy = (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2)) - 5;

	return yy;
}

float z(int i, int j, float n)
{
	float u = i / (n - 1);
	float v = j / (n - 1);
	float zz = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*sin(M_PI * v);

	return zz;
}


float Xu(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float xx = ((-450 * pow(u, 4)) + (900 * pow(u, 3)) - (810 * pow(u, 2)) + (360 * u) - 45)*cos(M_PI * v);

	return xx;
}

float Xv(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float xx = M_PI*((90 * pow(u, 5)) - (225 * pow(u, 4)) + (270 * pow(u, 3)) - (180 * pow(u, 2)) - 45 * u)*sin(M_PI * v);

	return xx;
}

float Yu(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float yy = ((640 * pow(u, 3)) - (960 * pow(u, 2)) + (320 * u))*sin(M_PI * v);

	return yy;
}

float Yv(int i, int j, float n)
{
	return 0;
}


float Zu(int i, int j, float n)
{
	float u = i / (n - 1);
	float v = j / (n - 1);
	float zz = ((-450 * pow(u, 4)) + (900 * pow(u, 3)) - (810 * pow(u, 2)) + (360 * u) - 45)*sin(M_PI * v);

	return zz;
}

float Zv(int i, int j, float n)
{
	float u = i / (n - 1);
	float v = j / (n - 1);
	float zz = -M_PI*((90 * pow(u, 5)) - (225 * pow(u, 4)) + (270 * pow(u, 3)) - (180 * pow(u, 2)) + 45 * u)*cos(M_PI * v);

	return zz;
}

void Jajko()
{
	int N = 100;

	point3 ** tab = new point3 *[N];
	// **************************************************************
	// Rzutowanie punkt�w o wsp�rzednych 3d na macierz
	//***************************************************************
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

	// **************************************************************
	//	Wygenerowanie tablicy kolor�w
	//***************************************************************
	point3  **colors;
	colors = new point3*[N];

	for (int i = 0; i < N; i++) {
		colors[i] = new point3[N];
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			colors[i][j][0] = float(rand() % 1000) / 1000;
			colors[i][j][1] = float(rand() % 1000) / 1000;
			colors[i][j][2] = float(rand() % 1000) / 1000;
		}
	}
	// **************************************************************
	//	Wyswietlenie jajka w postaci punktow
	//***************************************************************
	if (model == 1)
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glVertex3fv(tab[i][j]);
			}
		}
		glEnd();
	}
	// **************************************************************
	// Wyswietlenie jajka w postaci siatki
	//***************************************************************
	else if (model == 2)
	{
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {

				// linie pionowe
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3fv(tab[i][j]);
				glVertex3fv(tab[(i + 1) % N][j]);
				glEnd();
				//linie uko�ne
				glBegin(GL_LINES);
				if (((j + 1) != N) && ((i + 1) != N)) {
					glVertex3fv(tab[i + 1][j]);
					glVertex3fv(tab[i][j + 1]);
				}
				else {
					if (i > 0) {
						glVertex3fv(tab[i][j]);
						glVertex3fv(tab[N - i - 1][0]);
					}
				}
				glEnd();
				//linie poziome
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				if ((j + 1) == N) {
					if (i > 0) {
						glVertex3fv(tab[N - i][0]);
						glVertex3fv(tab[i][j]);
					}
				}
				else {
					glVertex3fv(tab[i][j + 1]);
					glVertex3fv(tab[i][j]);
				}
				glEnd();
			}
		}
	}
	// **************************************************************
	// Wyswietlenie pokolorwanego jajaka
	//*************************************************************** 
	else if (model == 3)
	{
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if ((j + 1) != N) {
					glBegin(GL_TRIANGLES);
					
					glColor3fv(colors[i][j]);
					glVertex3fv(tab[i][j]);
					glColor3fv(colors[(i + 1) % N][j]);
					glVertex3fv(tab[(i + 1) % N][j]);
					glColor3fv(colors[i][j + 1]);
					glVertex3fv(tab[i][j + 1]);
					glEnd();

					glBegin(GL_TRIANGLES);
					glColor3fv(colors[(i + 1) % N][j]);
					glVertex3fv(tab[(i + 1) % N][j]);
					glColor3fv(colors[(i + 1) % N][j + 1]);
					glVertex3fv(tab[(i + 1) % N][j + 1]);
					glColor3fv(colors[i][j + 1]);
					glVertex3fv(tab[i][j + 1]);
					glEnd();
				}
				else {
					if (i > 0) {
						glBegin(GL_TRIANGLES);
						glColor3fv(colors[i][j]);
						glVertex3fv(tab[i][j]);
						glColor3fv(colors[(i + 1) % N][j]);
						glVertex3fv(tab[(i + 1) % N][j]);
						glColor3fv(colors[N - i][0]);
						glVertex3fv(tab[N - i][0]);
						glEnd();
					}
					glBegin(GL_TRIANGLES);
					glColor3fv(colors[(i + 1) % N][j]);
					glVertex3fv(tab[(i + 1) % N][j]);
					glColor3fv(colors[(N - i) % N][0]);
					glVertex3fv(tab[(N - i) % N][0]);
					glColor3fv(colors[N - i - 1][0]);
					glVertex3fv(tab[N - i - 1][0]);
					glEnd();

				}
			}
		}
	}
}

void spinEgg()
{

	theta[0] -= 0.5;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.5;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.5;
	if (theta[2] > 360.0) theta[2] -= 360.0;
	Sleep(50);

	glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wy�ej
	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);

	Jajko();
	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na bia�y
								 //glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
								 //glRotated(60.0, 1.0, 1.0, 1.0);  // Obr�t o 60 stopni

								 //glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

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
// Funkcja ustalaj�ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny

	/************************************************************************************* /
	//  Definicja materia�u z jakiego zrobiony jest czajnik 
	//  i definicja �r�d�a �wiat�a
	/*************************************************************************************/

	/*************************************************************************************/
	// Definicja materia�u z jakiego zrobiony jest czajnik 

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego                

	GLfloat mat_shininess = { 20.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni

	/*************************************************************************************/
	// Definicja �r�d�a �wiat�a

	GLfloat light_position[] = { (GLfloat) 0.0, (GLfloat)0.0, (GLfloat)10.0, (GLfloat)1.0 };
	// po�o�enie �r�d�a

	GLfloat light_ambient[] = { (GLfloat)0.1, (GLfloat)0.1,(GLfloat) 0.1, (GLfloat)1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { (GLfloat)1.0 };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { (GLfloat)0.05 };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { (GLfloat) 0.001 };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	/*************************************************************************************/
	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a
	/*************************************************************************************/
	// Ustawienie patrametr�w materia�u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametr�w �r�d�a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny 

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny 
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora 

	/*************************************************************************************/

}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
	// wymiar�w okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie��cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okre�lenie tak zwanej
	// przestrzeni ograniczaj�cej pozwalaj�cej zachowa� w�a�ciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj�cej s�u�y funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu                                   
	glLoadIdentity();
	// Czyszcenie macierzy bie��cej
}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void  main(int argc, char** argv)
{
	glutInit(&argc, argv);
	cout << sin(M_PI);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk�ad wsp�rz�dnych 3-D");
	glutKeyboardFunc(keys);
	//glutIdleFunc(spinEgg);
	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/
