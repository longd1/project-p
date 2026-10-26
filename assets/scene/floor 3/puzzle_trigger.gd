extends Area2D


# ==========================================
# PUZZLE SCENE
# ==========================================

@export var puzzle_scene: PackedScene


# ==========================================
# STATE
# ==========================================

var activated: bool = false
var puzzle_open: bool = false


# ==========================================
# READY
# ==========================================

func _ready() -> void:
	body_entered.connect(_on_body_entered)


# ==========================================
# PLAYER ENTER
# ==========================================

func _on_body_entered(body: Node2D) -> void:
	if activated:
		return

	if puzzle_open:
		return

	if not body is Player:
		return

	open_puzzle()


# ==========================================
# OPEN PUZZLE
# ==========================================

func open_puzzle() -> void:
	if puzzle_scene == null:
		push_error("PuzzleTrigger: Chưa gán Puzzle Scene.")
		return

	puzzle_open = true

	var puzzle = puzzle_scene.instantiate()

	get_tree().current_scene.add_child(puzzle)

	puzzle.challenge_finished.connect(
		_on_challenge_finished
	)


# ==========================================
# PUZZLE RESULT
# ==========================================

func _on_challenge_finished(success: bool) -> void:
	puzzle_open = false

	if success:
		activated = true

		monitoring = false

		print("Puzzle completed!")
	else:
		print("Puzzle failed!")
