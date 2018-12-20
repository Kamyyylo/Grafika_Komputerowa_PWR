#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <vector>

using namespace std;
GLfloat light_positionSecond[] = { -30.0, 0.0, -10.0, 1.0 }; // położenie źródła
GLfloat light_position[] = { 30.0, 0.0, -10.0, 1.0 }; // położenie źródła
typedef float point3[3];
 const int N = 80;
//static int model = 2;
static GLfloat angle[] = { 0.0, 0.0, 0.0 };
static float colors_tab[N][N][3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po?o?enia obserwatora
static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat theta2 = 0.0;   // kat obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
int zoomIT = 0;
int zoomItOld = 0;
static int zoom = 30;


static GLint status = 0;      // stan klawiszy
							  // 0 - nie nacisnieto zadnego klawisza
							  // 1 - nacisniety zostal lewy klawisz
							  // 2 - nacisniety prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;

static int x_pos_old2 = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old2 = 0;


static int fi_pos_old = 0;
static int fi_pos_old2 = 0;
static int theta_pos_old = 0;
static int theta_pos_old2 = 0;

static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c? i poprzedni? kursora myszy
static int delta_y = 0;

static int delta_fi = 0;
static int delta_fi2 = 0;
static int delta_theta = 0;
static int delta_theta2 = 0;

double R = 13;
float fi = 0.0;
float fi2 = 0.0;
#define M_PI 3.14159265358979323846
//funkcja obracaj¹ca model, pobrana ze strony zsk
void spinEgg()
{

	angle[0] -= 0.5;
	if (angle[0] > 360.0) angle[0] -= 360.0;

	angle[1] -= 0.5;
	if (angle[1] > 360.0) angle[1] -= 360.0;

	angle[2] -= 0.5;
	if (angle[2] > 360.0) angle[2] -= 360.0;

	glutPostRedisplay(); //odswiezenie zawartosci aktualnego okna
}

// Funkcja rysujaca osie ukladu wspólrzednych
void Axes(void)
{

	point3  x_min = { -50.0, 0.0, 0.0 };
	point3  x_max = { 50.0, 0.0, 0.0 };
	// poczatek i koniec obrazu osi x

	point3  y_min = { 0.0, -100.0, 0.0 };
	point3  y_max = { 0.0, 50.0, 0.0 };
	// poczatek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -50.0 };
	point3  z_max = { 0.0, 0.0, 50.0 };
	//  poczatek i koniec obrazu osi z

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
void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;			  // Wciśniety lewy przycisk myszy
	}

	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		status = 2;			 //Wciśnięty prawy przycisk myszy
	}

	else

		status = 0;         // nie zosta? wci?ni?ty ?aden klawisz
}

/*************************************************************************************/

// Funkcja "monitoruje" po?o?enie kursora myszy i ustawia warto?ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora mys
	delta_y = y - y_pos_old;
	x_pos_old = x;				// przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
	y_pos_old = y;
	glutPostRedisplay();		// przerysowanie obrazu sceny
}

/*************************************************************************************/

