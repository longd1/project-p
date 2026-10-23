
extends Area2D

@export var puzzle_scene: PackedScene

# Chỉ true sau khi Player đã vượt qua thử thách.
var activated: bool = false

# Ngăn mở nhiều bảng thử thách cùng lúc.
var challenge_running: bool = false


func _ready() -> void:
	body_entered.connect(_on_body_entered)


func _on_body_entered(body: Node2D) -> void:
	# Đã vượt qua hoặc thử thách đang chạy thì không mở lại.
	if activated or challenge_running:
		return

	# Chỉ kích hoạt khi Player đi vào vùng thử thách.
	if body.name != "Player" and not body.is_in_group("player"):
		return

	if puzzle_scene == null:
		push_error(
			"Chưa gán puzzle_challenge.tscn vào Puzzle Scene!"
		)
		return

	challenge_running = true

	# Tạo giao diện thử thách trên Floor 4 hiện tại.
	var puzzle = puzzle_scene.instantiate()

	puzzle.challenge_finished.connect(
		_on_challenge_finished
	)

	get_tree().current_scene.add_child(puzzle)


func _on_challenge_finished(success: bool) -> void:
	challenge_running = false

	if success:
		# Ghi nhớ rằng Player đã vượt qua thử thách.
		# Khi quay lại Room 6, puzzle sẽ không mở lại.
		activated = true
		return

	# Thất bại: trở về Main Menu.
	get_tree().change_scene_to_file(
		"res://assets/UI/main_menu_ui/main_menu_ui.tscn"
	)
