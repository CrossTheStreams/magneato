# Load the rails application
require File.expand_path('../application', __FILE__)
CFG = YAML.load_file("#{Rails.root.to_s}/config/config.yml")[Rails.env]


# Initialize the rails application
Magneato::Application.initialize!
