//
//  main.cpp
//  ReadImageFile
//
//  Created by Rafael Eyng on 3/28/15.
//  Copyright (c) 2015 Rafael Eyng. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include "Image.h"
#include <fstream>

using namespace std;

int frame = 0;
Image *balls[7];
Image *field;

int generateRandomRgb() {
    int r = rand() * 255;
    int g = rand() * 255;
    int b = rand() * 255;
    int rgb = (r << 16) | (g << 8) | b;
    return rgb;
}

void play(int value) {
//    glRasterPos2d(0.5, 0.5);
    Image *ball = balls[frame%7];
    glDrawPixels(field->getWidth(), field->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, field->getPixels());
    glDrawPixels(ball->getWidth(), ball->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, ball->getPixels());
    glFlush();
    frame++;
    glutTimerFunc(100, play, 1);
    
}

Image* loadImage(char *path) {
    ifstream arq(path);
    char BUFFER[256];
    arq >> BUFFER;
    
    if (BUFFER[1] == '3' || BUFFER[1] == '7') {
//        printf("Modo texto\n");
    } else {
//        printf("Modo binário\n");
    }
    
    int w, h, max;
    arq >> w >> h;
    arq >> max;
    
    Image *image = new Image(w, h);
    for (int i = 0; i < w * h; i++) {
        int a, r, g, b;
        arq >> a >> r >> g >> b;
        int x = i % w;
        int y = i / w;
        y = h - y - 1; // inverte o y
        int pixel = (a << 24) | (r << 16) | (g << 8) | b;
        image->setPixels(pixel, x, y);
    }
    return image;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    play(1);
}

void loadImages() {
    // field
    field = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/field.ptm");
    
    // balls
    balls[0] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T0.ptm");
    balls[1] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T1.ptm");
    balls[2] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T2.ptm");
    balls[3] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T3.ptm");
    balls[4] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T4.ptm");
    balls[5] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T5.ptm");
    balls[6] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/bola/bolaAnimadaPTM_T6.ptm");
}

void init (void)
{
    /*  select clearing (background) color       */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    
    /*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glViewport(0, 0, 640, 480);
    
    srand(time(NULL));
    loadImages();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    init ();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   /* ISO C requires main to return int. */
}
