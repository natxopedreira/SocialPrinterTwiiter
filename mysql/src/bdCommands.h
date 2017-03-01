//
//  bdCommands.h
//  mysql
//
//  Created by Natxo Pedreira gonzalez on 14/2/17.
//
//

#ifndef mysql_bdCommands_h
#define mysql_bdCommands_h

#include "ofMain.h"
#include "ofxMySQL.h"

class bdCommands {

private:
    
    ofxMySQL db;
    
    vector<string> camposDb;
    vector<string> camposPosiciones;
    
    vector<string> camposPreferencias;
    
public:
    
    bdCommands(){

        camposPreferencias.push_back("screen_rows");
        camposPreferencias.push_back("screen_cols");
        camposPreferencias.push_back("screen_file");
        camposPreferencias.push_back("cell_print_width");
        camposPreferencias.push_back("cell_print_height");
        camposPreferencias.push_back("print_file");
        
        camposDb.push_back("idtwits");
        camposDb.push_back("twitId");
        camposDb.push_back("urlPhoto");
        camposDb.push_back("destPhoto");
        camposDb.push_back("procesado");
        
        camposPosiciones.push_back("idposiciones");
        camposPosiciones.push_back("id");
        camposPosiciones.push_back("posx");
        camposPosiciones.push_back("posy");
        camposPosiciones.push_back("ancho");
        camposPosiciones.push_back("alto");
        camposPosiciones.push_back("ocupado");
        camposPosiciones.push_back("twitt");
        camposPosiciones.push_back("posXprint");
        camposPosiciones.push_back("posYprint");
    
    }
    
    
    //--------------------------------------------------------------
    // abre la conexion con la base de datos
    void openDbConnection(){
        db.ofxMySQL::connect("127.0.0.1", "root", "3mpathy", "socialPrinter");
    }
    
    
    //--------------------------------------------------------------
    // abre la conexion con la base de datos
    void closeDbConnection(){
        db.ofxMySQL::close();
    }
    
    void limpiaPosiciones(){
        openDbConnection();
            db.ofxMySQL::query("truncate socialPrinter.posiciones");
        closeDbConnection();
    }
    
    //--------------------------------------------------------------
    // comprueba si hay posiciones creadas
    bool chekIfExitstPositionCells(int posicion){
        
        bool position = false;
        vector<vector<string> > pos;
        
        db.ofxMySQL::getStrings(pos, "socialPrinter.posiciones", camposPosiciones," where idposiciones='"+ofToString(posicion)+"' ","");
        
        if (pos.size()>0) {
            position = true;
        }
        

        //cout << pos.size() << endl;
        return position;
    }

    //--------------------------------------------------------------
    int existePosicion(int posicion){
        int salida = 0;
        openDbConnection();
        salida =  db.ofxMySQL::queryCuantasHay("SELECT * from posiciones where idposiciones = '"+ofToString(posicion)+"'");
        closeDbConnection();
        return salida;
    }

    //--------------------------------------------------------------
    // crea cell
    void setCellData(int cuantas, int c, int r, int ancho, int alto, string ocupada="no", int idVoid= 0){
        openDbConnection();
        
        string cadena = "INSERT INTO posiciones (id,posx,posy,ancho,alto,ocupado,twitt) VALUES ("
        +ofToString(cuantas)+","
        +ofToString(c)+","
        +ofToString(r)+","
        +ofToString(ancho)+","
        +ofToString(alto)+","
        +"'no',"
        +ofToString(idVoid)+")";
        
        db.ofxMySQL::query(cadena);
        
        closeDbConnection();
    }
    
    void updateCellData(string _query){
        openDbConnection();

        db.ofxMySQL::query(_query);

        closeDbConnection();
    }


    vector<vector<string>> getPreferences(){
        openDbConnection();

        vector<vector<string> > results;
        db.ofxMySQL::getStrings(results, "preferences", camposPreferencias, " ", " LIMIT 1");

        closeDbConnection();

        return results;
    }


    //--------------------------------------------------------------
    // busca un tweet
    vector<vector<string>> getTweet(){
        
        openDbConnection();

        vector<vector<string> > results;
        db.ofxMySQL::getStrings(results, "twits", camposDb, "WHERE procesado='no' ", " ORDER BY idtwits DESC LIMIT 1");
        
        return results;
    }
    
    
    //--------------------------------------------------------------
    // mira si hay celdas libres
    bool checkForFreeCells(){
        
        bool b = false;
        vector<vector<string> > resultsPosiciones;
        db.ofxMySQL::getStrings(resultsPosiciones, "posiciones", camposPosiciones, "WHERE ocupado='no' ", "");
        
        closeDbConnection();
        
        if (resultsPosiciones.size()>0) {
            b = true;
        }
        
        return b;
    }
    
    
    //--------------------------------------------------------------
    // marca el tweet como procesado
    void checkTweetProcessed(int cual){
        
        openDbConnection();
            db.ofxMySQL::query("UPDATE twits SET procesado='si' WHERE idtwits="+ofToString(cual));
        closeDbConnection();
        
    }
    
    //--- cuantas celdas hay libres
    int countFreeCells(){
        vector<vector<string> > resultsPosiciones;
        openDbConnection();
        db.ofxMySQL::getStrings(resultsPosiciones, "posiciones", camposPosiciones, "WHERE ocupado='no' ", "");
        closeDbConnection();
        return resultsPosiciones.size();
    }
    
    //--------------------------------------------------------------
    // seleccionas una posicion random en la bd
    vector<int> getRandomPosition(){
        
        openDbConnection();

        
        vector<int> dataCell;
        vector<vector<string> > posicionPalTweet;
        db.ofxMySQL::getStrings(posicionPalTweet, "posiciones", camposPosiciones, " WHERE ocupado='no' " ," ORDER BY RAND() LIMIT 1");
        
        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(2)));
        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(3)));
        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(4)));
        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(5)));
        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(0)));

        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(8)));
        dataCell.push_back(ofToInt(posicionPalTweet.at(0).at(9)));
        
        
       // closeDbConnection();
        
        
                           /*
        ofVec4f v4(ofToInt(posicionPalTweet.at(0).at(2)),
                   ofToInt(posicionPalTweet.at(0).at(3)),
                   ofToInt(posicionPalTweet.at(0).at(4)),
                   ofToInt(posicionPalTweet.at(0).at(5)));*/
        
        return dataCell;
    }
    
    
    //--------------------------------------------------------------
    // seteamos esa posicion como ocupada
    bool setCellOcupada(int cual, int idTwittQueOcupa){
        
        bool action = db.ofxMySQL::query("UPDATE posiciones SET ocupado='si', twitt="
                           +ofToString(cual)
                           +" WHERE idposiciones="
                           +ofToString(idTwittQueOcupa));
        
       /* cout << "UPDATE posiciones SET ocupado='si', twitt="
        +ofToString(cual)
        +" WHERE idposiciones="
        +ofToString(idTwittQueOcupa) << endl;*/
        
        closeDbConnection();
        
        return action;
    }
    
    
};

#endif
