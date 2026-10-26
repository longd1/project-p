
extends CanvasLayer

# Gán Boss trong Inspector nếu cần.
@export var boss_path: NodePath

# Tên hiển thị phía trên thanh máu.
@export var boss_display_name: String = "Messmer"

# Tốc độ thanh máu chuyển động mượt.
@export var health_lerp_speed: float = 8.0

var boss: Node = null
var preview_mode: bool = false

# Lưu nguyên kích thước và scale đã thiết kế trong scene.
var full_bar_size: Vector2
var full_bar_scale: Vector2


@onready var background: Panel = $background
@onready var bar: Panel = $bar
@onready var boss_name_label: Label = $bossname


func _ready() -> void:
	# Lưu kích thước ban đầu, không tự đặt lại màu sắc.
	full_bar_size = bar.size
	full_bar_scale = bar.scale

	# Thu nhỏ thanh máu từ mép trái sang phải.
	bar.pivot_offset = Vector2.ZERO

	

	# Tìm Boss theo NodePath được gán trong Inspector.
	if not boss_path.is_empty():
		boss = get_node_or_null(boss_path)

	# Nếu chưa gán NodePath, thử tìm node tên "Boss".
	if not is_instance_valid(boss):
		var current_scene := get_tree().current_scene

		if current_scene != null:
			boss = current_scene.find_child(
				"Boss",
				true,
				false
			)

	# Chạy riêng scene HUD để xem trước.
	if not is_instance_valid(boss):
		if get_tree().current_scene == self:
			preview_mode = true
			visible = true
			bar.scale = full_bar_scale
		else:
			visible = false
			push_warning(
				"BossHUD: Không tìm thấy Boss. "
				+ "Hãy gán Boss Path trong Inspector."
			)


func _process(delta: float) -> void:
	# Giữ thanh máu đầy khi xem trước scene.
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

	# Tính phần trăm máu còn lại.
	var health_ratio: float = clampf(
		current_health / max_health,
		0.0,
		1.0
	)

	var target_scale_x: float = (
		full_bar_scale.x * health_ratio
	)

	# Làm thanh máu giảm dần, giữ nguyên kích thước gốc.
	var weight: float = clampf(
		health_lerp_speed * delta,
		0.0,
		1.0
	)

	bar.scale.x = lerpf(
		bar.scale.x,
		target_scale_x,
		weight
	)
