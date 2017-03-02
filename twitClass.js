var Twit = require('twit');
//var mysql      = require('mysql');
var util = require('util');
//var EventEmitter = require('events').EventEmitter;

// private
var _consumer_key = 'eG1KKlVy5oOX3jB3KJ1TmtE7N';
var _consumer_secret = 'eDJODgk8KsrOOZVnyUgJG0GhYDrl2aFrbrbeOaLRjMiGGGqBrM';
var _access_token = '3200731204-UgjlOzhZE9B2J2nt1H985btjYLO2FMIzRASjX9K';
var _access_token_secret = 'XrSF0rg2ptiw9ehqhBS5WNUoRnkmOFV8zoGqzweQQwpZR';
var _timeout = 60*1000;
var T = null;
var stream = null;


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------




// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

function twitClass(_consumer_key, _consumer_secret, _access_token, _access_token_secret, _timeout, _host, _user, _pass, _dbName){

	this.defaultMaxListeners = 0

	this.consumer_key = _consumer_key;
	this.consumer_secret = _consumer_secret;
	this.access_token = _access_token;
	this.access_token_secret = _access_token_secret;
	this.timeout = _timeout;

	
	this.twittObejct = new Twit({
	  consumer_key:         this.consumer_key,
	  consumer_secret:      this.consumer_secret,
	  access_token:         this.access_token,
	  access_token_secret:  this.access_token_secret,
	  timeout_ms:           this.timeout,  // optional HTTP request timeout to apply to all requests.
	})


	
}


// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
twitClass.prototype.addStream = function(hastag){

	var self = this;
	this.stream = this.twittObejct.stream('statuses/filter', { track: hastag })

	
}


// ---------------------------------------------------------------------------------------
module.exports = twitClass;