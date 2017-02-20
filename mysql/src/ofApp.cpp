#include "ofApp.h"
#include <cups/cups.h>
//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetLogLevel(OF_LOG_ERROR);

    //tweetStream.limpiaPosiciones();

    // imagen a componer
    pepperMosaico.loadImage("coca-cola-logo-splash.png");
    pepperMosaicoToPrint.loadImage("coca-cola-logo-splash-print.png");
    
    //
    tweetStream.setImageDestinoMosaico(&pepperMosaico, &pepperMosaicoToPrint);
    tweetStream.setWindowSize(1920,1200);

    // decimos el tamano de photo paImprimir
    tweetStream.setPhotoSizeToPrint(432, 288);
    
    // decimos el numero de columnas y de filas
    tweetStream.createGridPositions(4, 3);
    
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
