extends Area2D


# ==================================================
# SCENE PATHS
# ==================================================

const NEXT_FLOOR_PATH: String = \
	"res://assets/scene/floor 5/floor 5.tscn"

const FLOOR_TRANSITION_UI: PackedScene = preload(
	"res://assets/UI/floortransitui.tscn"
)


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
		"_change_floor"
	)


# ==================================================
# CHANGE FLOOR
# ==================================================

func _change_floor() -> void:
	var tree: SceneTree = get_tree()

	if tree == null:
		triggered = false
		return


	# ==========================================
	# CREATE UPGRADE UI
	# ==========================================

	var transition_ui = \
		FLOOR_TRANSITION_UI.instantiate()

	tree.root.add_child(
		transition_ui
	)


	# ==========================================
	# PROGRESSION
	# ==========================================

	Global.advance_floor()


	# ==========================================
	# LOAD FLOOR 5
	# ==========================================

	var error: Error = tree.change_scene_to_file(
		NEXT_FLOOR_PATH
	)

	if error != OK:
		push_error(
			"Floor 4 Exit: Không thể load Floor 5."
		)

		transition_ui.queue_free()

		triggered = false
		return
