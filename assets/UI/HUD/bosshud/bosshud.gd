
extends CanvasLayer

@export var boss_path: NodePath
@export var activation_area_path: NodePath = NodePath(
	"../../tp_portal/room2_to_centre"
)
@export var health_lerp_speed: float = 8.0

var boss: Node = null
var activation_area: Area2D = null

var preview_mode: bool = false
var boss_room_entered: bool = false

var full_bar_scale: Vector2


@onready var background: Panel = $background
@onready var bar: Panel = $bar
@onready var boss_name_label: Label = $bossname


func _ready() -> void:
	# Lưu scale ban đầu của thanh máu.
	full_bar_scale = bar.scale
	bar.pivot_offset = Vector2.ZERO

	# Không thay đổi vị trí, kích thước, màu sắc
	# hoặc nội dung của bossname.

	# Tìm Boss.
	if not boss_path.is_empty():
		boss = get_node_or_null(boss_path)

	if not is_instance_valid(boss):
		var current_scene := get_tree().current_scene

		if current_scene != null:
			boss = current_scene.find_child(
				"Boss",
				true,
				false
			)

	# Khi chạy riêng bosshud.tscn, chỉ dùng để xem trước.
	if get_tree().current_scene == self:
		preview_mode = true
		visible = true
		bar.scale = full_bar_scale
		return

	# Khi chạy Floor 5, mặc định ẩn Boss HUD.
	boss_room_entered = false
	visible = false

	# Tìm vùng kích hoạt khi Player đi vào phòng Boss.
	if not activation_area_path.is_empty():
		activation_area = get_node_or_null(
			activation_area_path
		) as Area2D

	if is_instance_valid(activation_area):
		if not activation_area.body_entered.is_connected(
			_on_activation_area_body_entered
		):
			activation_area.body_entered.connect(
				_on_activation_area_body_entered
			)
	else:
		push_warning(
			"BossHUD: Không tìm thấy vùng kích hoạt. "
			+ "Hãy kiểm tra Activation Area Path."
		)

	if not is_instance_valid(boss):
		push_warning(
			"BossHUD: Không tìm thấy node Boss."
		)


func _on_activation_area_body_entered(body: Node2D) -> void:
	if boss_room_entered:
		return

	# Chỉ kích hoạt khi Player đi vào vùng này.
	if body.name != "Player" and not body.is_in_group("player"):
		return

	if not is_instance_valid(boss):
		return

	boss_room_entered = true
	visible = true


func _process(delta: float) -> void:
	# Chạy riêng HUD để kiểm tra giao diện.
	if preview_mode:
		return

	# Chưa vào phòng Boss thì giữ HUD ẩn.
	if not boss_room_entered:
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

	# Tính tỷ lệ máu còn lại.
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

	# Chỉ cập nhật lượng máu, không thay đổi giao diện gốc.
	bar.scale.x = lerpf(
		bar.scale.x,
		target_scale_x,
		weight
	)
