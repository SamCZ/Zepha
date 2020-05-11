-- Load Libraries
runfile(_PATH .. "modules/dump")
runfile(_PATH .. "modules/math")
runfile(_PATH .. "modules/vector")
runfile(_PATH .. "modules/gui")

-- Register base models (if not on main menu)
if zepha.register_blockmodel then runfile(_PATH .. "models/_index") end
if zepha.player then runfile(_PATH .. "hud") end