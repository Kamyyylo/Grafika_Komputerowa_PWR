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
// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego

void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna

{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okreœlenie okna obserwatora.
	// Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
	// Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)
	

	glTranslatef(-0.40f, 0.35f, 0.0f);
	if (horizontal <= vertical)

		glOrtho(-200, 200, -200 / AspectRatio, 200 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-200 *AspectRatio, 200 *AspectRatio, -200, 200, 1.0, -1.0);



	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych    

	glLoadIdentity();

}

/*************************************************************************************/


// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli




void main(void)

{

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB


	glutCreateWindow("Dywan Sierpinskiego");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/