local menu = zepha.gui(function()
    return Gui.Box {
        background = 'zeus_background_christmas_night',

        Gui.Box {
            id = 'particle_wrap',
            size = { '100cw', '100ch' }
        },

        Gui.Box {
            gap = 4,
            padding = 8,
            size = { '102dp', '100ch' },
            pos = { '20cw - 50sw', 0 },

            background = '#0135',

            Gui.Box {
                size = { '', '30dp' },
                margin = { 0, 0, 0, '8dp' },

                background = 'zeus_logo'
            },

            Gui.Box {
--                 callbacks = {
--                     primary = function() zepha.start_game_local() end
--                 },

                padding = '5.5dp',
                size = { '', '20dp' },

                cursor = 'pointer',
                background = 'crop(0, 0, 90, 20, zeus_button)',
                background_hover = 'crop(0, 20, 90, 20, zeus_button)',

                Gui.Text {
                    content = 'Local Play'
                }
            },

            Gui.Box {
--                 callbacks = {
--                     primary = function() zepha.start_game() end
--                 },

                padding = '5.5dp',
                size = { '', '20dp' },

                cursor = 'pointer',
                background = 'crop(0, 0, 90, 20, zeus_button)',
                background_hover = 'crop(0, 20, 90, 20, zeus_button)',

                Gui.Text {
                    content = 'Browse Servers'
                }
            }
        }
    }
end)

local particle_count = 100
local particle_wrap = menu:get('particle_wrap')
zepha.gui(function()
    for i = 1, particle_count do
        particle_wrap:append(Gui.Box {
            background = 'particle_dark',

            size = 3 + math.random() * 8 .. "dp",
            pos = { math.floor(math.random() * 640) .. "dp", math.floor(math.random() * 320) .. "dp" }
        })
    end
end)

local tick = 0
zepha.after(function()
    tick = tick + 0.016
    for i = 1, particle_count do
        local part = particle_wrap:get(i)
        local pos = part.pos
        local size = part.size[1]

        pos[1] = pos[1] + (-math.sin(tick) * 0.0125 - 0.0025) * size * 3
        pos[2] = pos[2] + 0.05 * size
        if pos[1] < -36 then pos[1] = 640 * 3 end
        if pos[2] > 320 * 3 then pos[2] = -36 end

        part.pos = pos
    end

    return true
end, 0.016)

zepha.set_gui(menu)