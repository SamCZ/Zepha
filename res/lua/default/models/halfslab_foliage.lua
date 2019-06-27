--
-- Basic 'slab' model, represents a halfslab.
-- Texture order is: top, bottom, left, right, front, back.
--

zeus.register_blockmodel("default:halfslab_foliage", {
    parts = {
        {
            face = "left",
            tex = 3,
            points = {
                0, 0, 0, 0, 0.5,
                0, 0, 1, 1, 0.5,
                0, 0.5, 1, 1, 0,
                0, 0.5, 0, 0, 0
            }
        }, {
            face = "right",
            tex = 4,
            points = {
                1, 0.5, 1, 1, 0,
                1, 0, 1, 1, 0.5,
                1, 0, 0, 0, 0.5,
                1, 0.5, 0, 0, 0
            }
        }, {
            face = "nocull",
            tex = 1,
            points = {
                0, 0.5, 0, 0, 0,
                0, 0.5, 1, 0, 1,
                1, 0.5, 1, 1, 1,
                1, 0.5, 0, 1, 0
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
            tex = 5,
            points = {
                0, 0, 1, 0, 0.5,
                1, 0, 1, 1, 0.5,
                1, 0.5, 1, 1, 0,
                0, 0.5, 1, 0, 0
            }
        }, {
            face = "back",
            tex = 6,
            points = {
                0, 0, 0, 0, 0.5,
                0, 0.5, 0, 0, 0,
                1, 0.5, 0, 1, 0,
                1, 0, 0, 1, 0.5
            }
        }, {
            --Floats begin here
            face = "front",
            tex = 7,
            points = {
                0, 0.5, 1, 0, 0,
                0, -0.3, 1.2, 0, 1,
                1, -0.3, 1.2, 1, 1,
                1, 0.5, 1, 1, 0
            }
        }, {
            face = "back",
            tex = 7,
            points = {
                1, -0.3, -0.2, 1, 1,
                0, -0.3, -0.2, 0, 1,
                0, 0.5, 0, 0, 0,
                1, 0.5, 0, 1, 0
            }
        }, {
            face = "right",
            tex = 7,
            points = {
                1.2, -0.3, 1, 1, 1,
                1.2, -0.3, 0, 0, 1,
                1, 0.5, 0, 0, 0,
                1, 0.5, 1, 1, 0
            }
        }, {
            face = "left",
            tex = 7,
            points = {
                0, 0.5, 0, 0, 0,
                -0.2, -0.3, 0, 0, 1,
                -0.2, -0.3, 1, 1, 1,
                0, 0.5, 1, 1, 0
            }
        }
    }
})