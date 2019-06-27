local function create_model_def(speed)
    local mod = ""
    if speed > 0 then mod = "rotate_x" end

    return {
        parts = {
            {
                face = "left", -- Small part 1
                tex = 3,
                points = {
                    0, 7/16, 7/16, 0, 2/10,
                    0, 7/16, 9/16, 2/16, 2/10,
                    0, 9/16, 9/16, 2/16, 0,
                    0, 9/16, 7/16, 0, 0
                },
                shader_mod = {
                    type = mod,
                    speed = speed
                }
            }, {
                face = "right", -- Small part 2
                tex = 4,
                points = {
                    1, 9/16, 9/16, 2/16, 2/10,
                    1, 7/16, 9/16, 4/16, 2/10,
                    1, 7/16, 7/16, 4/16, 0,
                    1, 9/16, 7/16, 2/16, 0
                },
                shader_mod = {
                    type = mod,
                    speed = speed
                }
            }, {
                face = "nocull", -- Top
                tex = 1,
                points = {
                    0, 9/16, 7/16, 0, 2/10,
                    0, 9/16, 9/16, 0, 4/10,
                    1, 9/16, 9/16, 1, 4/10,
                    1, 9/16, 7/16, 1, 2/10
                },
                shader_mod = {
                    type = mod,
                    speed = speed
                }
            }, {
                face = "nocull", -- Bottom
                tex = 2,
                points = {
                    0, 7/16, 7/16, 0, 6/10,
                    1, 7/16, 7/16, 1, 6/10,
                    1, 7/16, 9/16, 1, 8/10,
                    0, 7/16, 9/16, 0, 8/10
                },
                shader_mod = {
                    type = mod,
                    speed = speed
                }
            }, {
                face = "nocull", -- Front
                tex = 5,
                points = {
                    0, 7/16, 9/16, 0, 8/10,
                    1, 7/16, 9/16, 1, 8/10,
                    1, 9/16, 9/16, 1, 1,
                    0, 9/16, 9/16, 0, 1
                },
                shader_mod = {
                    type = mod,
                    speed = speed
                }
            }, {
                face = "nocull", -- Back
                tex = 6,
                points = {
                    0, 7/16, 7/16, 0, 4/10,
                    0, 9/16, 7/16, 0, 6/10,
                    1, 9/16, 7/16, 1, 6/10,
                    1, 7/16, 7/16, 1, 4/10
                },
                shader_mod = {
                    type = mod,
                    speed = speed
                }
            }
        }
    }
end

zeus.register_blockmodel("kinetic:axle_0", create_model_def(0));
zeus.register_blockmodel("kinetic:axle_1", create_model_def(0.5));
zeus.register_blockmodel("kinetic:axle_2", create_model_def(1));
zeus.register_blockmodel("kinetic:axle_3", create_model_def(2));