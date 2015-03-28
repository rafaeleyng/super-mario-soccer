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

int generateRandomRgb() {
    int r = rand() * 255;
    int g = rand() * 255;
    int b = rand() * 255;
    int rgb = (r << 16) | (g << 8) | b;
    return rgb;
}

void file(void) {
    
//    ifstream arq("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/bola0.ppm");
    ifstream arq("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/bola0.ptm");

    char BUFFER[256];
    arq >> BUFFER;
    
    if (BUFFER[1] == '3') {
        printf("Modo texto\n");
        // modo texto
    } else {
        printf("Modo binário\n");
        // modo binário
    }
    
    arq >> BUFFER;
    if (BUFFER[0] == '#') {
        // descartar a linha de comentário
        arq.getline(BUFFER, 256);
    }
    
    int w, h, max;
    arq >> w >> h;
    arq >> max;
    
    printf("w: %d, h: %d, max value: %d\n", w, h, max);
    
    Image *image = new Image(w, h);
    
    for (int i = 0; i < w * h; i++) {

        int a, r, g, b;
        arq >> a >> r >> g >> b;
        int x = i % w;
        int y = i / w;
        int pixel = (a << 24) | (r << 16) | (g << 8) | b;
        image->setPixels(pixel, x, y);
    }
    
    glDrawPixels(image->getWidth(), image->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image->getPixels());
    
    glFlush();
    
}

void display(void) {
    file();
    return;
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    int w = 640;
    int h = 480;
    
    Image *image = new Image(w, h);
    
    for (int i = 0; i < w * h; i++) {
        int x = i % w;
        int y = i / w;
        image->setPixels(generateRandomRgb(), x, y);
    }
    
    glDrawPixels(image->getWidth(), image->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image->getPixels());
    
    glFlush();
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
