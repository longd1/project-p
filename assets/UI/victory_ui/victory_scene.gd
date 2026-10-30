
extends ColorRect


# ==================================================
# SCENES
# ==================================================

@export var first_floor_scene: PackedScene

const MAIN_MENU_PATH: String = \
	"res://assets/UI/main_menu_ui/main_menu_ui.tscn"


# ==================================================
# NODES
# ==================================================

@onready var play_again_button: Button = $playagain
@onready var main_menu_button: Button = $main_menu


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	if not play_again_button.pressed.is_connected(
		_on_play_again_pressed
	):
		play_again_button.pressed.connect(
			_on_play_again_pressed
		)

	if not main_menu_button.pressed.is_connected(
		_on_main_menu_pressed
	):
		main_menu_button.pressed.connect(
			_on_main_menu_pressed
		)


# ==================================================
# PLAY AGAIN
# ==================================================

func _on_play_again_pressed() -> void:
	if first_floor_scene == null:
		push_error(
			"Victory UI: Chưa gán scene Floor 1 trong Inspector!"
		)
		return


	# ==========================================
	# RESET RUN
	# ==========================================

	Global.reset_progression()


	# ==========================================
	# LOAD FLOOR 1
	# ==========================================

	var error: Error = \
		get_tree().change_scene_to_packed(
			first_floor_scene
		)


	if error != OK:
		push_error(
			"Victory UI: Không thể tải Floor 1."
		)


# ==================================================
# MAIN MENU
# ==================================================

func _on_main_menu_pressed() -> void:

	# ==========================================
	# RESET RUN
	# ==========================================

	Global.reset_progression()


	# ==========================================
	# LOAD MAIN MENU
	# ==========================================

	var error: Error = \
		get_tree().change_scene_to_file(
			MAIN_MENU_PATH
		)


	if error != OK:
		push_error(
			"Victory UI: Không thể tải Main Menu tại: "
			+ MAIN_MENU_PATH
		)
