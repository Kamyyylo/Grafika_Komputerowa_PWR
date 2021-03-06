// TrojkatZTeksturami.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include "stdafx.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <math.h>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;

typedef float point3[3];
float PI = 3.1415926535897932384626433832795;
int model = 3;
int N = 100;
int s[5] = { 0, 0, 0, 0, 0 };						//Tablica pozwala wyswietlac kazdy z wierzchołkow
bool state_1 = false;
bool state_2 = false;
bool state_3 = false;
bool state_4 = false;
bool state_5 = false;
bool state_6 = false;
struct punkty
{
	double x;
	double y;
	double z;

};
static GLfloat viewer[] = { 0.0, 0.0, 250.0 };
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
	//_CRT_SECURE_NO_WARNINGS
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

// inicjalizacja po?o?enia obserwatora

/*************************************************************************************/

// Funkcja rysuj?ca osie uk?adu wspó?rz?dnych

static GLfloat theta = 0.0;   // k?t obrotu obiektu
static GLfloat theta2 = 0.0;   // k?t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;      // stan klawiszy myszy
							  // 0 - nie naci?ni?to ?adnego klawisza
							  // 1 - naci?ni?ty zosta? lewy klawisz
							  // 2 - naciœniêty prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c? i poprzedni? kursora myszy
static int delta_y = 0;
static int delta_z = 0;

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
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

// Funkcja "bada" stan myszy i ustawia warto?ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
	}

	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		z_pos_old = y;
		status = 2;
	}

	else

		status = 0;         // nie zosta? wci?ni?ty ?aden klawisz
}

/*************************************************************************************/

// Funkcja "monitoruje" po?o?enie kursora myszy i ustawia warto?ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	delta_y = y - y_pos_old;
	double pom = y - z_pos_old;
	if (status == 2 && viewer[2] + pom < 270 && viewer[2] + pom > 30) viewer[2] += pom;

	x_pos_old = x;            // podstawienie bie?acego po?o?enia jako poprzednie
	y_pos_old = y;
	z_pos_old = y;

	glutPostRedisplay();    // przerysowanie obrazu sceny
}

/*************************************************************************************/



/*************************************************************************************/

// Funkcja okre?laj?ca co ma by? rysowane (zawsze wywo?ywana, gdy trzeba
// przerysowa? scen?)



void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz?cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2] / 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy?ej

	if (status == 1)                    // je?li lewy klawisz myszy wci?ni?ty
	{
		theta += delta_x * pix2angle;    // modyfikacja k?ta obrotu o k?t proporcjonalny
		theta2 += delta_y * pix2angle;
	}                                 // do ró?nicy po?o?e? kursora myszy

	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);


	glColor3f(1.0f, 1.0f, 1.0f);
	if (s[0] != state_1)				//Wciśnieta cyfra 1
	{
		glBegin(GL_TRIANGLES);
		//pierwszy trójkąt
		glNormal3f(0.0f, 0.0f, 1.0f);	//Wektory normalne z poprzednich zajeć
		glTexCoord2f(0.0f, 0.0f);		//-->Funkcja określająca, które wierzchołki 
		glVertex3f(0.0f, 10.0f, 5.0f);	//trójkąta naniesionego na wzorzec tekstury
		glNormal3f(0.0f, 0.0f, 1.0f);	//odpowiadają którym wierzchołkom
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-10.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glEnd();
	}

	if (s[1] != state_2)
	{
		//drugi trójkąt
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 5.0f);
		glNormal3f(10.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(10.0f, 0.0f, 10.0f);
		glEnd();
	}

	if (s[2] != state_3)
	{
		//trzeci trójkąt
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 5.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 10.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(-10.0f, 0.0f, 10.0f);
		glEnd();
	}

	if (s[3] != state_4)
	{
		//czwarty trójkąt
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 5.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-10.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(-10.0f, 0.0f, 10.0f);
		glEnd();
	}

	if (s[4] != state_5)
	{	//piaty trójkąt
		glBegin(GL_POLYGON); //poligon pozwala na stworzenie podstawy piramidy
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(10.0f, 0.0f, 10.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(-10.0f, 0.0f, 10.0f);
		glEnd();
	}


	glFlush();
	// Przekazanie polece? rysuj?cych do wykonania



	glutSwapBuffers();
	// 

}
/*************************************************************************************/

// Funkcja ustalaj?ca stan renderowania



void MyInit(void)
{
	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	pBytes = LoadTGAImage("doge_2.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz?cy (wype?nienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta?ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysoko?? i szeroko?? okna) s?
// przekazywane do funkcji za ka?dym razem gdy zmieni si? rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze??czenie macierzy bie??cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie??cej

	gluPerspective(70, 1.0, 5.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko?ci okna okna widoku (viewport) w zale?no?ci
	// relacji pomi?dzy wysoko?ci? i szeroko?ci? okna

	glMatrixMode(GL_MODELVIEW);
	// Prze??czenie macierzy bie??cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

}

/*************************************************************************************/

// G?ówny punkt wej?cia programu. Program dzia?a w trybie konsoli


void keys(unsigned char key, int x, int y)
{
	if (key == '1') state_1 = true;
	if (key == '2') state_2 = true;
	if (key == '3') state_3 = true;
	if (key == '4') state_4 = true;
	if (key == '5') state_5 = true;
	if (key == '6')
	{
		state_1 = false;
		state_2 = false;
		state_3 = false;
		state_4 = false;
		state_5 = false;
	}
	RenderScene(); // przerysowanie obrazu sceny
}

void main(void)
{



	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutCreateWindow("Trójkąt z teksturą");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj? zwrotn? odpowiedzialn?
	// za zmiany rozmiaru okna                        



	MyInit();
	// Funkcja MyInit() (zdefiniowana powy?ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst?pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W??czenie mechanizmu usuwania niewidocznych elementów sceny

	glutKeyboardFunc(keys);

	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}



/*************************************************************************************/