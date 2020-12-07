health.damage_player = function(player, damage)
    local health = health.health_values[player.id]
    health.health = health.health - damage
    health.buffer = damage

    zepha.send_message("@auri:health:damage", health)
end