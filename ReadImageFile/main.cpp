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
#include <math.h>

#define PI 3.1415926535898
using namespace std;

int frame = 0;
const int numberOfBalls = 4;
int adjustBallX;
double ballX = 360;
int screenW = 800;
int screenH = 717;
const double originalT = 100;
int numberOfInstants = 100;
double t = numberOfInstants;
int velocidade = 100;
int horizontY = 240;

// images
Image *ballsL[numberOfBalls];
Image *ballsM[numberOfBalls];
Image *ballsS[numberOfBalls];
Image *field;
Image *goalkeeperL;
Image *goalkeeperR;
Image *barrierL0;
Image *barrierL1;
Image *barrierR0;
Image *barrierR1;

int calcBallY() {
    // TODO implementar a balística de verdade
//    double x, y, g = 9.8;
//    double angulo = 30.0;
//    double a = angulo / 180 * PI;
//    x = velocidade * cos(a) * t;
//    y = v * sin(a) * ( (x / velocidade) * cos(a) ) - ( (g*x*x) / 2*(v*v * cos(a) * cos(a) ) );
//    t += 0.1;
    
    // TODO essa implementação é fake
    double startHeight = 80;
    double heightToGain = 300.;
    double minHeightOnFall = horizontY;
    double heightRatio;
    double halfOriginalT = originalT / 2;
    double height;
    
    if (t > (halfOriginalT)) {
        heightRatio = (originalT - t) / halfOriginalT;
        height = heightRatio * heightToGain;
        height += startHeight;
    } else {
        heightRatio = t / halfOriginalT;
        
        double originalRange = heightToGain;
        double fallRange = (startHeight + heightToGain) - minHeightOnFall;
        heightRatio = (heightRatio / originalRange) * fallRange;
        
        height = heightRatio * heightToGain;
        height += minHeightOnFall;

    }
    
    t -= 1;
    return height;
}

int calcBallX() {
    double lateralDisplacement = -100;
    double tRatio = 1.0 / numberOfInstants;
    double howMuchToDisplace = tRatio * lateralDisplacement;
    ballX += howMuchToDisplace;
//    printf("howMuchToDisplace: %.2f\n", howMuchToDisplace);
    return ballX;
}

int generateRandomRgb() {
    int r = rand() * 255;
    int g = rand() * 255;
    int b = rand() * 255;
    int rgb = (r << 16) | (g << 8) | b;
    return rgb;
}

Image* getBallSprite() {
    Image *ball;
    int index = frame % numberOfBalls;
    if (t > 67) {
        ball = ballsL[index];
        adjustBallX = 0;
    } else if (t > 33) {
        ball = ballsM[index];
        adjustBallX = 10;
    } else {
        ball = ballsS[index];
        adjustBallX = 20;
    }
    return ball;
}

Image* plotStuff() {
    Image *ball = getBallSprite();
    Image *result = new Image(screenW, screenH);
    
    result->plot(field, 0, 0);
    result->plot(goalkeeperL, 300, horizontY + 20);
    result->plot(barrierL0, 100, 180);
    result->plot(barrierL1, 200, 180);
    result->plot(barrierR0, 300, 180);
    result->plot(barrierR1, 400, 180);
    result->plot(ball, calcBallX() + adjustBallX, calcBallY());
    
    return result;
}

void play(int value) {
    //    glRasterPos2d(0.5, 0.5);
    Image *resultImage = plotStuff();
    glDrawPixels(resultImage->getWidth(), resultImage->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, resultImage->getPixels());
    glFlush();
    frame++;
    if (t >= 0) {
        if (t == 99) {
            glutTimerFunc(2000, play, 1);
        } else {
            glutTimerFunc(15, play, 1);
        }
    }
}

Image* loadImage(char const *path) {
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
    
    // barrier
    barrierL0 = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/barrierL0.ptm");
    barrierL1 = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/barrierL1.ptm");
    barrierR0 = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/barrierR0.ptm");
    barrierR1 = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/barrierR1.ptm");

    // goalkeeper
    goalkeeperL = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/goalkeeperL.ptm");
    goalkeeperR = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/goalkeeperR.ptm");
    
    // balls large
    ballsL[0] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballL/ball0.ptm");
    ballsL[1] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballL/ball1.ptm");
    ballsL[2] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballL/ball2.ptm");
    ballsL[3] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballL/ball3.ptm");

    // balls medium
    ballsM[0] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballM/ball0.ptm");
    ballsM[1] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballM/ball1.ptm");
    ballsM[2] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballM/ball2.ptm");
    ballsM[3] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballM/ball3.ptm");

    // balls small
    ballsS[0] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballS/ball0.ptm");
    ballsS[1] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballS/ball1.ptm");
    ballsS[2] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballS/ball2.ptm");
    ballsS[3] = loadImage("/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/ballS/ball3.ptm");
}

void init (void)
{
    /*  select clearing (background) color       */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    
    /*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glViewport(0, 0, screenW, screenH);
    
    loadImages();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (screenW, screenH);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    init ();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   /* ISO C requires main to return int. */
}
