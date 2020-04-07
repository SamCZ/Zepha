-- Load Libraries
runfile(_PATH .. "dump")
runfile(_PATH .. "math")
runfile(_PATH .. "vector")
runfile(_PATH .. "gui")

-- Register base models (if not on main menu)
if zepha.register_blockmodel then runfile(_PATH .. "models/_index") end