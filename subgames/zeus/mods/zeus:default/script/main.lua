print("Hello from default")

runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")
runfile(_PATH .. "biomes/_index")

local blockTypes = {
    "@aurailus:tnt:tnt",
    "zeus:default:grass_slab",
    "zeus:kinetic:axle_0",
    "zeus:default:stone",
    "zeus:default:wood",
    "zeus:default:cobblestone",
    "zeus:default:dirt",
    "zeus:default:grass",
    "zeus:default:leaves",
    "zeus:default:sand",
    "zeus:default:sandstone",
    "zeus:default:tallgrass_5"
}

for i, v in ipairs(blockTypes) do
    zepha.register_keybind("zeus:default:block_" .. i, {
        description = "Select Block " .. i,
        default = zepha.keys[tostring(i)],
        on_press = function() zepha.player:set_selected_block(v) end
    });
end

local chat_down = false
zepha.register_keybind("zeus:default:open_chat", {
    description = "Open Chat",
    default = zepha.keys.t,
    on_press = function() print "Opened chat!" end
})