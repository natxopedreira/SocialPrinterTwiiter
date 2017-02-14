#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetLogLevel(OF_LOG_ERROR);
    
    // imagen a componer
    pepperMosaico.loadImage("coca-cola-logo-splash.png");
    
    
    tweetStream.setImageDestinoMosaico(&pepperMosaico);
    
    tweetStream.setWindowSize(1920,1200);
    
    // decimos el numero de columnas y de filas
    tweetStream.createGridPositions(20, 16);
    
    /// decimos el tamano de photo que queremos
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
    
    vector<posiciones> cuadradikos = tweetStream.getPosicionesGrid();
    
    tweetStream.pantalla.draw(0,0);
    
    //imgBlend.draw(0, 0);
    
   /*
    ofPushStyle();
    
        ofSetColor(255, 255, 255,150);
        pepperMosaico.draw(0, 0);
    
    ofPopStyle();
    

        
    ofPushStyle();
    ofNoFill();
    ofSetColor(100);
    
    for (int i = 0; i < cuadradikos.size(); i++) {
        ofDrawRectangle(ofPoint(cuadradikos.at(i).x, cuadradikos.at(i).y), cuadradikos.at(i).w, cuadradikos.at(i).h);
    }
    
    ofPopStyle();
    */
    
    // debig area
    ofPushStyle();
    if (tweetStream.processing) {
        ofDrawBitmapStringHighlight(tweetStream.debugData(), ofPoint(20,20), ofColor(255,0,0), ofColor(0,0,0));
    }else{
        ofDrawBitmapStringHighlight(tweetStream.debugData(), ofPoint(20,20), ofColor(255,100,0), ofColor(0,0,0));
    }
    
    
    ofPopStyle();
    
    
    
    ofSetWindowTitle("fps:" + ofToString(int(ofGetFrameRate())) + " num thumbs:" +  ofToString(tweetStream.contadorMiniaturas));
}



//--------------------------------------------------------------
ofColor ofApp::getAverage(const ofPixels& pix, int x, int y, int w, int h) {
    float r = 0, g = 0, b = 0;
    for(int j = y; j < y + h; j++) {
        for(int i = x; i < x + w; i++) {
            const ofColor& cur = pix.getColor(i, j);
            r += cur.r;
            g += cur.g;
            b += cur.b;
        }
    }
    float n = w * h;
    return ofColor(r / n, g / n, b / n);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    tweetStream.getLastTweet();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
