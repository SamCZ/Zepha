for i = 1, 5, 1 do
    zepha.register_block(":tall_grass_" .. i, {
        name = "Tall Grass",

        culls = false,
        solid = false,
        model = "base:cross_plant",
        textures = { "tint(0, zeus:default:tallgrass_"..i..")" },
        far_render = false,

        selection_box = {{1/16, 0, 1/16, 15/16, 0.30 + i * 0.1, 15/16}},

        tool_props = {
            health = 5,
            multipliers = {
                snap = 1.8,
                grab = 1.2,
                _other = 0.35
            }
        },

        light_propagates = true,

        yields = function(pos)
            if math.random() > 0.8 then return "zeus:materials:plant_fibre" end
        end
    })
end