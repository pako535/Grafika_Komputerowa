/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>

#define M_PI 3.14159265358979323846

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

int model = 1;  // 1- punkty, 2- siatka, 3 - wype�nione tr�jk�ty
static GLfloat thetax = 0.0;   // k�t obrotu obiektu
static GLfloat thetay = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naci�ni�to �adnego klawisza
							   // 1 - naci�ni�ty zosta� lewy klawisz
							   // 2 - naci�ni�ty zosta� prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int zoom_old = 0;

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
static int delta_y = 0; 	   // i poprzedni� kursora myszy 
static int delta_zoom = 0;



static GLfloat xo = 10;
static GLfloat yo = 10;
static GLfloat zo = 10;



static int R = 10;

// inicjalizacja po�o�enia obserwatora
/*************************************************************************************/
// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y
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
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba 
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
	else if (model == 3)
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
	else if (model == 2)
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




void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(3.0, 3.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
	// Zdefiniowanie po�o�enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej

	//if (status == 2)
	//{

	//	//viewer[0] += delta_y*pix2angle;
	//	//viewer[1] += delta_y*pix2angle;
	//	viewer[2] += delta_zoom*pix2angle;

	//	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//}

	if (status == 1 || status == 2 )                     // je�li lewy klawisz myszy wci�ni�ty
	{
		//thetax += delta_x*pix2angle;    // modyfikacja k�ta obrotu o kat proporcjonalny
		//glRotatef(thetax, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t

		//thetay += delta_y*pix2angle;
		//glRotatef(thetay, 1.0, 0.0, 0.0);  //obr�t obiektu o nowy k�t

		viewer[0] +=  delta_x*pix2angle;
		//viewer[1] += yo; // delta_y*pix2angle;
		//viewer[2] += zo;// *pix2angle;

		//gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	}                                  // do r�nicy po�o�e� kursora myszy




									   //glRotatef(theta, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t


	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia�y
	Jajko();
	//glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();

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
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny
}
/*************************************************************************************/

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej


		y_pos_old = y;


		status = 1;		 // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		zoom_old = y;

		status = 2;
	}
	else

		status = 0;          // nie zosta� wci�ni�ty �aden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy

	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie

	

	delta_y = y - y_pos_old;

	y_pos_old = y;

	delta_zoom = y - zoom_old;

	zoom_old = y;
	
	R = zoom_old;

	xo = R*cos(delta_x)*cos(delta_y);
	yo = R*sin(delta_y);
	zo = R*sin(delta_x)*cos(delta_y);


	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej 

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej 

}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1200, 1200);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutKeyboardFunc(keys);

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy


	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna                       

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}