//
//  Image.h
//  ReadImageFile
//
//  Created by Rafael Eyng on 3/28/15.
//  Copyright (c) 2015 Rafael Eyng. All rights reserved.
//

#ifndef __ReadImageFile__Image__
#define __ReadImageFile__Image__

#include <stdio.h>

class Image {
public:
    Image(int w, int h) {
        width = w;
        height = h;
        pixels = new int[w*h];
    }
    void setPixels(int rgb, int x, int y) {
        pixels[(y * width) + x] = rgb;
    }
    int getPixel(int x, int y) {
        return pixels[(y * width) + x];
    }
    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }
    int *getPixels() {
        return pixels;
    }
    
private:
    int *pixels;
    int width, height;
};

#endif /* defined(__ReadImageFile__Image__) */
