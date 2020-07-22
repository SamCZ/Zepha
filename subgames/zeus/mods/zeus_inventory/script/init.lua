_G["inventory"] = {}

if zepha.server then runfile(_PATH .. "register") end
if zepha.client then
    runfile(_PATH .. "menu")
    runfile(_PATH .. "chest")
end