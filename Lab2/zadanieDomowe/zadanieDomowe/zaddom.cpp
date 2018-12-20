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
	

	glTranslatef(-0.3f, -0.3f, 0.0f);
	if (horizontal <= vertical)

		glOrtho(-100, 100, -100 / AspectRatio, 100 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100 *AspectRatio, 100 *AspectRatio, -100, 100, 1.0, -1.0);



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