//
//  ImageUtil.h
//  SuperMarioSoccer
//
//  Created by Rafael Eyng on 4/5/15.
//  Copyright (c) 2015 Rafael Eyng. All rights reserved.
//

#ifndef __SuperMarioSoccer__ImageUtil__
#define __SuperMarioSoccer__ImageUtil__

#include <fstream>
#include <math.h>
#include "Image.h"

using namespace std;

class ImageUtil {
public:
    static Image* loadImage(char const *path) {
        char *IMAGES_PATH = "/Users/rafael/Google Drive/unisinos/03_processamento_grafico/exercicios/SuperMarioSoccer/SuperMarioSoccer/img/";
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
    
};

#endif /* defined(__SuperMarioSoccer__ImageUtil__) */
