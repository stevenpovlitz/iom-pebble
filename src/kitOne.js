// Adding Some PebbleKit JavaScript to My Watchapp

// runs once app is started, could be before C code runs
Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);

var numRecieved = 0;

// will run callback function every time an app message is recieved
Pebble.addEventListener('appmessage',
  function(e) {
    numRecieved++;
    console.log("Mesage # " + numRecieved);
    console.log('Received message: ' + JSON.stringify(e.payload));
    
    //deparse the integer into its 3 parts, log them (then shove em to firebase)
    
  }
);

