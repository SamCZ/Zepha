-- Load Libraries
require(_PATH .. "modules/fenv")

require(_PATH .. "modules/math")
require(_PATH .. "modules/string")

require(_PATH .. "modules/gui")
require(_PATH .. "modules/dump")
require(_PATH .. "modules/table")
require(_PATH .. "modules/after")
require(_PATH .. "modules/vector")
require(_PATH .. "modules/entity")
require(_PATH .. "modules/callbacks")
require(_PATH .. "modules/serialization")

-- Register base models (if not on main menu)
if zepha.client or zepha.server then require(_PATH .. "game/init") end