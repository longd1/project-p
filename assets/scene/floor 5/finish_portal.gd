extends Area2D


# ==================================================
# SCENE
# ==================================================

const VICTORY_SCENE_PATH: String = \
	"res://assets/UI/victory_ui/victory_scene.tscn"


# ==================================================
# STATE
# ==================================================

var triggered: bool = false


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	if not body_entered.is_connected(
		_on_body_entered
	):
		body_entered.connect(
			_on_body_entered
		)


# ==================================================
# PLAYER ENTER
# ==================================================

func _on_body_entered(
	body: Node2D
) -> void:

	if triggered:
		return

	if not body is Player:
		return

	triggered = true

	call_deferred(
		"_open_victory_scene"
	)


# ==================================================
# VICTORY
# ==================================================

func _open_victory_scene() -> void:

	var error: Error = \
		get_tree().change_scene_to_file(
			VICTORY_SCENE_PATH
		)


	if error != OK:
		push_error(
			"FinishPortal: "
			+ "Không thể load Victory Scene."
		)

		triggered = false
