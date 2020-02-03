--
-- Cube model with extra side faces that pop out of the model.
-- Useful for making grass, snow, textures that have a raised material on top of them.
-- Texture order is: top, bottom, left, right, front, back, raised.
--

local amp = 0.1

zepha.register_blockmodel("base:block_foliage", {
    parts = {
        {
            face = "left",
            tex = 3,
            points = {
                0, 0, 0, 0, 1,
                0, 0, 1, 1, 1,
                0, 1, 1, 1, 0,
                0, 1, 0, 0, 0
            }
        }, {
            face = "right",
            tex = 3,
            points = {
                1, 1, 1, 0, 0,
                1, 0, 1, 0, 1,
                1, 0, 0, 1, 1,
                1, 1, 0, 1, 0
            }
        }, {
            face = "top",
            tex = 1,
            points = {
                0, 1, 0, 0, 0,
                0, 1, 1, 0, 1,
                1, 1, 1, 1, 1,
                1, 1, 0, 1, 0
            }
        }, {
            face = "bottom",
            tex = 2,
            points = {
                0, 0, 0, 0, 0,
                1, 0, 0, 1, 0,
                1, 0, 1, 1, 1,
                0, 0, 1, 0, 1
            }
        }, {
            face = "front",
            tex = 3,
            points = {
                0, 0, 1, 0, 1,
                1, 0, 1, 1, 1,
                1, 1, 1, 1, 0,
                0, 1, 1, 0, 0
            }
        }, {
            face = "back",
            tex = 3,
            points = {
                0, 0, 0, 1, 1,
                0, 1, 0, 1, 0,
                1, 1, 0, 0, 0,
                1, 0, 0, 0, 1
            }
        }, {
            -- Hovered faces for biome tinting begin here
            face = "left",
            tex = 4,
            points = {
                -0.003, 0, 0, 0, 1,
                -0.003, 0, 1, 1, 1,
                -0.003, 1, 1, 1, 0,
                -0.003, 1, 0, 0, 0
            }
        }, {
            face = "right",
            tex = 4,
            points = {
                1.003, 1, 1, 0, 0,
                1.003, 0, 1, 0, 1,
                1.003, 0, 0, 1, 1,
                1.003, 1, 0, 1, 0
            }
        }, {
            face = "front",
            tex = 4,
            points = {
                0, 0, 1.003, 0, 1,
                1, 0, 1.003, 1, 1,
                1, 1, 1.003, 1, 0,
                0, 1, 1.003, 0, 0
            }
        }, {
            face = "back",
            tex = 4,
            points = {
                0, 0, -0.003, 1, 1,
                0, 1, -0.003, 1, 0,
                1, 1, -0.003, 0, 0,
                1, 0, -0.003, 0, 1
            }
        }, {
            --Floats begin here
            face = "front",
            tex = 5,
            points = {
                0, 1, 1, 0, 0,
                0.005, 0.2, 1.2, 0, 1,
                1.005, 0.2, 1.2, 1, 1,
                1, 1, 1, 1, 0
            },
            shader_mod = {
                type = "sway_attached",
                amplitude = amp
            }
        }, {
            face = "back",
            tex = 5,
            points = {
                1.005, 0.2, -0.2, 0, 1,
                0.005, 0.2, -0.2, 1, 1,
                0, 1, 0, 1, 0,
                1, 1, 0, 0, 0
            },
            shader_mod = {
                type = "sway_attached",
                amplitude = amp
            }
        }, {
            face = "right",
            tex = 5,
            points = {
                1.2, 0.2, 1.005, 0, 1,
                1.2, 0.2, 0.005, 1, 1,
                1, 1, 0, 1, 0,
                1, 1, 1, 0, 0
            },
            shader_mod = {
                type = "sway_attached",
                amplitude = amp
            }
        }, {
            face = "left",
            tex = 5,
            points = {
                0, 1, 0, 0, 0,
                -0.2, 0.2, 0.005, 0, 1,
                -0.2, 0.2, 1.005, 1, 1,
                0, 1, 1, 1, 0
            },
            shader_mod = {
                type = "sway_attached",
                amplitude = amp
            }
        }
    }
})