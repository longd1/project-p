extends Control


# ==========================================
# SCENE PATHS
# ==========================================

const FLOOR_1_PATH: String = \
	"res://assets/scene/floor 1/floor1_completed_scene.tscn"

const MAIN_MENU_PATH: String = \
	"res://assets/UI/main_menu_ui/main_menu_ui.tscn"


# ==========================================
# NODES
# ==========================================

@onready var retry_button: Button = \
	$dead_panel/retry_button

@onready var main_menu_button: Button = \
	$dead_panel/main_menu_button


# ==========================================
# READY
# ==========================================

func _ready() -> void:
	retry_button.pressed.connect(
		_on_retry_pressed
	)

	main_menu_button.pressed.connect(
		_on_main_menu_pressed
	)

	hide()


# ==========================================
# SHOW DEAD UI
# ==========================================

func show_dead_ui() -> void:
	show()


# ==========================================
# RETRY
# ==========================================

func _on_retry_pressed() -> void:
	Global.reset_progression()

	get_tree().change_scene_to_file(
		FLOOR_1_PATH
	)


# ==========================================
# MAIN MENU
# ==========================================

func _on_main_menu_pressed() -> void:
	Global.reset_progression()

	get_tree().change_scene_to_file(
		MAIN_MENU_PATH
	)
