//
//  processTweet.h
//  mysql
//
//  Created by Natxo Pedreira gonzalez on 11/2/17.
//
//

#ifndef mysql_processTweet_h
#define mysql_processTweet_h

#include "ofMain.h"
#include "ofxAsyncTexture.h"
#include "bdCommands.h"
#include "ofxImageSequenceRecorder.h"

/*
 get last tweet from db
 download the image
 and check record processed to the db
 */

class posiciones{
    
public:
    posiciones(int _x=0, int _y=0, int _w=0, int _h=0){
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }
    
    int x;
    int y;
    
    int w;
    int h;
};


class tweet {
    
public:
    tweet(string _imgUrl="", string _destUrl="")
    {
        imgUrl = _imgUrl;
        destUrl = _destUrl;
    }
    
    string imgUrl;
    string destUrl;
    int recordId;
    
    posiciones posicionEnGrid;
};



class processTweet {

private:
    
    ofImage * imgDestinoMosaico;
    ofImage * imgDestinoMosaicoPrint;
    
    int photoAncho;
    int photoAlto;
    int windowWidth;
    int windowHeight;
    int celdasOcupadas;

    int anchoPrint;
    int altoPrint;
    
    long lastTimeQuery;
    
    ofPoint cellSize;
    
    tweet lastTweet;
    bdCommands dabatase;
    
    vector<posiciones> posicionesGrid;
    
    void processImage(shared_ptr<ofTexture>& tex, int new_w, int new_h);
    

public:
    
    processTweet();
    
    void getLastTweet();
    void updateProcess();
    void limpiaPosiciones();
    

    void createGridPositions(int columnas, int filas);
    void setWindowSize(int w, int h);
    
    void setPhotoSize(int _ancho, int _alto);
    void setPhotoSizeToPrint(int _ancho, int _alto);

    void setImageDestinoMosaico(ofImage *img, ofImage *imgPrint);

    bool processing;

    vector<posiciones> getPosicionesGrid();
    
    ofPoint getGridCellSize();
    
    int contadorMiniaturas;
    
    string debugData();
    
    ofFbo fboThumb, fboThumbPrint, printerHack;
    ofFbo pantalla;

    ofxImageSequenceRecorder recorder;
    
    ofxAsyncTextureLoader loader;
    shared_ptr<ofTexture> myImage;
};

#endif
