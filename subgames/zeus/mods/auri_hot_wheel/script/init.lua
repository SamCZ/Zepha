_G["hot_wheel"] = {}

if zepha.server then
    require(_PATH .. "register")
end

if zepha.client then
    require(_PATH .. "interface")
    require(_PATH .. "keys")
end
