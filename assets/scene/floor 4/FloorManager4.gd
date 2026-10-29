extends Node


# ==================================================
# MINI BOSSES
# ==================================================

@export var mini_boss_1: Node
@export var mini_boss_2: Node


# ==================================================
# EXIT
# ==================================================

@export var exit_area: Area2D
@export var exit_collision: CollisionShape2D


# ==================================================
# STATE
# ==================================================

var required_kills: int = 0
var kill_count: int = 0

var floor_completed: bool = false


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	_setup_exit()
	_connect_enemies()
	_connect_mini_bosses()


# ==================================================
# SETUP EXIT
# ==================================================

func _setup_exit() -> void:
	if exit_area == null:
		push_error(
			"FloorManager4: Exit Area chưa được gán."
		)
		return

	exit_area.monitoring = false

	if exit_collision != null:
		exit_collision.set_deferred(
			"disabled",
			true
		)


# ==================================================
# NORMAL ENEMIES
# ==================================================

func _connect_enemies() -> void:
	var enemies = get_tree().get_nodes_in_group(
		"enemy"
	)

	for enemy in enemies:
		if not enemy.has_signal("died"):
			push_error(
				"FloorManager4: "
				+ enemy.name
				+ " không có signal died."
			)
			continue

		if not enemy.died.is_connected(
			_on_enemy_died
		):
			enemy.died.connect(
				_on_enemy_died
			)

		required_kills += 1

	print(
		"Floor 4 normal enemies: ",
		enemies.size()
	)


# ==================================================
# MINI BOSSES
# ==================================================

func _connect_mini_bosses() -> void:
	_connect_mini_boss(
		mini_boss_1
	)

	_connect_mini_boss(
		mini_boss_2
	)


func _connect_mini_boss(
	mini_boss: Node
) -> void:
	if mini_boss == null:
		return

	if not mini_boss.has_signal("died"):
		push_error(
			"FloorManager4: "
				+ mini_boss.name
				+ " không có signal died."
		)
		return

	if not mini_boss.died.is_connected(
		_on_mini_boss_died
	):
		mini_boss.died.connect(
			_on_mini_boss_died
		)

	required_kills += 1


# ==================================================
# NORMAL ENEMY DIED
# ==================================================

func _on_enemy_died(_enemy = null) -> void:
	_register_kill()


# ==================================================
# MINI BOSS DIED
# ==================================================

func _on_mini_boss_died(_mini_boss = null) -> void:
	_register_kill()


# ==================================================
# REGISTER KILL
# ==================================================

func _register_kill() -> void:
	if floor_completed:
		return

	kill_count += 1

	print(
		"Floor 4 defeated: ",
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

	print("Floor 4 completed!")

	_enable_exit()


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
