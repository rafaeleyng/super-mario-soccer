//
//  Image.h
//  SuperMarioSoccer
//
//  Created by Rafael Eyng on 4/5/15.
//  Copyright (c) 2015 Rafael Eyng. All rights reserved.
//

#ifndef __SuperMarioSoccer__Image__
#define __SuperMarioSoccer__Image__

class Image {
public:
    Image(int w, int h) {
        width = w;
        height = h;
        pixels = new int[w*h];
    }
    ~Image() {
        delete pixels;
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
    void plot(Image *image, int xStart, int yStart) {
        for (int i = 0; i < image->getWidth() * image->getHeight(); i++) {
            int x = i % image->getWidth();
            int y = i / image->getWidth();
            int pixel = image->getPixel(x, y);
            x += xStart;
            y += yStart;
            int a = (pixel >> 24) & 0x100;
            //            int r = (pixel >> 16) & 0x100;
            //            int g = (pixel >> 8) & 0x100;
            //            int b = pixel & 0x100;
            // TODO - calcular o alpha de verdade
            if (a != 0 && (x < this->getWidth() && y < this->getHeight())) {
                this->setPixels(pixel, x, y);
            }
        }
    }
    
private:
    int *pixels;
    int width, height;
};

#endif /* defined(__SuperMarioSoccer__Image__) */
