extends Control


const GUIDE_UI_SCENE := "res://assets/UI/main_menu_ui/guide_ui/guide_ui.tscn"
const OPTIONS_UI_SCENE := "res://assets/UI/main_menu_ui/options_ui/options_ui.tscn"
const FLOOR_1_PATH = "res://assets/scene/floor 1/floor1_completed_scene.tscn"


@onready var start_button: Button = $buttons/start_button
@onready var guide_button: Button = $buttons/guide_button
@onready var option_button: Button = $buttons/option_button


func _ready() -> void:
	# GUIDE
	guide_button.pressed.connect(_on_guide_pressed)

	# OPTIONS
	option_button.pressed.connect(_on_options_pressed)

	# PLAY GAME
	start_button.pressed.connect(_on_start_pressed)


# ==========================================
# GUIDE
# ==========================================

func _on_guide_pressed() -> void:
	get_tree().change_scene_to_file(GUIDE_UI_SCENE)


# ==========================================
# OPTIONS
# ==========================================

func _on_options_pressed() -> void:
	get_tree().change_scene_to_file(OPTIONS_UI_SCENE)


# ==========================================
# START GAME
# ==========================================

func _on_start_pressed() -> void:
	var error = get_tree().change_scene_to_file(FLOOR_1_PATH)

	if error != OK:
		push_error(
			"Không thể mở Floor 1. Hãy kiểm tra đường dẫn scene: "
			+ FLOOR_1_PATH
		)
