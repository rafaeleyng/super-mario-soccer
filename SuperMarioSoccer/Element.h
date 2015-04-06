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
    Element(Image *image, double x, double y) {
        this->image = image;
        this->x = x;
        this->y = y;
    }
    Element(char const *path, double x, double y) : Element(ImageUtil::loadImage(path), x, y) {}
    Element(double x, double y) {
        this->setImage(NULL);
        this->setX(x);
        this->setY(y);
    }
    Element() {}
    void setImage(Image *image) {
        this->image = image;
    }
    Image* getImage() {
        return this->image;
    }
    void setX(double x) {
        this->x = x;
    }
    double getX() {
        return this->x;
    }
    void setY(double y) {
        this->y = y;
    }
    double getY() {
        return this->y;
    }
    void plotOn(Image *someImage) {
        someImage->plot(this->getImage(), this->getX(), this->getY());
    }
    
private:
    Image *image;
    double x;
    double y;
};

#endif /* defined(__SuperMarioSoccer__Element__) */
