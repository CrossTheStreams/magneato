$(document).ready(function() {
  init_count = ensure_reps_count(localStorage["reps_count"]);

  channel = dispatcher.subscribe('rep_channel')

  channel.bind('rep_new', function(data) {    
    console.log('channel event received: ' + data);
    update_count($("#reps"));
  });

});

function update_count (elem) {
  var old_count = parseInt($(elem).text());
  new_count = old_count + 1
  localStorage["reps_count"] = new_count;
  return $(elem).text(new_count.toString());
};

function ensure_reps_count(count) {
  var ret_count;
  if (typeof(count) == "undefined") {
    ret_count = 0; 
  }
  else if (typeof(parseInt(count) == "number")) {
    ret_count = count; 
  }
  $("#reps").text(ret_count.toString()).fadeIn()
  return ret_count;
};