void Egg() {
	float tab[100][100][3];
	float u = 0.0;
	float v = 0.0;
	point3 **vectors3d = new point3*[N];
	point3 **vectorNorm = new point3*[N];
	for (int i = 0; i < N; i++) {
		vectors3d[i] = new point3[N];
		vectorNorm[i] = new point3[N];
	}
	//zamiana dziedziny parametrycznej na postaæ 3-D wed³ug okrelonego wzoru
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			u = (float)i / (N - 1);
			v = (float)j / (N - 1);

			tab[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI * v); // X(u,v)
			tab[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5; //Y(u,v)
			tab[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI * v); //Z(u,v)
		float ux, uz, uy, vz, vy, vx, length;

		// obliczenie wartosci wektorow normalnych
		ux = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(3.14*v);
		uy = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
		uz = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(3.14*v);

		vx = 3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(3.14*v);
		vy = 0;
		vz = -3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(3.14*v);

		vectorNorm[i][j][0] = uy * vz - uz * vy;
		vectorNorm[i][j][1] = uz * vx - ux * vz;
		vectorNorm[i][j][2] = ux * vy - uy * vx;

		length = sqrt(vectorNorm[i][j][0] * vectorNorm[i][j][0] + vectorNorm[i][j][1] * vectorNorm[i][j][1] + vectorNorm[i][j][2] * vectorNorm[i][j][2]);
		if (i == 0 || i == N)
		{
			vectorNorm[i][j][0] = 0;
			vectorNorm[i][j][1] = -1;
			vectorNorm[i][j][2] = 0;
		}
		else if (i < N / 2)
		{
			vectorNorm[i][j][0] = (uy*vz - uz * vy) / length;
			vectorNorm[i][j][1] = (uz*vx - ux * vz) / length;
			vectorNorm[i][j][2] = (ux*vy - uy * vx) / length;
		}
		else if (i > N / 2)
		{
			vectorNorm[i][j][0] = -1 * (uy*vz - uz * vy) / length;
			vectorNorm[i][j][1] = -1 * (uz*vx - ux * vz) / length;
			vectorNorm[i][j][2] = -1 * (ux*vy - uy * vx) / length;
		}
		else
		{
			vectorNorm[i][j][0] = 0;
			vectorNorm[i][j][1] = 1;
			vectorNorm[i][j][2] = 0;
		}
	}
}
		int i = 0;
		for ( i = 0; i < N-1 ;) 
		{
			for (int j = 0; j < N-1 ; j++)
			{
				glColor3f(255.0f, 255.0f, 255.0f);
				glBegin(GL_TRIANGLES);

				glNormal3fv(vectorNorm[i][j + 1]);
				glVertex3fv(tab[i][j + 1]);

				glNormal3fv(vectorNorm[i + 1][j]);
				glVertex3fv(tab[i + 1][j]);

				glNormal3fv(vectorNorm[i + 1][j + 1]);
				glVertex3fv(tab[i + 1][j + 1]);



				glNormal3fv(vectorNorm[i + 1][j]);
				glVertex3fv(tab[i + 1][j]);


				glNormal3fv(vectorNorm[i][j + 1]);
				glVertex3fv(tab[i][j + 1]);


				glNormal3fv(vectorNorm[i][j]);
				glVertex3fv(tab[i][j]);

				glEnd();
				
			}
			i++;
		}

}





void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym

	glLoadIdentity();
	gluLookAt(0, 0, viewer[2], 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	
	if(status==1){
		theta += delta_x * pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
		fi += delta_y * pix2angle;
	}

	if (status == 2) {
		theta2 += delta_x * pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
		fi2 += delta_y * pix2angle;
	}

	float pi = 3.14;						
	if (fi > 2 * pi) fi = 2 * pi;		//Dziedzina dla pi i fi
	if (theta > 2 * pi) theta = 2 * pi;
	if (fi < 0) fi = 0;
	if (theta < 0) theta = 0;


	if (fi2 > 2 * pi) fi2 = 2 * pi;		
	if (theta2 > 2 * pi) theta2 = 2 * pi;
	if (fi2 < 0) fi2 = 0;
	if (theta2 < 0) theta2 = 0;

	//Ustalenie położenia źródła swiatła pierwszego według instrukcji
	light_position[0] = zoom * cos(theta2) * cos(fi2);
	light_position[1] = zoom * sin(fi2);
	light_position[2] = zoom * sin(theta2) * cos(fi2);
	//Ustalenie położenia źródła swiatła drugiego według instrukcji
	light_positionSecond[0] = zoom * cos(theta) * cos(fi);
	light_positionSecond[1] = zoom * sin(fi);
	light_positionSecond[2] = zoom * sin(theta) * cos(fi);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light_positionSecond);

	glColor3f(1.0f, 23.0f, 1.0f);
	Egg();
	// Narysowanie czajnika

	glFlush();
	// Przekazanie poleceń rysujących do wykonania

	glutSwapBuffers();

}


