extends Control


@onready var volume_button: OptionButton = $options_content/volume_row/volume_button
@onready var gamemode_button: OptionButton = $options_content/gamemode_row/gamemode_button
@onready var back_button: Button = $back_button


var current_volume: String = "ON"
var current_game_mode: String = "NORMAL"


func _ready() -> void:
	# Setup Volume
	_setup_volume()
	volume_button.item_selected.connect(_on_volume_selected)

	# Setup Game Mode
	_setup_game_modes()
	gamemode_button.item_selected.connect(_on_gamemode_selected)

	# Back button
	back_button.pressed.connect(_on_back_pressed)


# ==========================================
# VOLUME
# ==========================================

func _setup_volume() -> void:
	volume_button.clear()

	volume_button.add_item("ON")
	volume_button.add_item("OFF")

	# ON mặc định
	volume_button.select(0)
	current_volume = "ON"


func _on_volume_selected(index: int) -> void:
	current_volume = volume_button.get_item_text(index)

	print("Volume: ", current_volume)


# ==========================================
# GAME MODE
# ==========================================

func _setup_game_modes() -> void:
	gamemode_button.clear()

	gamemode_button.add_item("EASY")
	gamemode_button.add_item("NORMAL")
	gamemode_button.add_item("HARD")

	# NORMAL mặc định
	gamemode_button.select(1)
	current_game_mode = "NORMAL"


func _on_gamemode_selected(index: int) -> void:
	current_game_mode = gamemode_button.get_item_text(index)

	print("Game Mode: ", current_game_mode)


# ==========================================
# BACK BUTTON
# ==========================================

func _on_back_pressed() -> void:
	get_tree().change_scene_to_file(
		"res://assets/UI/main_menu_ui/main_menu_ui.tscn"
	)
