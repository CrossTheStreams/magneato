class CounterController < ApplicationController

  def index
      
  end

  
  def ping_rep

    WebsocketRails[:rep_channel].trigger(:rep_new, {:ping => true})  

    render :text => "Yo dawg"

  end


end
