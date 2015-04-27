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
#include <time.h>


#define PI 3.1415926535898

int frame = 0;
int adjustBallX; // to compensate the different sprit sizes for the ball
int adjustBallY; // to compensate the difference of height of the horizont line from the starting ball position
int screenW = 800;
int screenH = 717;

bool didBallPassBarrier;
bool isGoal;

double t = 0.0;
double originalDistance = 700;
double ballH;
double distanceTraveled;
double distanceStep;

int const horizontY = 240;
int const initialBallX = 360;
int const initialBallY = 90;
int ballX;

// goal
double goalTop = 420;
double goalRight = 585;
double goalBottom = 240;
double goalLeft = 220;

// game state
const int GAME_STATE_STARTING = 0;
const int GAME_STATE_BEFORE = 1;
const int GAME_STATE_DURING = 2;
const int GAME_STATE_AFTER = 3;
int GAME_STATE = GAME_STATE_BEFORE;

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

Image *optionsDirection[3];
Image *optionsHeight[3];
Image *optionsStrength[3];

Image *imageField;
Image *imageGoalkeeperL;
Image *imageGoalkeeperR;

Image *imageBarrierL0;
Image *imageBarrierL1;
Image *imageBarrierR0;
Image *imageBarrierR1;

const int numberOfBalls = 4;
Image *imagesBallsL[numberOfBalls];
Image *imagesBallsM[numberOfBalls];
Image *imagesBallsS[numberOfBalls];

Image *imageGoalText;
Image *imageMissText;

// elements
Element *field;
Element *goalkeeper;
Element *barrier0;
Element *barrier1;
Element *ball;
Element *goalText;
Element *missText;

void changeState(int newState) {
    printf("state: %d\n", newState);
    GAME_STATE = newState;
}

void startNewGame() {
    distanceStep = 0; // TODO melhorar isso, não deveria ser necessário
    distanceTraveled = 0;
    t = 0.0;
    didBallPassBarrier = false;

    ball->setX(initialBallX);
    ball->setY(initialBallY);
    ball->setImage(imagesBallsL[0]);
    
    isGoal = false;
    ballX = initialBallX;
    
    srand(time(NULL));
    barrier0->setX((rand() % 180) + 180);
    barrier1->setX((rand() % 180) + 370);

}

void resetOptions() {
    OPTIONS_VALUES[0] = 1;
    OPTIONS_VALUES[1] = 1;
    OPTIONS_VALUES[2] = 1;
}

double getSpeed() {
    double values[3] = {80.0, 90.0, 100.0};
    return values[OPTIONS_VALUES[OPTION_STRENGTH]];
}

double getAngle() {
    double values[3] = {15.0, 30.0, 45.0};
    return values[OPTIONS_VALUES[OPTION_HEIGHT]];
}

double getLateralDisplacement() {
    double displacement = 130.0;
    double values[3] = {-displacement, 0.0, displacement};
    return values[OPTIONS_VALUES[OPTION_DIRECTION]];
}

int calcBallX() {
    double lateralDisplacement = getLateralDisplacement();
    double prop =(distanceTraveled / originalDistance);
    double howMuchToDisplace = prop * lateralDisplacement;
    ballX = initialBallX + howMuchToDisplace;
    return ballX;
}

Image* getBallSprite() {
    Image *ball;
    int index = frame % numberOfBalls;
    if (distanceTraveled < (originalDistance * 0.5)) {
        ball = imagesBallsL[index];
        adjustBallX = 0;
    } else if (distanceTraveled < (originalDistance * 0.75)) {
        ball = imagesBallsM[index];
        adjustBallX = 10;
    } else {
        ball = imagesBallsS[index];
        adjustBallX = 20;
    }
    return ball;
}

