_G["inventory"] = {}

if zepha.server then
    require './register'
else
    require './menu'
    require './chest'
end