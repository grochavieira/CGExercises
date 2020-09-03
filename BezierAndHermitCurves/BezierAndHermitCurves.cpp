#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "GL\glut.h"

#define myPI 3.14159265

#define MAIORX  200.0f
#define MENORX -200.0f
#define MAIORY  200.0f
#define MENORY -200.0f

#define MAIORX_BEZIER  400.0f
#define MENORX_BEZIER  0.0f
#define MAIORY_BEZIER  400.0f
#define MENORY_BEZIER  0.0f

#define LARGURA_WINDOW 400
#define ALTURA_WINDOW  400

#define POSWX 250
#define POSWY 150

#define tang1x -1000.0f
#define tang1y 1000.0f
#define tang2x -1000.0f
#define tang2y 1000.0f

GLfloat B[4][2]; 
GLfloat P[2][4]; 

GLboolean GET_POINTS = 0;   
GLint TOTAL_POINTS = 0;  
GLfloat bx, by;  



void Inicializa(void);   
void AlteraTamanhoJanela(GLsizei w, GLsizei h);  
void Teclado(unsigned char key, int x, int y);   
void Desenha(void);   
void myMousefuncBezierIterate(int button, int state, int x, int y);   
GLfloat calculaPontoBezierGrau3(float t, GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3);

GLfloat multiplyHermite(GLfloat t, GLfloat H[][4], GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3);
void DesenhaHermiteGrau3();
void DesenhaBezierGrau3_v3();
void DesenhaEixos();


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(POSWX, POSWY);

	glutInitWindowSize(LARGURA_WINDOW, ALTURA_WINDOW);

	glutCreateWindow("Uma Bela Curva de Bezier, ou Hermite, de Grau 3");

	glutDisplayFunc(Desenha);

	glutReshapeFunc(AlteraTamanhoJanela);

	glutKeyboardFunc(Teclado);

	glutMouseFunc(myMousefuncBezierIterate);

	Inicializa();

	glutMainLoop();

	return 0;
}

void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	DesenhaEixos();

	//DesenhaParabola();
	DesenhaBezierGrau3_v3(); // usar
	//DesenhaHermiteGrau3(); /// usar

	glFlush();
	//glutPostRedisplay();
}


