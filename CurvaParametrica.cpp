/*
 * Curva_paramétrica_3D.cpp
 *
 * Copyright 2014 Rafael Andrade <rafaelsandrade@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *  Desenha uma curva paramétrica em 3D
 *
 *  Comandos:
 *      '+' Aumenta a distância da curva
 *      '-' Diminue a distância da curva
 *      'ESC' fecha o programa
 *      Clicar e movimentar o mouse, muda a visualização
 */


#include <GL/glut.h>
#include <cmath>

#define PASSO_TEMPO 50  // Taxa de atualização da tela (20 FPS)

//  Angulos de visualização do observador
float obsteta = 0.79, obsfi = 1.5, distancia = 200.0;

//     Função para desenhar os Eixos X, Y e Z
void Eixos3D(float dim)
{
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3d(-dim, 0.0, 0.0);       glVertex3d(dim, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3d(0.0, -dim, 0.0);       glVertex3d(0.0, dim, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3d(0.0, 0.0, -dim);       glVertex3d(0.0, 0.0, dim);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(dim, 0.0, 0.0);  //  Desenha o Eixo X
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'X');
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos3f(0.0, dim, 0.0);  //  Desenha o Eixo Y
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'Y');
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos3f(0.0, 0.0, dim);  //  Desenha o Eixo Z
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,'Z');
}

//  Função da curva paramétrica r(t) = ( f(t), g(t), h(t) )
void funcao_r(double coord[3], double t)
{
    coord[0] = t / 2.0 * sin(t);    //  x = f(t)
    coord[1] = t / 2.0 * cos(t);    //  y = g(t)
    coord[2] = t / 2.0;             //  z = h(t)
}

//  Desenha os elementos na tela
void Desenhar(void)
{
    GLfloat posicao_luz[]={100.0, 100.0, 100.0, 1.0};   // Posição Luz
    double px = 0.0, py = 0.0, pz = 0.0;    // Coordenadas do olho do observador
    double obsx, obsy, obsz;    // Coordenadas da posição do observador
    double coord[3] = {0.0}; // Armazena as coordenadas x, y, z da curva
    float delta = 0.1; // Resolução da curva

    // Calcula a distância do observador
    obsx = px + distancia * sin(obsfi) * cos(obsteta);
    obsy = py + distancia * sin(obsfi) * sin(obsteta);
    obsz = pz + distancia * cos(obsfi);

    // Posiciona a Câmera
    glLoadIdentity();
    gluLookAt(obsx, obsy, obsz, px, py, pz, 0.0f,0.0f,1.0f);

    ///////////////////////////////// Desenho ////////////////////////////////////////////////
    glClearColor( 0.0 , 0.0 , 0.0, 1.0 ); /// Cor de fundo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLightfv (GL_LIGHT0, GL_POSITION, posicao_luz);
    glEnable(GL_LIGHTING); // Habilita a luz

    // Desenha a curva
    for (float t = -20.0 * M_PI; t < 20.0 * M_PI; t += delta)
    {
        glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0); /// Cor da curva
            funcao_r(coord, t);
            glVertex3d(coord[0], coord[1], coord[2]);
            funcao_r(coord, t + delta);
            glVertex3d(coord[0], coord[1], coord[2]);
        glEnd();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    glDisable(GL_LIGHTING);

    Eixos3D(15.0);     //  Desenha os eixos X, Y e Z

    glutSwapBuffers();
}

//   Função para iluminar o ambiente
void Iluminacao(void)
{
    GLfloat ambiente[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat difusa[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat especular[] = { 1.0, 1.0, 1.0, 1.0 };

    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void Timer(int w)
{
    glutPostRedisplay();
    glutTimerFunc(PASSO_TEMPO, Timer, 1);
}

//  Redimensiona a Janela
void Ajustedimensao(GLsizei w, GLsizei h)
{
    // Não deixa a largura da janela ser menor que 700
    if(w < 700) glutReshapeWindow(700, h);

    // Não deixa a altura da janela ser menor que 700
    if(h < 500) glutReshapeWindow(w, 500);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(24.0f, (GLfloat)w / (GLfloat)h, 0.5f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

//  Função para mudar a visualização do espaço pelo movimento do mouse
void MoveMouse(int x, int y)
{
    float passo = 0.04; // Velocidade de movimento do mouse
    float r = 0.0f, q = 0.0f;
    static int Xo = 0, Yo = 0;

    if(x > Xo) r = -passo;
    if(x < Xo) r = passo;
    if(y > Yo && obsfi > 0.02) q = -passo;
    if(y < Yo && obsfi < 3.13) q = passo;

    obsteta += r;
    obsfi += q;

    Xo = x;
    Yo = y;
}

//  Função do Teclado
void teclado(unsigned char key, int a, int b)
{
    switch (key)
    {
        case 27:    // Fecha o programa
            exit(0);
            break;

        case '-':   // Diminue a distancia da curva
            distancia *= 0.95;
            break;

        case '+':   // Aumenta a distancia da curva
            distancia *= 1.05;
            break;
    }
}

// Função Principal
int main(int argc, char*argv[])
{
    glutInit (&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Curva paramétrica 3D");
    glutDisplayFunc(Desenhar);
    glutReshapeFunc(Ajustedimensao);
    glutMotionFunc(MoveMouse);
    glutKeyboardFunc(teclado);
    Iluminacao();
    Timer(1);
    glutMainLoop();
    return 0;
}
