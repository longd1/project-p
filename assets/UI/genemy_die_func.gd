
extends CanvasLayer


# ==================================================
# TYPES
# ==================================================

enum RewardType {
	NONE,
	WEAPON
}


enum AftermathType {
	NOTHING,
	OPEN_GATE
}


# ==================================================
# TARGET
# ==================================================

@export_group("Target")

@export var target: Node


# ==================================================
# DEFEAT PRESENTATION
# ==================================================

@export_group("Defeat Presentation")

@export_range(0.0, 10.0, 0.1)
var video_delay: float = 1.0


# ==================================================
# REWARD
# ==================================================

@export_group("Reward")

@export var reward_type: RewardType = \
	RewardType.NONE


# Scene ground weapon dùng chung.
# Ví dụ:
# ground_weapon.tscn
@export var weapon_drop_scene: PackedScene


# Weapon cụ thể được drop.
# Ví dụ:
# katana2.tres
# butcher_machete.tres
@export var weapon_data: Resource


# ==================================================
# AFTERMATH
# ==================================================

@export_group("Aftermath")

@export var aftermath_type: AftermathType = \
	AftermathType.NOTHING


@export var end_gate: Node2D

@export var end_gate_collision: CollisionShape2D


# ==================================================
# NODES
# ==================================================

@onready var screen: Control = \
	$Screen


@onready var enemy_felled_video: VideoStreamPlayer = \
	$Screen/EnemyFelledVideo


@onready var defeat_sound: AudioStreamPlayer = \
	$DefeatSound


# ==================================================
# STATE
# ==================================================

var defeat_started: bool = false


# ==================================================
# READY
# ==================================================

func _ready() -> void:

	# ==========================================
	# PRESENTATION
	# ==========================================

	enemy_felled_video.hide()

	screen.mouse_filter = \
		Control.MOUSE_FILTER_IGNORE

	enemy_felled_video.mouse_filter = \
		Control.MOUSE_FILTER_IGNORE


	# ==========================================
	# CONNECT TARGET
	# ==========================================

	if target == null:
		push_error(
			"DefeatHandler: Chưa gán Target."
		)

	elif not target.has_signal("died"):
		push_error(
			"DefeatHandler: Target không có signal died."
		)

	elif not target.died.is_connected(
		_on_target_died
	):
		target.died.connect(
			_on_target_died
		)


	# ==========================================
	# END GATE INITIAL STATE
	# ==========================================

	if aftermath_type == AftermathType.OPEN_GATE:
		_disable_end_gate()


# ==================================================
# TARGET DIED
# ==================================================

func _on_target_died(
	dead_target = null
) -> void:

	if defeat_started:
		return

	defeat_started = true


	# ==========================================
	# SAVE DEATH POSITION
	# ==========================================

	var death_position: Vector2 = \
		Vector2.ZERO


	if dead_target is Node2D:
		death_position = \
			dead_target.global_position

	elif target is Node2D:
		death_position = \
			target.global_position


	print(
		"DefeatHandler: Target defeated at ",
		death_position
	)


	# ==========================================
	# SOUND STARTS IMMEDIATELY
	# ==========================================

	if defeat_sound.stream != null:
		defeat_sound.play()


	# ==========================================
	# VIDEO DELAY
	# ==========================================

	if video_delay > 0.0:
		await get_tree().create_timer(
			video_delay
		).timeout


	# ==========================================
	# VIDEO START
	# ==========================================

	if enemy_felled_video.stream != null:
		enemy_felled_video.show()

		enemy_felled_video.play()

		print(
			"DefeatHandler: "
			+ "Enemy Felled video started."
		)


	# ==========================================
	# PROCESS RESULT
	#
	# Reward / portal được xử lý ngay khi
	# video bắt đầu phát.
	# ==========================================

	_handle_reward(
		death_position
	)

	_handle_aftermath()


	# ==========================================
	# WAIT FOR VIDEO END
	# ==========================================

	if enemy_felled_video.stream != null:
		await enemy_felled_video.finished

		enemy_felled_video.hide()


# ==================================================
# HANDLE REWARD
# ==================================================

func _handle_reward(
	drop_position: Vector2
) -> void:

	match reward_type:

		RewardType.NONE:
			return


		RewardType.WEAPON:
			_spawn_weapon(
				drop_position
			)


# ==================================================
# SPAWN WEAPON
# ==================================================

func _spawn_weapon(
	drop_position: Vector2
) -> void:

	# ==========================================
	# VALIDATE
	# ==========================================

	if weapon_drop_scene == null:
		push_error(
			"DefeatHandler: "
			+ "Reward Type = WEAPON nhưng "
			+ "chưa gán Weapon Drop Scene."
		)
		return


	if weapon_data == null:
		push_error(
			"DefeatHandler: "
			+ "Reward Type = WEAPON nhưng "
			+ "chưa gán Weapon Data."
		)
		return


	# ==========================================
	# INSTANTIATE
	# ==========================================

	var spawned_weapon = \
		weapon_drop_scene.instantiate()


	if spawned_weapon == null:
		push_error(
			"DefeatHandler: "
			+ "Không thể instantiate "
			+ "Weapon Drop Scene."
		)
		return


	# ==========================================
	# CONFIGURE WEAPON BEFORE _READY
	#
	# Quan trọng:
	# setup_weapon() chạy trước add_child(),
	# nên khi GroundWeapon._ready() chạy,
	# nó đã có đúng katana/machete/etc.
	# ==========================================

	if not spawned_weapon.has_method(
		"setup_weapon"
	):
		push_error(
			"DefeatHandler: "
			+ "GroundWeapon không có "
			+ "setup_weapon()."
		)

		spawned_weapon.queue_free()
		return


	spawned_weapon.setup_weapon(
		weapon_data
	)


	# ==========================================
	# GET CURRENT FLOOR
	# ==========================================

	var current_scene = \
		get_tree().current_scene


	if current_scene == null:
		push_error(
			"DefeatHandler: "
			+ "Không tìm thấy current scene."
		)

		spawned_weapon.queue_free()
		return


	# ==========================================
	# ADD WEAPON
	# ==========================================

	current_scene.add_child(
		spawned_weapon
	)


	# ==========================================
	# POSITION
	# ==========================================

	if spawned_weapon is Node2D:
		spawned_weapon.global_position = \
			drop_position


	print(
		"DefeatHandler: Weapon spawned at ",
		drop_position
	)

	print(
		"DefeatHandler: Weapon data = ",
		weapon_data
	)


# ==================================================
# HANDLE AFTERMATH
# ==================================================

func _handle_aftermath() -> void:

	match aftermath_type:

		AftermathType.NOTHING:
			return


		AftermathType.OPEN_GATE:
			_enable_end_gate()


# ==================================================
# DISABLE END GATE
# ==================================================

func _disable_end_gate() -> void:

	if end_gate != null:
		end_gate.hide()


	if end_gate_collision != null:
		end_gate_collision.set_deferred(
			"disabled",
			true
		)


# ==================================================
# ENABLE END GATE
# ==================================================

func _enable_end_gate() -> void:

	if end_gate == null:
		push_error(
			"DefeatHandler: "
			+ "Aftermath Type = OPEN_GATE nhưng "
			+ "chưa gán End Gate."
		)
		return


	end_gate.show()


	if end_gate_collision != null:
		end_gate_collision.set_deferred(
			"disabled",
			false
		)


	print(
		"DefeatHandler: End gate opened."
	)
