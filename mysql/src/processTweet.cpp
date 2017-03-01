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

    recorder.startThread(false, true);
}
//--------------------------------------------------------------
void processTweet::setImageDestinoMosaico(ofImage *img, ofImage *imgPrint){
    imgDestinoMosaico = img;
    imgDestinoMosaicoPrint = imgPrint;
}

//--------------------------------------------------------------
void processTweet::limpiaPosiciones(){
    dabatase.limpiaPosiciones();
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

    printerHack.allocate(anchoPrint+10, altoPrint+10);

    printerHack.begin();
    ofClear(255,0);
    printerHack.end();
}

//--------------------------------------------------------------
ofPoint processTweet::getGridCellSize(){
    return cellSize;
}

//--------------------------------------------------------------
void processTweet::createGridPositions(int columnas, int filas){
    
    int ancho = windowWidth/columnas;
    int alto = windowHeight/filas;

    int anchoP = imgDestinoMosaicoPrint->getWidth()/columnas;
    int altoP = imgDestinoMosaicoPrint->getHeight()/filas;
    
    cellSize.set(ancho, alto);
    

    int cuantas = 0;
    posicionesGrid.clear();

    bool hasCrasheado = false;
    
    for (int r = 0; r < ofGetHeight(); r+=alto ) {
        for (int c = 0; c < ofGetWidth(); c+=ancho ) {

            posiciones posicion(c,r,ancho,alto);
            posicionesGrid.push_back(posicion);

            if(dabatase.existePosicion(cuantas+1)==0){

                //cout << "no existe" << endl;
                dabatase.setCellData(cuantas,c,r,ancho,alto,"no",0);
            }else{
                hasCrasheado = true;
            }
            cuantas++;
        }

    }




    cuantas = 0;

    for (int rP = 0; rP < imgDestinoMosaicoPrint->getHeight(); rP+=altoP ) {
        for (int cP = 0; cP < imgDestinoMosaicoPrint->getWidth(); cP+=anchoP ) {

            dabatase.updateCellData("UPDATE posiciones SET posXprint="+ofToString(cP)+", posYprint="+ofToString(rP)+" WHERE idposiciones = "+ofToString(cuantas+1)+ "");
            cuantas++;
        }
    }


    //+////////////////////////////////////////////////////////////////////////////////+//
    // si habia ya alguna miniatura es porque has reventando, entonces pinta el fbo con la imagen de backup

    if(hasCrasheado){
        ofImage backu;
        backu.load("/home/natxo/SocialPrinterTwiiter/imagenes/backupReinicio/backupFbo.png");

        pantalla.begin();
        //ofClear(0);
        backu.draw(0,0);
        pantalla.end();
    }
}


//--------------------------------------------------------------
vector<posiciones> processTweet::getPosicionesGrid(){
    return posicionesGrid;
}

//--------------------------------------------------------------
void processTweet::updateProcess(){

    // no esta currando y hace mas de 1 segundo desde la ultima vez
    if (!processing && (ofGetElapsedTimeMillis()-lastTimeQuery > 2000)) {
        getLastTweet();
    }
}

