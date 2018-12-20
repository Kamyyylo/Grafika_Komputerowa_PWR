

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

using namespace std;

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 100.0 };
// inicjalizacja po?o?enia obserwatora

/*************************************************************************************/

// Funkcja rysuj?ca osie uk?adu wspó?rz?dnych

static GLfloat theta = 0.0;   // k?t obrotu obiektu
static GLfloat theta2 = 0.0;   // k?t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;    // stan klawiszy myszy
							// 0 - nie naciśnięto żadnego klawisza
							// 1 - naciśnięty został lewy klawisz
							// 2 - naciśnięty prawy klawisz

static int x_pos_old = 0;   // poprzednia pozycja kursora myszy
static int y_pos_old = 0;	// poprzednia pozycja kursora myszy
static int z_pos_old = 0;	// poprzednia pozycja kursora myszy

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

void Mouse(int btn, int state, int x, int y)//Obsługa myszy
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;// przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;// przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		status = 1;   //Wciśniety lewy klawisz
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		z_pos_old = y;// przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		status = 2;   //Wciśniety prawy klawisz
	}
	else
		status = 0;   //Nie wciśniety żaden klawisz
}

/*************************************************************************************/

// Funkcja "monitoruje" po?o?enie kursora myszy i ustawia warto?ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)//Wprawienie obiektu w ruch
{
	delta_x = x - x_pos_old;//Obliczenie róznicy połozenia kursora myszy
	delta_y = y - y_pos_old;//Obliczenie róznicy połozenia kursora myszy
	if (status == 1)		//Jeżeli lewy klawisz wciśnięty
	{	//modyfikacja kątów pobrana z instrukcji
		theta += delta_x * pix2angle;//modyfikacja kąta obrotu o kąt proporcjonalny dla x do róznicy położenia kursora myszy
			theta2 += delta_y* pix2angle;//modyfikacja kąta obrotu o kąt proporcjonalny dla y do róznicy położenia kursora myszy
	}                           
	double pom = y - z_pos_old; //Zmienna pomocnicza do ustalania róznicy połozenia kursora myszy przy przybliżaniu.
	//Jeśli prawy klawisz wcisnięty to można przybliżać w zakresie od 70 do 150.
	if (status == 2 && viewer[2] + pom < 150 && viewer[2] + pom > 70) viewer[2] += pom;
	x_pos_old = x;            //Ustawienie biezacego położenia jako poprzednie
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

	gluLookAt(viewer[0], viewer[1], viewer[2]/10 , 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(3.0, 3.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
	// Zdefiniowanie po?o?enia obserwatora

	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy?ej

	

	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);


	glColor3f(0.0f, 0.5f, 1.0f);
	// Ustawienie koloru rysowania na niebieski

	glutWireTeapot(3.0);
	// Narysowanie czajnika

	glFlush();
	// Przekazanie polece? rysuj?cych do wykonania



	glutSwapBuffers();
	// 

}
/*************************************************************************************/

// Funkcja ustalaj?ca stan renderowania



void MyInit(void)
{

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
	// Czyszcznie macierzy biezacej

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);


	glMatrixMode(GL_MODELVIEW);
	// Prze??czenie macierzy bie??cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

}

/*************************************************************************************/



void main(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okre?lenie, ?e funkcja RenderScene b?dzie funkcj? zwrotn?
	// (callback function).  B?dzie ona wywo?ywana za ka?dym razem
	// gdy zajdzie potrzeba przerysowania okna



	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj? zwrotn? odpowiedzialn?
	// za zmiany rozmiaru okna                        



	MyInit();
	// Funkcja MyInit() (zdefiniowana powy?ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst?pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W??czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}



/*************************************************************************************/