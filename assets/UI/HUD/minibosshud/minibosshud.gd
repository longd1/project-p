
extends CanvasLayer

@export var miniboss_path: NodePath

@export var bar_width: float = 300.0
@export var bar_height: float = 16.0
@export var health_lerp_speed: float = 8.0

# Khoảng cách từ mép phải và mép trên màn hình.
@export var margin_right: float = 20.0
@export var top_offset: float = 50.0

var miniboss: Node = null
var preview_mode: bool = false

@onready var mnb_background: Panel = $mnb_background
@onready var mnb_bar: Panel = $mnb_bar


func _ready() -> void:
	_apply_styles()
	_update_layout()

	# Ưu tiên đường dẫn được gán trong Inspector.
	if not miniboss_path.is_empty():
		miniboss = get_node_or_null(miniboss_path)

	# Nếu chưa gán đường dẫn, tìm node tên MiniBoss.
	if not is_instance_valid(miniboss):
		var scene := get_tree().current_scene

		if scene != null:
			miniboss = scene.find_child(
				"MiniBoss",
				true,
				false
			)

	# Hỗ trợ xem trước khi chạy riêng scene HUD.
	if not is_instance_valid(miniboss):
		var scene := get_tree().current_scene

		if scene == self:
			preview_mode = true
			visible = true
			mnb_bar.size.x = bar_width
		else:
			visible = false
			push_warning(
				"MiniBossHUD: Không tìm thấy MiniBoss. "
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

	# Tính tỷ lệ máu còn lại.
	var health_ratio: float = clampf(
		current_health / max_health,
		0.0,
		1.0
	)

	var target_width: float = (
		bar_width * health_ratio
	)

	# Làm thanh máu giảm mượt.
	var weight: float = clampf(
		health_lerp_speed * delta,
		0.0,
		1.0
	)

	mnb_bar.size.x = lerpf(
		mnb_bar.size.x,
		target_width,
		weight
	)


func _update_layout() -> void:
	var viewport_size: Vector2 = (
		get_viewport().get_visible_rect().size
	)

	# Căn thanh máu về góc trên bên phải.
	var start_x: float = (
		viewport_size.x - bar_width - margin_right
	)

	mnb_background.position = Vector2(
		start_x,
		top_offset
	)

	mnb_background.size = Vector2(
		bar_width,
		bar_height
	)

	# Thanh máu bắt đầu trùng với nền.
	mnb_bar.position = mnb_background.position
	mnb_bar.size = mnb_background.size


func _apply_styles() -> void:
	# Nền đỏ sẫm.
	var background_style := StyleBoxFlat.new()
	background_style.bg_color = Color("#210D12")
	background_style.border_color = Color("#5A2630")
	background_style.set_border_width_all(1)
	background_style.set_corner_radius_all(7)

	mnb_background.add_theme_stylebox_override(
		"panel",
		background_style
	)

	# Thanh máu đỏ sáng.
	var bar_style := StyleBoxFlat.new()
	bar_style.bg_color = Color("#C91F2D")
	bar_style.set_corner_radius_all(7)

	mnb_bar.add_theme_stylebox_override(
		"panel",
		bar_style
	)
