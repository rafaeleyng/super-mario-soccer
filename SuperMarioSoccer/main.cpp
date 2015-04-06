//
//  main.cpp
//  SuperMarioSoccer
//
//  Created by Rafael Eyng on 4/5/15.
//  Copyright (c) 2015 Rafael Eyng. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <math.h>
#include "ImageUtil.h"
#include "Image.h"
#include "Element.h"

#define PI 3.1415926535898

int frame = 0;
const int numberOfBalls = 4;
int adjustBallX;
double ballX = 360;
int screenW = 800;
int screenH = 717;

double originalDistance = 100;
double currentDistance = originalDistance;

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
Image *arrow;
Image *optionDirection;
Image *optionHeight;
Image *optionStrength;

Image *imageField;
Image *imageGoalkeeperL;
Image *imageGoalkeeperR;

Image *imageBarrierL0;
Image *imageBarrierL1;
Image *imageBarrierR0;
Image *imageBarrierR1;

Image *optionsDirection[3];
Image *optionsHeight[3];
Image *optionsStrength[3];

Image *ballsL[numberOfBalls];
Image *ballsM[numberOfBalls];
Image *ballsS[numberOfBalls];


// elements
Element *field;
Element *goalkeeper;
Element *barrier0;
Element *barrier1;

void startNewGame() {
    printf("start new game");
    currentDistance = originalDistance;
}

void resetOptions() {
    OPTIONS_VALUES[0] = 1;
    OPTIONS_VALUES[1] = 1;
    OPTIONS_VALUES[2] = 1;
}

