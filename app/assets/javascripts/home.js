$(document).ready(function() {

  $("td").on("mousedown",function() {
    if ($(this).attr('data-workout') == 'arms') {
      $(this).find("img").attr('src','/'+$(this).attr('data-workout')+'_cell_dwn.png') 
    }
    $(this).css("background-color","#87d0ea")
  }); 

  $("td").on("mouseup",function() {
    $(this).find("img").attr('src','/cells/'+$(this).attr('data-workout')+'_cell.png')
    $(this).css("background-color","#f7f7f7")
  });

  $("td").on("mouseout",function() {
    $(this).find("img").attr('src','/cells/'+$(this).attr('data-workout')+'_cell.png')
    $(this).css("background-color","#f7f7f7")
  });

  $("td").on("click",function(){
    var callback_url = "/counter?workout="+$(this).attr('data-workout')
    document.location = callback_url
    return false
  })

  $("td a").on("click",function(){
    callback_url = "/counter?workout="+$(this).parents('td').attr('data-workout') 
    document.location = callback_url
    return false
  })

});

