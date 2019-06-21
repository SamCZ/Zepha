--
-- Diagonal cross-shaped model. Has 2 vertical faces that make an X pattern when looked at from above.
-- Useful for representing plants, grass, etc.
-- Only takes one texture, which is displayed on all faces.
--

local amp = 0.6

zeus.register_blockmodel("default:plantlike", {
    {
        face = "nocull",
        tex = 1,
        points = {
            0.1, 0, 0.1, 0, 1,
            0.9, 0, 0.9, 1, 1,
            0.9, 0.9, 0.9, 1, 0,
            0.1, 0.9, 0.1, 0, 0
        },
        shader_mod = {
            type = "sway_grounded",
            amplitude = amp
        }
    }, {
        face = "nocull",
        tex = 1,
        points = {
            0.9, 0.9, 0.9, 1, 0,
            0.9, 0, 0.9, 1, 1,
            0.1, 0, 0.1, 0, 1,
            0.1, 0.9, 0.1, 0, 0
        },
        shader_mod = {
            type = "sway_grounded",
            amplitude = amp
        }
    }, {
        face = "nocull",
        tex = 1,
        points = {
            0.9, 0.9, 0.1, 1, 0,
            0.9, 0, 0.1, 1, 1,
            0.1, 0, 0.9, 0, 1,
            0.1, 0.9, 0.9, 0, 0
        },
        shader_mod = {
            type = "sway_grounded",
            amplitude = amp
        }
    }, {
        face = "nocull",
        tex = 1,
        points = {
            0.1, 0, 0.9, 0, 1,
            0.9, 0, 0.1, 1, 1,
            0.9, 0.9, 0.1, 1, 0,
            0.1, 0.9, 0.9, 0, 0
        },
        shader_mod = {
            type = "sway_grounded",
            amplitude = amp
        }
    }
})