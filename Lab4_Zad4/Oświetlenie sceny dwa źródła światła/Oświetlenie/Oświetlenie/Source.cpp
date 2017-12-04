#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cstdlib>
#include <iostream>

#define PI 3.14159265

typedef float point3[3]; // Definicja typu przechowuj�cego wsp�rz�dne X,Y,Z punktu
const int N = 60; // Poziom szczeg�owo�ci - rysowana figura sk�ada si� z N^2 punkt�w lub wierzcho�k�w tr�jk�t�w

static GLfloat viewer[] = { 0.0, 0.0, 10.0 }; // Po�o�enie obserwatora
static GLfloat p = 1.0; // Wsp�rz�dna Y skr�cenia kamery
static GLfloat theta_x = 0.0, theta_y = 0.0, theta_zoom = 10.0, theta_x1 = 0.0, theta_y1 = 0.0 ;
static GLfloat pix2angle; // Przelicznik pikseli na stopie�

static GLint status = 0; // Stan wci�ni�cia przycisk�w myszy:
						 // 0) �aden przycisk nie jest wci�ni�ty,
						 // 1) wci�ni�ty zosta� lewy przycisk,
						 // 2) wci�ni�ty zosta� prawy przycisk

static int x_pos_old = 0; // Poprzednia pozycja X,Y kursora myszy
static int y_pos_old = 0;

static int delta_x = 0; // R�nica w po�o�eniu bie��cym i poprzednim
static int delta_y = 0;
GLfloat light_position1[] = { 0.0, 0.0, 10.0, 1.0 }; // po�o�enie �rode� �wiat�a
GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };


// **************************************************************
// Przeliczenie wspolrzednych dwu wymiarowych na wspolrzedne trzy wymiarowe dla x, y, z
//***************************************************************
float x(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float xx = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*cos(PI * v);

	return xx;
}


float y(int i, int j, float n)
{

	float u = i / (n - 1);
	float v = j / (n - 1);
	float yy = (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2));

	return yy;
}

float z(int i, int j, float n)
{
	float u = i / (n - 1);
	float v = j / (n - 1);
	float zz = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - 45 * u)*sin(PI * v);

	return zz;
}


// **************************************************************
// Wywo�anie funkcji rysuj�cej jajko
//***************************************************************
void rysuj_jajko()
{
	point3 points[N][N]; // Macierz przechowuj�ca wsp�rz�dne N^2 punkt�w z kt�rych sk�ada si� obraz
	point3 normal[N][N]; // Macierz wsp�rz�dnych wektor�w normalnych do wierzcho�k�w tr�jk�t�w

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
		
			float u = (float)i / (float)(N - 1);
			float v = (float)j / (float)(N - 1);

			// **************************************************************
			// Rzutowanie punktow o wsporzednych 3d na macierz
			//***************************************************************
			points[i][j][0] = x(i, j, N);
			points[i][j][1] = y(i, j, N);
			points[i][j][2] = z(i, j, N);

			// **************************************************************
			// Obliczenie wektor�w normalnych
			//***************************************************************
			float xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(PI * v);
			float yu = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
			float zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(PI * v);

			float xv = PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(PI * v);
			float yv = 0;
			float zv = -PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(PI * v);

			

			if (i < (N / 2))
			{
				normal[i][j][0] = (yu * zv - zu * yv);
				normal[i][j][1] = (zu * xv - xu * zv);
				normal[i][j][2] = (xu * yv - yu * xv);
			}
			else
			{
				normal[i][j][0] = -(yu * zv - zu * yv);
				normal[i][j][1] = -(zu * xv - xu * zv);
				normal[i][j][2] = -(xu * yv - yu * xv);
			}

			// Zamiana na wektory jednostkowe
			float len = sqrt(pow(normal[i][j][0], 2) + pow(normal[i][j][1], 2) + pow(normal[i][j][2], 2));
			for (int k = 0; k<3; k++)
				normal[i][j][k] /= len;
		}

	
	glColor3f(1.0f, 0.0f, 1.0f);

	// Wy�wietlenie wszystkich tr�jk�t�w
	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - 1; j++)
		{
			
			glBegin(GL_TRIANGLES);
			glNormal3fv(normal[i][j]); // Naniesienie wektora normalnego do punktu
			glVertex3f(points[i][j][0], points[i][j][1] - 5, points[i][j][2]); // Naniesienie punktu
			glNormal3fv(normal[i + 1][j]);
			glVertex3f(points[i + 1][j][0], points[i + 1][j][1] - 5, points[i + 1][j][2]);
			glNormal3fv(normal[i + 1][j + 1]);
			glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1] - 5, points[i + 1][j + 1][2]);
			glEnd();

			
			glBegin(GL_TRIANGLES);
			glNormal3fv(normal[i][j]);
			glVertex3f(points[i][j][0], points[i][j][1] - 5, points[i][j][2]);
			glNormal3fv(normal[i][j + 1]);
			glVertex3f(points[i][j + 1][0], points[i][j + 1][1] - 5, points[i][j + 1][2]);
			glNormal3fv(normal[i + 1][j + 1]);
			glVertex3f(points[i + 1][j + 1][0], points[i + 1][j + 1][1] - 5, points[i + 1][j + 1][2]);
			glEnd();
		}
}



