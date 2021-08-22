local menu = zepha.gui(function()
    return Gui.Box {
        background = 'zeus_background_christmas_night',

        Gui.Box {
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

local sizes = {}
local positions = {}
for _ = 1, 100 do
    table.insert(sizes, 6 + math.random() * 4)
    table.insert(positions, { math.floor(math.random() * 640), math.floor(math.random() * 320) })
end

local particle_wrap = menu:get(1)
for i, pos in ipairs(positions) do
    particle_wrap:append(zepha.Gui.Box {
        pos = pos,
        background = 'particle_dark',
        size = sizes[i]
    })
end

local tick = 0
zepha.after(function()
    tick = tick + 0.016
    for i, pos in ipairs(positions) do
        local part = particle_wrap:get(i)

        pos[1] = pos[1] + (-math.sin(tick) * 0.0125 - 0.0025) * sizes[i] * 3
        pos[2] = pos[2] + 0.05 * sizes[i]
        if pos[1] < -12 then pos[1] = 640 end
        if pos[2] > 320 then pos[2] = -12 end

        part.pos = pos
    end

    return true
end, 0.016)

zepha.set_gui(menu)