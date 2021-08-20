local menu = zepha.build_gui(function()
    return Gui.Box {
        background = 'zeus_background_christmas_night',

        Gui.Box {
            key = 'particle_wrap',
            size = { '100%', '100%' }
        },

        Gui.Box {
            pos = { '20% - 50s%', 0 },
            size = { 102, '100%' },

            background = '#0135',

            Gui.Box {
                pos = { 8, 8 },
                size = { 86, 30 },

                background = 'zeus_logo'
            },

            Gui.Button {
                id = 'button_play',

--                 callbacks = {
--                     primary = function() zepha.start_game_local() end
--                 },

                pos = { 6, 50 },
                size = { 90, 20 },

                content = 'Local Play',
                background = 'crop(0, 0, 90, 20, zeus_button)',
                background_hover = 'crop(0, 20, 90, 20, zeus_button)'
            },

            Gui.Button {
                id = 'button_servers',

--                 callbacks = {
--                     primary = function() zepha.start_game() end
--                 },

                pos = { 6, 74 },
                size = { 90, 20 },
                content = 'Browse Servers',
                background = 'crop(0, 0, 90, 20, zeus_button)',
                background_hover = 'crop(0, 20, 90, 20, zeus_button)'
            }
        }
    }
end)

-- zepha.after(function()
--     menu:remove("buttonPlay")
--     menu:remove("buttonServers")
--     menu:get("sidebar"):append(function(m) return Gui.Text({
--         position = {16, 120 },
--         color = "#f00",
--         content = "YOU LOSE" }
--     ) end)
-- end, 1)

local particle_wrap = menu:get('particle_wrap')
menu(function()
    for _ = 1, 20 do
        local scale = 6 + math.random() * 4
        particle_wrap:append(Gui.Rect {
            pos = { math.floor(math.random() * 600), math.floor(math.random() * 320) },
            background = 'particle_dark',
            size = { scale, scale }
        })
    end
end)

-- local tick = 0
-- zepha.after(function()
--     local i = 1
--     local part = particle_wrap:get(i)
--     tick = tick + 0.012
--
--     while part ~= nil do
--         local xO = (-math.sin(tick) * 0.0125 - 0.0025) * part.size[1]
--         local pos = { part.pos[1] + xO, part.pos[2] + 0.05 * part.size[1] }
--
--         if pos[2] > 320 then pos[2] = -12 end
--         if pos[1] < -12 then pos[1] = 600 end
--
--         i = i + 1
--         part.pos = pos
--         part = particle_wrap:get(i)
--     end
--
--     return true
-- end, 0.016)

zepha.set_gui(menu)
