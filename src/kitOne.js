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
    console.log('Received message: ' + JSON.stringify(e));

    if (numRecieved % 3 === 1){
      console.log("X: " + JSON.stringify(e.payload));
    }
    else if (numRecieved % 3 === 2){
      console.log("Y: " + JSON.stringify(e.payload));
    }
    else if (numRecieved % 3 === 0){
      console.log("Z: " + JSON.stringify(e.payload));
    } else {console.log("shit's fucked");}
  }
);

