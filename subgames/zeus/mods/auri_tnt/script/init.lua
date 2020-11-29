zepha.register_block("@auri:tnt:tnt", {
    name = "TNT",
    model = "base:block",
    textures = {
        "@auri:tnt:tnt_top",
        "@auri:tnt:tnt_bottom",
        "@auri:tnt:tnt_side"
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    drop = "@auri:tnt:tnt",
--     on_break = function(pos)
--         local amp = 20
--         for i = -amp, amp do
--             for j = -amp, amp do
--                 for k = -amp, amp do
--                     if V{i, j, k}:distance(V()) < amp then
--                         zepha.set_block(pos + offset, "air")
--                     end
--                 end
--             end
--         end
--     end
})