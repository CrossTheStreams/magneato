$(document).ready(function() {
  init_count = parseInt(localStorage["reps_count"]);

  if (typeof(init_count) == "undefined") {
    init_count = 0;
    ensure_reps_count(init_count);
  }
  else {
    ensure_reps_count(init_count) 
  }


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
  $("#reps").text(count.toString()).fadeIn()
};
