local amp_offset_h = 3/16
local amp_offset_v = 1/16
local amp_sway = 0.025

zepha.register_blockmodel('zeus:vegetation:clover', {
    mesh_mods = {
        {
            type = 'offset_x',
            amplitude = amp_offset_h,
        },
        {
            type = 'offset_y',
            amplitude = amp_offset_v,
        },
        {
            type = 'offset_z',
            amplitude = amp_offset_h,
        }
    },
    parts = {
        {
            face = 'nocull',
            tex = 1,
            points = {
                0, 2.5/16, 0, 0, 0,
                0, 2.5/16, 1, 0, 1,
                1, 2.5/16, 1, 1, 1,
                1, 2.5/16, 0, 1, 0
            },
            shader_mod = {
                type = 'sway_full_block',
                amplitude = amp_sway
             }
        },
        {
            face = 'nocull',
            tex = 2,
            points = {
                0, 1.5/16, 0, 0, 0,
                0, 1.5/16, 1, 0, 1,
                1, 1.5/16, 1, 1, 1,
                1, 1.5/16, 0, 1, 0
            },
            shader_mod = {
                type = 'sway_full_block',
                amplitude = amp_sway
            }
        }
    }
})