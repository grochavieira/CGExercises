#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;
//#include "stdafx.h"


#define myPI 3.14159265
#define MAIORX  50.0f
#define MENORX -50.0f
#define MAIORY  50.0f
#define MENORY -50.0f

#define LARGURA_WINDOW 500
#define ALTURA_WINDOW  500

#define POSWX 250
#define POSWY 150

struct Ponto
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};


Ponto P1, P2, P3, P4;

GLfloat Xc, Yc;


GLfloat T[4][4] = { { 1.0, 0.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 0.0, 1.0 }
};

GLfloat R[4][4] = { { 1.0, 0.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0, 0.0 },
{ 0.0, 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 0.0, 1.0 }
};


GLfloat S[4][4] = { { 1.0, 0.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0, 0.0 },
{ 0.0, 0.0, 1.0, 0.0 },
{ 0.0, 0.0, 0.0, 1.0 }
};


void Inicializa(void);
void AlteraTamanhoJanela(GLsizei w, GLsizei h);
void Desenha(void);
void DesenhaEixos();
void myMousefunc(int button, int state, int x, int y);
void myTranslation(struct Ponto* P, GLfloat Dx, GLfloat Dy, GLfloat Dz);
void myMatrixApply(GLfloat M[][4], struct Ponto* P);
void myMatrixMult(GLfloat A[][4], GLfloat B[][4], GLfloat M[][4]);
void DesenhaQuadrado(void);


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(POSWX, POSWY);
	glutInitWindowSize(LARGURA_WINDOW, ALTURA_WINDOW);

	glutCreateWindow("Desenho Generico em 2D");

	glutDisplayFunc(Desenha);

	glutReshapeFunc(AlteraTamanhoJanela);

	glutMouseFunc(myMousefunc);

	Inicializa();

	glutMainLoop();

	return 0;
}

void DesenhaQuadrado(void)
{
	glBegin(GL_QUADS);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(P1.x, P1.y, P1.z);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(P2.x, P2.y, P2.z);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(P3.x, P3.y, P3.z);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(P4.x, P4.y, P4.z);
	glEnd();
}

void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	DesenhaQuadrado();

	DesenhaEixos();

	glFlush();
}


