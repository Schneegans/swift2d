function flicker(elem, on, flicCount) {
  on = !on;

  if (flicCount == 0) {
    flicCount =  Math.floor(Math.random()*10)*2+1;
  }
  flicCount--;

  var time;
  if (on) {
    elem.addClass("light-on");
    if (flicCount > 0) {
      time = Math.floor((Math.random()*50) + 1);
    } else {
      time = Math.floor((Math.random()*10000) + 1);
    }
  } else {
    elem.removeClass("light-on");
    if (flicCount > 0) {
      time = Math.floor((Math.random()*10) + 1);
    } else {
      time = Math.floor((Math.random()*2000) + 1);
    }
  }
  setTimeout(function() { flicker(elem, on, flicCount); }, time);
}

(function( $ ) {

  $.fn.flicker = function() {
    return this.each(function() {
      var on = true;
      var flicCount = 1;
      console.log("huh");
      flicker($(this), on, flicCount);
    });
  };

}( jQuery ));
