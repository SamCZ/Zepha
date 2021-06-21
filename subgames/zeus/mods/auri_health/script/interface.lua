if not zepha.client then return end

local hud = zepha.player:get_hud()

health.internal._wrapper = zepha.build_gui(function()
    return Gui.Rect {
        key = 'health_wrapper'
    }
end)

function health.internal.update()
    local hp = health.my_health
    health.internal._width = hp.max * 8 + 1

    health.internal._wrapper:remove('@health:component')
    health.internal._wrapper:append(function()
        local elem = Gui.Rect {
            key = '@health:component',
            size = { health.internal._width, 9 },
        }

        -- Background
        for i = 1, hp.max do
            elem:append(Gui.Rect {
                size = { 9, 9 },
                position = { 8 * (i - 1), 0 },
                background = 'crop(0, 0, 9, 9, @auri:health:hearts)'
            })
        end

        local red_start, red_end = 0, math.ceil(hp.health * 2)
        local blue_start, blue_end = red_end, red_end + math.ceil(hp.buffer * 2)

        for i = 1, math.ceil(hp.max * 2) do
            local start = i % 2
            if i > red_start and i <= red_end then
                local segment = start ~= 0 and 9 or 18
                elem:append(Gui.Rect {
                    size = { 9, 9 },
                    position = { 8 * math.floor((i - 1) / 2), 0 },
                    background = 'crop(9, ' .. segment .. ', 9, 9, @auri:health:hearts)'
                })
            elseif i > blue_start and i <= blue_end then
                local segment = start ~= 0 and 9 or 18
                elem:append(Gui.Rect {
                    size = { 9, 9 },
                    position = { 8 * math.floor((i - 1) / 2), 0 },
                    background = 'crop(18, ' .. segment .. ', 9, 9, @auri:health:hearts)'
                })
            end
        end

        return elem
    end)
end

function health.render_default(render)
    hud:remove('@health:default')
    if render then
        hud:append(function()
            return Gui.Rect {
                key = '@health:default',

                size = { health.internal._width, 9 },
                position = { '50%', '100%' },
                position_anchor = { '50%', '200%' },

                health.internal._wrapper
            }
        end)
    end
end

function health.get_element()
    return health.internal._wrapper
end