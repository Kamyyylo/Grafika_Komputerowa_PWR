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
static int model = 0;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static float colors_tab[N][N][3];

#define M_PI 3.14159265358979323846

//funkcja obracaj¹ca model, pobrana ze strony zsk
void spinEgg()
{

	theta[0] -= 0.5;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.5;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.5;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay(); //odswiezenie zawartosci aktualnego okna
}


// Funkcja rysujaca osie ukladu wspólrzednych
void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// poczatek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// poczatek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  poczatek i koniec obrazu osi y

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



void Egg() {
	float tab[100][100][3];
	
	float u = 0.0;
	float v = 0.0;

	for (int i = 0; i<N; i++)
		for (int j = 0; j<N; j++)
		{
			//float u = (rand()%100)/100;
			u = (float)i / (N - 1);
			v = (float)j / (N - 1);
		
			tab[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 *pow(u, 2) - 45 * u)*cos(M_PI * v); // X(u,v)
			tab[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5; //Y(u,v)
			tab[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI * v); //Z(u,v)
		}



	glColor3f(0.0f, 1.0f, 0.0f);  //zielony kolor jajka


							
	if (model == 0)
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < N - 1; ++i) {
			for (int j = 0; j < N - 1; ++j)
			{
				glVertex3fv(tab[i][j]);
			}
		}
		glEnd();
	}

	//rysowanie linii
	if (model == 1)
	{
		
		for (int i = 0; i < N-1 ; i++) {
			for (int j = 0; j < N-1 ; j++)//N-1 bo wychodzi poza ramy
			{
				glBegin(GL_LINES);
				glVertex3fv(tab[i][j]);//x y z 3 wierzcholki
				glVertex3fv(tab[i][j + 1]);

				glVertex3fv(tab[i][j]);
				glVertex3fv(tab[i + 1][j]);

				glVertex3fv(tab[i + 1][j]);
				glVertex3fv(tab[i][j + 1]);

				glVertex3fv(tab[i+1][j ]);
				glVertex3fv(tab[i + 1][j+1]);
			}
		}
		glEnd();
	}
	//rysowanie kolorowych trójk¹tów
	if (model == 2)
	{
		for (int i = 0; i < N-1 ; i++) {
			for (int j = 0; j < N-1 ; j++)
			{

				glBegin(GL_TRIANGLES);

				glColor3fv(colors_tab[i][j+1]);
				glVertex3fv(tab[i][j + 1]);

				glColor3fv(colors_tab[i+1][j]);
				glVertex3fv(tab[i + 1][j]);

				glColor3fv(colors_tab[i+1][j+1]);
				glVertex3fv(tab[i + 1][j + 1]);



				glColor3fv(colors_tab[i+1][j]);
				glVertex3fv(tab[i + 1][j]);


				glColor3fv(colors_tab[i][j+1]);
				glVertex3fv(tab[i][j + 1]);


				glColor3fv(colors_tab[i][j]);
				glVertex3fv(tab[i][j]);

				glEnd();
				
			}
		}
	}

}


// Funkcja okreslajaca co ma byc rysowane (zawsze wywolywana gdy trzeba
// przerysowac scene)
void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczacym

	glLoadIdentity();
	// Czyszczenie macierzy biezacej

	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej wyzej

	glRotatef(theta[0], 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glRotatef(theta[2], 0.0, 0.0, 1.0);

	glRotated(45.0, 1.0, 0.0, 0.0);



	Egg();

	glFlush();
	// Przekazanie polecen rysujacych do wykonania



	glutSwapBuffers();
	//

}

//funkcja zwrotna do wyboru rodzaju modelu, pobrana ze strony zsk
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 0;
	if (key == 'w') model = 1;
	if (key == 's') model = 2;

	RenderScene(); // przerysowanie obrazu sceny
}


// Funkcja ustalajaca stan renderowania
void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcacy (wypelnienia okna) ustawiono na czarny

}


// Funkcja ma za zadanie utrzymanie stalych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokosc i szerokosc okna) sa
// przekazywane do funkcji za kazdym razem gdy zmieni sie rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

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

	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	// Przelaczenie macierzy biezacej na macierz widoku modelu                                    

	glLoadIdentity();
	// Czyszcenie macierzy biezacej

}

/*************************************************************************************/

float createRandomNumber()
{
	float randomNumber;
	double const generateRandom = rand() % 10;

	randomNumber = generateRandom / 10;

	return randomNumber;
}
void createRandomColors() {
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
	
			for (int l = 0; l < 3; l++)
			{
				colors_tab[i][j][l] = createRandomNumber();

			}

		}
	}
}
void main(void)
{
	createRandomColors();

	glutIdleFunc(spinEgg);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("JAJO");

	glutDisplayFunc(RenderScene);

	glutKeyboardFunc(keys);
	// Okreslenie, ze funkcja RenderScene bedzie funkcja zwrotna
	// (callback function).  Bedzie ona wywolywana za kazdym razem
	// gdy zajdzie potrzba przeryswania okna 

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcje zwrotna odpowiedzialna
	// zazmiany rozmiaru okna      

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyzej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystapieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// Wlaczenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT


}