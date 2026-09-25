extends Control


@onready var back_button: Button = $back_button


func _ready() -> void:
	back_button.pressed.connect(_on_back_pressed)


func _on_back_pressed() -> void:
	get_tree().change_scene_to_file(
		"res://assets/UI/main_menu_ui/main_menu_ui.tscn"
	)
