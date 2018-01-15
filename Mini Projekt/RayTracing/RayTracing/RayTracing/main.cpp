/*
* ---------------------------------------------------------------|
* Kurs:		GRAFIKA KOMPUTEROWA I KOMUNIKACJA CZLOWIEK-KOMPUTER  |
* Zadanie:	Metoda œledzenia promieni(Ray Tracing)				 |
*																 |
* Wykonal:	Maciej Konefal 2099555								 |
* Data:		25-01-2016											 |
*----------------------------------------------------------------|
*/

#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Definicja typu reprezentujacego punkt 3D
typedef float point3[3];

/*************************************************************************************/
// Zmienne globalne
/*************************************************************************************/
int imageSize = 400;						// Window width&height (pix)
float viewArea = 20.0;						// Rozmiar okna obserwatora

float lightPosition[10][3];					//// Parametry zrodla swiatla
float lightSpecular[10][3];
float lightDiffuse[10][3];
float lightAmbient[10][3];

float sphereRadius[10];						//// Parametry sfery, ktora jest rysowana
float spherePosition[10][3];
float sphereSpecular[10][3];
float sphereDiffuse[10][3];
float sphereAmbient[10][3];
float sphereSpecularhininess[10];

point3 globalAmbient;						// Parametry swiatla rozproszonego

											//// Parametry sledzonego promienia:
point3 startingPoint;						// punkt, generujacy promien wychodzacy
point3 startingDir = { 0.0, 0.0, -1.0 };	// wektor okreslajacy kierunek promienia

point3 normalVector;						// Wektor normalny do powierzchni
point3 reflectionVector;					// Wektor odbijajacy

											//// Dodatkowe zmienne pomocnicze
point3 intersPoint;							// Wspolrzedne punktu przeciecia sfery i promienia (x,y,z)
point3 intersColor;							// Parametry okreslajace skladowe koloru dla oswietlonego punktu na powierzchni sfery
point3 color;
point3 backColor;							// Barwa tla wczytana z pliku
GLubyte pixel[1][1][3];						// Skladowe koloru rysowanego piksela

int number = 1,
spheres = 0,								// Liczba sfer wczytana z pliku
lights = 0;									// Liczba zrodel swiatla wczytana z pliku

int	limit = 50;								// Graniczna ilosc iteracji

											// Parametry dotyczace proporcji
int image_width = 800;						// domyœlna wielkoœæ okna - wczytywana te¿ z pliku
int image_height = 600;
float viewport_width = 16.0;				// domyœlne proporcje obrazu - ale te¿ wczytywane z pliku
float viewport_height = 9.0;
float v_width, v_height;					// zmienne pomocnicze do zachowania proporcji


/*************************************************************************************/
// Deklaracje funkcji u¿ywanych w programie
/*************************************************************************************/

void WczytajPlik(string fileName);	// Funkcja wczytuje zadane informacje z pliku

int Intersect(point3 p, point3 v);			// Funkcja okreslajaca wspolrzedne punkt przeciecia promienia oraz obiektu

void Phong(int nr, float *viewerVec);		// Funkcja wyznacza oswietlenie punktu na powierzchni sfery zgodnie z modelem Phonga

void Trace(point3 p, point3 v, int step);	// Funkcja wyznacza kolor piksela dla promienia zaczynajacego sie w punkcie p i biegnacego w kierunku wskazywanym przez wektor v

void Normalization(point3 p);				// Funkcja wykonujaca normalizacje wektora

float Scalar(point3 p1, point3 p2);			// Funkcja obliczajaca iloczyn skalarny dwoch wektorow

void Normal(int nr);						// Funkcja wyznaczajaca wektor normalny w punkcie

void Reflect(point3 v);						// Funkcja oblicza kierunek odbicia promienia w punkcie

void Myinit(void);							// Funkcja inicjalizujaca, okreslajaca sposob rzutowania

void RenderScene(void);						// Funkcja rysujaca obraz oswietlonej sceny

void changeSize(GLsizei horizontal, GLsizei vertical); // Funkcja dbajaca o proporcje wyswietlanych obiektow

/*************************************************************************************/
// Funkcja glowna
/*************************************************************************************/

