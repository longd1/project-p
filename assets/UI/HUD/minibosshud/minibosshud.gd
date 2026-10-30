extends CanvasLayer


# ==================================================
# SETTINGS
# ==================================================

@export var miniboss_path: NodePath
@export var health_lerp_speed: float = 8.0


# ==================================================
# STATE
# ==================================================

var miniboss: Node = null
var player: Node2D = null

var preview_mode: bool = false

var full_bar_scale: Vector2


# ==================================================
# NODES
# ==================================================

@onready var mnb_background: Panel = $mnb_background
@onready var mnb_bar: Panel = $mnb_bar
@onready var miniboss_name_label: Label = $minibossname


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	full_bar_scale = mnb_bar.scale


	# ==========================================
	# FIND MINIBOSS
	# ==========================================

	if not miniboss_path.is_empty():
		miniboss = get_node_or_null(
			miniboss_path
		)


	if not is_instance_valid(miniboss):
		var current_scene := \
			get_tree().current_scene

		if current_scene != null:
			miniboss = current_scene.find_child(
				"MiniBoss",
				true,
				false
			)


	# ==========================================
	# FIND PLAYER
	# ==========================================

	player = get_tree().get_first_node_in_group(
		"player"
	) as Node2D


	# ==========================================
	# PREVIEW / INVALID TARGET
	# ==========================================

	if not is_instance_valid(miniboss):
		if get_tree().current_scene == self:
			preview_mode = true

			visible = true
			mnb_bar.scale = full_bar_scale

		else:
			visible = false

			push_warning(
				"MiniBossHUD: Không tìm thấy MiniBoss. "
				+ "Hãy gán MiniBoss Path trong Inspector."
			)

		return


	# Khi bắt đầu game, HUD chưa hiện.
	# Chỉ hiện khi John vào detection range.
	visible = false


# ==================================================
# PROCESS
# ==================================================

func _process(delta: float) -> void:
	if preview_mode:
		return


	# ==========================================
	# VALIDATE MINIBOSS
	# ==========================================

	if not is_instance_valid(miniboss):
		visible = false
		return


	# ==========================================
	# VALIDATE PLAYER
	# ==========================================

	if not is_instance_valid(player):
		player = get_tree().get_first_node_in_group(
			"player"
		) as Node2D

		if not is_instance_valid(player):
			visible = false
			return


	# ==========================================
	# REQUIRED METHODS
	# ==========================================

	if not miniboss.has_method("get_health"):
		visible = false
		return


	if not miniboss.has_method("get_max_health"):
		visible = false
		return


	if not miniboss.has_method("get_far_range"):
		visible = false
		return


	# ==========================================
	# CHECK DETECTION RANGE
	# ==========================================

	if not miniboss is Node2D:
		visible = false
		return


	var miniboss_2d := miniboss as Node2D


	var distance_to_player: float = \
		miniboss_2d.global_position.distance_to(
			player.global_position
		)


	var detection_range: float = float(
		miniboss.get_far_range()
	)


	# John nằm ngoài tầm phát hiện.
	if distance_to_player > detection_range:
		visible = false
		return


	# ==========================================
	# HEALTH
	# ==========================================

	var current_health: float = float(
		miniboss.get_health()
	)


	var max_health: float = float(
		miniboss.get_max_health()
	)


	if max_health <= 0.0:
		visible = false
		return


	if current_health <= 0.0:
		visible = false
		return


	# ==========================================
	# SHOW HUD
	# ==========================================

	visible = true


	# ==========================================
	# HEALTH RATIO
	# ==========================================

	var health_ratio: float = clampf(
		current_health / max_health,
		0.0,
		1.0
	)


	var target_scale_x: float = \
		full_bar_scale.x * health_ratio


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
