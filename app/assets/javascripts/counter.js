$(document).ready(function() {

  channel = dispatcher.subscribe('rep_channel')

  channel.bind('rep_new', function(data) {    
    console.log('channel event received: ' + data);
    update_count($("#reps"));
  });

});

function update_count (elem) {
  var init_count = parseInt($(elem).text());
  new_count = init_count + 1
  return $(elem).text(new_count.toString());
}

