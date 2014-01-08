(function(Pebble, window) {
  var settings = {};

  Pebble.addEventListener("ready", function(e) {
    console.log("JavaScript app ready and running!");
    settings = window.localStorage.getItem("settings");
    console.log("settings: " + settings);
    // Send settings to watch
    if(settings !== '') {
      console.log('Sending local settings to watch.');
      var options = JSON.parse(settings);
      Pebble.sendAppMessage(options, function(e) {
        console.log("Successfully delivered message with transactionId=" + e.data.transactionId);
      }, function(e) {
        console.log("Unable to deliver message with transactionId=" + e.data.transactionId + " Error is: " + e.error.message);
      });
    }
  });

  Pebble.addEventListener("showConfiguration", function() {
    console.log("showing configuration");
    //Pebble.openURL("http://pebble.rexmac.com/pebble-wordsquare/settings.html");
    settings = window.localStorage.getItem("settings");
    console.log("settings: " + settings);
    if(!settings) {
      settings = {};
    }
    console.log("URL: http://rexmac.dev/pws/settings.html#" + encodeURIComponent(JSON.stringify(settings)));
    Pebble.openURL("http://rexmac.dev/pws/settings.html#" + encodeURIComponent(JSON.stringify(settings)));
  });

  Pebble.addEventListener("webviewclosed", function(e) {
    console.log("configuration closed");
    // webview closed
    var options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
    if(Object.keys(options).length > 0) {
      console.log('storing options');
      window.localStorage.setItem("settings", JSON.stringify(options));

      // Send options to watch
      Pebble.sendAppMessage(options, function(e) {
        console.log("Successfully delivered message with transactionId=" + e.data.transactionId);
      }, function(e) {
        console.log("Unable to deliver message with transactionId=" + e.data.transactionId + " Error is: " + e.error.message);
      });
    }
  })
})(Pebble, window);

