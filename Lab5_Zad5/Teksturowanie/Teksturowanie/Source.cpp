#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cstdlib>
#include <iostream>

#define PI 3.14159265
#pragma warning (disable : 4996)
typedef float point3[3]; // Definicja typu przechowuj�cego wsp�rz�dne X,Y,Z punktu
const int N = 50; // Poziom szczeg�owo�ci - rysowana figura sk�ada si� z N^2 punkt�w lub wierzcho�k�w tr�jk�t�w

static GLfloat viewer[] = { 0.0, 0.0, 10.0 }; // Po�o�enie obserwatora
static GLfloat p = 1.0; // Wsp�rz�dna Y skr�cenia kamery
static GLfloat thetax = 0.0, thetay = 0.0, theta_zoom = 10.0;
static GLfloat pix2angle; // Przelicznik pikseli na stopie�

static GLint status = 0; // Stan wci�ni�cia przycisk�w myszy:
						 // 0) �aden przycisk nie jest wci�ni�ty,
						 // 1) wci�ni�ty zosta� lewy przycisk,
						 // 2) wci�ni�ty zosta� prawy przycisk

static int x_pos_old = 0; // Poprzednia pozycja X,Y kursora myszy
static int y_pos_old = 0;

static int delta_x = 0; // R�nica w po�o�eniu bie��cym i poprzednim
static int delta_y = 0;

