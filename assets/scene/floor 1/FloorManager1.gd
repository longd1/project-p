extends Node2D

@export var required_kills: int = 10

@export var reward_weapon: Area2D
#@export var reward_collision: CollisionShape2D

@export var exit_area: Area2D
@export var exit_collision: CollisionShape2D

var kill_count: int = 0
var floor_cleared: bool = false


func _ready() -> void:
	reward_weapon.hide()
	reward_weapon.monitoring = false
	#reward_collision.disabled = true

	exit_area.monitoring = false
	exit_collision.disabled = true

	for enemy in get_tree().get_nodes_in_group("enemy"):
		if enemy.has_signal("died"):
			enemy.died.connect(enemy_killed)


func enemy_killed() -> void:
	if floor_cleared:
		return

	kill_count += 1

	print("Enemies killed: ", kill_count, "/", required_kills)

	if kill_count >= required_kills:
		clear_floor()


func clear_floor() -> void:
	floor_cleared = true

	reward_weapon.show()
	reward_weapon.monitoring = true
	#reward_collision.set_deferred("disabled", false)

	exit_area.monitoring = true
	exit_collision.set_deferred("disabled", false)

	print("Floor cleared!")
