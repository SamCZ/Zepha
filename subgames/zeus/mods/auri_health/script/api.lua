health.player_damage = function(player, damage)
    local health = health.health_values[player.id]
    health.health = health.health - damage
    health.buffer = damage

    zepha.send_message("@auri:health:damage", health)
end

health.player_set = function(player, health, buffer)
    local health = health.health_values[player.id]
    health.health = health
    health.buffer = buffer or 0

    zepha.send_message("@auri:health:damage", health)
end

health.player_heal = function(player, add)
    local health = health.health_values[player.id]
    health.health = math.min(health.health + health.buffer + add, health.max)
    health.buffer = 0

    zepha.send_message("@auri:health:damage", health)
end