void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 1.0,1.0, 1.0, 1.0 };
	// współczynniki ka =[kar,kag,kab] dla światła otoczenia

	GLfloat mat_diffuse[] = { 5.0, 5.0, 5.0, 1.0 };
	// współczynniki kd =[kdr,kdg,kdb] światła rozproszonego

	GLfloat mat_specular[] = { 10.0, 10.0, 10.0, 1.0 };
	// współczynniki ks =[ksr,ksg,ksb] dla światła odbitego               

	GLfloat mat_shininess = { 200.0 };
	// współczynnik n opisujący połysk powierzchni

	GLfloat light_position[] = { 0.0, 0.0, -10.0,1.0 };

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };	// składowe intensywności świecenia źródła światła otoczenia
														// Ia = [Iar,Iag,Iab]
	GLfloat light_diffuse[] = { 0.0, 1.0, 0.0, 1.0 }; // składowe intensywności świecenia źródła światła powodującego
													  // odbicie dyfuzyjne Id = [Idr,Idg,Idb]
	GLfloat light_specular[] = {0.0, 1.0, 1.0, 0.0 };	// składowe intensywności świecenia źródła światła powodującego
														// odbicie kierunkowe Is = [Isr,Isg,Isb]
	GLfloat att_constant = { 1.0 }; // składowa stała ds dla modelu zmian oświetlenia w funkcji
									// odległości od źródła
	GLfloat att_linear = { (GLfloat) 0.05 }; // składowa liniowa dl dla modelu zmian oświetlenia w funkcji
											 // odległości od źródła
	GLfloat att_quadratic = { (GLfloat) 0.001 }; // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
												 // odległości od źródła


												 /*************************************************************************************/
												 // Definicja źródła światła numer 2 - blue
	GLfloat light_position2[] = { 0.0, 0.0, 0.0,1.0 };

	GLfloat light_ambient2[] = { 0.1, 0.1, 0.1, 1.0 };	// składowe intensywności świecenia źródła światła otoczenia
															// Ia = [Iar,Iag,Iab]
	GLfloat light_diffuse2[] = { 1.0, 0.0,0.0, 10.0 }; // składowe intensywności świecenia źródła światła powodującego
														 // odbicie dyfuzyjne Id = [Idr,Idg,Idb]
	GLfloat light_specular2[] = { 1.0, 0.0,1.0, 10.0 };	// składowe intensywności świecenia źródła światła powodującego
															// odbicie kierunkowe Is = [Isr,Isg,Isb]
	GLfloat att_constant2 = { 1.0 }; // składowa stała ds dla modelu zmian oświetlenia w funkcji
										// odległości od źródła
	GLfloat att_linear2 = { (GLfloat) 0.05 }; // składowa liniowa dl dla modelu zmian oświetlenia w funkcji
												 // odległości od źródła
	GLfloat att_quadratic2 = { (GLfloat) 0.001 }; // składowa kwadratowa dq dla modelu zmian oświetlenia w funkcji
													 // odległości od źródła

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów źródła
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear2);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic2);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);


	/*************************************************************************************/
	// Ustawienie opcji systemu oświetlania sceny

	glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
	glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
	glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
	glEnable(GL_LIGHT1);     // włączenie źródła o numerze 0
	glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora


}

// Funkcja ma za zadanie utrzymanie stalych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokosc i szerokosc okna) sa
// przekazywane do funkcji za kazdym razem gdy zmieni sie rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	pix2angle *= 0.009;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble aspect = (GLdouble)horizontal / vertical;

	gluPerspective(70, (float)aspect, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkości okna okna widoku (viewport) w zależności
	// relacji pomiędzy wysokością i szerokością okna

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu 

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej

}
void main(void)
{
	//glutIdleFunc(spinEgg);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);

	glutCreateWindow("JAJO");

	glutDisplayFunc(RenderScene);

	   
	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcje zwrotna odpowiedzialna
	// zazmiany rozmiaru okna   
	MyInit();
	// Funkcja MyInit() (zdefiniowana powyzej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystapieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// Wlaczenie mechanizmu usuwania powierzchni niewidocznych
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light_positionSecond);
	
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT


}