
extends Area2D


# ==================================================
# TELEPORT
# ==================================================

@export_group("Teleport")

@export var player_target: Marker2D
@export var camera_target: Marker2D

@export var camera: Camera2D

@export var camera_zoom: Vector2 = \
	Vector2(1.15, 1.15)


# ==================================================
# BOSS
# ==================================================

@export_group("Boss")

@export var boss: Node


# ==================================================
# BOSS MUSIC
# ==================================================

@export_group("Boss Music")

@export var boss_music: AudioStreamPlayer

@export_range(0.1, 10.0, 0.1)
var music_fade_out_time: float = 4.0

@export var fade_out_volume_db: float = -60.0


# ==================================================
# STATE
# ==================================================

var music_started: bool = false
var music_fading: bool = false

var original_music_volume_db: float = 0.0


# ==================================================
# READY
# ==================================================

func _ready() -> void:

	# ==========================================
	# SAVE ORIGINAL MUSIC VOLUME
	# ==========================================

	if boss_music != null:
		original_music_volume_db = \
			boss_music.volume_db


	# ==========================================
	# CONNECT BOSS DEATH
	# ==========================================

	if boss == null:
		push_warning(
			"Boss Room: Boss chưa được gán."
		)

	elif not boss.has_signal("died"):
		push_warning(
			"Boss Room: Boss không có signal died."
		)

	elif not boss.died.is_connected(
		_on_boss_died
	):
		boss.died.connect(
			_on_boss_died
		)


# ==================================================
# PLAYER ENTER BOSS ROOM
# ==================================================

func _on_body_entered(
	body: Node2D
) -> void:

	if not body is Player:
		return


	# ==========================================
	# TELEPORT PLAYER
	# ==========================================

	if player_target != null:
		body.global_position = \
			player_target.global_position


	# ==========================================
	# MOVE CAMERA
	# ==========================================

	if camera != null:

		if camera_target != null:
			camera.global_position = \
				camera_target.global_position

		camera.zoom = camera_zoom


	# ==========================================
	# START BOSS MUSIC
	# ==========================================

	_start_boss_music()


# ==================================================
# START MUSIC
# ==================================================

func _start_boss_music() -> void:

	if music_started:
		return


	if boss_music == null:
		push_warning(
			"Boss Room: Boss Music chưa được gán."
		)
		return


	if boss_music.stream == null:
		push_warning(
			"Boss Room: Boss Music chưa có Audio Stream."
		)
		return


	music_started = true


	# Đảm bảo volume quay về mức ban đầu.
	boss_music.volume_db = \
		original_music_volume_db


	boss_music.play()


	print(
		"Boss Room: Boss music started."
	)


# ==================================================
# BOSS DIED
# ==================================================

func _on_boss_died(
	_boss = null
) -> void:

	if music_fading:
		return


	if boss_music == null:
		return


	if not boss_music.playing:
		return


	music_fading = true


	print(
		"Boss Room: Boss died. "
		+ "Fading music..."
	)


	# ==========================================
	# FADE MUSIC
	# ==========================================

	var tween: Tween = \
		create_tween()


	tween.set_trans(
		Tween.TRANS_LINEAR
	)


	tween.set_ease(
		Tween.EASE_IN_OUT
	)


	tween.tween_property(
		boss_music,
		"volume_db",
		fade_out_volume_db,
		music_fade_out_time
	)


	tween.finished.connect(
		_on_music_fade_finished
	)


# ==================================================
# MUSIC FADE FINISHED
# ==================================================

func _on_music_fade_finished() -> void:

	if boss_music == null:
		return


	boss_music.stop()


	# Reset volume để lần chạy sau
	# nhạc không bắt đầu ở -60 dB.
	boss_music.volume_db = \
		original_music_volume_db


	music_fading = false


	print(
		"Boss Room: Boss music stopped."
	)
