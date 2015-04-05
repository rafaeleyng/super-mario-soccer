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

const char *IMAGES_PATH = "/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/ReadImageFile/ReadImageFile/img/";
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

// game state
const int GAME_STATE_INITIAL = 0;
const int GAME_STATE_PLAYING = 1;
const int GAME_STATE_END = 2;
int GAME_STATE = GAME_STATE_INITIAL;

// options
const int OPTION_DIRECTION = 0;
const int OPTION_HEIGHT = 1;
const int OPTION_STRENGTH = 2;
int FIRST_OPTION = OPTION_DIRECTION;
int LAST_OPTION = OPTION_STRENGTH;
int CURRENT_OPTION = OPTION_DIRECTION;
int OPTIONS_VALUES[3];

// keys
const int KEY_KICK = 32; // spacebar
const int KEY_UP = 119; // w
const int KEY_RIGHT = 100; // d
const int KEY_DOWN = 115; // s
const int KEY_LEFT = 97; // a

// images
Image *optionDirection;
Image *optionHeight;
Image *optionStrength;

Image *optionsDirection[3];
Image *optionsHeight[3];
Image *optionsStrength[3];

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

double getSpeed() {
    int optionValue = OPTIONS_VALUES[OPTION_STRENGTH];
    if (optionValue == 0) {
        return 50;
    }
    if (optionValue == 1) {
        return 100;
    }
    return 200;
}

double getAngle() {
    int optionValue = OPTIONS_VALUES[OPTION_HEIGHT];
    if (optionValue == 0) {
        return 30;
    }
    if (optionValue == 1) {
        return 45;
    }
    return 60;
}

double getLateralDisplacement() {
    int optionValue = OPTIONS_VALUES[OPTION_DIRECTION];
    double displacement = 130.0;
    if (optionValue == 0) {
        return - displacement;
    }
    if (optionValue == 1) {
        return 0;
    }
    return displacement;
}

int calcBallY() {
    // TODO implementar a balística de verdade
//    double x, y, g = 9.8;
//    double angulo = 30.0;
//    double a = angulo / 180 * PI;
//    x = velocidade * cos(a) * t;
//    y = v * sin(a) * ( (x / velocidade) * cos(a) ) - ( (g*x*x) / 2*(v*v * cos(a) * cos(a) ) );
//    t += 0.1;
    
    // TODO essa implementação é fake
    double startHeight = 90;
    double heightToGain = 300.;
    double minHeightOnFall = horizontY;
    double heightRatio;
    double halfOriginalT = originalT / 2;
    double height;
    
    // subida
    if (t > (halfOriginalT)) {
        heightRatio = (originalT - t) / halfOriginalT;
        height = heightRatio * heightToGain;
        height += startHeight;
    }
    // queda
    else {
        heightRatio = t / halfOriginalT;
        
        double originalRange = heightToGain;
        double fallRange = (startHeight + heightToGain) - minHeightOnFall;
        heightRatio = (heightRatio / originalRange) * fallRange;
        
        height = heightRatio * heightToGain;
        height += minHeightOnFall;

    }
    
    return height;
}

