
extends CanvasLayer

var player: Node = null

var full_hp_bar_scale: Vector2

@onready var hp_background: Control = $hp_background
@onready var hp_bar: Control = $hp_bar

@onready var heal_flask_icon: TextureRect = $heal_flask_icon
@onready var heal_flask_count: Label = $heal_flask_count

const HEALTH_LERP_SPEED: float = 10.0

var last_flask_count: int = -1


func _ready() -> void:
	# Giữ nguyên thiết kế thanh máu trong scene.
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

	_update_health_bar(delta)
	_update_heal_flask_count()


func _update_health_bar(delta: float) -> void:
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

	var health_ratio: float = 0.0

	if max_health > 0.0:
		health_ratio = clampf(
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


func _update_heal_flask_count() -> void:
	if not player.has_method("get_current_heal_flasks"):
		return

	var flask_count: int = int(
		player.get_current_heal_flasks()
	)

	# Chỉ cập nhật chữ khi số bình thay đổi.
	if flask_count != last_flask_count:
		heal_flask_count.text = str(flask_count)
		last_flask_count = flask_count
