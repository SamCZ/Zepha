require(_PATH .. "blocks/index")
require(_PATH .. "entity/index")

-- Flying toggles
local function toggleFlying()
    zepha.player.flying = not zepha.player.flying
end

zepha.register_keybind("zeus:default:toggle_flying", {
    description = "Toggle Flying",
    default = zepha.keys.f,
    on_press = toggleFlying
})

local last_press = -1
zepha.register_keybind("zeus:default:double_jump_fly", {
    description = "Double Jump to Toggle Flying",
    default = zepha.keys.space,
    on_press = function()
        local press = zepha.time.s();
        if press - last_press < 0.25 then toggleFlying() end
        last_press = press;
    end
})