//--------------------------------------------------------------
void processTweet::getLastTweet(){
    
    processing = true;
    
    // data
    vector<vector<string> > results = dabatase.getTweet();

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

    //cout << " results.size()" <<  results.size() << endl;
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
void processTweet::processImage(shared_ptr<ofTexture>& tex, int new_w, int new_h){
    
    
    // seleccionas una posicion random en la bd
    
    vector<int> cell = dabatase.getRandomPosition();
    
    posiciones pos(cell.at(0), cell.at(1), cell.at(2), cell.at(3));
    lastTweet.posicionEnGrid = pos;
    
    ofPoint ptoPrint(cell.at(5),cell.at(6));
    
    dabatase.setCellOcupada(lastTweet.recordId,cell.at(4));


    myImage = tex;
    

    // textura
    // posicionXpantalla posicionYpantalla
    // posicionXimpresion posicionYimpresion
    // anchoPantalla altoPantalla
    // anchoImpresion altoImpresion

    // ofTexture

    // ofPoint posicionPantalla
    // ofPoint posicionImpresion

    // ofPoint anchoPantalla
    // ofPoint altoPantalla

    // ofPoint pantallaSize
    // ofPoint imageMosaicPrintSize

    ofPixels pxsPrint;
    tex->readToPixels(pxsPrint);

    ofPixels pxs;
    tex->readToPixels(pxs);

    ofImage thumbPrint;
    ofImage thumb;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// RESIZE IMAGE TO PRINT ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////


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

    thumbPrint.setFromPixels(pxsPrint);
    thumbPrint.setImageType(OF_IMAGE_GRAYSCALE);

    ofEnableAlphaBlending();

    ofPushStyle();
    fboThumbPrint.begin();

        ofSetColor(255,255,255,255);
        // dibujamos la miniatura
        thumbPrint.draw(0,0);

        ofSetColor(255,255,255,150);
        // dibujamos la porcion de la imagen del mosaico

        //imgDestinoMosaicoPrint->drawSubsection(0, 0, nuevoAnchoP, nuevoAltoP,ptoPrint.x, ptoPrint.y);
        imgDestinoMosaicoPrint->drawSubsection(0, 0, nuevoAnchoP, nuevoAltoP, ptoPrint.x, ptoPrint.y);

    fboThumbPrint.end();
    ofPopStyle();

    ofDisableAlphaBlending();


    printerHack.begin();
    ofClear(255,255);
    fboThumbPrint.draw(5,5);
    printerHack.end();

    //ofPixels volcadoFinalPrint;
    printerHack.readToPixels(pxsPrint);

    // guardamos la imagen pa imprimir
    size_t lastindex = lastTweet.destUrl.find_last_of(".");
    string rawname = lastTweet.destUrl.substr(0, lastindex);

    recorder.addFrame(pxsPrint, "/home/natxo/SocialPrinterTwiiter/imagenes/impresion/" + rawname);



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// RESIZE IMAGE TO SCREEEN ///////////////////////////////////////////////////////////////////////////////
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///



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
    
    

    thumb.setFromPixels(pxs);
    thumb.setImageType(OF_IMAGE_GRAYSCALE);
    //thumb.setFromPixels(pxs, nuevoAncho, nuevoAlto, OF_IMAGE_GRAYSCALE);
    
    ofEnableAlphaBlending();

    ofPushStyle();
    fboThumb.begin();
    
        ofSetColor(255,255,255,255);
        // dibujamos la miniatura
        thumb.draw(0,0);
    
        ofSetColor(255,255,255,120);
        // dibujamos la porcion de la imagen del mosaico
        imgDestinoMosaico->drawSubsection(0, 0, nuevoAncho, nuevoAlto,lastTweet.posicionEnGrid.x, lastTweet.posicionEnGrid.y);
    
    fboThumb.end();
    ofPopStyle();
    
    ofDisableAlphaBlending();


// pintamos el fbo de la pantalla
    pantalla.begin();
            ofSetColor(255,255,255,255);
            fboThumb.draw(lastTweet.posicionEnGrid.x, lastTweet.posicionEnGrid.y);
    pantalla.end();
    

    //  guardamos el fbo de la pantalla por seascaso
    ofPixels pxsPantalla;
    pantalla.readToPixels(pxsPantalla);
    pxsPantalla.setNumChannels(3);
    ofSaveImage(pxsPantalla, "/home/natxo/SocialPrinterTwiiter/imagenes/backupReinicio/backupFbo.png");

    
    contadorMiniaturas++;
    celdasOcupadas++;
    


    processing = false;
    dabatase.checkTweetProcessed(lastTweet.recordId);
    lastTimeQuery = ofGetElapsedTimeMillis();
    


    // guardamos la imagen
    //ofSaveImage(pxs, "/home/natxo/SocialPrinterTwiiter/imagenes/" + lastTweet.destUrl);
}


