
extends CanvasLayer

var player: Node = null

@onready var hp_background: Control = $hp_background
@onready var hp_bar: Control = $hp_bar

const BASE_MAX_HEALTH: float = 100.0

const BASE_BACKGROUND_WIDTH: float = 175.0
const BASE_HEALTHBAR_WIDTH: float = 169.0

const HEALTH_LERP_SPEED: float = 10.0


func _ready() -> void:
	# Tìm Player trong scene Floor hiện tại.
	var current_scene: Node = get_tree().current_scene

	if current_scene != null:
		player = current_scene.find_child(
			"Player",
			true,
			false
		)

	if not is_instance_valid(player):
		push_warning(
			"PlayerHUD: Không tìm thấy node Player "
			+ "trong scene hiện tại."
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

	# Tránh chia cho 0.
	if max_health <= 0.0:
		hp_background.size.x = 0.0
		hp_bar.size.x = 0.0
		return

	# Nền thanh máu thay đổi theo máu tối đa.
	var max_health_ratio: float = (
		max_health / BASE_MAX_HEALTH
	)

	hp_background.size.x = (
		BASE_BACKGROUND_WIDTH * max_health_ratio
	)

	var max_healthbar_width: float = (
		BASE_HEALTHBAR_WIDTH * max_health_ratio
	)

	# Tính lượng máu còn lại.
	var health_ratio: float = clampf(
		current_health / max_health,
		0.0,
		1.0
	)

	var target_width: float = (
		max_healthbar_width * health_ratio
	)

	# Giảm thanh máu mượt mà.
	var lerp_weight: float = clampf(
		HEALTH_LERP_SPEED * delta,
		0.0,
		1.0
	)

	hp_bar.size.x = lerpf(
		hp_bar.size.x,
		target_width,
		lerp_weight
	)
