extends Control
@onready var hp_bar = $playerStatus/HPBar
@onready var hp_text = $playerStatus/HPText
@onready var mana_bar = $playerStatus/progressBar
@onready var mana_text = $playerStatus/manaText
func update_player_status(player):
	var hp = player.get_health()
	var max_hp = player.get_max_health()

	var mana = player.get_mana()
	var max_mana = player.get_max_mana()

	hp_bar.max_value = max_hp
	hp_bar.value = hp
	hp_text.text = str(hp) + " / " + str(max_hp)

	mana_bar.max_value = max_mana
	mana_bar.value = mana
	mana_text.text = str(mana) + " / " + str(max_mana)