Image* getGameImage() {
    Image *result = new Image(screenW, screenH);
    
    field->plotOn(result);
    goalkeeper->plotOn(result);
    
    if (didBallPassBarrier) {
        ball->plotOn(result);
        barrier0->plotOn(result);
        barrier1->plotOn(result);
    } else {
        barrier0->plotOn(result);
        barrier1->plotOn(result);
        ball->plotOn(result);
    }
    
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

void updateBall() {
    // sprite
    ball->setImage(getBallSprite());
    // x
    int ballXLocal = calcBallX();
    int ballAdjustX = adjustBallX;
    ball->setX(ballXLocal + ballAdjustX);
    // y
    adjustBallY = (distanceTraveled / originalDistance) * horizontY;
    int height = ballH + adjustBallY;
    height = height > adjustBallY ? height : adjustBallY; // so the ball never go lower than the ground
    ball->setY(height);
}

void calcBallPosition(double elapsedTime) {
    
    double speed = getSpeed();
    double angleDeg = getAngle();
    double g = 10;
    
    double angleRad = angleDeg * (3.14159 / 180.0);
    double distance = speed * cosf(angleRad) * elapsedTime;
    distanceTraveled = distance;
    
    double temp = distance / (speed * cos(angleRad));
    double height = distance * tan(angleRad) - 0.5 * g * (temp * temp);
    ballH = height + initialBallY;
    
//    printf("distance: %.2f, height: %.2f\n", distance, height);
    
    if (distanceStep == 0) {
        distanceStep = distance; // TODO melhorar a forma de calcular isso
    }
}

bool didElementCollideWithElement(Element *e1, Element *e2) {
    return
    (
     (e1->getX() >= e2->getX() && e1->getX() <= e2->getXEnd())
     || (e1->getXEnd() >= e2->getX() && e1->getXEnd() <= e2->getXEnd())
     )
    &&
    (
     (e1->getY() >= e2->getY() && e1->getY() <= e2->getYEnd())
     || (e1->getYEnd() >= e2->getY() && e1->getYEnd() <= e2->getYEnd())
     )
    ;
}

bool willPassElement(double zBefore, double zAfter, Element *element) {
    return zBefore < element->getZPosition() && zAfter >= element->getZPosition();
}

bool checkCollisions() {
    double zBefore = ball->getZPosition();
    ball->setZPosition(distanceTraveled / originalDistance);
    didBallPassBarrier = ball->getZPosition() >= barrier0->getZPosition();
    double zAfter = ball->getZPosition();
    
    if (willPassElement(zBefore, zAfter, barrier0)) {
        bool didCollide = didElementCollideWithElement(ball, barrier0);
        if (didCollide) {
            printf("barreira 0\n");
            return true;
        }
    }

    if (willPassElement(zBefore, zAfter, barrier1)) {
        bool didCollide = didElementCollideWithElement(ball, barrier1);
        if (didCollide) {
            printf("barreira 1\n");
            return true;
        }
    }
    
    if (willPassElement(zBefore, zAfter, goalkeeper)) {
        bool didCollide = didElementCollideWithElement(ball, goalkeeper);
        if (didCollide) {
            printf("Defendeu!\n");
            return true;
        }
    }
    
    return false;
}

bool didHitGoal() {
    return
    (
     (ball->getX() >= goalLeft && ball->getX() <= goalRight)
     || (ball->getXEnd() >= goalLeft && ball->getXEnd() <= goalRight)
     )
    &&
    (
     (ball->getY() >= goalBottom && ball->getY() <= goalTop)
     || (ball->getYEnd() >= goalBottom && ball->getYEnd() <= goalTop)
     )
    ;
}

void play() {
    t += 0.2;
    calcBallPosition(t);

    updateBall();
    Image *gameImage = getGameImage();
    drawImage(gameImage);
    frame++;
    
    bool didCollide = checkCollisions();
    
    if (didCollide) {
        isGoal = false;
        changeState(GAME_STATE_AFTER);
        return;
    }
    
    if (distanceTraveled >= originalDistance) {
        isGoal = didHitGoal();
        changeState(GAME_STATE_AFTER);
        return;
    }
}

void updateGoalkeeper() {
    int goalkeeperStep = 4;
    if (goalkeeper->getDirection() == 0) {
        if (goalkeeper->getX() > goalLeft) {
            goalkeeper->setX(goalkeeper->getX() - goalkeeperStep);
        } else {
            goalkeeper->setDirection(1);
            goalkeeper->setImage(imageGoalkeeperR);
        }
    } else {
        if (goalkeeper->getX() + goalkeeper->getImage()->getWidth() < goalRight) {
            goalkeeper->setX(goalkeeper->getX() + goalkeeperStep);
        } else {
            goalkeeper->setDirection(0);
            goalkeeper->setImage(imageGoalkeeperL);
        }
    }
    // faz o swing vertical
    goalkeeper->setY((horizontY + 25) + cos(goalkeeper->getX()/10.) * 20);
}

void drawGame(int value) {
    updateGoalkeeper();
    
    switch (GAME_STATE) {
        case GAME_STATE_BEFORE: {
            Image *gameImage = getGameImage();
            Image *optionsImage = getOptionsImage();
            gameImage->plot(optionsImage, 0, 0);
            drawImage(gameImage);
            
            // TODO revisar se isso é necessário
            delete gameImage;
            delete optionsImage;
            gameImage = nullptr;
            optionsImage = nullptr;
            break;
        }
        case GAME_STATE_DURING: {
            play();
            break;
        }
        case GAME_STATE_AFTER: {
            Image *gameImage = getGameImage();
            if (isGoal) {
                goalText->plotOn(gameImage);
            } else {
                missText->plotOn(gameImage);
            }
            drawImage(gameImage);
            return;
            break;
        }
    }
    glutTimerFunc(50, drawGame, 1);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGame(0);
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
}

void keyboard(unsigned char key, int x, int y) {
    switch (GAME_STATE) {
        case GAME_STATE_BEFORE:
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
//                    glutPostRedisplay();
                    break;
                case KEY_KICK:
                    changeState(GAME_STATE_DURING);
//                    glutPostRedisplay();
                    break;
            }
            break;
        case GAME_STATE_AFTER:
            switch (key) {
                case 'q':
                case 'Q':
                    exit(0);
                    break;
                case 'r':
                case 'R':
                    startNewGame();
                    changeState(GAME_STATE_BEFORE);
                    glutPostRedisplay();
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
    imagesBallsL[0] = ImageUtil::loadImage("ballL/ball0.ptm");
    imagesBallsL[1] = ImageUtil::loadImage("ballL/ball1.ptm");
    imagesBallsL[2] = ImageUtil::loadImage("ballL/ball2.ptm");
    imagesBallsL[3] = ImageUtil::loadImage("ballL/ball3.ptm");
    
    // balls medium
    imagesBallsM[0] = ImageUtil::loadImage("ballM/ball0.ptm");
    imagesBallsM[1] = ImageUtil::loadImage("ballM/ball1.ptm");
    imagesBallsM[2] = ImageUtil::loadImage("ballM/ball2.ptm");
    imagesBallsM[3] = ImageUtil::loadImage("ballM/ball3.ptm");
    
    // balls small
    imagesBallsS[0] = ImageUtil::loadImage("ballS/ball0.ptm");
    imagesBallsS[1] = ImageUtil::loadImage("ballS/ball1.ptm");
    imagesBallsS[2] = ImageUtil::loadImage("ballS/ball2.ptm");
    imagesBallsS[3] = ImageUtil::loadImage("ballS/ball3.ptm");
    
    // banners
    imageGoalText = ImageUtil::loadImage("text/goal.ptm");
    imageMissText = ImageUtil::loadImage("text/miss.ptm");
}

void initElements() {
    field = new Element(imageField, 0, 0);
    goalkeeper = new Element(imageGoalkeeperL, 300, horizontY + 20);
    goalkeeper->setDirection(0);
    barrier0 = new Element(imageBarrierL0, 0, 150, 0.5);
    barrier1 = new Element(imageBarrierR0, 0, 150, 0.5);
    ball = new Element(imagesBallsL[0], initialBallX, initialBallY, 0); // TODO melhorar o lugar de inicialização da posicão da bola
    
    goalText = new Element(imageGoalText, 280, 500);
    missText = new Element(imageMissText, 280, 500);
}

void init (void) {
    resetOptions(); // TODO tirar daqui
    glClearColor(1, 1, 1, 1);
    
    /*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glViewport(0, 0, screenW, screenH);
    
    initImages();
    initElements();
    startNewGame();

}

int main(int argc, char** argv) {
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