class CounterController < ApplicationController

  def index
       
    @workout = params[:workout]
    @workout = "Arms" if @workout.blank?

  end

  
  def ping_rep

    WebsocketRails[:rep_channel].trigger(:rep_new, {:ping => true})  
    
    render :text => "Yo dawg"

  end

  def clear_reps
    
  end


end
