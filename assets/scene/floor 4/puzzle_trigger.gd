
extends Area2D

@export var puzzle_scene: PackedScene

var activated: bool = false


func _ready() -> void:
	body_entered.connect(_on_body_entered)


func _on_body_entered(body: Node2D) -> void:
	if activated:
		return

	# Nhận diện Player bằng tên hoặc group.
	if body.name != "Player" and not body.is_in_group("player"):
		return

	if puzzle_scene == null:
		push_error("Chưa gán puzzle_challenge.tscn vào Puzzle Scene!")
		return

	activated = true
	monitoring = false

	var puzzle = puzzle_scene.instantiate()

	# Nhận kết quả trước khi đưa giao diện vào scene tree.
	puzzle.challenge_finished.connect(_on_challenge_finished)

	get_tree().current_scene.add_child(puzzle)


func _on_challenge_finished(success: bool) -> void:
	if success:
		# Đóng giao diện câu đố.
		# Player vẫn ở Floor 4 và có thể tiếp tục khám phá.
		return

	# Thất bại: tự động quay về Main Menu.
	get_tree().change_scene_to_file(
        "res://assets/UI/main_menu_ui.tscn"
	)
