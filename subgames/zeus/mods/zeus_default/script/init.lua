runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")

local chat_down = false
zepha.register_keybind("zeus:default:open_chat", {
    description = "Open Chat",
    default = zepha.keys.t,
    on_press = function() print "Opened chat!" end
})

-- Flying toggles
local function toggleFlying()
    zepha.player.flying = not zepha.player.flying
end

zepha.register_keybind("zeus:default:toggle_flying", {
    description = "Toggle Flying",
    default = zepha.keys.f,
    on_press = toggleFlying
})

local last_press = -100
zepha.register_keybind("zeus:default:double_jump_fly", {
    description = "Double Jump to Toggle Flying",
    default = zepha.keys.space,
    on_press = function()
        local press = zepha.time.s();
        if press - last_press < 0.25 then toggleFlying() end
        last_press = press;
    end
})