// **************************************************************
// Funkcja rysuj�ca uk�ad wsp�rz�dnych
//***************************************************************
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz�tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz�tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	// pocz�tek i koniec obrazu osi y

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



// **************************************************************
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)
//***************************************************************

void RenderScene(void)
{	

	GLfloat R = 15;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, p, 0.0);
	Axes();

	if (status == 1) // Je�li wci�ni�to lewy przycisk myszy
	{
		// Obliczenie po�o�enia pierwszego �r�d�a �wiat�a 
		theta_x += delta_x * pix2angle / 30.0;
		theta_y += delta_y * pix2angle / 30.0;

		light_position[0] = R*cos(theta_x)*cos(theta_y);
		light_position[1] = R*sin(theta_y);
		light_position[2] = R*sin(theta_x)*cos(theta_y);

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (status == 2) // Je�li wci�ni�to prawy przycisk myszy						 
	{
		// Obliczenie po�o�enia pierwszego �r�d�a �wiat�a 
		theta_x1 += delta_x * pix2angle / 30.0;
		theta_y1 += delta_y * pix2angle / 30.0;


		light_position1[0] = R*cos(theta_x1)*cos(theta_y1);
		light_position1[1] = R*sin(theta_y1);
		light_position1[2] = R*sin(theta_x1)*cos(theta_y1);

		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	}

	// Wywo�anie funkcji rysuj�cej jajko
	rysuj_jajko();

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
}

// **************************************************************
// Funkcja ob�uguj�ca zdarzenia myszy
//***************************************************************
void Mouse(int btn, int state, int x, int y)
{
	// Je�li wci�ni�to lewy przycisk myszy
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	// Je�li wci�ni�to prawy przycisk myszy
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		y_pos_old = y;
		status = 2;
	}
	// Je�li zwolniono przyciski myszy
	else
		status = 0;
}

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;
	x_pos_old = x;
	delta_y = y - y_pos_old;
	y_pos_old = y;

	glutPostRedisplay();
}


void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny

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


	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = 1.0;
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = 0.05;
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = 0.001;
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
	// Ustawienie parametr�w dla drugiego �r�d�a �wiat�a

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny 

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny 
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_LIGHT1);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora 

							 /*************************************************************************************/
}

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	gluPerspective(100.0, 1.0, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale?no?ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
}




// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Labki");

	// Uruchomienie obs�ugi zdarze� wci�ni�cia klawisza, przycisku myszy
	// oraz jej przesuni�cia, wybranymi funkcjami
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

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