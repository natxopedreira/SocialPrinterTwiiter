'use_strict';

var fs = require('fs');
var express = require('express');
var app = express();
var tw = require('./twitClass.js');
var osc = require("osc");
var WebSocket = require("ws");
var mysql      = require('mysql');
var path = require('path');
var mustacheExpress = require('mustache-express');
var bodyParser = require('body-parser');
var fileUpload = require('express-fileupload');
var mv = require('mv');
var Q = require("q");

var twitter = new tw('eG1KKlVy5oOX3jB3KJ1TmtE7N', // consumer_key
                      'eDJODgk8KsrOOZVnyUgJG0GhYDrl2aFrbrbeOaLRjMiGGGqBrM', // consumer_secret
                      '3200731204-UgjlOzhZE9B2J2nt1H985btjYLO2FMIzRASjX9K', // access_token
                      'XrSF0rg2ptiw9ehqhBS5WNUoRnkmOFV8zoGqzweQQwpZR', // access_token_secret
                      60*1000, // timeout
                      'localhost', // host
                      'root', // user
                      '3mpathy', // pass
                      'socialPrinter'); // dbName


var  dbCon =  mysql.createPool({
      host            : 'localhost',
      user            : 'root',
      password        : '3mpathy',
      database        : 'socialPrinter',
      connectionLimit : 10,               // this is the max number of connections before your pool starts waiting for a release
      multipleStatements : true           // I like this because it helps prevent nested sql statements, it can be buggy though, so be careful
  });


twitter.addStream('zara, moda, fashion, inditex, mango');

twitter.stream.on('tweet', function (tweet) {

      var imagenes = tweet.entities.media;

      if(imagenes){

        for (var i = imagenes.length - 1; i >= 0; i--) {

          var imagenUrl = imagenes[i].media_url;
          var filename = imagenUrl.substring(imagenUrl.lastIndexOf('/')+1);

          var imagenDestino = filename;
          var extension = path.extname(imagenUrl);

          if(extension== ".jpg" || extension== ".jpeg" || extension== ".png"){

            var msgJson = { twitId: tweet.id, urlPhoto: imagenUrl, destPhoto: imagenDestino, procesado:'no' }
          
            storeTwitt(msgJson);
          
          }
          
        }
      }

  }
);


function storeTwitt(msgJson){

    dbCon.getConnection(function (err, conn) {
        if (err)
            return console.log(400);
        // if you got a connection...
        conn.query('INSERT IGNORE INTO socialPrinter.twits SET ?', msgJson, function(err, rows) {
            if(err) {
                conn.release();
                console.log("err " + err)
                return console.log(400, 'Couldnt get a connection');
            }

            console.log("storeTwitt");
            // CLOSE THE CONNECTION
            conn.release();
            
        })
    });
}

//  =============================================================


app.use(bodyParser.urlencoded({ extended: false })) 
app.use(bodyParser.json());

app.engine('html', mustacheExpress());          // register file extension mustache
app.set('view engine', 'html');                 // register file extension for partials
app.set('views', __dirname + '/WebPanelPublic/views');
app.use(express.static(__dirname + '/WebPanelPublic')); // set static folder
app.use(express.static(__dirname + '/WebPanelPublic/uploads')); // set static folder

// configure upload middleware
app.use(fileUpload());


//  =============================================================
// raiz, mostramos las preferencias
app.get('/', function (req, res) {

   dbCon.query('SELECT * FROM preferences', function(err, rows){

    res.render('control', {data : rows[0]});
  });

})


//  =============================================================
// pagina para actualizar las fotos
app.get('/uploadFotos', function (req, res) {
  dbCon.query('SELECT * FROM preferences', function(err, rows){
    res.render('uploadPhotos.html', {data : rows[0]});
  });
})


//  =============================================================
// subimos las iamgenes y actualizamos la db
app.post('/upload', function(req, res) {
  if (!req.files)
    return res.status(400).send('No files were uploaded.');
  

  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file 
  let sampleFile = req.files.imagen_pantalla;
  let sampleFilePrint = req.files.imagen_impresion;
 
  // Use the mv() method to place the file somewhere on your server 
  sampleFile.mv('/home/natxo/SocialPrinterTwiiter/WebPanelPublic/uploads/'+req.files.imagen_pantalla.name, function(err) {
    if (err)
      return res.status(500).send(err);
 
    //res.send('File uploaded!');
  });

  sampleFilePrint.mv('/home/natxo/SocialPrinterTwiiter/WebPanelPublic/uploads/'+req.files.imagen_impresion.name, function(err) {
    if (err)
      return res.status(500).send(err);


    var data = {
      screen_file: req.files.imagen_pantalla.name,
      print_file: req.files.imagen_impresion.name
    }

    dbCon.query('UPDATE preferences SET ? WHERE idpreferences=?', [data, 1], function(error) {

        if (error) {
            res.send('error');
            //console.log(error.message);
        } else {
            res.render('uploadPhotosOK');
            //console.log('success');    
        }
    });


  });

});


//  =============================================================
// actualizamos las preferencias
app.post('/updatePreferences',function(req, res){

    var idUpdate = req.body.idpreferencias;
    var idpreferences = {idpreferences:idUpdate};

    delete req.body.idpreferencias;

    dbCon.query('UPDATE preferences SET ? WHERE idpreferences=?', [req.body, idpreferences.idpreferences], function(error) {
        if (error) {
            res.send('error');
        } else {
            res.send('success');  
        }
    });

})



//  =============================================================
// estadisticas
app.get('/chartsData', function (req, res) {
  var dataJson=[];


  function cuantosTweets(){
    var defered = Q.defer();

    dbCon.query("SELECT count(*) as total from twits",defered.makeNodeResolver());
    return defered.promise;
  }

  function cuantosProcesados(){
    var defered = Q.defer();

    dbCon.query("SELECT count(*) as totalProcesado from twits WHERE procesado='si' ",defered.makeNodeResolver());
    return defered.promise;
  }

  function cuantasCasillas(){
    var defered = Q.defer();

    dbCon.query("SELECT count(*) as casillas from posiciones ",defered.makeNodeResolver());
    return defered.promise;

  }

  function cuantasCasillasLibres(){
    var defered = Q.defer();

    dbCon.query("SELECT count(*) as casillasLibres from posiciones WHERE ocupado = 'no'",defered.makeNodeResolver());
    return defered.promise;
    
  }


    Q.all([cuantosTweets(),cuantosProcesados(),cuantasCasillas(),cuantasCasillasLibres()]).then(function(results){
        console.log(results[0][0][0]);
        console.log(results[1][0][0]);

        if(results[0][0][0]){
          dataJson.push({'total':results[0][0][0].total});
        }

        if(results[1][0][0]){
          dataJson.push({'totalProcesado':results[1][0][0].totalProcesado});
        }   

        if(results[2][0][0]){
          dataJson.push({'casillas':results[2][0][0].casillas});
        } 

        if(results[3][0][0]){
          dataJson.push({'casillasLibres':results[3][0][0].casillasLibres});
        } 

        res.render('stadisticas.html',  {data : dataJson[0], totalProcesado: dataJson[1], casillas: dataJson[2], casillasLibres: dataJson[3], ocupadas: dataJson[2]-dataJson[3]} );
        //res.send(JSON.stringify(dataJson));
        // Hint : your third query would go here
    });



});

//  =============================================================
// iniciamos el webserver
var server = app.listen(8081, function () {
   var host = server.address().address
   var port = server.address().port
   
   console.log("Example app listening at http://%s:%s", host, port)
})

