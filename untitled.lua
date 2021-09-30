function zepha.reconcile_available(old_block, old_meta, block, meta)
	local meta_def = (zepha.registered_blocks[block] or {}).meta or {}
	for k, v in pairs(old_meta) do
		if meta_def[k] == type(v) then meta[k] = v end
	end
end
