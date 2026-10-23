extends Node

var max_heal_flasks: int = 3
var heal_amount: int = 30

var damage_multiplier: float = 1.0
var bonus_max_health: int = 0


func upgrade_damage(amount: float) -> void:
	damage_multiplier += amount


func upgrade_health(amount: int) -> void:
	bonus_max_health += amount


func upgrade_flask(amount: int) -> void:
	max_heal_flasks += 2 * amount
	heal_amount += 20 * amount


func reset_progression() -> void:
	damage_multiplier = 1.0
	bonus_max_health = 0

	max_heal_flasks = 3
	heal_amount = 30