// Funkcja wykonuj�ca w�a�ciwe rysowanie obiektu - jajka
void rysuj_jajko()
{
	point3 punkty[N][N]; // Macierz przechowuj�ca wsp�rz�dne N^2 punkt�w z kt�rych sk�ada si� obraz
	point3 nor[N][N]; // Macierz wsp�rz�dnych wektor�w normalnych do wierzcho�k�w tr�jk�t�w

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			// Obliczenie wsp�rz�dnych punkt�w i zapisanie ich do macierzy, korzystaj�c ze wzor�w podanych w instrukcji laboratoryjnej
			float u = (float)i / (float)(N - 1);
			float v = (float)j / (float)(N - 1);
			punkty[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(3.14*v);
			punkty[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			punkty[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(3.14*v);

			// Obliczenie wektor�w normalnych do punkt�w tworz�cych figur�
			float xu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(PI * v);
			float yu = 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
			float zu = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(PI * v);

			float xv = PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(PI * v);
			float yv = 0;
			float zv = -PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(PI * v);

			if (i < (N / 2))
			{
				nor[i][j][0] = (yu * zv - zu * yv);
				nor[i][j][1] = (zu * xv - xu * zv);
				nor[i][j][2] = (xu * yv - yu * xv);
			}
			else
			{
				nor[i][j][0] = -(yu * zv - zu * yv);
				nor[i][j][1] = -(zu * xv - xu * zv);
				nor[i][j][2] = -(xu * yv - yu * xv);
			}

			// Zamiana na wektory jednostkowe
			float len = sqrt(pow(nor[i][j][0], 2) + pow(nor[i][j][1], 2) + pow(nor[i][j][2], 2));
			for (int k = 0; k<3; k++)
				nor[i][j][k] /= len;
		}

	// Ustawienie koloru rysowania na bia�y
	glColor3f(1.0f, 1.0f, 1.0f);

	// Wy�wietlenie wszystkich tr�jk�t�w
	for (int i = N/2; i < N - 1; i++)
		for (int j = 0; j < N - 1; j++)
		{
			// Wy�wietlenie tr�jk�ta g�rnego w danym "kwadracie"
			glBegin(GL_TRIANGLES);

			glNormal3fv(nor[i][j]); // Naniesienie wektora normalnego do punktu			
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(punkty[i + 1][j + 1][0], punkty[i + 1][j + 1][1] - 5, punkty[i + 1][j + 1][2]);

			glNormal3fv(nor[i + 1][j]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(punkty[i + 1][j][0], punkty[i + 1][j][1] - 5, punkty[i + 1][j][2]);

			glNormal3fv(nor[i + 1][j + 1]);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f(punkty[i][j][0], punkty[i][j][1] - 5, punkty[i][j][2]); // Naniesienie punktu
			
			
			glEnd();

			// Wy�wietlenie tr�jk�ta dolnego w danym "kwadracie"
			glBegin(GL_TRIANGLES);
			glNormal3fv(nor[i][j]);			
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(punkty[i][j][0], punkty[i][j][1] - 5, punkty[i][j][2]);
			
			glNormal3fv(nor[i][j + 1]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(punkty[i][j + 1][0], punkty[i][j + 1][1] - 5, punkty[i][j + 1][2]);
			
			glNormal3fv(nor[i + 1][j + 1]);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f(punkty[i + 1][j + 1][0], punkty[i + 1][j + 1][1] - 5, punkty[i + 1][j + 1][2]);
			glEnd();
		}


	for (int i = 0 ; i < N/2; i++)
		for (int j = 0; j < N - 1; j++)
		{
			// Wy�wietlenie tr�jk�ta g�rnego w danym "kwadracie"
			glBegin(GL_TRIANGLES);

			glNormal3fv(nor[i][j]); // Naniesienie wektora normalnego do punktu
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f(punkty[i][j][0], punkty[i][j][1] - 5, punkty[i][j][2]); // Naniesienie punktu

			glNormal3fv(nor[i + 1][j]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(punkty[i + 1][j][0], punkty[i + 1][j][1] - 5, punkty[i + 1][j][2]);

			glNormal3fv(nor[i + 1][j + 1]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(punkty[i + 1][j + 1][0], punkty[i + 1][j + 1][1] - 5, punkty[i + 1][j + 1][2]);
		

			glEnd();

			// Wy�wietlenie tr�jk�ta dolnego w danym "kwadracie"
			glBegin(GL_TRIANGLES);
			glNormal3fv(nor[i][j]);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f(punkty[i + 1][j + 1][0], punkty[i + 1][j + 1][1] - 5, punkty[i + 1][j + 1][2]);
			
			glNormal3fv(nor[i][j + 1]);
			glTexCoord2f(1.0f, 0.0f);

			glVertex3f(punkty[i][j + 1][0], punkty[i][j + 1][1] - 5, punkty[i][j + 1][2]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(punkty[i][j][0], punkty[i][j][1] - 5, punkty[i][j][2]);
			
			
			glEnd();
		}



}

// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych
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

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, p, 0.0);
	Axes();

	if (status == 1) // Je�li wci�ni�to lewy przycisk myszy
	{
		// Obliczenie po�o�enia obserwatora - k�ta azymutu i elewacji (X, Y) wzgl�dem �rodka sfery
		thetax += delta_x * pix2angle / 30.0;
		thetay += delta_y * pix2angle / 30.0;
	}
	else if (status == 2) // Je�li wci�ni�to prawy przycisk myszy
						  // Obliczenie odleg�o�ci od �rodka sfery, obserwatora
		theta_zoom += delta_y / 10.0;

	// Ograniczenie zakresu obliczanych warto�ci, do tych podanych w instrukcji
	if (thetay > 3.1415) thetay -= 2 * 3.1415;
	else if (thetay <= -3.1415) thetay += 2 * 3.1415;

	if (thetay > 3.1415 / 2 || thetay < -3.1415 / 2) p = -1.0;
	else p = 1.0;

	// Obliczenie po�o�enia we wsp�rz�dnych kartezja�skich, obserwatora
	viewer[0] = theta_zoom * cos(thetax)*cos(thetay);
	viewer[1] = theta_zoom * sin(thetay);
	viewer[2] = theta_zoom * sin(thetax)*cos(thetay);

	// Wywo�anie funkcji rysuj�cej jajko
	rysuj_jajko();

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
}

// Funkcja obs�uguj�ca zdarzenie wci�ni�cia przycisku myszy
void Mouse(int btn, int state, int x, int y)
{
	// Je�li wci�ni�to lewy przycisk myszy
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Ustawienie obecnego po�o�enia jako poprzedniego, na potrzeby funkcji Motion
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	// Je�li wci�ni�to prawy przycisk myszy
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// Ustawienie obecnego po�o�enia jako poprzedniego, na potrzeby funkcji Motion
		y_pos_old = y;
		status = 2;
	}
	// Je�li zwolniono przyciski myszy
	else
		status = 0;
}

// Funkcja obs�uguj�ca zdarzenie przesuni�cia myszy
void Motion(GLsizei x, GLsizei y)
{
	// Obliczenie przesuni�cia wzgl�dem po�o�enia poprzedniego
	delta_x = x - x_pos_old;
	x_pos_old = x;
	delta_y = y - y_pos_old;
	y_pos_old = y;

	glutPostRedisplay();
}





/*************************************************************************************/
// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie 
// FileName, alokuje pami�� i zwraca wska�nik (pBits) do bufora w kt�rym 
// umieszczone s� dane. 
// Ponadto udost�pnia szeroko�� (ImWidth), wysoko�� (ImHeight) obrazu
// tekstury oraz dane opisuj�ce format obrazu wed�ug specyfikacji OpenGL 
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytuj�cej dane z pliku TGA.
// Dzia�a tylko dla obraz�w wykorzystuj�cych 8, 24, or 32 bitowy kolor.
// Nie obs�uguje plik�w w formacie TGA kodowanych z kompresj� RLE.
/*************************************************************************************/
GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
	/*************************************************************************************/
	// Struktura dla nag��wka pliku  TGA

#pragma pack(1)            
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;

	/*************************************************************************************/
	// Warto�ci domy�lne zwracane w przypadku b��du 

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;
	/*************************************************************************************/
	// Przeczytanie nag��wka pliku 

	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	/*************************************************************************************/
	// Odczytanie szeroko�ci, wysoko�ci i g��bi obrazu 

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	/*************************************************************************************/
	// Sprawdzenie, czy g��bia spe�nia za�o�one warunki (8, 24, lub 32 bity) 

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/
	// Obliczenie rozmiaru bufora w pami�ci

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	/*************************************************************************************/
	// Alokacja pami�ci dla danych obrazu

	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}

	/*************************************************************************************/
	// Ustawienie formatu OpenGL

	switch (sDepth)
	{
	case 3:
		*ImFormat = GL_BGR_EXT;
		*ImComponents = GL_RGB8;
		break;
	case 4:
		*ImFormat = GL_BGRA_EXT;
		*ImComponents = GL_RGBA8;
		break;
	case 1:
		*ImFormat = GL_LUMINANCE;
		*ImComponents = GL_LUMINANCE8;
		break;
	};

	fclose(pFile);

	return pbitsperpixel;
}
/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	/*************************************************************************************/
	// Zmienne dla obrazu tekstury

	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/*************************************************************************************/
	// Teksturowanie b�dzie prowadzone tyko po jednej stronie �ciany 

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/
	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("t_256.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/
	// Zdefiniowanie tekstury 2-D 

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/
	// Zwolnienie pami�ci

	free(pBytes);

	/*************************************************************************************/
	// W��czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/
	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/
	// Okre�lenie sposobu nak�adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny

	/*************************************************************************************/
	//  Definicja materia�u z jakiego zrobione jest jajko
	//  i definicja �r�d�a �wiat�a
	/*************************************************************************************/

	/*************************************************************************************/
	// Definicja materia�u z jakiego zrobiony jest czajnik 

	//GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	//// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	//GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego                

	//GLfloat mat_shininess = { 20.0 };
	//// wsp�czynnik n opisuj�cy po�ysk powierzchni

	///*************************************************************************************/
	//// Definicja �r�d�a �wiat�a

	//GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	//// po�o�enie �r�d�a

	//GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	//// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	//// Ia = [Iar,Iag,Iab]

	//GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	//// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	//// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	//GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	//// odbicie kierunkowe Is = [Isr,Isg,Isb]

	//GLfloat att_constant = 1.0;
	//// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji 
	//// odleg�o�ci od �r�d�a

	//GLfloat att_linear = 0.05;
	//// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji 
	//// odleg�o�ci od �r�d�a

	//GLfloat att_quadratic = 0.001;
	//// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	//// odleg�o�ci od �r�d�a

	///*************************************************************************************/
	//// Ustawienie parametr�w materia�u i �r�d�a �wiat�a
	///*************************************************************************************/
	//// Ustawienie patrametr�w materia�u

	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	///*************************************************************************************/
	//// Ustawienie parametr�w �r�d�a

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	///*************************************************************************************/
	//// Ustawienie opcji systemu o�wietlania sceny 

	//glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	//glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny 
	//glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	//glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora 

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
	glutCreateWindow("Unicorn blood");

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