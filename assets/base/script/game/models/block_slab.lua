--
-- Basic 'slab' model, represents a halfslab.
-- Texture order is: top, bottom, left, right, front, back.
--

zepha.register_blockmodel(':block_slab', {
    parts = {
        {
            face = 'back',
            tex = 3,
            points = {
                0, 0, 0, 0, 0.5,
                0, 0, 1, 1, 0.5,
                0, 0.5, 1, 1, 0,
                0, 0.5, 0, 0, 0
            }
        }, {
            face = 'front',
            tex = 4,
            points = {
                1, 0.5, 1, 0, 0,
                1, 0, 1, 0, 0.5,
                1, 0, 0, 1, 0.5,
                1, 0.5, 1, 0, 0
            }
        }, {
            face = 'nocull',
            tex = 1,
            points = {
                0, 0.5, 0, 0, 0,
                0, 0.5, 1, 0, 1,
                1, 0.5, 1, 1, 1,
                1, 0.5, 0, 1, 0
            }
        }, {
            face = 'bottom',
            tex = 2,
            points = {
                0, 0, 0, 0, 0,
                1, 0, 0, 1, 0,
                1, 0, 1, 1, 1,
                0, 0, 1, 0, 1
            }
        }, {
            face = 'left',
            tex = 5,
            points = {
                0, 0, 1, 0, 0.5,
                1, 0, 1, 1, 0.5,
                1, 0.5, 1, 1, 0,
                0, 0.5, 1, 0, 0
            }
        }, {
            face = 'right',
            tex = 6,
            points = {
                0, 0, 0, 1, 0.5,
                0, 0.5, 0, 1, 0,
                1, 0.5, 0, 0, 0,
                1, 0, 0, 0, 0.5
            }
        }
    }
})