//
//  ofxGiantImage.h
//
//  Created by Tim Knapen on 15/03/13.
//  www.timknapen.be
//
/*
 
 This loads and displays huge images.
 
 To do this it creates a grid of tiles of ofTextures and draws them only when they are necessary.
 
 It has been tested to work very smoothly on 
 Macbook (13-inch, Aluminum, Late 2008) / 2 GHz Intel Core 2 Duo / 8 GB 1333 MHz DDR3 / NVIDIA GeForce 9400M 256 MB
 Mac Mini (2013) / 2,5-GHz dual-core Intel Core i5 / 4 GB (2x 2 GB) 1600-MHz DDR3-geheugen / Intel HD Graphics 4000
 on a 2560 x 1400 pixels display
 
 The image used to test is 20.000 × 15.000 pixels ( = 300.000.000 pixels :-)
 
 NOTES:
 1. Loading an image of this size will take a while, but don't worry, once it is loaded, the app runs smoothly.
 2. For now, this only works with RGB images.
 3. Draw( x, y, width, height) works a bit different than expected:
 it will not draw the image translated to x,y at the size width,height but it will draw whatever part of the image is contained within the rectangle defined by x, y, width, height.
 */

#ifndef  __ofxGiantImage__
#define  __ofxGiantImage__

#include "ofMain.h"


class ofxGiantImage {

    
public:
    ofxGiantImage();
    ~ofxGiantImage();
    
    void loadImage(string filePath); // load an image, just like ofImage
    void draw(float x, float y, float w, float h); // draw the tiles that fall within the rect x,y,w,h
    void drawBounds(float x, float y, float w, float h); // For debugging, draws the tiles' bounding rects

    float width;    // total width of all tiles combined = the width of the loaded image
    float height;   // total height of all tiles combined = the height of the loaded image
    
    int wtiles;     // number of tiles in the horizontal direction
    int htiles;     // number of tiles in the vertical direction
    int tileSize;   // default to 256 pixels, arbitrary choice
    
    vector <ofTexture*> tiles;
    
};

#endif
