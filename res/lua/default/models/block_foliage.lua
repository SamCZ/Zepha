--
-- Cube model with extra side faces that pop out of the model.
-- Useful for making grass, snow, textures that have a raised material on top of them.
-- Texture order is: top, bottom, left, right, front, back, raised.
--

zeus.register_blockmodel("default:block_foliage", {
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
        tex = 4,
        points = {
            1, 1, 1, 1, 0,
            1, 0, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 1, 0, 0, 0
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
        tex = 5,
        points = {
            0, 0, 1, 0, 1,
            1, 0, 1, 1, 1,
            1, 1, 1, 1, 0,
            0, 1, 1, 0, 0
        }
    }, {
        face = "back",
        tex = 6,
        points = {
            0, 0, 0, 0, 1,
            0, 1, 0, 0, 0,
            1, 1, 0, 1, 0,
            1, 0, 0, 1, 1
        }
    }, {
        --Floats begin here
        face = "front",
        tex = 7,
        points = {
            0, 1, 1, 0, 0,
            0, 0.2, 1.2, 0, 1,
            1, 0.2, 1.2, 1, 1,
            1, 1, 1, 1, 0
        }
    }, {
        face = "back",
        tex = 7,
        points = {
            1, 0.2, -0.2, 1, 1,
            0, 0.2, -0.2, 0, 1,
            0, 1, 0, 0, 0,
            1, 1, 0, 1, 0
        }
    }, {
        face = "right",
        tex = 7,
        points = {
            1.2, 0.2, 1, 1, 1,
            1.2, 0.2, 0, 0, 1,
            1, 1, 0, 0, 0,
            1, 1, 1, 1, 0
        }
    }, {
        face = "left",
        tex = 7,
        points = {
            0, 1, 0, 0, 0,
            -0.2, 0.2, 0, 0, 1,
            -0.2, 0.2, 1, 1, 1,
            0, 1, 1, 1, 0
        }
    }
})