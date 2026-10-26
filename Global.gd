extends Node


# ==========================================
# FLOOR
# ==========================================

var current_floor: int = 1


# ==========================================
# PLAYER PROGRESSION
# ==========================================

var damage_multiplier: float = 1.0
var bonus_max_health: int = 0


# ==========================================
# HEALING FLASK
# ==========================================

var max_heal_flasks: int = 3
var heal_amount: int = 30


# ==========================================
# DAMAGE UPGRADE
# ==========================================

func upgrade_damage(amount: float) -> void:
	damage_multiplier += amount
	print("Current Floor: ", current_floor)
	print("Damage Multiplier: ", damage_multiplier, "x")
	print("Bonus Max HP: +", bonus_max_health)


# ==========================================
# HEALTH UPGRADE
# ==========================================

func upgrade_health(amount: int) -> void:
	bonus_max_health += amount
	print("Current Floor: ", current_floor)
	print("Damage Multiplier: ", damage_multiplier, "x")
	print("Bonus Max HP: +", bonus_max_health)

# ==========================================
# FLOOR BONUS
# +2 flasks
# +20 healing
# ==========================================

func upgrade_flask() -> void:
	max_heal_flasks += 2
	heal_amount += 20
	print("flasks upgraded")

# ==========================================
# ADVANCE FLOOR
# ==========================================

func advance_floor() -> void:
	current_floor += 1
	upgrade_flask()


# ==========================================
# RESET RUN
# ==========================================

func reset_progression() -> void:
	current_floor = 1

	damage_multiplier = 1.0
	bonus_max_health = 0

	max_heal_flasks = 3
	heal_amount = 30
