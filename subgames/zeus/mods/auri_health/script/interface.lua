local hud = zepha.player:get_hud()

health.get_component = function()
    return zepha.build_gui(function()
        local elem = Gui.Rect {
            key = "health_root",
            size = { 10 * 8 + 1, 9 },
        }

        for i = 1, 10 do
            local crop_x = (i < 5 and 0 or i < 8 and 1 or 2) * 9
            elem:append(Gui.Rect {
                size = { 9, 9 },
                position = { 8 * (i - 1), 0 },
                background = "crop(" .. tostring(crop_x) .. ", 0, 9, 9, @auri:health:hearts)"
            })
        end

        return elem
    end)
end

health.default_render = function(render)
    if health._default_elem then hud:remove(health._default_elem) end
    health._default_elem = nil

    if render then
        health._default_elem = zepha.build_gui(function()
            return Gui.Rect {
                size = { 10 * 8 + 1, 9 },
                position = { "50%", "100%" },
                position_anchor = { "50%", "200%" },

                health.get_component()
            }
        end)
        hud:append(health._default_elem)
    end
end