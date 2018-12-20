#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <vector>

using namespace std;

typedef float point3[3];
const int N = 100;
//static int model = 2;
static GLfloat angle[] = { 0.0, 0.0, 0.0 };
static float colors_tab[N][N][3];
static GLfloat viewer[] = { 0.0, 0.0, 0.0 };
// inicjalizacja po?o?enia obserwatora
static GLfloat theta = 0.0;   // kąt obrotu obiektu
static GLfloat theta2 = 0.0;   // kat obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_x;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_y;     // przelicznik pikseli na stopnie

static GLint status = 0;      // stan klawiszy myszy
							  // 0 - nie nacisnieto zadnego klawisza
							  // 1 - nacisniety zostal lewy klawisz
							  // 2 - nacisniety prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c? i poprzedni? kursora myszy
static int delta_y = 0;
static int delta_z = 0;

double R = 13;
float fi = 0.0;
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
	delta_x = x - x_pos_old;//Obliczenie różnocy położenia kursora myszy
	delta_y = y - y_pos_old;//Obliczenie różnocy położenia kursora myszy

	if (status == 1)		//Jeśli lewy klawisz wciśnięty
	{
		theta += delta_x * pix2angle_x;		//modyfikacja kąta obrotu 
		fi += delta_y * pix2angle_y;		//do róznicy połozenia kursora myszy
		if (theta >= 360.0)					//Jeśli kąt >= 360 stopni
			theta = 0.0;					// to kąt = 0
		if (fi >= 360.0)
			fi = 0.0;
	}
	else if (status == 2) {	//Jeśli lewy klawisz wciśnięty
		R += 0.01* delta_y;	//Przyblizanie się obserwatora do obiektu
		if (R <= 8.0)		//ustalone maksymalne przyblizenia i oddalenia
			R = 8.0;		//aby nie wejść w środek jajka
		if (R >= 13.0)
			R = 13.0;
	}
	x_pos_old = x;          //Ustawienie aktualnego położenia jako poprzednie
	y_pos_old = y;
	z_pos_old = y;
	glutPostRedisplay();    // przerysowanie obrazu sceny
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

		// obliczenie wartosci wektorow normalnych z instrukcji
		ux = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(3.14*v);
		uy = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
		uz = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(3.14*v);

		vx = 3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(3.14*v);
		vy = 0;
		vz = -3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(3.14*v);

		vectorNorm[i][j][0] = uy * vz - uz * vy; //opisanie wektorow normalnych wedlug instrukcji
		vectorNorm[i][j][1] = uz * vx - ux * vz;
		vectorNorm[i][j][2] = ux * vy - uy * vx;

		length = sqrt(vectorNorm[i][j][0] * vectorNorm[i][j][0] + vectorNorm[i][j][1] * vectorNorm[i][j][1] + vectorNorm[i][j][2] * vectorNorm[i][j][2]);
	
		 if (i < N / 2)  //jesli jestesmt w pierwszej polowie jajka
		{
			vectorNorm[i][j][0] = (uy*vz - uz * vy) / length;
			vectorNorm[i][j][1] = (uz*vx - ux * vz) / length;
			vectorNorm[i][j][2] = (ux*vy - uy * vx) / length;
		}
		 else if (i > N / 2) //jesli jestesmy w drugiej polowie jajka
		{
			vectorNorm[i][j][0] = -1 * (uy*vz - uz * vy) / length;
			vectorNorm[i][j][1] = -1 * (uz*vx - ux * vz) / length;
			vectorNorm[i][j][2] = -1 * (ux*vy - uy * vx) / length;
		}
		 else if (i == 0 || i == N)
		 {
			 vectorNorm[i][j][0] = 0;
			 vectorNorm[i][j][1] = -1;
			 vectorNorm[i][j][2] = 0;
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
	// Czyszczenie okna aktualnym kolorem czyszczacym

	glLoadIdentity();
	// Czyszczenie macierzy biezacej

	
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wyzej
	
	viewer[0] = R * cos(theta) * cos(fi);
	viewer[1] = R * sin(fi);
	viewer[2] = R * sin(theta) * cos(fi);

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(fi), 0.0);
	//cout << R << endl;

	// do ró?nicy po?o?e? kursora myszy

	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);

	//Axes();
	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia?y
	glRotatef(angle[0], 1.0, 0.0, 0.0);

	glRotatef(angle[1], 0.0, 1.0, 0.0);

	glRotatef(angle[2], 0.0, 0.0, 1.0);

	glRotated(45.0, 1.0, 0.0, 0.0);

	Egg();

	glFlush();
	// Przekazanie polecen rysujacych do wykonania
	glutSwapBuffers();

}
// Funkcja ustalajaca stan renderowania
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 1.0,1.0, 1.0, 1 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 100.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { (GLfloat) 0.05 };
	GLfloat att_quadratic = { (GLfloat) 0.001 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
	glEnable(GL_LIGHTING);   // właczenie systemu oświetlenia sceny
	glEnable(GL_LIGHT0);     // włączenie źródła o numerze 0
	glEnable(GL_DEPTH_TEST); // włączenie mechanizmu z-bufora
}

// Funkcja ma za zadanie utrzymanie stalych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokosc i szerokosc okna) sa
// przekazywane do funkcji za kazdym razem gdy zmieni sie rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	//pix2angle = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_x = 360.0*0.1 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0*0.1 / (float)vertical;
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreslajacej proporcje
	// wymiarów okna 

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkosciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  

	glMatrixMode(GL_PROJECTION);
	// Przelaczenie macierzy biezacej na macierz projekcji 

	glLoadIdentity();
	// Czyszcznie macierzy biezacej            

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspólczynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreslenie tak zwanej
	// przestrzeni ograniczajacej pozwalajacej zachowac wlasciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczjacej sluzy funkcja
	// glOrtho(...)            

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego
	glMatrixMode(GL_MODELVIEW);
	// Przelaczenie macierzy biezacej na macierz widoku modelu                                    

	glLoadIdentity();
	// Czyszcenie macierzy biezacej

}
void main(void)
{
	//glutIdleFunc(spinEgg);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(700, 700);

	glutCreateWindow("JAJO");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcje zwrotna odpowiedzialna
	// zazmiany rozmiaru okna      

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyzej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystapieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// Wlaczenie mechanizmu usuwania powierzchni niewidocznych

	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT


}