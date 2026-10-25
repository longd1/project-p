
extends CanvasLayer

var player: Node = null

var full_hp_bar_scale: Vector2

@onready var hp_background: Control = $hp_background
@onready var hp_bar: Control = $hp_bar

const HEALTH_LERP_SPEED: float = 10.0


func _ready() -> void:
	# Lưu scale ban đầu của thanh máu.
	full_hp_bar_scale = hp_bar.scale

	# Tìm Player trong Floor hiện tại.
	var current_scene: Node = get_tree().current_scene

	if current_scene != null:
		player = current_scene.find_child(
			"Player",
			true,
			false
		)

	if not is_instance_valid(player):
		push_warning(
			"PlayerHUD: Không tìm thấy node Player."
		)


func _process(delta: float) -> void:
	if not is_instance_valid(player):
		return

	if not player.has_method("get_health"):
		return

	if not player.has_method("get_max_health"):
		return

	var max_health: float = float(
		player.get_max_health()
	)

	var current_health: float = float(
		player.get_health()
	)

	if max_health <= 0.0:
		hp_bar.scale.x = 0.0
		return

	var health_ratio: float = clampf(
		current_health / max_health,
		0.0,
		1.0
	)

	var target_scale_x: float = (
		full_hp_bar_scale.x * health_ratio
	)

	var weight: float = clampf(
		HEALTH_LERP_SPEED * delta,
		0.0,
		1.0
	)

	hp_bar.scale.x = lerpf(
		hp_bar.scale.x,
		target_scale_x,
		weight
	)
