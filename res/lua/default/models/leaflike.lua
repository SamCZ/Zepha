--
-- Cube model with extra inner faces that pop out of the model.
-- Useful for making bushy leaves.
-- Texture order is: cube textures, pop textures.
--

local amp = 0.2

zeus.register_blockmodel("default:leaflike", {
    parts = {
        {
            face = "left",
            tex = 1,
            points = {
                0, 0, 0, 0, 1,
                0, 0, 1, 1, 1,
                0, 1, 1, 1, 0,
                0, 1, 0, 0, 0
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }, {
            face = "right",
            tex = 1,
            points = {
                1, 1, 1, 1, 0,
                1, 0, 1, 1, 1,
                1, 0, 0, 0, 1,
                1, 1, 0, 0, 0
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }, {
            face = "top",
            tex = 1,
            points = {
                0, 1, 0, 0, 0,
                0, 1, 1, 0, 1,
                1, 1, 1, 1, 1,
                1, 1, 0, 1, 0
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }, {
            face = "bottom",
            tex = 1,
            points = {
                0, 0, 0, 0, 0,
                1, 0, 0, 1, 0,
                1, 0, 1, 1, 1,
                0, 0, 1, 0, 1
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }, {
            face = "front",
            tex = 1,
            points = {
                0, 0, 1, 0, 1,
                1, 0, 1, 1, 1,
                1, 1, 1, 1, 0,
                0, 1, 1, 0, 0
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }, {
            face = "back",
            tex = 1,
            points = {
                0, 0, 0, 0, 1,
                0, 1, 0, 0, 0,
                1, 1, 0, 1, 0,
                1, 0, 0, 1, 1
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }, {
            face = "nocull",
            tex = 2,
            points = {
                -0.31, 1.30, -0.3, 0, 0,
                -0.31, -0.30, -0.31, 0, 1,
                1.3, -0.30, 1.3, 1, 1,
                1.3, 1.30, 1.29, 1, 0,

                1.3, -0.30, 1.3, 1, 1,
                -0.31, -0.30, -0.31, 0, 1,
                -0.31, 1.30, -0.3, 0, 0,
                1.3, 1.30, 1.29, 1, 0,

                -0.31, 1.30, 1.29, 0, 0,
                -0.31, -0.30, 1.3, 0, 1,
                1.3, -0.30, -0.31, 1, 1,
                1.3, 1.30, -0.3, 1, 0,

                1.3, -0.30, -0.31, 1, 1,
                -0.31, -0.30, 1.3, 0, 1,
                -0.31, 1.30, 1.29, 0, 0,
                1.3, 1.30, -0.3, 1, 0
            },
            shader_mod = {
                type = "sway_full_block",
                amplitude = amp
            }
        }
    }
})