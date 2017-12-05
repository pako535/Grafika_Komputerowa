#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cstdlib>
#include <iostream>

#define PI 3.14159265
#pragma warning (disable : 4996)
typedef float point3[3]; // Definicja typu przechowuj¹cego wspó³rzêdne X,Y,Z punktu
const int N = 50; // Poziom szczegó³owoœci - rysowana figura sk³ada siê z N^2 punktów lub wierzcho³ków trójk¹tów

static GLfloat viewer[] = { 0.0, 0.0, 10.0 }; // Po³o¿enie obserwatora
static GLfloat p = 1.0; // Wspó³rzêdna Y skrócenia kamery
static GLfloat thetax = 0.0, thetay = 0.0, theta_zoom = 10.0;
static GLfloat pix2angle; // Przelicznik pikseli na stopieñ

static GLint status = 0; // Stan wciœniêcia przycisków myszy:
						 // 0) ¿aden przycisk nie jest wciœniêty,
						 // 1) wciœniêty zosta³ lewy przycisk,
						 // 2) wciœniêty zosta³ prawy przycisk

static int x_pos_old = 0; // Poprzednia pozycja X,Y kursora myszy
static int y_pos_old = 0;

static int delta_x = 0; // Ró¿nica w po³o¿eniu bie¿¹cym i poprzednim
static int delta_y = 0;

