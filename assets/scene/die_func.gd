extends CanvasLayer

@export var video_delay: float = 1.2

@onready var player: Player = $"../Player"
@onready var death_video: VideoStreamPlayer = $VideoStreamPlayer


func _ready() -> void:
	death_video.hide()

	player.died.connect(_on_player_died)
	death_video.finished.connect(_on_death_video_finished)


func _on_player_died() -> void:
	await get_tree().create_timer(video_delay).timeout

	death_video.show()
	death_video.play()


func _on_death_video_finished() -> void:
	death_video.hide()
	
