//
//  processTweet.cpp
//  mysql
//
//  Created by Natxo Pedreira gonzalez on 11/2/17.
//
//

#include "processTweet.h"

processTweet::processTweet(){
    
    processing = false;
    
    lastTweet.recordId = 0;
    lastTweet.imgUrl = "";
    lastTweet.destUrl = "";
    
    celdasOcupadas = 0;
    windowWidth = 0;
    windowHeight = 0;
    
    photoAncho = 0;
    photoAlto = 0;
    
    loader.setup();
    
    lastTimeQuery = ofGetElapsedTimeMillis();
    
    contadorMiniaturas = 0;
    
    cellSize.set(0, 0);

    pantalla.allocate(ofGetWidth(), ofGetHeight());
    
    pantalla.begin();
    ofClear(0);
    pantalla.end();
}
//--------------------------------------------------------------
void processTweet::setImageDestinoMosaico(ofImage *img, ofImage *imgPrint){
    imgDestinoMosaico = img;
    imgDestinoMosaicoPrint = imgPrint;
}


//--------------------------------------------------------------
void processTweet::setWindowSize(int w, int h){
    windowWidth = w;
    windowHeight = h;
}

//--------------------------------------------------------------
void processTweet::setPhotoSize(int _ancho, int _alto){
    photoAncho = _ancho;
    photoAlto = _alto;
    
    fboThumb.allocate(photoAncho, photoAlto);
    
    fboThumb.begin();
    ofClear(255,0);
    fboThumb.end();
}

//--------------------------------------------------------------
void processTweet::setPhotoSizeToPrint(int _ancho, int _alto){
    anchoPrint = _ancho;
    altoPrint = _alto;

    fboThumbPrint.allocate(anchoPrint, altoPrint);

    fboThumbPrint.begin();
    ofClear(255,0);
    fboThumbPrint.end();
}

//--------------------------------------------------------------
ofPoint processTweet::getGridCellSize(){
    return cellSize;
}

//--------------------------------------------------------------
void processTweet::createGridPositions(int columnas, int filas){
    
    int ancho = windowWidth/columnas;
    int alto = windowHeight/filas;
    
    cellSize.set(ancho, alto);
    
    bool positionCellsExists = dabatase.chekIfExitstPositionCells();
    
    
    
    int cuantas = 0;
    posicionesGrid.clear();
    
    for (int r = 0; r < ofGetHeight(); r+=alto ) {
        for (int c = 0; c < ofGetWidth(); c+=ancho ) {
            
            posiciones posicion(c,r,ancho,alto);
            posicionesGrid.push_back(posicion);
            
            if(!positionCellsExists){
                
                dabatase.setCellData(cuantas,c,r,ancho,alto,"no",0);
            }
            cuantas++;
        }
        
    }
    
    cout << cuantas << endl;
    
}



//--------------------------------------------------------------
vector<posiciones> processTweet::getPosicionesGrid(){
    return posicionesGrid;
}

//--------------------------------------------------------------
void processTweet::getLastTweet(){
    
    processing = true;
    
    // data
    vector<vector<string> > results = dabatase.getTweet();
    //db.ofxMySQL::getStrings(results, "twits", camposDb, "WHERE procesado='no' ", " ORDER BY idtwits DESC LIMIT 1");
    
    // hay posciones
    bool freeCellsExist = dabatase.checkForFreeCells();
    
    
    // si no hay posiciones libres en el grid para de trabajar
    if(!freeCellsExist){
        processing = false;
        return;
    }
    
    if(results.size() < 1){
        // no hay data asi que pedimos el siguiente
        lastTweet.recordId = 0;
        lastTweet.imgUrl = "";
        lastTweet.destUrl = "";
        
        processing = false;
        lastTimeQuery = ofGetElapsedTimeMillis();
        
    }else if (results.size()>0){
        
        // tenemos data asi que pillamos el ultimo tweet
        lastTweet.recordId = ofToInt(results.at(0).at(0));
        lastTweet.imgUrl = results.at(0).at(2);
        lastTweet.destUrl = results.at(0).at(3);
        
        
        
        loader.loadTextureAsync(lastTweet.imgUrl, [this] (shared_ptr<ofTexture> tex) {
            
            if (tex != NULL) {
                // tenemos la imagen asi que lo marcamos como feito
                processImage(tex, photoAncho, photoAlto);
            }else{
                processing = false;
                dabatase.checkTweetProcessed(lastTweet.recordId);
                lastTimeQuery = ofGetElapsedTimeMillis();
            }
            
            
        });
    }
    
    
    
    
    
}


//--------------------------------------------------------------
string processTweet::debugData(){
    string data =
    "processing " + ofToString(processing)
    +"\nlastTimeQuery " + ofToString(lastTimeQuery)
    +"\nposicionesGrid.size() "+ ofToString(posicionesGrid.size())
    +"\ncontadorMiniaturas " + ofToString(contadorMiniaturas)
    +"\nceldas ocupadas "+ofToString(celdasOcupadas)
    +"\nceldas libres "+ofToString(dabatase.countFreeCells());

    return data;
}

//--------------------------------------------------------------
void processTweet::updateProcess(){
    
    // no esta currando y hace mas de 1 segundo desde la ultima vez
    if (!processing && (ofGetElapsedTimeMillis()-lastTimeQuery > 2000)) {
        
        getLastTweet();
        
    }
}

