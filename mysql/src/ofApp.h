#pragma once

#include "ofMain.h"
#include "processTweet.h"




class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void exit();

    ofFbo salidaPsd;
    
    processTweet tweetStream;
    ofImage pepperMosaico, pepperMosaicoToPrint;
    
    vector<posiciones> coloresGridDestinoPos;
    vector<ofColor> coloresGrid;
    
};
