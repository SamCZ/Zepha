-- Load Libraries
runfile(_PATH .. "modules/fenv")

runfile(_PATH .. "modules/gui")
runfile(_PATH .. "modules/dump")
runfile(_PATH .. "modules/math")
runfile(_PATH .. "modules/table")
runfile(_PATH .. "modules/after")
runfile(_PATH .. "modules/vector")
runfile(_PATH .. "modules/entity")
runfile(_PATH .. "modules/callbacks")

-- Register base models (if not on main menu)
if zepha.client or zepha.server then runfile(_PATH .. "game/init") end