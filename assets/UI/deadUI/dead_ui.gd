extends Control


@onready var main_menu_button: Button = $dead_panel/main_menu_button


func _ready() -> void:
	main_menu_button.pressed.connect(_on_main_menu_pressed)

	hide()


func show_dead_ui() -> void:
	show()


func _on_main_menu_pressed() -> void:
	get_tree().change_scene_to_file("res://assets/UI/main_menu_ui/main_menu_ui.tscn")
