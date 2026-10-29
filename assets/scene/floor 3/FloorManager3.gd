extends Node


# ==================================================
# SETTINGS
# ==================================================

@export var required_kills: int = 2

@export var defeat_targets: Array[Node] = []

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
	_connect_targets()


# ==================================================
# SETUP REWARD
# ==================================================

func _setup_reward() -> void:
	if reward_weapon == null:
		push_error(
			"FloorManager3: Reward Weapon chưa được gán."
		)
		return

	reward_weapon.hide()
	reward_weapon.monitoring = false


# ==================================================
# SETUP EXIT
# ==================================================

func _setup_exit() -> void:
	if exit_area == null:
		push_error(
			"FloorManager3: Exit Area chưa được gán."
		)
		return

	exit_area.monitoring = false

	if exit_collision != null:
		exit_collision.set_deferred(
			"disabled",
			true
		)


# ==================================================
# CONNECT COMBAT TARGETS
# ==================================================

func _connect_targets() -> void:
	for target in defeat_targets:
		if target == null:
			continue

		if not target.has_signal("died"):
			push_error(
				"FloorManager3: "
				+ target.name
				+ " không có signal died."
			)
			continue

		if not target.died.is_connected(_on_target_died):
			target.died.connect(
				_on_target_died
			)


# ==================================================
# TARGET DIED
# ==================================================

func _on_target_died(_target = null) -> void:
	if floor_completed:
		return

	kill_count += 1

	print(
		"Floor 3 kills: ",
		kill_count,
		"/",
		required_kills
	)

	if kill_count >= required_kills:
		_complete_floor()


# ==================================================
# COMPLETE FLOOR
# ==================================================

func _complete_floor() -> void:
	if floor_completed:
		return

	floor_completed = true

	print("Floor 3 completed!")

	_show_reward()
	_enable_exit()


# ==================================================
# SHOW REWARD
# ==================================================

func _show_reward() -> void:
	if reward_weapon == null:
		return

	reward_weapon.show()
	reward_weapon.monitoring = true


# ==================================================
# ENABLE EXIT
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
