print("Hello from default")

runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")

vector = {}

vector.new = function(x, y, z)
    if y == nil then return {x = x[1], y = x[2], z = x[3]} end
    return {x = x, y = y, z = z}
end

v = vector.new

local table = v{1, 2, 3}
table2 = v(4, 5, 6)

print(dump(table))
print(dump(table2))

local blockTypes = {
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
    zepha.register_keybind("block_" .. i, {
        description = "Select Block " .. i,
        default = zepha.keys[tostring(i)],
        on_press = function()
            print("Setting block to " .. i)
            zepha.player:set_selected_block(v)
        end
    });
end

-- zepha.register_keybind("open_inventory", {
--     description = "Open Inventory",
--     default = zepha.keys.e,
--     on_press = function()
--         if zepha.player.menu_state == "" then
--             zepha.show_menu("inventory", {
--                 key = "form_root",
--                 type = "rect",
--
--                 size = "1920px, 1080px",
--                 background = "#0004",
--
--                 children = {{
--                     key = "inv_root",
--                     type = "rect",
--
--                     position = "202px, 138px",
--                     size = "236px, 118px",
--                     padding = "20px, 10px, 8px, 10px",
--                     background = "inventory"
--                 }, {
--                     key = "hotwheel_root",
--                     type = "rect",
--
--                     position = "296px, 268px",
--                     size = "52px, 52px",
--                     background = "inventory_wheel"
--                 } }
--             });
--         else
--             zepha.close_menu()
--         end
-- --         print(zepha.player.menu_state)
-- --         zepha.show_menu([[
-- --             rect[form_root]
-- --                 size=1920px 1080px
-- --                 background=#0004
-- --
-- --                 rect[inv_root]
-- --                     position=202px 138px
-- --                     size=236px 138px
-- --                     padding=20px 10px 8px 10px
-- --                     background=img("inventory")
-- --                 end
-- --
-- --                 rect[hotwheel_root]
-- --                     position=296px 268px
-- --                     size=52px 52px
-- --                     background=img("inventory_wheel")
-- --                 end
-- --             end
-- --         ]]);
--     end
-- })
--
-- local chat_down = false
-- zepha.register_keybind("open_chat", {
--     description = "Open Chat",
--     default = zepha.keys.t,
--     on_press = function()
--         chat_down = true
--     end,
--     on_release = function()
--         chat_down = false
--     end
-- })
-- zepha.delay(function()
--     if chat_down then
--         local pos = zepha.player.pos
--         pos.y = pos.y - 1
--         zepha.set_block(pos, "default:stone")
--     end
--     return true
-- end, 1/20)