//--------------------------------------------------------------
void processTweet::processImage(shared_ptr<ofTexture>& tex, int new_w, int new_h){
    
    
    // seleccionas una posicion random en la bd
    
    //getRandomPosition(int cual, int idTwittQueOcupa)
    
    vector<int> cell = dabatase.getRandomPosition();
    
    posiciones pos(cell.at(0), cell.at(1), cell.at(2), cell.at(3));
    lastTweet.posicionEnGrid = pos;
    
    
    bool ok = dabatase.setCellOcupada(lastTweet.recordId,cell.at(4));


    myImage = tex;
    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// RESIZE IMAGE TO PRINT ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ofPixels pxsPrint;
    tex->readToPixels(pxsPrint);

    float wP = pxsPrint.getWidth();
    float hP = pxsPrint.getHeight();

    int destXP = 0;
    int destYP = 0;

    int nuevoAnchoP = anchoPrint;
    int nuevoAltoP = round(nuevoAnchoP*(hP/wP));

    if(nuevoAltoP<altoPrint){
        nuevoAltoP = altoPrint;
        nuevoAnchoP = round(nuevoAltoP*(wP/hP));
    }

    if (nuevoAnchoP == anchoPrint) {
        // sobra alto
        if (nuevoAltoP>altoPrint) {
            destYP = (nuevoAltoP-altoPrint)/2;
        }
    }

    if (nuevoAltoP == altoPrint) {
        // sobra ancho
        if (nuevoAnchoP>anchoPrint) {
            destXP = (nuevoAnchoP-anchoPrint)/2;
        }
    }

    // refacemos el tamano
    pxsPrint.resize(nuevoAnchoP, nuevoAltoP);
    pxsPrint.crop(destXP/2, destYP/2, anchoPrint, altoPrint);


    // volcamos a la imagen en bn
    ofImage thumbPrint;
    thumbPrint.setFromPixels(pxsPrint);
    thumbPrint.setImageType(OF_IMAGE_GRAYSCALE);

    ofEnableAlphaBlending();

    ofPushStyle();
    fboThumbPrint.begin();

        ofSetColor(255,255,255,255);
        // dibujamos la miniatura
        thumb.draw(0,0);

        ofSetColor(255,255,255,150);
        // dibujamos la porcion de la imagen del mosaico
        imgDestinoMosaicoPrint->drawSubsection(0, 0, nuevoAnchoP, nuevoAltoP,lastTweet.posicionEnGrid.x, lastTweet.posicionEnGrid.y);

    fboThumbPrint.end();
    ofPopStyle();

    ofDisableAlphaBlending();

    ofPixels volcadoFinalPrint;

    fboThumbPrint.readToPixels(volcadoFinalPrint);

    // guardamos la imagen pa imprimir
    ofSaveImage(volcadoFinalPrint, "/home/natxo/SocialPrinterTwiiter/imagenes/impresion/" + lastTweet.destUrl);



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// RESIZE IMAGE TO SCREEEN ///////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ofPixels pxs;
    tex->readToPixels(pxs);


    float w = pxs.getWidth();
    float h = pxs.getHeight();
    
    int destX = 0;
    int destY = 0;
    
    int nuevoAncho = new_w;
    int nuevoAlto = round(nuevoAncho*(h/w));
    
    if(nuevoAlto<new_h){
        nuevoAlto = new_h;
        nuevoAncho = round(nuevoAlto*(w/h));
    }
    
    if (nuevoAncho == new_w) {
        // sobra alto
        if (nuevoAlto>new_h) {
            destY = (nuevoAlto-new_h)/2;
        }
    }
    
    if (nuevoAlto == new_h) {
        // sobra ancho
        if (nuevoAncho>new_w) {
            destX = (nuevoAncho-new_w)/2;
        }
    }
    
    pxs.resize(nuevoAncho, nuevoAlto);
    pxs.crop(destX/2, destY/2, new_w, new_h);
    
    
    ofImage thumb;
    thumb.setFromPixels(pxs);
    thumb.setImageType(OF_IMAGE_GRAYSCALE);
    //thumb.setFromPixels(pxs, nuevoAncho, nuevoAlto, OF_IMAGE_GRAYSCALE);
    
    ofEnableAlphaBlending();

    ofPushStyle();
    fboThumb.begin();
    
        ofSetColor(255,255,255,255);
        // dibujamos la miniatura
        thumb.draw(0,0);
    
        ofSetColor(255,255,255,150);
        // dibujamos la porcion de la imagen del mosaico
        imgDestinoMosaico->drawSubsection(0, 0, nuevoAncho, nuevoAlto,lastTweet.posicionEnGrid.x, lastTweet.posicionEnGrid.y);
    
    fboThumb.end();
    ofPopStyle();
    
    ofDisableAlphaBlending();
    
    pantalla.begin();
            ofSetColor(255,255,255,255);
            fboThumb.draw(lastTweet.posicionEnGrid.x, lastTweet.posicionEnGrid.y);
    pantalla.end();
    
    
    contadorMiniaturas++;
    celdasOcupadas++;
    
    processing = false;
    dabatase.checkTweetProcessed(lastTweet.recordId);
    lastTimeQuery = ofGetElapsedTimeMillis();
    


    // guardamos la imagen
    //ofSaveImage(pxs, "/home/natxo/SocialPrinterTwiiter/imagenes/" + lastTweet.destUrl);
}