void main(int argc, char** argv)
{
	string fileName = "";

	
	cout << "#######################################################" << endl;
	cout << "## PROJEKT: Metoda sledzenia promieni (Ray Tracing)   #" << endl;
	cout << "## Pawel Biel 225949	                              #" << endl;
	cout << "#######################################################" << endl << endl;

	cout << "Podaj jaki plik chcesz wczytac: ";
	cin >> fileName;
	fileName = fileName + ".txt";
	glutInit(&argc, argv);
    WczytajPlik(fileName);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   
	glutInitWindowSize(600, 600);


    glutCreateWindow("Projekt - RayTracing - 225949");
	glutReshapeFunc(changeSize);
    //Myinit();
    glutDisplayFunc(RenderScene);
    glutMainLoop();
}


// Funkcja wczytuje zadane informacje z pliku
void WczytajPlik(string fileName)
{
	string buffer = "";
	float value = 0.0;
	int i = 0;

	fstream file(fileName.c_str(), ios::in);
	if (!file.is_open())
	{
		cout << "Brak pliku ze scena (" << fileName << ")" << endl;
		system("PAUSE");
		exit(0);
	}
	while (!file.eof())
	{
		file >> buffer;

		if (buffer == "dimensions") 					//Rozmiar okna
		{
			file >> value;
			imageSize = value;
		}

		if (buffer == "background") 					//Kolor tla
		{
			for (i = 0; i<3; i++) {
				file >> value;
				backColor[i] = value;
			}
		}

		if (buffer == "global") 						//Parametry swiatla rozproszonego
		{
			for (i = 0; i<3; i++) {
				file >> value;
				globalAmbient[i] = value;
			}
		}

		while (buffer == "sphere" && !file.eof()) 		//Parametry rysowanej sfery
		{
			file >> value;
			sphereRadius[spheres] = value;
			for (i = 0; i<3; i++) {
				file >> value;
				spherePosition[spheres][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				sphereSpecular[spheres][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				sphereDiffuse[spheres][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				sphereAmbient[spheres][i] = value;
			}
			file >> value;
			sphereSpecularhininess[spheres] = value;
			spheres++;
			file >> buffer;
		}

		while (buffer == "source" && !file.eof()) 		//Parametry zrodel swiatla
		{
			for (i = 0; i<3; i++) {
				file >> value;
				lightPosition[lights][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				lightSpecular[lights][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				lightDiffuse[lights][i] = value;
			}
			for (i = 0; i<3; i++) {
				file >> value;
				lightAmbient[lights][i] = value;
			}
			lights++;
			file >> buffer;
		}
	}
	file.close();
}

// Funkcja okreslajaca wspolrzedne punktu przeciecia promienia oraz obiektu
int Intersect(point3 p, point3 v) {
	float r, a, b, c, d;
	float length = 1000000000000;
	int status = -1;

	for (int i = 0; i<spheres; i++) {
		a = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		b = 2 * (v[0] * (p[0] - spherePosition[i][0]) + v[1] * (p[1] - spherePosition[i][1]) + v[2] * (p[2] - spherePosition[i][2]));
		c = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] - 2 * (spherePosition[i][0] * p[0] + spherePosition[i][1] * p[1] + spherePosition[i][2] * p[2]) + spherePosition[i][0] * spherePosition[i][0] + spherePosition[i][1] * spherePosition[i][1] + spherePosition[i][2] * spherePosition[i][2] - sphereRadius[i] * sphereRadius[i];
		d = b*b - 4 * a*c;

		if (d >= 0) {
			r = (-b - sqrt(d)) / (2 * a);

			if (r > 0 && r < length) {
				intersPoint[0] = p[0] + r*v[0];
				intersPoint[1] = p[1] + r*v[1];
				intersPoint[2] = p[2] + r*v[2];
				length = sqrt((intersPoint[0] - p[0])*(intersPoint[0] - p[0]) + (intersPoint[1] - p[1])*(intersPoint[1] - p[1]) + (intersPoint[2] - p[2])*(intersPoint[2] - p[2]));
				status = i;
			}
		}
	}
	return status;
}

// Funkcja wyznacza kolor piksela dla promienia zaczynajacego sie w punkcie p i biegnacego w kierunku wskazywanym przez wektor v
void Trace(point3 p, point3 v, int step)
{
	if (step > limit)
		return;

	number = Intersect(p, v);
	if (number >= 0) {
		Normal(number);
		Reflect(v);
		Phong(number, v);
		color[0] += intersColor[0];
		color[1] += intersColor[1];
		color[2] += intersColor[2];
		Trace(intersPoint, reflectionVector, step + 1);
	}
	else
		return;
}

// Funkcja wyznacza oswietlenie punktu na powierzchni sfery zgodnie z modelem Phonga
void Phong(int nr, point3 viewerVec)
{
	point3 reflectionVec;									// wektor kierunku odbicia swiatla
	point3 lightVec;										// wektor wskazujacy zrodlo swiatla
	float n_dot_l, v_dot_r;									// dodatkowe zmienne pomocnicze

	point3 viewer = { -viewerVec[0], -viewerVec[1], -viewerVec[2] };

	intersColor[0] = 0;
	intersColor[1] = 0;
	intersColor[2] = 0;

	for (int i = 0; i<lights; i++) {
		lightVec[0] = lightPosition[i][0] - intersPoint[0]; // wektor wskazujacy kierunek zrodla swiatla
		lightVec[1] = lightPosition[i][1] - intersPoint[1];
		lightVec[2] = lightPosition[i][2] - intersPoint[2];

		Normalization(lightVec);							// Wykonanie normalizacji wektora kierunku swiecenia zrodla swiatla          

		n_dot_l = Scalar(lightVec, normalVector);

		reflectionVec[0] = 2 * (n_dot_l)*normalVector[0] - lightVec[0];
		reflectionVec[1] = 2 * (n_dot_l)*normalVector[1] - lightVec[1];
		reflectionVec[2] = 2 * (n_dot_l)*normalVector[2] - lightVec[2];

		// obliczenie wektora opisujacego kierunek swiatla odbitego od punktu na powierzchni sfery

		Normalization(reflectionVec);						// normalizacja wektora kierunku swiatla odbitego

		v_dot_r = Scalar(reflectionVec, viewer);

		if (v_dot_r < 0)								    // obserwator nie widzi oswietlanego punktu
			v_dot_r = 0;

		// sprawdz czy punkt na powierzchni sfery jest oswietlany przez zrodlo swiatla

		if (n_dot_l > 0)									// punkt jest oswietlany, oswietlenie wyliczane za pomoca modelu Phonga
		{                   
			float x = sqrt((lightPosition[i][0] - intersPoint[0])*(lightPosition[i][0] - intersPoint[0]) + (lightPosition[i][1] - intersPoint[1])*(lightPosition[i][1] - intersPoint[1]) + (lightPosition[i][2] - intersPoint[2])*(lightPosition[i][2] - intersPoint[2]));
			intersColor[0] += (1.0 / (1.0 + 0.01*x + 0.001*x*x))*(sphereDiffuse[nr][0] * lightDiffuse[i][0] * n_dot_l + sphereSpecular[nr][0] * lightSpecular[i][0] * pow(double(v_dot_r), (double)sphereSpecularhininess[nr])) + sphereAmbient[nr][0] * lightAmbient[i][0];
			intersColor[1] += (1.0 / (1.0 + 0.01*x + 0.001*x*x))*(sphereDiffuse[nr][1] * lightDiffuse[i][1] * n_dot_l + sphereSpecular[nr][1] * lightSpecular[i][1] * pow(double(v_dot_r), (double)sphereSpecularhininess[nr])) + sphereAmbient[nr][1] * lightAmbient[i][1];
			intersColor[2] += (1.0 / (1.0 + 0.01*x + 0.001*x*x))*(sphereDiffuse[nr][2] * lightDiffuse[i][2] * n_dot_l + sphereSpecular[nr][2] * lightSpecular[i][2] * pow(double(v_dot_r), (double)sphereSpecularhininess[nr])) + sphereAmbient[nr][2] * lightAmbient[i][2];
		}
		else

		// punkt nie jest oswietlany, uwzgledniane jedynie swiatlo rozproszone
		intersColor[0] += sphereAmbient[nr][0] * globalAmbient[0];
		intersColor[1] += sphereAmbient[nr][1] * globalAmbient[1];
		intersColor[2] += sphereAmbient[nr][2] * globalAmbient[2];
	}
}

// Funkcja wyznaczajaca wektor normalny w punkcie
void Normalization(point3 p)
{
	float d = 0.0;
	int i;

	for (i = 0; i < 3; i++)
	{
		d += p[i] * p[i];
	}

	d = sqrt(d);

	if (d>0.0)
		for (i = 0; i < 3; i++)
		{
			p[i] /= d;
		}
}

// Funkcja obliczajaca iloczyn skalarny dwoch wektorow
float Scalar(point3 p1, point3 p2)
{
	float res = p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2];
	return res;
}

// Funkcja oblicza kierunek odbicia promienia w punkcie
void Reflect(point3 v) {
	float	n_dot_i;
	float   invert[3] = { -v[0], -v[1], -v[2] };

	Normalization(invert);

	n_dot_i = Scalar(invert, normalVector);
	reflectionVector[0] = 2 * (n_dot_i)*normalVector[0] - invert[0];
	reflectionVector[1] = 2 * (n_dot_i)*normalVector[1] - invert[1];
	reflectionVector[2] = 2 * (n_dot_i)*normalVector[2] - invert[2];

	Normalization(reflectionVector);
}

// Funkcja wyznaczajaca wektor normalny w punkcie
void Normal(int nr)
{
	normalVector[0] = intersPoint[0] - spherePosition[nr][0];
	normalVector[1] = intersPoint[1] - spherePosition[nr][1];
	normalVector[2] = intersPoint[2] - spherePosition[nr][2];

	Normalization(normalVector);
}

// Funkcja dbajaca o zachowanie proporcji wyswietlanych obiektow
void changeSize(GLsizei horizontal, GLsizei vertical)
{
	// zabezpieczenie przed dzieleniem przez 0
	if (vertical == 0)
		vertical = 1;
	// zmieñ wymiary obrazu na bie¿¹ce wymiary okna
	image_height = vertical;
	image_width = horizontal;

	// wylicz proporcje obrazu
	float aspect = (float)image_width / (float)image_height;
	// przypisz zadane proprcje do zmiennych pomocniczych
	v_height = viewport_height;
	v_width = viewport_width;
	// jeœli policzone proporcje ró¿ni¹ siê od tych zadanych, przelicz wysokoœæ
	if (aspect != (v_height / v_width))
	{
		v_height = v_width / aspect;
	}

	// zdefiniuj rozmiar sceny
	glViewport(0, 0, image_width, image_height);
	// jeszcze raz przedefiniuj sposób rzutowania,
	// bo mog³y siê zmieniæ proporcje
	Myinit();

}

// Funkcja inicjalizujaca, okreslajaca sposob rzutowania
void Myinit(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-v_width / 2, v_width / 2, -v_height / 2, v_height / 2, -v_width / 2, v_width / 2);
	glMatrixMode(GL_MODELVIEW);
}

// Funkcja rysujaca obraz oswietlonej sceny
void RenderScene(void)
{
	int  x, y;											 // Calkowitoliczbowe wspolrzedne rysowanego piksela 
	float x_fl, y_fl;									 // Zmiennoprzecinkowe wspolrzedne rysowanego piksela 
	int imageSize_2;									 // Polowa rozmiaru obrazu w pikselach
	
	int im_size_2y = image_height / 2;
	int im_size_2x = image_width / 2;

	imageSize_2 = imageSize / 2;						 // Wyznaczenie polowy rozmiaru obrazu w pikselach
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
		
	for (y = im_size_2y; y > -im_size_2y; y--)		 // Rysowanie - przekatna w prawo i w dol
	{
		for (x = -im_size_2x; x < im_size_2x; x++)
		{
			x_fl = (float)x / (image_width / (v_width));
			y_fl = (float)y / (image_height / (v_height));

			
			startingPoint[0] = x_fl;					//// Przeliczenie wspolrzednych (x, y) w pikselach na pozycje
			startingPoint[1] = y_fl; 					// Zmiennoprzecinkowa w oknie obserwatora
			startingPoint[2] = v_width;

			
			color[0] = 0.0;								// Wyznaczenie poczatku sledzonego promienia dla rysowanego piksela
			color[1] = 0.0;
			color[2] = 0.0;
			
			Trace(startingPoint, startingDir, 1);		// Wyznaczenie koloru piksela
			if (color[0] == 0.0) color[0] = backColor[0];
			if (color[1] == 0.0) color[1] = backColor[1];
			if (color[2] == 0.0) color[2] = backColor[2];

			// Przeprowadzenie Konwersji wartosci wyliczonego oswietlenia na liczby od 0 do 255
			color[0] > 1 ? pixel[0][0][0] = 255 : pixel[0][0][0] = color[0] * 255;
			color[1] > 1 ? pixel[0][0][1] = 255 : pixel[0][0][1] = color[1] * 255;
			color[2] > 1 ? pixel[0][0][2] = 255 : pixel[0][0][2] = color[2] * 255;

			glRasterPos3f(x_fl, y_fl, 0);							// Inkrementacja pozycji rastrowej dla rysowania piksela

			glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);	// Rysowanie kolejnego piksela na ekranie

		}
	}
	glFlush();
}

