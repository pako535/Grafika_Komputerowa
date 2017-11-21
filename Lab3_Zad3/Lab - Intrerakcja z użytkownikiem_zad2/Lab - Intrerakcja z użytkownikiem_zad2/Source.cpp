/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzêdnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>

#define M_PI 3.14159265358979323846

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

int model = 1;  // 1- punkty, 2- siatka, 3 - wype³nione trójk¹ty
static GLfloat thetax = 0.0;   // k¹t obrotu obiektu
static GLfloat thetay = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - naciœniêty zostaæ lewy klawisz
							   // 2 - naciœniêty zosta³ prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int zoom_old = 0;

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_y = 0; 	   // i poprzedni¹ kursora myszy 
static int delta_zoom = 0;



static GLfloat xo = 10;
static GLfloat yo = 10;
static GLfloat zo = 10;



static int R = 10;

// inicjalizacja po³o¿enia obserwatora
/*************************************************************************************/
// Funkcja rysuj¹ca osie uk³adu wspó?rz?dnych

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
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba 
// przerysowaæ scenê)
// **************************************************************
// Przeliczenie wspo³rzêdnych dwu wymiarowych na wspolrzedne trzy wymiarowe dla x, y, z
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
	// Rzutowanie punktów o wspó³rzednych 3d na macierz
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
	//	Wygenerowanie tablicy kolorów
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
				//linie ukoœne
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
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(3.0, 3.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
	// Zdefiniowanie po³o¿enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej

	//if (status == 2)
	//{

	//	//viewer[0] += delta_y*pix2angle;
	//	//viewer[1] += delta_y*pix2angle;
	//	viewer[2] += delta_zoom*pix2angle;

	//	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//}

	if (status == 1 || status == 2 )                     // jeœli lewy klawisz myszy wciêniêty
	{
		//thetax += delta_x*pix2angle;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		//glRotatef(thetax, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t

		//thetay += delta_y*pix2angle;
		//glRotatef(thetay, 1.0, 0.0, 0.0);  //obrót obiektu o nowy k¹t

		viewer[0] +=  delta_x*pix2angle;
		//viewer[1] += yo; // delta_y*pix2angle;
		//viewer[2] += zo;// *pix2angle;

		//gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	}                                  // do ró¿nicy po³o¿eñ kursora myszy




									   //glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t


	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia³y
	Jajko();
	//glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
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
// Funkcja ustalaj¹ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny
}
/*************************************************************************************/

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej


		y_pos_old = y;


		status = 1;		 // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		zoom_old = y;

		status = 2;
	}
	else

		status = 0;          // nie zosta³ wciêniêty ¿aden klawisz 
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy

	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

	

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

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej 

}
/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1200, 1200);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutKeyboardFunc(keys);

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy


	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// za zmiany rozmiaru okna                       

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}