// Funkcja wykonuj¹ca w³aœciwe rysowanie obiektu - jajka
void rysuj_jajko()
{
	point3 punkty[N][N]; // Macierz przechowuj¹ca wspó³rzêdne N^2 punktów z których sk³ada siê obraz
	point3 nor[N][N]; // Macierz wspó³rzêdnych wektorów normalnych do wierzcho³ków trójk¹tów

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			// Obliczenie wspó³rzêdnych punktów i zapisanie ich do macierzy, korzystaj¹c ze wzorów podanych w instrukcji laboratoryjnej
			float u = (float)i / (float)(N - 1);
			float v = (float)j / (float)(N - 1);
			punkty[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(3.14*v);
			punkty[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			punkty[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(3.14*v);

			// Obliczenie wektorów normalnych do punktów tworz¹cych figurê
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

	// Ustawienie koloru rysowania na bia³y
	glColor3f(1.0f, 1.0f, 1.0f);

	// Wyœwietlenie wszystkich trójk¹tów
	for (int i = N/2; i < N - 1; i++)
		for (int j = 0; j < N - 1; j++)
		{
			// Wyœwietlenie trójk¹ta górnego w danym "kwadracie"
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

			// Wyœwietlenie trójk¹ta dolnego w danym "kwadracie"
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
			// Wyœwietlenie trójk¹ta górnego w danym "kwadracie"
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

			// Wyœwietlenie trójk¹ta dolnego w danym "kwadracie"
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

// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych
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

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba
// przerysowaæ scenê)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, p, 0.0);
	Axes();

	if (status == 1) // Jeœli wciœniêto lewy przycisk myszy
	{
		// Obliczenie po³o¿enia obserwatora - k¹ta azymutu i elewacji (X, Y) wzglêdem œrodka sfery
		thetax += delta_x * pix2angle / 30.0;
		thetay += delta_y * pix2angle / 30.0;
	}
	else if (status == 2) // Jeœli wciœniêto prawy przycisk myszy
						  // Obliczenie odleg³oœci od œrodka sfery, obserwatora
		theta_zoom += delta_y / 10.0;

	// Ograniczenie zakresu obliczanych wartoœci, do tych podanych w instrukcji
	if (thetay > 3.1415) thetay -= 2 * 3.1415;
	else if (thetay <= -3.1415) thetay += 2 * 3.1415;

	if (thetay > 3.1415 / 2 || thetay < -3.1415 / 2) p = -1.0;
	else p = 1.0;

	// Obliczenie po³o¿enia we wspó³rzêdnych kartezjañskich, obserwatora
	viewer[0] = theta_zoom * cos(thetax)*cos(thetay);
	viewer[1] = theta_zoom * sin(thetay);
	viewer[2] = theta_zoom * sin(thetax)*cos(thetay);

	// Wywo³anie funkcji rysuj¹cej jajko
	rysuj_jajko();

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
}

// Funkcja obs³uguj¹ca zdarzenie wciœniêcia przycisku myszy
void Mouse(int btn, int state, int x, int y)
{
	// Jeœli wciœniêto lewy przycisk myszy
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Ustawienie obecnego po³o¿enia jako poprzedniego, na potrzeby funkcji Motion
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	// Jeœli wciœniêto prawy przycisk myszy
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// Ustawienie obecnego po³o¿enia jako poprzedniego, na potrzeby funkcji Motion
		y_pos_old = y;
		status = 2;
	}
	// Jeœli zwolniono przyciski myszy
	else
		status = 0;
}

// Funkcja obs³uguj¹ca zdarzenie przesuniêcia myszy
void Motion(GLsizei x, GLsizei y)
{
	// Obliczenie przesuniêcia wzglêdem po³o¿enia poprzedniego
	delta_x = x - x_pos_old;
	x_pos_old = x;
	delta_y = y - y_pos_old;
	y_pos_old = y;

	glutPostRedisplay();
}





/*************************************************************************************/
// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie 
// FileName, alokuje pamiêæ i zwraca wskaŸnik (pBits) do bufora w którym 
// umieszczone s¹ dane. 
// Ponadto udostêpnia szerokoœæ (ImWidth), wysokoœæ (ImHeight) obrazu
// tekstury oraz dane opisuj¹ce format obrazu wed³ug specyfikacji OpenGL 
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytuj¹cej dane z pliku TGA.
// Dzia³a tylko dla obrazów wykorzystuj¹cych 8, 24, or 32 bitowy kolor.
// Nie obs³uguje plików w formacie TGA kodowanych z kompresj¹ RLE.
/*************************************************************************************/
GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
	/*************************************************************************************/
	// Struktura dla nag³ówka pliku  TGA

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
	// Wartoœci domyœlne zwracane w przypadku b³êdu 

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;
	/*************************************************************************************/
	// Przeczytanie nag³ówka pliku 

	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	/*************************************************************************************/
	// Odczytanie szerokoœci, wysokoœci i g³êbi obrazu 

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	/*************************************************************************************/
	// Sprawdzenie, czy g³êbia spe³nia za³o¿one warunki (8, 24, lub 32 bity) 

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/
	// Obliczenie rozmiaru bufora w pamiêci

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	/*************************************************************************************/
	// Alokacja pamiêci dla danych obrazu

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

// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	/*************************************************************************************/
	// Zmienne dla obrazu tekstury

	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/*************************************************************************************/
	// Teksturowanie bêdzie prowadzone tyko po jednej stronie œciany 

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/
	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("t_256.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/
	// Zdefiniowanie tekstury 2-D 

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/
	// Zwolnienie pamiêci

	free(pBytes);

	/*************************************************************************************/
	// W³¹czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/
	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/
	// Okreœlenie sposobu nak³adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny

	/*************************************************************************************/
	//  Definicja materia³u z jakiego zrobione jest jajko
	//  i definicja Ÿród³a œwiat³a
	/*************************************************************************************/

	/*************************************************************************************/
	// Definicja materia³u z jakiego zrobiony jest czajnik 

	//GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	//// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	//GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego                

	//GLfloat mat_shininess = { 20.0 };
	//// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	///*************************************************************************************/
	//// Definicja Ÿród³a œwiat³a

	//GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	//// po³o¿enie Ÿród³a

	//GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	//// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	//// Ia = [Iar,Iag,Iab]

	//GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	//// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	//// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	//GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	//// odbicie kierunkowe Is = [Isr,Isg,Isb]

	//GLfloat att_constant = 1.0;
	//// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji 
	//// odleg³oœci od Ÿród³a

	//GLfloat att_linear = 0.05;
	//// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji 
	//// odleg³oœci od Ÿród³a

	//GLfloat att_quadratic = 0.001;
	//// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	//// odleg³oœci od Ÿród³a

	///*************************************************************************************/
	//// Ustawienie parametrów materia³u i Ÿród³a œwiat³a
	///*************************************************************************************/
	//// Ustawienie patrametrów materia³u

	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	///*************************************************************************************/
	//// Ustawienie parametrów Ÿród³a

	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	//glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	///*************************************************************************************/
	//// Ustawienie opcji systemu oœwietlania sceny 

	//glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	//glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny 
	//glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	//glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora 

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
	glutCreateWindow("Unicorn blood");

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