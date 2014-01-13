(function(Pebble, window) {
  var settings = {};

  Pebble.addEventListener("ready", function(e) {
    settings = window.localStorage.getItem("chronocode-settings");
    if(settings !== "") {
      var options = JSON.parse(settings);
      Pebble.sendAppMessage(options);
    }
  });

  Pebble.addEventListener("showConfiguration", function() {
    settings = window.localStorage.getItem("chronocode-settings");
    if(!settings) {
      settings = "{}";
    }
    Pebble.openURL("https://s3.amazonaws.com/pebble.rexmac.com/chronocode/settings.html?v=2-1-1#" + encodeURIComponent(JSON.stringify(settings)));
  });

  Pebble.addEventListener("webviewclosed", function(e) {
    var rt = typeof e.response,
        options = (rt === "undefined" ? {} : JSON.parse(decodeURIComponent(e.response)));
    if(Object.keys(options).length > 0) {
      window.localStorage.setItem("chronocode-settings", JSON.stringify(options));
      Pebble.sendAppMessage(options);
    }
  })
})(Pebble, window);

