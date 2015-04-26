//
//  Element.h
//  SuperMarioSoccer
//
//  Created by Rafael Eyng on 4/5/15.
//  Copyright (c) 2015 Rafael Eyng. All rights reserved.
//

#ifndef __SuperMarioSoccer__Element__
#define __SuperMarioSoccer__Element__

#include <stdio.h>
#include "Image.h"
#include "ImageUtil.h"
class Image;

class Element {
public:
    Element(Image *image, int x, int y, double zPosition) {
        this->image = image;
        this->x = x;
        this->y = y;
        this->zPosition = zPosition;
    }
    Element(Image *image, int x, int y) : Element(image, x, y, 1) {}
    Element(int x, int y) {
        this->setImage(NULL);
        this->setX(x);
        this->setY(y);
        this->setZPosition(1);
    }
    Element() {}
    void setImage(Image *image) {
        this->image = image;
    }
    Image* getImage() {
        return this->image;
    }
    void setX(int x) {
        this->x = x;
    }
    int getX() {
        return this->x;
    }
    void setY(int y) {
        this->y = y;
    }
    int getY() {
        return this->y;
    }
    int getWidth() {
        return this->image->getWidth();
    }
    int getHeight() {
        return this->image->getHeight();
    }
    int getXEnd() {
        return this->getX() + this->getWidth();
    }
    int getYEnd() {
        return this->getY() + this->getHeight();
    }
    void plotOn(Image *someImage) {
        someImage->plot(this->getImage(), this->getX(), this->getY());
    }
    int getDirection() {
        return this->direction;
    }
    void setDirection(int direction) {
        this->direction = direction;
    }
    double getZPosition() {
        return this->zPosition;
    }
    void setZPosition(double zPosition) {
        this->zPosition = zPosition;
    }
    
private:
    Image *image;
    int x;
    int y;
    int direction;
    double zPosition; // percentual of the total depth
};

#endif /* defined(__SuperMarioSoccer__Element__) */
