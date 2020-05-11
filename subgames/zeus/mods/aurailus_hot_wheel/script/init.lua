if zepha.server then
    runfile(_PATH .. "register")
end

if zepha.client then
    _G["aurailus_hot_wheel"] = {}

    runfile(_PATH .. "ui")
    runfile(_PATH .. "keys")
end
