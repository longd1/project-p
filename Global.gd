extends Node


# ==================================================
# SIGNALS
# ==================================================

signal weapon_changed(weapon)


# ==================================================
# PROGRESSION
# ==================================================

var current_floor: int = 1

var damage_multiplier: float = 1.0
var bonus_max_health: int = 0

var max_heal_flasks: int = 3
var heal_amount: int = 30


# ==================================================
# EQUIPMENT
# ==================================================

var current_weapon = null


# ==================================================
# DAMAGE
# ==================================================

func upgrade_damage(amount: float) -> void:
	damage_multiplier += amount


# ==================================================
# HEALTH
# ==================================================

func upgrade_health(amount: int) -> void:
	bonus_max_health += amount


# ==================================================
# FLASK
# ==================================================

func upgrade_flask() -> void:
	max_heal_flasks += 1
	heal_amount += 20


# ==================================================
# WEAPON
# ==================================================

func set_current_weapon(weapon) -> void:
	if weapon == null:
		push_warning(
			"Global: Tried to equip null weapon."
		)
		return

	current_weapon = weapon

	print(
		"GLOBAL WEAPON UPDATED: ",
		current_weapon
	)

	weapon_changed.emit(
		current_weapon
	)


func get_current_weapon():
	return current_weapon


# ==================================================
# FLOOR
# ==================================================

func advance_floor() -> void:
	current_floor += 1
	upgrade_flask()


# ==================================================
# RESET RUN
# ==================================================

func reset_progression() -> void:
	current_floor = 1

	damage_multiplier = 1.0
	bonus_max_health = 0

	max_heal_flasks = 3
	heal_amount = 30

	current_weapon = null

	weapon_changed.emit(null)

	print(
		"GLOBAL WEAPON RESET"
	)
