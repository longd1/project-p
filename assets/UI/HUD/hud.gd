extends Control
@onready var hp_bar = $playerStatus/HPBar
@onready var hp_text = $playerStatus/HPText
func update_player_status(player):
	var hp = player.get_health()
	var max_hp = player.get_max_health()

	hp_bar.max_value = max_hp
	hp_bar.value = hp
	hp_text.text = str(hp) + " / " + str(max_hp)
