extends CanvasLayer


# ==========================================
# SETTINGS
# ==========================================

@export var video_delay: float = 0.0

const FLOOR_1_PATH: String = \
	"res://assets/scene/floor 1/floor1_completed_scene.tscn"


# ==========================================
# NODES
# ==========================================

@onready var player = get_tree().get_first_node_in_group("player")
@onready var death_video: VideoStreamPlayer = $VideoStreamPlayer


# ==========================================
# STATE
# ==========================================

var death_sequence_started: bool = false


# ==========================================
# READY
# ==========================================

func _ready() -> void:
	death_video.hide()

	if player != null:
		player.died.connect(_on_player_died)

	death_video.finished.connect(_on_death_video_finished)


# ==========================================
# PLAYER DIED
# ==========================================

func _on_player_died() -> void:
	trigger_death_sequence()


# ==========================================
# TRIGGER DEATH
# Có thể được gọi bởi Player, Timer, puzzle...
# ==========================================

func trigger_death_sequence() -> void:
	if death_sequence_started:
		return

	death_sequence_started = true

	await get_tree().create_timer(video_delay).timeout

	death_video.show()
	death_video.play()

	_on_death_video_started()


# ==========================================
# VIDEO STARTED
# Được gọi NGAY KHI video bắt đầu phát.
# ==========================================

func _on_death_video_started() -> void:
	print("Death video started")


# ==========================================
# VIDEO FINISHED
# ==========================================

func _on_death_video_finished() -> void:
	death_video.hide()

	Global.reset_progression()

	get_tree().change_scene_to_file(
		FLOOR_1_PATH
	)