int calcBallX() {
    double lateralDisplacement = getLateralDisplacement();
    double tRatio = 1.0 / numberOfInstants;
    double howMuchToDisplace = tRatio * lateralDisplacement;
    ballX += howMuchToDisplace;
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

Image* getGameImage() {
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

Image* getOptionsImage() {
    Image *optionsImage = new Image(screenW, 70);
    for (int i = 0; i < optionsImage->getHeight() * optionsImage->getWidth(); i++) {
        int a = 255, r = 0, g = 0, b = 0;
        int x = i % screenW;
        int y = i / screenW;
        int pixel = (a << 24) | (r << 16) | (g << 8) | b;
        optionsImage->setPixels(pixel, x, y);
    }
    
    int labelY = 10;
    // plot option labels
    optionsImage->plot(optionDirection, 10, labelY);
    optionsImage->plot(optionHeight, 210, labelY);
    optionsImage->plot(optionStrength, 410, labelY);
    
    // plot option values
    optionsImage->plot(optionsDirection[OPTIONS_VALUES[OPTION_DIRECTION]], 110, labelY);
    optionsImage->plot(optionsHeight[OPTIONS_VALUES[OPTION_HEIGHT]], 310, labelY);
    optionsImage->plot(optionsStrength[OPTIONS_VALUES[OPTION_STRENGTH]], 460, labelY);
    
    
    return optionsImage;
}

void drawImage(Image *image) {
    glDrawPixels(image->getWidth(), image->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image->getPixels());
    glFlush();
}

void play(int value) {
    t -= 1;
    Image *gameImage = getGameImage();
    drawImage(gameImage);
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
    char str[300];
    strcpy(str, IMAGES_PATH);
    strcat(str, path);
    
    ifstream arq(str);
    char BUFFER[256];
    arq >> BUFFER;
    
    if (BUFFER[1] == '3' || BUFFER[1] == '7') {
        // modo texto
    } else {
        // modo binário
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
    
    Image *gameImage = getGameImage();
    Image *optionsImage = getOptionsImage();
    gameImage->plot(optionsImage, 0, 0);
    drawImage(gameImage);

    if (GAME_STATE == GAME_STATE_PLAYING) {
        play(1);
    }
}

void loadImages() {
    // options
    optionDirection = loadImage("options/direction.ptm");
    optionHeight = loadImage("options/height.ptm");
    optionStrength = loadImage("options/strength.ptm");
    
    optionsDirection[0] = loadImage("options/direction0.ptm");
    optionsDirection[1] = loadImage("options/direction1.ptm");
    optionsDirection[2] = loadImage("options/direction2.ptm");
    optionsHeight[0] = loadImage("options/height0.ptm");
    optionsHeight[1] = loadImage("options/height1.ptm");
    optionsHeight[2] = loadImage("options/height2.ptm");
    optionsStrength[0] = loadImage("options/strength0.ptm");
    optionsStrength[1] = loadImage("options/strength1.ptm");
    optionsStrength[2] = loadImage("options/strength2.ptm");
    
    // field
    field = loadImage("field.ptm");
    
    // barrier
    barrierL0 = loadImage("enemies/barrierL0.ptm");
    barrierL1 = loadImage("enemies/barrierL1.ptm");
    barrierR0 = loadImage("enemies/barrierR0.ptm");
    barrierR1 = loadImage("enemies/barrierR1.ptm");

    // goalkeeper
    goalkeeperL = loadImage("enemies/goalkeeperL.ptm");
    goalkeeperR = loadImage("enemies/goalkeeperR.ptm");
    
    // balls large
    ballsL[0] = loadImage("ballL/ball0.ptm");
    ballsL[1] = loadImage("ballL/ball1.ptm");
    ballsL[2] = loadImage("ballL/ball2.ptm");
    ballsL[3] = loadImage("ballL/ball3.ptm");

    // balls medium
    ballsM[0] = loadImage("ballM/ball0.ptm");
    ballsM[1] = loadImage("ballM/ball1.ptm");
    ballsM[2] = loadImage("ballM/ball2.ptm");
    ballsM[3] = loadImage("ballM/ball3.ptm");

    // balls small
    ballsS[0] = loadImage("ballS/ball0.ptm");
    ballsS[1] = loadImage("ballS/ball1.ptm");
    ballsS[2] = loadImage("ballS/ball2.ptm");
    ballsS[3] = loadImage("ballS/ball3.ptm");
}

void handleOptions(int key) {
    if ((key == KEY_LEFT && CURRENT_OPTION == FIRST_OPTION)
        || (key == KEY_RIGHT && CURRENT_OPTION == LAST_OPTION)
        || (key == KEY_UP && OPTIONS_VALUES[CURRENT_OPTION] == 2)
        || (key == KEY_DOWN && OPTIONS_VALUES[CURRENT_OPTION] == 0)) {
        return;
    }
    
    if (key == KEY_LEFT) {
        CURRENT_OPTION--;
    } else if (key == KEY_RIGHT) {
        CURRENT_OPTION++;
    } else if (key == KEY_UP) {
        OPTIONS_VALUES[CURRENT_OPTION]++;
    } else if (key == KEY_DOWN) {
        OPTIONS_VALUES[CURRENT_OPTION]--;
    }
//    printf("Options: %d, %d, %d\n", OPTIONS_VALUES[0], OPTIONS_VALUES[1], OPTIONS_VALUES[2]);
}

void keyboard(unsigned char key, int x, int y) {
    switch (GAME_STATE) {
        case GAME_STATE_INITIAL:
            switch (key) {
                case 'q':
                case 'Q':
                    exit(0);
                    break;
                case KEY_UP:
                case KEY_RIGHT:
                case KEY_DOWN:
                case KEY_LEFT:
                    handleOptions(key);
                    glutPostRedisplay();
                    break;
                case KEY_KICK:
                    GAME_STATE = GAME_STATE_PLAYING;
                    glutPostRedisplay();
                    break;
            }
            break;
        case GAME_STATE_END:
            switch (key) {
                case 'q':
                case 'Q':
                    exit(0);
                    break;
                case 'r':
                case 'R':
                    printf("Play again\n");
            }
    }
}

void init (void)
{
    glClearColor(1, 1, 1, 1);
    
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
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;   /* ISO C requires main to return int. */
}
