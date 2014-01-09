(function(Pebble, window) {
  var settings = {};

  Pebble.addEventListener("ready", function(e) {
    settings = window.localStorage.getItem("settings");
    if(settings !== '') {
      var options = JSON.parse(settings);
      Pebble.sendAppMessage(options);
    }
  });

  Pebble.addEventListener("showConfiguration", function() {
    settings = window.localStorage.getItem("settings");
    if(!settings) {
      settings = {};
    }
    Pebble.openURL("http://rexmac.com/projects/pebble/chronocode/settings.html#" + encodeURIComponent(JSON.stringify(settings)));
  });

  Pebble.addEventListener("webviewclosed", function(e) {
    var options = JSON.parse(decodeURIComponent(e.response));
    if(Object.keys(options).length > 0) {
      window.localStorage.setItem("settings", JSON.stringify(options));
      Pebble.sendAppMessage(options);
    }
  })
})(Pebble, window);

