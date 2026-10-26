extends Area2D


# ==========================================
# SCENES
# ==========================================

const FLOOR_TRANSIT_UI = preload(
	"res://assets/UI/changesceneui/floortransitui.tscn"
)

const NEXT_FLOOR_PATH: String = (
	"res://assets/scene/floor 2/floor2_completed_scene.tscn"
)


# ==========================================
# STATE
# ==========================================

var triggered: bool = false


# ==========================================
# EXIT
# ==========================================

func _on_body_entered(body: Node2D) -> void:
	if triggered:
		return

	if body is Player:
		triggered = true

		set_deferred("monitoring", false)

		call_deferred("_change_floor")


# ==========================================
# CHANGE FLOOR
# ==========================================

func _change_floor() -> void:
	Global.advance_floor()

	var tree := get_tree()

	var error := tree.change_scene_to_file(
		NEXT_FLOOR_PATH
	)

	if error != OK:
		push_error("Không thể load Floor tiếp theo.")
		return

	var transit_ui = FLOOR_TRANSIT_UI.instantiate()

	tree.root.add_child(transit_ui)