double getSpeed() {
    int optionValue = OPTIONS_VALUES[OPTION_STRENGTH];
    if (optionValue == 0) {
        return 1.0;
    }
    if (optionValue == 1) {
        return 2.0;
    }
    return 3.0;
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
    double halfDistance = originalDistance / 2;
    double height;
    
    // subida
    if (currentDistance > halfDistance) {
        heightRatio = (originalDistance - currentDistance) / halfDistance;
        height = heightRatio * heightToGain;
        height += startHeight;
    }
    // queda
    else {
        heightRatio = currentDistance / halfDistance;
        
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
    double tRatio = 1.0 / (originalDistance / getSpeed());
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
    if (currentDistance > (originalDistance * 0.67)) {
        ball = ballsL[index];
        adjustBallX = 0;
    } else if (currentDistance > (originalDistance * 0.33)) {
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
    
    field->plotOn(result);
    goalkeeper->plotOn(result);
    barrier0->plotOn(result);
    barrier1->plotOn(result);
    
    result->plot(ball, calcBallX() + adjustBallX, calcBallY());
    
    return result;
}

Image* getOptionsImage() {
    //    resetOptions();
    
    Image *optionsImage = new Image(screenW, 70);
    for (int i = 0; i < optionsImage->getHeight() * optionsImage->getWidth(); i++) {
        int a = 255, r = 0, g = 0, b = 0;
        int x = i % screenW;
        int y = i / screenW;
        int pixel = (a << 24) | (r << 16) | (g << 8) | b;
        optionsImage->setPixels(pixel, x, y);
    }
    
    // plot option labels
    int optionLabelY = 30;
    int labelSpacing = 200;
    int label0X = 50;
    int label1X = label0X + optionDirection->getWidth() + labelSpacing;
    int label2X = label1X + optionHeight->getWidth() + labelSpacing;
    optionsImage->plot(optionDirection, label0X, optionLabelY);
    optionsImage->plot(optionHeight, label1X, optionLabelY);
    optionsImage->plot(optionStrength, label2X, optionLabelY);
    
    // plot option values
    int optionValueY = 10;
    int margin = 20;
    optionsImage->plot(optionsDirection[OPTIONS_VALUES[OPTION_DIRECTION]], label0X + optionDirection->getWidth() + margin, optionValueY);
    optionsImage->plot(optionsHeight[OPTIONS_VALUES[OPTION_HEIGHT]], label1X + optionHeight->getWidth() + margin, optionValueY);
    optionsImage->plot(optionsStrength[OPTIONS_VALUES[OPTION_STRENGTH]], label2X + optionStrength->getWidth() + margin, optionValueY);
    
    // selected option arrow
    int arrowX;
    if (CURRENT_OPTION == 0) {
        arrowX = label0X;
    } else if (CURRENT_OPTION == 1) {
        arrowX = label1X;
    } else if (CURRENT_OPTION == 2) {
        arrowX = label2X;
    }
    arrowX -= arrow->getWidth() + 5;
    optionsImage->plot(arrow, arrowX, optionLabelY - 4);
    
    return optionsImage;
}

void drawImage(Image *image) {
    glDrawPixels(image->getWidth(), image->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image->getPixels());
    glFlush();
}

void updateDistance() {
    currentDistance -= getSpeed();
}

void play(int value) {
    updateDistance();
    Image *gameImage = getGameImage();
    drawImage(gameImage);
    frame++;
    
    if (currentDistance <= 0) {
        GAME_STATE = GAME_STATE_END;
    } else {
        glutTimerFunc(15, play, 1);
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch (GAME_STATE) {
        case GAME_STATE_INITIAL: {
            Image *gameImage = getGameImage();
            Image *optionsImage = getOptionsImage();
            gameImage->plot(optionsImage, 0, 0);
            drawImage(gameImage);
            
            break;
        }
        case GAME_STATE_PLAYING: {
            startNewGame();
            play(1);
            break;
        }
            
        case GAME_STATE_END: {
            break;
        }
    }
    
}

void initImages() {
    // options
    arrow = ImageUtil::loadImage("options/arrow.ptm");
    optionDirection = ImageUtil::loadImage("options/direction.ptm");
    optionHeight = ImageUtil::loadImage("options/height.ptm");
    optionStrength = ImageUtil::loadImage("options/strength.ptm");
    
    optionsDirection[0] = ImageUtil::loadImage("options/direction0.ptm");
    optionsDirection[1] = ImageUtil::loadImage("options/direction1.ptm");
    optionsDirection[2] = ImageUtil::loadImage("options/direction2.ptm");
    optionsHeight[0] = ImageUtil::loadImage("options/height0.ptm");
    optionsHeight[1] = ImageUtil::loadImage("options/height1.ptm");
    optionsHeight[2] = ImageUtil::loadImage("options/height2.ptm");
    optionsStrength[0] = ImageUtil::loadImage("options/strength0.ptm");
    optionsStrength[1] = ImageUtil::loadImage("options/strength1.ptm");
    optionsStrength[2] = ImageUtil::loadImage("options/strength2.ptm");
    
    // field
    imageField = ImageUtil::loadImage("field.ptm");
    
    // barrier
    imageBarrierL0 = ImageUtil::loadImage("enemies/barrierL0.ptm");
    imageBarrierL1 = ImageUtil::loadImage("enemies/barrierL1.ptm");
    imageBarrierR0 = ImageUtil::loadImage("enemies/barrierR0.ptm");
    imageBarrierR1 = ImageUtil::loadImage("enemies/barrierR1.ptm");
    
    // goalkeeper
    imageGoalkeeperL = ImageUtil::loadImage("enemies/goalkeeperL.ptm");
    imageGoalkeeperR = ImageUtil::loadImage("enemies/goalkeeperR.ptm");
    
    // balls large
    ballsL[0] = ImageUtil::loadImage("ballL/ball0.ptm");
    ballsL[1] = ImageUtil::loadImage("ballL/ball1.ptm");
    ballsL[2] = ImageUtil::loadImage("ballL/ball2.ptm");
    ballsL[3] = ImageUtil::loadImage("ballL/ball3.ptm");
    
    // balls medium
    ballsM[0] = ImageUtil::loadImage("ballM/ball0.ptm");
    ballsM[1] = ImageUtil::loadImage("ballM/ball1.ptm");
    ballsM[2] = ImageUtil::loadImage("ballM/ball2.ptm");
    ballsM[3] = ImageUtil::loadImage("ballM/ball3.ptm");
    
    // balls small
    ballsS[0] = ImageUtil::loadImage("ballS/ball0.ptm");
    ballsS[1] = ImageUtil::loadImage("ballS/ball1.ptm");
    ballsS[2] = ImageUtil::loadImage("ballS/ball2.ptm");
    ballsS[3] = ImageUtil::loadImage("ballS/ball3.ptm");
}

void initElements() {
    field = new Element(imageField, 0, 0);
    goalkeeper = new Element(imageGoalkeeperL, 300, horizontY + 20);
    barrier0 = new Element(imageBarrierL0, 100, 180);
    barrier1 = new Element(imageBarrierR0, 300, 180);
    // TODO
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
                    GAME_STATE = GAME_STATE_INITIAL;
                    glutPostRedisplay();
            }
    }
}

void init (void)
{
    resetOptions(); // TODO tirar daqui
    glClearColor(1, 1, 1, 1);
    
    /*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glViewport(0, 0, screenW, screenH);
    
    initImages();
    initElements();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (screenW, screenH);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Super Mario Soccer");
    init ();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;   /* ISO C requires main to return int. */
}