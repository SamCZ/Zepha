_G["inventory"] = {}

if zepha.server then require(_PATH .. "register") end
if zepha.client then
    require(_PATH .. "menu")
    require(_PATH .. "chest")
end