local menu = zepha.build_gui(function()
    return Gui.Body {
        background = "zeus_background_christmas_night",

        Gui.Rect {
            key = "particle_wrap",
            size = { pc(100), pc(100) }
        },

        Gui.Rect {
            key = "sidebar",
            position = { pc(20), 0 },
            position_anchor = { pc(50), 0 },
            size = { 102, pc(100) },
            background = "#0135",

            Gui.Rect {
                key = "logo",
                position = { 8, 8 },
                size = { 86, 30 },
                background = "zeus_logo"
            },

            Gui.Button {
                key = "buttonPlay",

                callbacks = {
                    primary = function() zepha.start_game_local() end
                },

                position = { 6, 50 },
                size = { 90, 20 },
                background = "crop(0, 0, 90, 20, zeus_button)",
                background_hover = "crop(0, 20, 90, 20, zeus_button)",
                content = "Local Play"
            },

            Gui.Button {
                key = "buttonServers",

                callbacks = {
                    primary = function() zepha.start_game() end
                },

                position = { 6, 74 },
                size = { 90, 20 },
                background = "crop(0, 0, 90, 20, zeus_button)",
                background_hover = "crop(0, 20, 90, 20, zeus_button)",
                content = "Browse Servers"
            }
        }
    }
end)

-- zepha.delay(function()
--     menu:remove("buttonPlay")
--     menu:remove("buttonServers")
--     menu:get("sidebar"):append(function(m) return Gui.Text({
--         position = {16, 120 },
--         color = "#f00",
--         content = "YOU LOSE" }
--     ) end)
-- end, 1)

local particle_wrap = menu:get("particle_wrap")
menu(function()
    for _ = 1, 20 do
        local scale = 6 + math.random() * 4
        particle_wrap:append(Gui.Rect {
            position = { math.floor(math.random() * 600), math.floor(math.random() * 320) },
            background = "particle_dark",
            size = { scale, scale }
        })
    end
end)

local tick = 0
zepha.delay(function()
    local i = 1
    local part = particle_wrap:get(i)
    tick = tick + 0.012
    while part ~= nil do
        local xO = (-math.sin(tick) * 0.0125 - 0.0025) * part.size[1]
        local pos = { part.position[1] + xO, part.position[2] + 0.05 * part.size[1] }
        if pos[2] > 320 then pos[2] = -12 end
        if pos[1] < -12 then pos[1] = 600 end
        part.position = pos
        i = i + 1
        part = particle_wrap:get(i)
    end
    return true
end, 0.016)



zepha.set_gui(menu)