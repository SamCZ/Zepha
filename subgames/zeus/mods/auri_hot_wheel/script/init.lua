_G["hot_wheel"] = {}

if zepha.server then
    runfile(_PATH .. "register")
end

if zepha.client then
    runfile(_PATH .. "interface")
    runfile(_PATH .. "keys")
end
