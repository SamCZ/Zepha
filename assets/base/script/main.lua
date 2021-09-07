-- Load Libraries
require './modules/fenv'
require './modules/math'
require './modules/string'

require './modules/gui'
require './modules/dump'
require './modules/table'
require './modules/after'
require './modules/vector'
require './modules/entity'
require './modules/callbacks'
require './modules/serialization'

-- Register base models (if not on main menu)
if zepha.client or zepha.server then
    require './game'
end
