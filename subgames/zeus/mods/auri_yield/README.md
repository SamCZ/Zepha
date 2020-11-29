# Item Collection

**By Auri Collings (Aurailus)**

## Description

Item Collection is a mod that enables blocks to yield zero or more items when broken.

## Technical Information

### Mode Setting 

Item Collection can be configured to yield items in multiple ways:

1. Adding blocks directly to the user's inventory. `"direct"`
2. Creating a dropped-item entity that can be picked up by a player standing nearby. `"ent_nearby"`
3. Creating a dropped-item entity that can be picked up by interacting with it. `"ent_interact"`

This can be configured using the `/item_collection:set_mode` chat command, or by setting the `mode` parameter inside of the config file. (UNIMPLEMENTED)

### Inventory

Item Collection will, by default, attempt to add all items to a `main` list inside of the player's inventory. This can be configured using the `/item_collection:set_list` chat command, or by setting the `list` parameter inside of the config file. (UNIMPLEMENTED)

### Block Definition

Item Collection will look for a `yields` parameter in the definition of the block broken. This field can either be a string containing an item name, an item name followed by a number denoting how many of the item to yield, or a function which returns a string in one of the two previous formats. The following code blocks are examples of valid formats:

Drop a dirt block:
```lua
yields = "zeus:default:dirt"
```

Drop four dirt blocks:
```lua
yields = "zeus:default:dirt 4"
```

Drop a dirt block 50% of the time:
```lua
yields = function(pos)
	if math.random() >= 0.5 then
		return "zeus:default:dirt"
	else
		return ""
	end
end
```

If the key is missing, or the value or returned value is `nil` or an empty string, no item will be yielded.
