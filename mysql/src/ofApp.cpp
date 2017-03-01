#include "ofApp.h"
#include <cups/cups.h>
//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetLogLevel(OF_LOG_ERROR);

    int screen_rows = 0;
    int screen_cols = 0;
    string screen_file = "";
    int cell_print_width = 0;
    int cell_print_height = 0;
    string print_file = "";


#ifdef BDPREFERENCES
        vector<vector<string> > preferencias = tweetStream.dabatase.getPreferences();

        screen_rows = ofToInt(preferencias.at(0).at(0));
        screen_cols = ofToInt(preferencias.at(0).at(1));
        screen_file = preferencias.at(0).at(2);
        cell_print_width = ofToInt(preferencias.at(0).at(3));
        cell_print_height = ofToInt(preferencias.at(0).at(4));
        print_file = preferencias.at(0).at(5);

#else

        screen_rows = 4;
        screen_cols = 3;
        screen_file = "coca-cola-logo-splash.png";
        cell_print_width = 432;
        cell_print_height = 288;
        print_file = "coca-cola-logo-splash-print.png";

#endif

    /*
        para imprimir hay que meter a mano el tamaño de las impresiones y
        que estas al multiplicarlo por el numero de filas/columnas coincidan
        con el tamaño de la imagen pa imprimir

        pa pantalla no hay que hacer nada solo decir el numero de filas y columnas y el sistema calcula el tamaño de las casillas
    */


    // imagen a componer
    pepperMosaico.loadImage(screen_file);
    pepperMosaicoToPrint.loadImage(print_file);
    
    //
    tweetStream.setImageDestinoMosaico(&pepperMosaico, &pepperMosaicoToPrint);
    tweetStream.setWindowSize(1920,1200);

    // decimos el tamano de photo paImprimir
    // tiene que coincidir en que el  acho  de esta por el numero de colunmanas sea igual al ancho de la pepperMosaicoToPrint
    tweetStream.setPhotoSizeToPrint(cell_print_width, cell_print_height);
    
    // para pantalla aqui calcula el solo el tamaño de las casillas
    // decimos el numero de columnas y de filas
    tweetStream.createGridPositions(screen_rows, screen_cols);
    
    // decimos el tamano de photo que queremos
    tweetStream.setPhotoSize(tweetStream.getGridCellSize().x, tweetStream.getGridCellSize().y);
  
    // buscamos el primero
    tweetStream.getLastTweet();
}

//--------------------------------------------------------------
void ofApp::update(){

    tweetStream.updateProcess();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    

    
    tweetStream.pantalla.draw(0,0);
/*
    // debig area
    ofPushStyle();
    if (tweetStream.processing) {
        ofDrawBitmapStringHighlight(tweetStream.debugData(), ofPoint(20,20), ofColor(255,0,0), ofColor(0,0,0));
    }else{
        ofDrawBitmapStringHighlight(tweetStream.debugData(), ofPoint(20,20), ofColor(255,100,0), ofColor(0,0,0));
    }
    ofPopStyle();
*/

    ofSetWindowTitle("fps:" + ofToString(int(ofGetFrameRate())) + " num thumbs:" +  ofToString(tweetStream.contadorMiniaturas));
}


//--------------------------------------------------------------
void ofApp::exit(){
    tweetStream.recorder.waitForThread();
}
