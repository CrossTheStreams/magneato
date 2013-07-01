magneeto_listen = false;

$(document).ready(function() {

  init_count = ensure_reps_count(localStorage["reps_count"]);

  channel = dispatcher.subscribe('rep_channel')

  channel.bind('rep_new', function(data) {    
    console.log('channel event received: ' + data);
    if (magneeto_listen) {
      update_count($("#reps"));   
    } 
  });

  $(document).keydown(function() {
    if (event.which == 189) {
      update_count($("#reps"));  
    }
  });

  $("#back").on("click",function() {
    document.location = "/"
  });

  $("#Exercise").on("click",function() {
    toggle_drop_down($(this).siblings("ul")); 
  });

  $("#Weight").on("click",function() {
    toggle_drop_down($(this).siblings("ul"));
  });

  $("#start-btn").on("click",function() {
    if (magneeto_listen == false) {
      magneeto_listen = true;
      global_highlight("rgb(178, 243, 184)")   
    }
  });

  $("#stop-btn").on("click",function() {
    if (magneeto_listen == true) {
      localStorage["reps_count"] = 0;
      $("#reps").text("0");
      magneeto_listen = false;
      global_highlight("rgb(248, 156, 161)")   
    }
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
   bind_menu_items()
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

function bind_menu_items() {
  $("#Exercise").siblings("ul").children("li").not(".item-0").on("click",function() {
    toggle_drop_down($("#Exercise").siblings("ul"));
    $("li").off("click")
  });
  $("#Weight").siblings("ul").children("li").not(".item-0").on("click",function() {
    toggle_drop_down($("#Weight").siblings("ul"));
    $("li").off("click")
  });
}



function update_count (elem) {
  var old_count = parseInt($(elem).text());
  new_count = old_count + 1
  localStorage["reps_count"] = new_count;
   $(elem).animate({
    "opacity":"0"
    },0,function(){
      $(elem).text(new_count.toString());
      $(elem).animate({
        "opacity":"1" 
      },50,function(){})
   })
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
  if (parseInt(ret_count) > 0) {
    magneeto_listen = true; 
  }
  $("#reps").text(ret_count.toString()).fadeIn()
  return ret_count;
};
