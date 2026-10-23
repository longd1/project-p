
extends CanvasLayer

@export var boss_path: NodePath

@export var bar_width: float = 400.0
@export var bar_height: float = 18.0
@export var health_lerp_speed: float = 8.0

# Vị trí thanh máu trong viewport.
@export var margin_left: float = 20.0
@export var top_offset: float = 50.0

var boss: Node = null
var preview_mode: bool = false

@onready var background: Panel = $background
@onready var bar: Panel = $bar


func _ready() -> void:
	_apply_styles()
	_update_layout()

	# Tìm Boss theo đường dẫn được gán.
	if not boss_path.is_empty():
		boss = get_node_or_null(boss_path)

	# Nếu chưa tìm thấy, tìm node tên Boss trong Floor.
	if not is_instance_valid(boss):
		var scene := get_tree().current_scene

		if scene != null:
			boss = scene.find_child("Boss", true, false)

	# Chạy riêng bosshud.tscn: hiển thị đầy thanh để xem giao diện.
	if not is_instance_valid(boss):
		var scene := get_tree().current_scene

		if scene == self:
			preview_mode = true
			visible = true
			bar.size.x = bar_width
		else:
			visible = false
			push_warning(
				"BossHUD: Không tìm thấy Boss. "
				+ "Hãy kiểm tra Boss Path."
			)


func _process(delta: float) -> void:
	if preview_mode:
		return

	if not is_instance_valid(boss):
		visible = false
		return

	if not boss.has_method("get_health"):
		return

	if not boss.has_method("get_max_health"):
		return

	var current_health: float = float(
		boss.get_health()
	)
	var max_health: float = float(
		boss.get_max_health()
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

	var target_width: float = bar_width * health_ratio
	var weight: float = clampf(
		health_lerp_speed * delta,
		0.0,
		1.0
	)

	bar.size.x = lerpf(
		bar.size.x,
		target_width,
		weight
	)



func _update_layout() -> void:
	var viewport_size: Vector2 = (
		get_viewport().get_visible_rect().size
	)

	# Căn thanh máu về phía bên phải màn hình.
	var start_x: float = (
		viewport_size.x - bar_width - margin_left
	)

	background.position = Vector2(
		start_x,
		top_offset
	)

	background.size = Vector2(
		bar_width,
		bar_height
	)

	# Thanh máu khớp với nền khi đầy máu.
	bar.position = background.position
	bar.size = background.size



func _apply_styles() -> void:
	# Nền đen đỏ.
	var background_style := StyleBoxFlat.new()
	background_style.bg_color = Color("#210D12")
	background_style.border_color = Color("#5A2630")
	background_style.set_border_width_all(1)
	background_style.set_corner_radius_all(7)

	background.add_theme_stylebox_override(
		"panel",
		background_style
	)

	# Phần máu hiện tại màu đỏ.
	var bar_style := StyleBoxFlat.new()
	bar_style.bg_color = Color("#C91F2D")
	bar_style.set_corner_radius_all(7)

	bar.add_theme_stylebox_override(
		"panel",
		bar_style
	)
