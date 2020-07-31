_G["aurailus_hot_wheel"] = {}

if zepha.server then
    runfile(_PATH .. "register")
end

if zepha.client then
    -- Delay so that we can access player.
    zepha.after(function()
        runfile(_PATH .. "ui")
        runfile(_PATH .. "keys")
    end, 0)
end
