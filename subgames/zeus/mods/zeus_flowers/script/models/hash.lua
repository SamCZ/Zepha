--
-- A flower blockmodel that is a short hash similar to wheat in Minecraft,
-- with multiple layers of topfaces.
--

local offset_amp = 0.2
local amp = 0.025

zepha.register_blockmodel('zeus:flowers:hash', {
    mesh_mods = {
        {
            type = 'offset_x',
            amplitude = offset_amp,
        },
        {
            type = 'offset_z',
            amplitude = offset_amp,
        }
    },
    parts = {
        {
            face = 'nocull',
            tex = 4,
            points = {
                4/16, 0, 0, 0, 1,
                4/16, 0, 1, 1, 1,
                4/16, 0.5, 1, 1, 0,
                4/16, 0.5, 0, 0, 0
            },
            shader_mod = {
                type = 'sway_attached',
                amplitude = amp
            }
        }, {
            face = 'nocull',
            tex = 5,
            points = {
                12/16, 0.5, 1, 0, 0,
                12/16, 0, 1, 0, 1,
                12/16, 0, 0, 1, 1,
                12/16, 0.5, 0, 1, 0
            },
            shader_mod = {
                type = 'sway_attached',
                amplitude = amp
            }
        }, {
            face = 'nocull',
            tex = 5,
            points = {
                0, 0, 12/16, 0, 1,
                1, 0, 12/16, 1, 1,
                1, 0.5, 12/16, 1, 0,
                0, 0.5, 12/16, 0, 0
            },
            shader_mod = {
                type = 'sway_attached',
                amplitude = amp
            }
        }, {
            face = 'nocull',
            tex = 6,
            points = {
                0, 0, 4/16, 1, 1,
                0, 0.5, 4/16, 1, 0,
                1, 0.5, 4/16, 0, 0,
                1, 0, 4/16, 0, 1
            },
            shader_mod = {
                type = 'sway_attached',
                amplitude = amp
            }
        }, {
            face = 'nocull',
            tex = 1,
            points = {
                0, 4/16, 0, 0, 0,
                0, 4/16, 1, 0, 1,
                1, 4/16, 1, 1, 1,
                1, 4/16, 0, 1, 0
            },
            shader_mod = {
                type = 'sway_full_block',
                amplitude = amp
            }
        },
        {
            face = 'nocull',
            tex = 2,
            points = {
                0, 3/16, 0, 0, 0,
                0, 3/16, 1, 0, 1,
                1, 3/16, 1, 1, 1,
                1, 3/16, 0, 1, 0
            },
            shader_mod = {
                type = 'sway_full_block',
                amplitude = amp
             }
        },
        {
            face = 'nocull',
            tex = 3,
            points = {
                0, 2/16, 0, 0, 0,
                0, 2/16, 1, 0, 1,
                1, 2/16, 1, 1, 1,
                1, 2/16, 0, 1, 0
            },
            shader_mod = {
                type = 'sway_full_block',
                amplitude = amp
            }
        }
    }
})