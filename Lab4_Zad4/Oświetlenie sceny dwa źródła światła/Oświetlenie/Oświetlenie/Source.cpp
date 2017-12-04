#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cstdlib>
#include <iostream>

#define PI 3.14159265

typedef float point3[3]; // Definicja typu przechowuj¹cego wspó³rzêdne X,Y,Z punktu
const int N = 60; // Poziom szczegó³owoœci - rysowana figura sk³ada siê z N^2 punktów lub wierzcho³ków trójk¹tów

static GLfloat viewer[] = { 0.0, 0.0, 10.0 }; // Po³o¿enie obserwatora
static GLfloat p = 1.0; // Wspó³rzêdna Y skrócenia kamery
static GLfloat theta_x = 0.0, theta_y = 0.0, theta_zoom = 10.0, theta_x1 = 0.0, theta_y1 = 0.0 ;
static GLfloat pix2angle; // Przelicznik pikseli na stopieñ

static GLint status = 0; // Stan wciœniêcia przycisków myszy:
						 // 0) ¿aden przycisk nie jest wciœniêty,
						 // 1) wciœniêty zosta³ lewy przycisk,
						 // 2) wciœniêty zosta³ prawy przycisk

static int x_pos_old = 0; // Poprzednia pozycja X,Y kursora myszy
static int y_pos_old = 0;

static int delta_x = 0; // Ró¿nica w po³o¿eniu bie¿¹cym i poprzednim
static int delta_y = 0;
GLfloat light_position1[] = { 0.0, 0.0, 10.0, 1.0 }; // po³o¿enie Ÿrode³ œwiat³a
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
// Wywo³anie funkcji rysuj¹cej jajko
//***************************************************************
void rysuj_jajko()
{
	point3 points[N][N]; // Macierz przechowuj¹ca wspó³rzêdne N^2 punktów z których sk³ada siê obraz
	point3 normal[N][N]; // Macierz wspó³rzêdnych wektorów normalnych do wierzcho³ków trójk¹tów

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
			// Obliczenie wektorów normalnych
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

	// Wyœwietlenie wszystkich trójk¹tów
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
// Funkcja rysuj¹ca uk³ad wspó³rzêdnych
//***************************************************************
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	// pocz¹tek i koniec obrazu osi y

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
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba
// przerysowaæ scenê)
//***************************************************************

void RenderScene(void)
{	

	GLfloat R = 15;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, p, 0.0);
	Axes();

	if (status == 1) // Jeœli wciœniêto lewy przycisk myszy
	{
		// Obliczenie po³o¿enia pierwszego Ÿród³a œwiat³a 
		theta_x += delta_x * pix2angle / 30.0;
		theta_y += delta_y * pix2angle / 30.0;

		light_position[0] = R*cos(theta_x)*cos(theta_y);
		light_position[1] = R*sin(theta_y);
		light_position[2] = R*sin(theta_x)*cos(theta_y);

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	}
	else if (status == 2) // Jeœli wciœniêto prawy przycisk myszy						 
	{
		// Obliczenie po³o¿enia pierwszego Ÿród³a œwiat³a 
		theta_x1 += delta_x * pix2angle / 30.0;
		theta_y1 += delta_y * pix2angle / 30.0;


		light_position1[0] = R*cos(theta_x1)*cos(theta_y1);
		light_position1[1] = R*sin(theta_y1);
		light_position1[2] = R*sin(theta_x1)*cos(theta_y1);

		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	}

	// Wywo³anie funkcji rysuj¹cej jajko
	rysuj_jajko();

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
}

// **************************************************************
// Funkcja ob³uguj¹ca zdarzenia myszy
//***************************************************************
void Mouse(int btn, int state, int x, int y)
{
	// Jeœli wciœniêto lewy przycisk myszy
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	// Jeœli wciœniêto prawy przycisk myszy
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		y_pos_old = y;
		status = 2;
	}
	// Jeœli zwolniono przyciski myszy
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
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego                

	GLfloat mat_shininess = { 20.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	/*************************************************************************************/
	// Definicja Ÿród³a œwiat³a


	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = 1.0;
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = 0.05;
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = 0.001;
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	/*************************************************************************************/
	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a
	/*************************************************************************************/
	// Ustawienie patrametrów materia³u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie parametrów dla drugiego Ÿród³a œwiat³a

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny 

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny 
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_LIGHT1);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora 

							 /*************************************************************************************/
}

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej

	gluPerspective(100.0, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale?no?ci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej
}




// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
void main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Labki");

	// Uruchomienie obs³ugi zdarzeñ wciœniêcia klawisza, przycisku myszy
	// oraz jej przesuniêcia, wybranymi funkcjami
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

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