#pragma once

#include "ofMain.h"
#include "processTweet.h"
#include "ofxPSBlend.h"

/*
 custom #include "ofxMySQL.h" 
 */
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void exit();
 
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    ofColor getAverage(const ofPixels& pix, int x, int y, int w, int h);
    
    ofxPSBlend psBlend;
    
    ofFbo salidaPsd;
    
    processTweet tweetStream;
    ofImage pepperMosaico;
    
    vector<posiciones> coloresGridDestinoPos;
    vector<ofColor> coloresGrid;
    
};
