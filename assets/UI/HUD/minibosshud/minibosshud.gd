
extends CanvasLayer

@export var miniboss_path: NodePath
@export var health_lerp_speed: float = 8.0

var miniboss: Node = null
var preview_mode: bool = false

var full_bar_scale: Vector2

@onready var mnb_background: Panel = $mnb_background
@onready var mnb_bar: Panel = $mnb_bar
@onready var miniboss_name_label: Label = $minibossname


func _ready() -> void:
	# Giữ nguyên scale được thiết kế trong scene.
	full_bar_scale = mnb_bar.scale

	# Không thay đổi vị trí, kích thước, màu sắc
	# hoặc nội dung của minibossname.

	if not miniboss_path.is_empty():
		miniboss = get_node_or_null(miniboss_path)

	if not is_instance_valid(miniboss):
		var current_scene := get_tree().current_scene

		if current_scene != null:
			miniboss = current_scene.find_child(
				"MiniBoss",
				true,
				false
			)

	if not is_instance_valid(miniboss):
		if get_tree().current_scene == self:
			preview_mode = true
			visible = true
			mnb_bar.scale = full_bar_scale
		else:
			visible = false
			push_warning(
				"MiniBossHUD: Không tìm thấy Mini Boss. "
				+ "Hãy gán MiniBoss Path trong Inspector."
			)


func _process(delta: float) -> void:
	if preview_mode:
		return

	if not is_instance_valid(miniboss):
		visible = false
		return

	if not miniboss.has_method("get_health"):
		return

	if not miniboss.has_method("get_max_health"):
		return

	var current_health: float = float(
		miniboss.get_health()
	)

	var max_health: float = float(
		miniboss.get_max_health()
	)

	if max_health <= 0.0 or current_health <= 0.0:
		visible = false
		return

	visible = true

	var health_ratio: float = clampf(
		current_health / max_health,
		0.0,
		1.0
	)

	var target_scale_x: float = (
		full_bar_scale.x * health_ratio
	)

	var weight: float = clampf(
		health_lerp_speed * delta,
		0.0,
		1.0
	)

	mnb_bar.scale.x = lerpf(
		mnb_bar.scale.x,
		target_scale_x,
		weight
	)
