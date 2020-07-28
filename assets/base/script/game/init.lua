runfile(_PATH .. "models/init")
runfile(_PATH .. "player_interact")
runfile(_PATH .. "inventory")
runfile(_PATH .. "tools")

if zepha.player then runfile(_PATH .. "hud") end