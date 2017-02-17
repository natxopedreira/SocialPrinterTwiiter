
var Twit = require('twit');
var fs = require('fs');
var file = 'twitter.json';
var mysql      = require('mysql');

var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : '3mpathy',
  database : 'socialPrinter'
});

var T = new Twit({
  consumer_key:         'eG1KKlVy5oOX3jB3KJ1TmtE7N',
  consumer_secret:      'eDJODgk8KsrOOZVnyUgJG0GhYDrl2aFrbrbeOaLRjMiGGGqBrM',
  access_token:         '3200731204-UgjlOzhZE9B2J2nt1H985btjYLO2FMIzRASjX9K',
  access_token_secret:  'XrSF0rg2ptiw9ehqhBS5WNUoRnkmOFV8zoGqzweQQwpZR',
  timeout_ms:           60*1000,  // optional HTTP request timeout to apply to all requests.
})

var stream = T.stream('statuses/filter', { track: 'zara, moda, fashion, inditex, mango' })

// conectamos a twitter para pillar todos los tweets con un hastag
stream.on('tweet', function (tweet) {
  
  // array con las imagenes de cada tweet
  var imagenes = tweet.entities.media;

  // hay imahenes
  if(imagenes){

    	// loop entre todas las imagenes
  	for (var i = imagenes.length - 1; i >= 0; i--) {

  		var imagenUrl = imagenes[i].media_url;
  		var filename = imagenUrl.substring(imagenUrl.lastIndexOf('/')+1);
  		//var imagenDestino = '/home/natxo/SocialPrinterTwiiter/imagenes/'+filename;
      var imagenDestino = filename;

  		var msgJson = {
        twitId: tweet.id,
  			urlPhoto: imagenUrl,
  			destPhoto: imagenDestino,
  			procesado:'no'
  		}

      connection.query('INSERT IGNORE INTO socialPrinter.twits SET ?', msgJson,function(err, results) {
        if(err){
          console.log("err " + err)
        }
      } );

    }
  }
})