void DesenhaEixos()
{
	glColor3f(0.0, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(200.0f, MENORY_BEZIER);
	glVertex2f(200.0f, MAIORY_BEZIER);
	glEnd();

	glColor3f(0.0, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(MENORX_BEZIER, 200.0f);
	glVertex2f(MAIORX_BEZIER, 200.0f);
	glEnd();

}


void DesenhaHermiteGrau3()
{
	GLfloat H[4][4] = { { 2.0f, -2.0f,  1.0f,  1.0f },
	{ -3.0f,  3.0f, -2.0f, -1.0f },
	{ 0.0f,  0.0f,  1.0f,  0.0f },
	{ 1.0f,  0.0f,  0.0f,  0.0f } };


	if ((TOTAL_POINTS == 0) && (GET_POINTS))
	{
		P[0][0] = bx; 
		P[0][1] = by; 
		P[0][2] = tang1x; 
		P[0][3] = tang1y;  
		TOTAL_POINTS = TOTAL_POINTS + 1;
	}

	if ((TOTAL_POINTS > 0) && (GET_POINTS) && (TOTAL_POINTS < 2))
	{
		if ((P[TOTAL_POINTS - 1][0] != bx) && (P[TOTAL_POINTS - 1][1] != by))
		{
			P[TOTAL_POINTS][0] = bx; 
			P[TOTAL_POINTS][1] = by; 
			P[TOTAL_POINTS][2] = tang2x; 
			P[TOTAL_POINTS][3] = tang2y; 
			TOTAL_POINTS = TOTAL_POINTS + 1;
			if (TOTAL_POINTS > 2)  TOTAL_POINTS = 2;
		}
	}

	glColor3f(1.0, 0.0f, 0.0f);
	glPointSize(5.0f);
	for (int i = 0; i < TOTAL_POINTS; i++)
	{
		glBegin(GL_POINTS);
		glVertex2f(P[i][0], P[i][1]);
		glEnd();
	}

	if (TOTAL_POINTS == 2)
	{
		GLfloat xStart, yStart, xEnd, yEnd;
		xStart = P[0][0];
		yStart = P[0][1];

		GLfloat pontosControle[4];
		glColor3f(0.0, 1.0f, 0.5f);
		for (GLfloat t = 0; t < 1; t += 0.01)
		{
			
			xEnd = multiplyHermite(t, H, P[0][0], P[1][0], P[0][2], P[1][2]);
			yEnd = multiplyHermite(t, H, P[0][1], P[1][1], P[0][3], P[1][3]);

			glBegin(GL_LINE_STRIP);
			glVertex2f(xStart, yStart);
			glVertex2f(xEnd, yEnd);
			glEnd();
			xStart = xEnd;
			yStart = yEnd;
		}
	}

}

// fun��o que multiplica as 3 matrizes de Hermite T * H * M, onde M = {X,Y}
GLfloat multiplyHermite(GLfloat t, GLfloat H[][4], GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3 )
{
	GLfloat T[4] = { pow(t, 3), pow(t,2), t, 1 };
	GLfloat M[4] = { p0, p1, p2, p3 };
	GLfloat HM[4];

	for (int i = 0; i < 4; i++)
	{
		HM[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			HM[i] = HM[i] + H[i][j] * M[j];
		}
	}

	GLfloat R = 0;
	for (int i = 0; i < 4; i++)
	{
		R = R + T[i] * HM[i];
	}


	return R;
}




void DesenhaBezierGrau3_v3()
{
	const GLint DIMX = MENORX_BEZIER + MAIORX_BEZIER + 1;
	const GLint DIMY = MENORY_BEZIER + MAIORY_BEZIER + 1;

	if ((TOTAL_POINTS == 0) && (GET_POINTS))
	{
		B[0][0] = bx; 
		B[0][1] = by; 
		TOTAL_POINTS = TOTAL_POINTS + 1;
	}

	if ((TOTAL_POINTS > 0) && (GET_POINTS) && (TOTAL_POINTS < 4))
	{
		if ((B[TOTAL_POINTS - 1][0] != bx) && (B[TOTAL_POINTS - 1][1] != by))
		{
			B[TOTAL_POINTS][0] = bx;
			B[TOTAL_POINTS][1] = by;
			TOTAL_POINTS = TOTAL_POINTS + 1;
			if (TOTAL_POINTS > 4)  TOTAL_POINTS = 4;
		}
	}


	glColor3f(1.0, 0.0f, 0.0f);
	glPointSize(5.0f);
	for (int i = 0; i < TOTAL_POINTS; i++)
	{
		glBegin(GL_POINTS);
		glVertex2f(B[i][0], B[i][1]);
		glEnd();
	}

	
	if (TOTAL_POINTS == 4)
	{
		GLfloat xStart, yStart, xEnd, yEnd;
		xStart = B[0][0];
		yStart = B[0][1];

		GLfloat pontosControle[4];
		glColor3f(0.0, 1.0f, 0.5f);
		for (GLfloat t = 0; t < 1; t += 0.01)
		{
			xEnd = calculaPontoBezierGrau3(t, B[0][0], B[1][0], B[2][0], B[3][0]);
			yEnd = calculaPontoBezierGrau3(t, B[0][1], B[1][1], B[2][1], B[3][1]);

			glBegin(GL_LINE_STRIP);
			glVertex2f(xStart, yStart);
			glVertex2f(xEnd, yEnd);
			glEnd();
			xStart = xEnd;
			yStart = yEnd;
		}

		

	}


}

GLfloat calculaPontoBezierGrau3(GLfloat t, GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3)
{
	GLfloat vetorT[4] = { pow(t, 3), pow(t, 2), t, 1 };

	GLfloat matrizBezier[4][4] = { { -1.0, 3.0, -3.0, 1.0 },
									{ 3.0, -6.0, 3.0, 0.0 },
									{ -3.0, 3.0, 0.0, 0.0 },
									{ 1.0, 0.0, 0.0, 0.0 }
	};

	GLfloat M[4] = { 0, 0, 0, 0 };

	for (int i = 0; i < 4; i++)
	{
		GLfloat total = 0;
		for (int j = 0; j < 4; j++)
		{
			total += vetorT[j] * matrizBezier[i][j];
		}
		M[i] = total;
	}

	GLfloat ponto = M[0] * p0 + M[1] * p1 + M[2] * p2 + M[3] * p3;

	//GLfloat ponto = p0 * pow((1 - t), 3) + 3 * p1 * t * pow((1 - t), 2) + 3 * p2 * pow(t, 2) * (1 - t) + p3 * pow(t, 3);
	return ponto;
}


void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLfloat largura, altura;

	if (h == 0) h = 1;

	largura = w;
	altura = h;

	glViewport(0, 0, largura, altura);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	if (largura <= altura)
		gluOrtho2D(MENORX_BEZIER, MAIORX_BEZIER, MENORY_BEZIER * altura / largura, MAIORY_BEZIER * altura / largura);
	else
		gluOrtho2D(MENORX_BEZIER * largura / altura, MAIORX_BEZIER * largura / altura, MENORY_BEZIER, MAIORY_BEZIER);
}


void Teclado(unsigned char key, int x, int y)
{
	if (key == 27) 
		exit(0);
}


void Inicializa(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


void myMousefuncBezierIterate(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
	{
		bx = x;
		by = 400 - y;
		printf("\n %f %f \b", bx, by);
		GET_POINTS = 1;
		break;
	}
	case GLUT_RIGHT_BUTTON: {

		GET_POINTS = 0;
		TOTAL_POINTS = 0;
		break;
	}
	case GLUT_MIDDLE_BUTTON: {

		break;
	}
	};

	glutPostRedisplay();
}




