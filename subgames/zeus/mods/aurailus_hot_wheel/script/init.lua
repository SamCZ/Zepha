_G["aurailus_hot_wheel"] = {}

if zepha.server then
    runfile(_PATH .. "register")
end

if zepha.client then
    runfile(_PATH .. "ui")
    runfile(_PATH .. "keys")
end
