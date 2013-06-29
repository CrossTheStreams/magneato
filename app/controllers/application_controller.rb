class ApplicationController < ActionController::Base
  protect_from_forgery
  include Mobylette::RespondToMobileRequests

  mobylette_config do |config|
    config[:fallback_chains] = { 
      mobile: [:mobile, :html],
    }
  end 

end
