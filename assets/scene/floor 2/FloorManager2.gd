extends Node


# ==================================================
# SETTINGS
# ==================================================

@export var required_kills: int = 9

@export var reward_weapon: Area2D
@export var exit_area: Area2D
@export var exit_collision: CollisionShape2D


# ==================================================
# STATE
# ==================================================

var kill_count: int = 0
var floor_completed: bool = false


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	_setup_reward()
	_setup_exit()
	_connect_enemies()


# ==================================================
# INITIAL SETUP
# ==================================================

func _setup_reward() -> void:
	if reward_weapon == null:
		push_error("Floor2Manager: Reward Weapon chưa được gán.")
		return

	reward_weapon.hide()
	reward_weapon.monitoring = false


func _setup_exit() -> void:
	if exit_area == null:
		push_error("Floor2Manager: Exit Area chưa được gán.")
		return

	exit_area.monitoring = false

	if exit_collision != null:
		exit_collision.disabled = true


# ==================================================
# ENEMIES
# ==================================================

func _connect_enemies() -> void:
	var enemies = get_tree().get_nodes_in_group("enemy")

	for enemy in enemies:
		if enemy.has_signal("died"):
			if not enemy.died.is_connected(_on_enemy_died):
				enemy.died.connect(_on_enemy_died)

	print(
		"Floor 2 enemies connected: ",
		enemies.size()
	)


# ==================================================
# ENEMY DEATH
# ==================================================

func _on_enemy_died() -> void:
	if floor_completed:
		return

	kill_count += 1

	print(
		"Floor 2 kills: ",
		kill_count,
		"/",
		required_kills
	)

	if kill_count >= required_kills:
		_complete_floor()


# ==================================================
# FLOOR COMPLETE
# ==================================================

func _complete_floor() -> void:
	if floor_completed:
		return

	floor_completed = true

	print("Floor 2 completed!")

	_show_reward()
	_enable_exit()


# ==================================================
# REWARD
# ==================================================

func _show_reward() -> void:
	if reward_weapon == null:
		return

	reward_weapon.show()
	reward_weapon.monitoring = true


# ==================================================
# EXIT
# ==================================================

func _enable_exit() -> void:
	if exit_area == null:
		return

	exit_area.monitoring = true

	if exit_collision != null:
		exit_collision.set_deferred(
			"disabled",
			false
		)
