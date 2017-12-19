/*************************************************************************************/
// Program rysuje obraz barwnej sfery oœwietlonej przez jedno barwne Ÿród³o œwiat³a.
// Œrodek sfery znajduje siê w œrodku uk³adu wspó³rzêdnych.
// Do obliczania oœwietlenia wykorzystywany jest model Phonga.
/*************************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

typedef float point[3];

/*************************************************************************************/
// Prototypy u¿ywanych funkcji
/*************************************************************************************/

/*************************************************************************************/
// Funkcja obliczaj¹ca punkt przeciêcia promienia i powierzchni sfery

int Trace(float *p, float *v);

/*************************************************************************************/
// Funkcja obliczaj¹ca oœwietlenie punktu na powierzchni sfery wed³ug modelu Phonga 

void Phong(void);

/*************************************************************************************/
// Funkcja obliczaj¹ca iloczyn skalarny dwóch wektorów 

float dotProduct(point p1, point p2);

/*************************************************************************************/
// Funkcja normalizuj¹ca wektor

void Normalization(point p);

/*************************************************************************************/
// Zmienne globalne
/*************************************************************************************/
/*************************************************************************************/
// Rozmiar obrazu w pikselach (obraz jest kwadratem)

int im_size = 400;

/*************************************************************************************/
// Rozmiar okna obserwatora 

float viewport_size = 3.0;

/*************************************************************************************/
// Po³o¿enie i parametry Ÿród³a œwiat³a

float    light_position[] = { 3.0, 2.5, 5.0 };
float    light_specular[] = { 1.0, 1.0, 0.0 };
float    light_diffuse[] = { 0.0, 1.0, 1.0 };
float    light_ambient[] = { 0.0, 0.0, 0.0 };

/*************************************************************************************/
// Promieñ i parametry rysowanej sfery

float    sphere_radius = 1.0;
float    sphere_specular[] = { 0.8, 0.8, 0.8 };
float    sphere_diffuse[] = { 0.6, 0.7, 0.8 };
float    sphere_ambient[] = { 1.0, 1.0, 1.0 };
float    sphere_specularhininess = 30.0;

/*************************************************************************************/
// Parametry œwiat³a rozproszonego
float global_a[] = { 0.25, 0.15, 0.1 };

/*************************************************************************************/
// Parametry "œledzonego" promienia

float starting_point[3];                                 // punkt, z którego wychodzi promieñ
float starting_directions[] = { 0.0, 0.0, -1.0 }; // wektor opisuj¹cy kierunek promienia

												  /*************************************************************************************/
												  // Zmienne pomocnicze
float inter[3];                  // wspó³rzêdne (x,y,z) punktu przeciêcia promienia i sfery
int inters;                      // zmienna okreœlaj¹ca, czy sfera zosta³a przeciêta przez
float inters_c[3];               // sk³adowe koloru dla oœwietlonego punktu na powierzchni sfery
GLubyte pixel[1][1][3];          // sk³adowe koloru rysowanego piksela

								 /*************************************************************************************/
								 // Funkcja oblicza punkt przeciêcia promienia i powierzchni sfery
								 // Argument p jest punktem pocz¹tkowym promienia a v wektorem opisuj¹cym 
								 // kierunek biegu promienia
								 // Funkcja zwraca 1 jeœli promieñ przecina sferê, 0 gdy nie przecina.
								 /*************************************************************************************/
int Trace(float *p, float *v)
{
	float a, b, c, d, r;

	a = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	b = 2 * (p[0] * v[0] + p[1] * v[1] + p[2] * v[2]);
	c = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] - 1.0;

	d = b * b - 4 * a*c;

	if (d >= 0)                              // jest co najmniej jeden punkt przeciêcia
	{
		r = (-b - sqrt(d)) / (2 * a);     // parametr dla bli¿szego punktu przeciêcia

		inter[0] = p[0] + r * v[0];    // wspó³rzêdne punktu przeciêcia
		inter[1] = p[1] + r * v[1];
		inter[2] = p[2] + r * v[2];

		return 1;                         // jest punkt przeciêcia
	}
	else                                    // promieñ nie przecina sfery
		return 0;
}
/*************************************************************************************/
// Funkcja oblicza oœwietlenie punktu na powierzchni sfery u¿ywaj¹c modelu Phonga
/*************************************************************************************/
void Phong(void)
{
	float normal_vector[3];                      // wektor normalny do powierzchni 
	float light_vec[3];                             // wektor wskazuj¹cy Ÿróde³
	float reflection_vector[3];                  // wektor kierunku odbicia œwiat³a
	float viewer_v[3] = { 0.0, 0.0, 1.0 };   // wektor kierunku obserwacji
	float n_dot_l, v_dot_r;                      // zmienne pomocnicze

	normal_vector[0] = inter[0];         // wektor normalny do powierzchni sfery        
	normal_vector[1] = inter[1];
	normal_vector[2] = inter[2];

	light_vec[0] = light_position[0] - inter[0]; // wektor wskazuj¹cy kierunek Ÿród³a 
	light_vec[1] = light_position[1] - inter[1];
	light_vec[2] = light_position[2] - inter[2];

	Normalization(light_vec);                        // normalizacja wektora kierunku œwiecenia Ÿród³a            

	n_dot_l = dotProduct(light_vec, normal_vector);

	reflection_vector[0] = 2 * (n_dot_l)*normal_vector[0] - light_vec[0];
	reflection_vector[1] = 2 * (n_dot_l)*normal_vector[1] - light_vec[1];
	reflection_vector[2] = 2 * (n_dot_l)*normal_vector[2] - light_vec[2];

	// obliczenie wektora opisuj¹cego kierunek œwiat³a odbitego od punktu na powierzchni sfery
	Normalization(reflection_vector); // normalizacja wektora kierunku œwiat³a odbitego 

	v_dot_r = dotProduct(reflection_vector, viewer_v);
	if (v_dot_r < 0)                        // obserwator nie widzi oœwietlanego punktu
		v_dot_r = 0;

	// sprawdzenie czy punkt na powierzchni sfery jest oœwietlany przez Ÿród³o
	if (n_dot_l > 0)     // punkt jest oœwietlany,oœwietlenie wyliczane jest ze wzorów dla modelu Phonga
	{
		inters_c[0] = (sphere_diffuse[0] * light_diffuse[0] * n_dot_l + sphere_specular[0] * light_specular[0] * pow(double(v_dot_r), 20.0)) + sphere_ambient[0] * light_ambient[0] + sphere_ambient[0] * global_a[0];
		inters_c[1] = (sphere_diffuse[1] * light_diffuse[1] * n_dot_l + sphere_specular[1] * light_specular[1] * pow(double(v_dot_r), 20.0)) + sphere_ambient[1] * light_ambient[1] + sphere_ambient[1] * global_a[1];
		inters_c[2] = (sphere_diffuse[2] * light_diffuse[2] * n_dot_l + sphere_specular[2] * light_specular[2] * pow(double(v_dot_r), 20.0)) + sphere_ambient[2] * light_ambient[2] + sphere_ambient[2] * global_a[2];
	}
	else                // punkt nie jest oœwietlany, uwzglêdniane jest tylko œwiat³o rozproszone   
	{
		inters_c[0] = sphere_ambient[0] * global_a[0];
		inters_c[1] = sphere_ambient[1] * global_a[1];
		inters_c[2] = sphere_ambient[2] * global_a[2];
	}
}
/*************************************************************************************/
// Funkcja przeprowadza normalizacjê wektora
/*************************************************************************************/
void Normalization(point p)
{
	float d = 0.0;
	int i;

	for (i = 0; i<3; i++)
		d += p[i] * p[i];

	d = sqrt(d);

	if (d>0.0)
		for (i = 0; i<3; i++)
			p[i] /= d;
}

