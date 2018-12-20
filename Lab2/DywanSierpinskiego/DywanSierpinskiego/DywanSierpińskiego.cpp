#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>


float yourSize =4;
float squareSize = yourSize * 2; //zwiekszam rozmiar podwojnie dla lepszej widocznosci

float squareColor(void)
{
	float x = rand() % 100;
	return x/100;   
}

float perturbation(void)
{
	float x = rand() % 500;
	return x / 100; 
}

void smallestSquare(float x, float y)
{
	float _x = x - 3 * squareSize;
	float _y = y + 3 * squareSize;
	int a = 0;


	for (int j = 0; j < 3; j++)
	{
		
		for (int i = 0; i < 3; i++)
		{
			if (a != 4)
			{
				//float p =  perturbation();
				glBegin(GL_POLYGON);
				glColor3f(squareColor(), squareColor(), squareColor()); //Losuje kolor pierwszego rogu
				glVertex2f(_x+ perturbation(), _y+ perturbation());
				glColor3f(squareColor(), squareColor(), squareColor()); // losuje kolor drugiego rogu
				glVertex2f(_x+ perturbation() +  squareSize, _y+ perturbation());
				glColor3f(squareColor(), squareColor(), squareColor()); // losuje kolor trzeciego rogu
				glVertex2f(_x+ perturbation() +  squareSize, _y+ perturbation() -  squareSize);
				glColor3f(squareColor(), squareColor(), squareColor()); // losuje kolor czwartek rogu
				glVertex2f(_x+ perturbation(), _y+ perturbation() - squareSize);

				glEnd(); 
				glFlush();
				a++;
			}
			else a++;

			_x = _x + 1 * squareSize;
		}
		_x = _x - 3 * squareSize;
		_y = _y - 1 * squareSize;
	}
}

void middleSquare(float x, float y)
{
	int a = 0;
	for (int j = 0; j < 3; j++)
	{

		for (int i = 0; i < 3; i++)
		{
			if (a != 4)
			{
				smallestSquare(x, y);
				a++;
			}
			else a++;

			x = x + 3 * squareSize;
		}
		x = x - 9 * squareSize;
		y = y - 3 * squareSize;
	}
}

void bigestSquare(float x, float y)
{
	int a = 0;
	for (int j = 0; j < 3; j++)
	{

		for (int i = 0; i < 3; i++)
		{
			if (a != 4)
			{
				middleSquare(x, y);
				a++;
			}
			else a++;

			x = x + 9 * squareSize;
		}
		x = x - 27 * squareSize;
		y = y - 9 * squareSize;
	}
}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	bigestSquare(0, 0);

}


void MyInit(void)

{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	

}

/*************************************************************************************/
// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego

void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna

{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora

	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okre�lenie okna obserwatora.
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)
	

	glTranslatef(-0.40f, 0.35f, 0.0f);
	if (horizontal <= vertical)

		glOrtho(-200, 200, -200 / AspectRatio, 200 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-200 *AspectRatio, 200 *AspectRatio, -200, 200, 1.0, -1.0);



	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych    

	glLoadIdentity();

}

/*************************************************************************************/


// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli




void main(void)

{

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB


	glutCreateWindow("Dywan Sierpinskiego");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/