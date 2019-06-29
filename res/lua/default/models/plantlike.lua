--
-- Diagonal cross-shaped model. Has 2 vertical faces that make an X pattern when looked at from above.
-- Useful for representing plants, grass, etc.
-- Only takes one texture, which is displayed on all faces.
--

local wave_amplitude = 0.6
local offset_amplitude = 0.3

zeus.register_blockmodel("default:plantlike", {
    mesh_mods = {
        {
            type = "offset_x",
            amplitude = offset_amplitude,
        },
        {
            type = "offset_z",
            amplitude = offset_amplitude,
        }
    },
    parts = {
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
                type = "sway_attached",
                amplitude = wave_amplitude
            },
            mesh_mod = {
                type = "offset",
                scale = 0.2
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
                type = "sway_attached",
                amplitude = wave_amplitude
            },
            mesh_mod = {
                type = "offset",
                scale = 0.2
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
                type = "sway_attached",
                amplitude = wave_amplitude
            },
            mesh_mod = {
                type = "offset",
                scale = 0.2
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
                type = "sway_attached",
                amplitude = wave_amplitude
            },
            mesh_mod = {
                type = "offset",
                scale = 0.2
            }
        }
    }
})