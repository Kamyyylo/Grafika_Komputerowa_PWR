#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>



float triangleSize = 60;
float height = triangleSize*sqrt(3)/2;

float triangleColor(void)
{
	float x = rand() % 100;
	return x/100;   
}

float perturbation(void)
{
	float x = rand() % 100;
	return x / 100; 
}

void smallestTriangle(float x, float y)
{

		
			
				//float p =  perturbation();
				glBegin(GL_TRIANGLES);
				glColor3f(triangleColor(), triangleColor(), triangleColor()); //Losuje kolor pierwszego rogu
				glVertex2f(x+perturbation(),y+perturbation());
				glColor3f(triangleColor(), triangleColor(), triangleColor()); // losuje kolor drugiego rogu
				glVertex2f(x+triangleSize/4 + perturbation(),y + perturbation());
				glColor3f(triangleColor(), triangleColor(), triangleColor()); // losuje kolor trzeciego rogu
				glVertex2f(x + triangleSize / 8 + perturbation(), y+ (triangleSize / 4 * sqrt(3) / 2) + perturbation());

				glEnd(); 
				glFlush();
				
	     
				//float p =  perturbation();
				glBegin(GL_TRIANGLES);
				glColor3f(triangleColor(), triangleColor(), triangleColor()); //Losuje kolor pierwszego rogu
				glVertex2f(x+triangleSize/4 + perturbation(), y + perturbation());
				glColor3f(triangleColor(), triangleColor(), triangleColor()); // losuje kolor drugiego rogu
				glVertex2f(x + triangleSize / 2 + perturbation(), y + perturbation());
				glColor3f(triangleColor(), triangleColor(), triangleColor()); // losuje kolor trzeciego rogu
				glVertex2f(x + (triangleSize / 4)+triangleSize/8 + perturbation(), y + (triangleSize / 4 * sqrt(3) / 2) + perturbation());

				glEnd();
				glFlush();
				

		glBegin(GL_TRIANGLES);
		glColor3f(triangleColor(), triangleColor(), triangleColor()); //Losuje kolor pierwszego rogu
		glVertex2f(x+triangleSize/8 + perturbation(),y+(triangleSize / 4 * sqrt(3) / 2) + perturbation());
		glColor3f(triangleColor(), triangleColor(), triangleColor()); // losuje kolor drugiego rogu
		glVertex2f(x + (triangleSize / 4)+triangleSize/8 + perturbation(), y + (triangleSize / 4 * sqrt(3) / 2) + perturbation());
		glColor3f(triangleColor(), triangleColor(), triangleColor()); // losuje kolor trzeciego rogu
		glVertex2f(x +(triangleSize/4) + perturbation(), y+ (triangleSize / 2 * sqrt(3) / 2) + perturbation());
		

		glEnd();
		glFlush();
	
	
}

void middleTriangle(float x, float y)
{
	/*for (int i = 0; i < 2; i++)
	{

		smallestSquare(x, y);



		x = x + squareSize/2;
	}
	smallestSquare(squareSize / 4, squareSize / 2*sqrt(3)/2);*/
	smallestTriangle(x, y);
	smallestTriangle((x+triangleSize/2), y);
	smallestTriangle((x + triangleSize / 4), y+((triangleSize/2)*sqrt(3)/2));

}

void bigestTriangle(float x, float y)
{

		/*for (int i = 0; i < 2; i++)
		{
			
				middleSquare(x, y);
			
			

			x = x + squareSize;
		}
		middleSquare(squareSize / 2, height);*/
		middleTriangle(x, y);
		middleTriangle(x+triangleSize, y);
		middleTriangle((x + triangleSize/2),(y+triangleSize*sqrt(3)/2));

}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	bigestTriangle(0, 0);

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
	

	glTranslatef(-0.3f, -0.3f, 0.0f);
	if (horizontal <= vertical)

		glOrtho(-100, 100, -100 / AspectRatio, 100 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100 *AspectRatio, 100 *AspectRatio, -100, 100, 1.0, -1.0);



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