/*************************************************************************************/
// Funkcja oblicza iloczyn skalarny wektorów
/*************************************************************************************/
float dotProduct(point p1, point p2)
{
	return (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]);
}

/*************************************************************************************/
// Funkcja rysuj¹ca obraz oœwietlonej sceny
/*************************************************************************************/
void Display(void)
{
	int  x, y;               // pozycja rysowanego piksela "ca³kowitoliczbowa"
	float x_fl, y_fl;      // pozycja rysowanego piksela "zmiennoprzecinkowa"
	int im_size_2;       // po³owa rozmiaru obrazu w pikselach


	im_size_2 = im_size / 2;    // obliczenie po³owy rozmiaru obrazu w pikselach

	glClear(GL_COLOR_BUFFER_BIT);

	glFlush();

	// rysowanie pikseli od lewego górnego naro¿nika do prawego dolnego naro¿nika

	for (y = im_size_2; y > -im_size_2; y--)
	{
		for (x = -im_size_2; x < im_size_2; x++)
		{

			x_fl = (float)x / (im_size / viewport_size);
			y_fl = (float)y / (im_size / viewport_size);

			// przeliczenie pozycji(x,y) w pikselach na pozycjê "zmiennoprzecinkow¹" w oknie obserwatora

			starting_point[0] = x_fl;
			starting_point[1] = y_fl;
			starting_point[2] = viewport_size;

			// wyznaczenie pocz¹tku œledzonego promienia dla rysowanego piksela

			inters = Trace(starting_point, starting_directions);

			// obliczenie punktu przeciêcia ze sfer¹

			if (inters == 1)    // promieñ przecina sferê
			{
				Phong(); // obliczenie oœwietlenia przy pomocy modelu Phonga

						 // konwersja wartoœci wyliczonego oœwietlenia na liczby od 0 do 255

				if (inters_c[0] > 1)            // sk³adowa czerwona R                
					pixel[0][0][0] = 255;
				else
					pixel[0][0][0] = inters_c[0] * 255;

				if (inters_c[1] > 1)            // sk³adowa zielona G    
					pixel[0][0][1] = 255;
				else
					pixel[0][0][1] = inters_c[1] * 255;

				if (inters_c[2] > 1)            // sk³adowa niebieska B
					pixel[0][0][2] = 255;
				else
					pixel[0][0][2] = inters_c[2] * 255;
			}
			else    // promieñ nie przeciana sfery piksel jest rysowany w kolorze t³a
			{
				pixel[0][0][0] = 0;             //  sk³adowa R t³a
				pixel[0][0][1] = 0;             //  sk³adowa G t³a
				pixel[0][0][2] = 51;            //  sk³adowa B t³a 
			}
			glRasterPos3f(x_fl, y_fl, 0);

			// inkrementacja pozycji rastrowej dla rysowania piksela

			glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

			// narysowanie kolejnego piksela na ekranie
		}
	}
	glFlush();
}

/*************************************************************************************/
// Funkcja inicjalizuj¹ca definiuj¹ca sposób rzutowania
/*************************************************************************************/
void Myinit(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-viewport_size / 2, viewport_size / 2, -viewport_size / 2, viewport_size / 2, -viewport_size / 2, viewport_size / 2);
	glMatrixMode(GL_MODELVIEW);
}

/*************************************************************************************/
// Funkcja g³ówna
/*************************************************************************************/
void main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(im_size, im_size);
	glutCreateWindow("Ray Casting - oœwietlona sfera");
	Myinit();
	glutDisplayFunc(Display);
	glutMainLoop();
}
/*************************************************************************************/