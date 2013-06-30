$(document).ready(function() {
  init_count = ensure_reps_count(localStorage["reps_count"]);

  channel = dispatcher.subscribe('rep_channel')

  channel.bind('rep_new', function(data) {    
    console.log('channel event received: ' + data);
    update_count($("#reps"));
  });

  $("#Exercise").on("click",function() {
    toggle_drop_down($(this).siblings("ul")); 
  });

  $("#Weight").on("click",function() {
    toggle_drop_down($(this).siblings("ul"));
  });


});

function toggle_drop_down (elem) {
  var drop_down = $(elem),
  drop_open = drop_down.attr('data-open'),
  opac_set,
  top_set
  if (drop_open == "true") {
   opac_set = 0;
   top_set = 90;  
   drop_down.attr('data-open','false');
  }
  else {
   drop_down.css("top","90")
   opac_set = 1;
   top_set = 120;
   drop_down.attr('data-open','true');
  }
  drop_down.animate({
    "opacity":opac_set,
    "top":top_set
  },250,function(){
     if (drop_open == "true") {
       drop_down.css("top","-1500px") 
     }
  }); 
}


function update_count (elem) {
  var old_count = parseInt($(elem).text());
  new_count = old_count + 1
  localStorage["reps_count"] = new_count;
  return $(elem).text(new_count.toString());
};

function select_drop_item (selection) {

}

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
