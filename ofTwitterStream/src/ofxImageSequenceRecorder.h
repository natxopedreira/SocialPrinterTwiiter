/*

Based on code by Memo from this thread:
http://forum.openframeworks.cc/index.php?topic=1687.0

*/

#include "ofMain.h"    

typedef struct { 
    string fileName;    
    ofPixels image;
} QueuedImage;

class ofxImageSequenceRecorder : public ofThread {    
public:    

    int counter;
    queue<QueuedImage> q;

      
    ofxImageSequenceRecorder(){  

        
    }  

       
    void threadedFunction() {    
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
                ofSaveImage(i.image, i.fileName);


                string commando = "lpr -P  Dai_Nippon_Printing_DP-DS620 " +i.fileName;

                ofSystem(commando);

                q.pop();
            }
        }
        

        
    }   
    

    void addFrame(ofPixels imageToSave, string _filename) {

        
        //cout << _filename << endl;
        //char fileName[100]; 
        //snprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());     
        string fileName = _filename + ofToString(counter) + "_" + ".png";
        //counter++;
        
        counter++;

        QueuedImage qImage;
        
        qImage.fileName = fileName;
        qImage.image = imageToSave; 
        
        q.push(qImage);
        
    }    
};  
