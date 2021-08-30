local menu = zepha.gui(function()
    return Gui.Box {
        background = '#0003',

        Gui.Box {
            id = 'inventory',

            size = { '342dp', '187dp' },
            pos = { '50cw - 50sw', '50ch - 50sh' },

            Gui.Box {
                id = 'backpack',

                pos = { 1, 0 },
                size = { '106dp', '187dp' },
                padding = { '19dp', '9dp', '8dp', '9dp' },

                background = 'zeus:inventory:backpack'

--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 0 },
--
--                     list = 'main',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 18 * 3 },
--
--                     list = 'hot_wheel_1',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 18 * 4 },
--
--                     list = 'hot_wheel_2',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 18 * 5 },
--
--                     list = 'hot_wheel_3',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 18 * 6 },
--
--                     list = 'hot_wheel_4',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 18 * 7 },
--
--                     list = 'hot_wheel_5',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 0, 18*8 },
--
--                     list = 'hot_wheel_6',
--                     source = 'current_player'
--                 }
            },
            Gui.Box {
                id = 'player_frame',

                pos = { '105dp', 0 },
                size = { '106dp', '187dp' },

                background = 'zeus:inventory:player_frame',

--                 Gui.Model {
--                     pos = { 52, 150 },
--                     size = { 64, 64 },
--
--                     type = 'model',
--                     anim_range = { 0, 100 },
--                     source = 'zeus:default:player',
--                     texture = 'zeus:default:player'
--                 }
            },
            Gui.Box {
                id = 'equipment',

                pos = { '209dp', '1dp' },
                size = { '132dp', '80dp' },
                padding = { '18dp', '8dp', '8dp', '8dp' },
                background = 'zeus:inventory:equipment',

                Gui.Box {
                    id = 'player_clamp',

                    pos = { '41dp', '1dp' },
                    size = { '34dp', '52dp' },
                    overflow = 'hidden'
                }
            },
            Gui.Box {
                id = 'dynamic',

                pos = { '209dp', '80dp' },
                size = { '132dp', '107dp' },
                padding = '8dp',

                background = 'zeus:inventory:dynamic'

--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 1, 1 },
--
--                     list = 'mod',
--                     source = 'current_player',
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 41, 32 },
--
--                     list = 'craft',
--                     source = 'current_player'
--                 },
--                 Gui.InventoryList {
--                     gap = { 2, 2 },
--                     pos = { 81, 41 },
--
--                     list = 'craft_result',
--                     source = 'current_player'
--                 }
            }
        }
    }
end)

inventory.open_inventory = function()
    if zepha.player.menu ~= nil and zepha.player.menu ~= menu then return end
    zepha.player.menu = not zepha.player.menu and menu or nil
end

zepha.register_keybind('zeus:inventory:open_inventory', {
    description = 'Open Inventory',
    default = zepha.keys['.'],
    on_press = inventory.open_inventory
})
