
extends ColorRect

@export var first_floor_scene: PackedScene

const MAIN_MENU_PATH: String = "res://assets/UI/main_menu_ui/main_menu_ui.tscn"

@onready var play_again_button: Button = $playagain
@onready var main_menu_button: Button = $main_menu


func _ready() -> void:
	# Kết nối hai nút trong Victory UI.
	play_again_button.pressed.connect(_on_play_again_pressed)
	main_menu_button.pressed.connect(_on_main_menu_pressed)


func _on_play_again_pressed() -> void:
	# Kiểm tra Floor 1 đã được gán chưa.
	if first_floor_scene == null:
		push_error(
			"Victory UI: Chưa gán scene Floor 1 trong Inspector!"
		)
		return

	var error: Error = get_tree().change_scene_to_packed(
		first_floor_scene
	)

	if error != OK:
		push_error("Victory UI: Không thể tải Floor 1.")


func _on_main_menu_pressed() -> void:
	var error: Error = get_tree().change_scene_to_file(
		MAIN_MENU_PATH
	)

	if error != OK:
		push_error(
			"Victory UI: Không thể tải Main Menu tại: "
			+ MAIN_MENU_PATH
		)