void DesenhaEixos()
{
	glColor3f(0.0, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(0.0f, MENORY);
	glVertex2f(0.0f, MAIORY);
	glEnd();

	glColor3f(0.0, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(MENORX, 0.0f);
	glVertex2f(MAIORX, 0.0f);
	glEnd();
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{

	GLfloat largura, altura;

	if (h == 0) h = 1;

	largura = w;
	altura = h;

	glViewport(0, 0, largura, altura);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (largura <= altura)
		gluOrtho2D(MENORX, MAIORX, MENORY * altura / largura, MAIORY * altura / largura);
	else
		gluOrtho2D(MENORX * largura / altura, MAIORX * largura / altura, MENORY, MAIORY);
}

void Inicializa(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	P1.x = 0.0f; P1.y = 0.f; P1.z = 0.f;
	P2.x = 0.0f; P2.y = 20.f; P2.z = 0.f;
	P3.x = 20.0f; P3.y = 20.f; P3.z = 0.f;
	P4.x = 20.0f; P4.y = 0.0f; P4.z = 0.f;
}

void myMousefunc(int button, int state, int x, int y)
{
	GLfloat centroX = (P1.x + P2.x + P3.x + P4.x) / 4;
	GLfloat centroY = (P1.y + P2.y + P3.y + P4.y) / 4;

	float mouseX = x;
	float mouseY = y;
	float convertedMouseX = (mouseX / (LARGURA_WINDOW / 100) - MAIORX);
	float convertedMouseY = -(mouseY / (ALTURA_WINDOW / 100) - MAIORY);

	switch (button) {
		case GLUT_LEFT_BUTTON && (state == GLUT_DOWN):
		{	
			float newX = convertedMouseX - centroX;
			float newY = convertedMouseY - centroY;
			
			myTranslation(&P1, newX, newY, 0);
			myTranslation(&P2, newX, newY, 0);
			myTranslation(&P3, newX, newY, 0);
			myTranslation(&P4, newX, newY, 0);
			printf("\nvoce pressiona o botao esquerdo do mouse");
			break;
		}
		case GLUT_RIGHT_BUTTON: {
			/* Processo Passo a Passo

			T[0][3] = -convertedMouseX;
			T[1][3] = -convertedMouseY;

			myMatrixApply(T, &P1);
			myMatrixApply(T, &P2);
			myMatrixApply(T, &P3);
			myMatrixApply(T, &P4);
			
			float angle = 20.0;
			R[0][0] = cos(angle * myPI / 180.0);
			R[0][1] = -sin(angle * myPI / 180.0);
			R[1][0] = sin(angle * myPI / 180.0);
			R[1][1] = cos(angle * myPI / 180.0);

			myMatrixApply(R, &P1);
			myMatrixApply(R, &P2);
			myMatrixApply(R, &P3);
			myMatrixApply(R, &P4);
			
			T[0][3] = convertedMouseX;
			T[1][3] = convertedMouseY;

			myMatrixApply(T, &P1);
			myMatrixApply(T, &P2);
			myMatrixApply(T, &P3);
			myMatrixApply(T, &P4);
			*/

			GLfloat M1[4][4] = { { 1.0, 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0, 0.0 },
			{ 0.0, 0.0, 1.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 }
			};

			GLfloat M2[4][4] = { { 1.0, 0.0, 0.0, 0.0 },
			{ 0.0, 1.0, 0.0, 0.0 },
			{ 0.0, 0.0, 1.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 }
			};

			T[0][3] = convertedMouseX;
			T[1][3] = convertedMouseY;

			float angle = 20.0;
			R[0][0] = cos(angle * myPI / 180.0);
			R[0][1] = -sin(angle * myPI / 180.0);
			R[1][0] = sin(angle * myPI / 180.0);
			R[1][1] = cos(angle * myPI / 180.0);

			myMatrixMult(T, R, M1);

			T[0][3] = -convertedMouseX;
			T[1][3] = -convertedMouseY;
			
			myMatrixMult(M1, T, M2);

			myMatrixApply(M2, &P1);
			myMatrixApply(M2, &P2);
			myMatrixApply(M2, &P3);
			myMatrixApply(M2, &P4);

			printf("\nvoce pressiona o botao direito do mouse");
			break;
		}
		case GLUT_MIDDLE_BUTTON: {
			printf("\nvoce pressiona o botao do meio do mouse");
			break;
		}
	};

	switch (state) {
	case GLUT_DOWN: {
		printf("\nvoce esta pressionando o botao do mouse");
		break;
	}
	case GLUT_UP: {
		printf("\nvoce soltou o botao do mouse");
		break;
	}
	};

	glutPostRedisplay();

}

void myTranslation(struct Ponto* P, GLfloat Dx, GLfloat Dy, GLfloat Dz)
{
	GLfloat M[4][4] = { { 1.0, 0.0, 0.0, Dx },
	{ 0.0, 1.0, 0.0, Dy },
	{ 0.0, 0.0, 1.0, Dz },
	{ 0.0, 0.0, 0.0, 1.0 }
	};
	myMatrixApply(M, P);
}

void myMatrixApply(GLfloat M[][4], struct Ponto* P)
{
	GLfloat Pt[4];
	Pt[0] = P->x;
	Pt[1] = P->y;
	Pt[2] = P->z;
	Pt[3] = 1.0;
	P->x = 0; P->y = 0; P->z = 0;
	for (int i = 0; i < 4; i++)
	{
		P->x = P->x + M[0][i] * Pt[i];
		P->y = P->y + M[1][i] * Pt[i];
		P->z = P->z + M[2][i] * Pt[i];
	}
	printf("%f %f %f\n",P->x,P->y,P->z);
}

void myMatrixMult(GLfloat A[][4], GLfloat B[][4], GLfloat M[][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			M[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				M[i][j] = M[i][j] + A[i][k] * B[k][j];
			}
		}
	}
}


