
extends CanvasLayer


# ==================================================
# SIGNALS
# ==================================================

signal reward_selected(
	option_id: int,
	damage_bonus_percent: float,
	max_health_bonus: int
)


# ==================================================
# UPGRADE SETTINGS
# Giá trị mặc định khớp với floortransitui.tscn.
# ==================================================

@export_group("Card 1 - Berserker")
@export var damage_upgrade_1: float = 12.0

@export_group("Card 2 - Balanced")
@export var damage_upgrade_2: float = 6.0
@export var health_upgrade_2: int = 25

@export_group("Card 3 - Vitality")
@export var health_upgrade_3: int = 50


# ==================================================
# STATE
# ==================================================

var choice_made: bool = false


# ==================================================
# UI NODES
# ==================================================

@onready var card_1: Button = $card1
@onready var card_2: Button = $card2
@onready var card_3: Button = $card3

@onready var title_1: Label = $card1/title1
@onready var content_1: Label = $card1/content1

@onready var title_2: Label = $card2/title2
@onready var content_2: Label = $card2/content2

@onready var title_3: Label = $card3/title3
@onready var content_3: Label = $card3/content3


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	# Label không được chặn click của Button.
	title_1.mouse_filter = Control.MOUSE_FILTER_IGNORE
	content_1.mouse_filter = Control.MOUSE_FILTER_IGNORE

	title_2.mouse_filter = Control.MOUSE_FILTER_IGNORE
	content_2.mouse_filter = Control.MOUSE_FILTER_IGNORE

	title_3.mouse_filter = Control.MOUSE_FILTER_IGNORE
	content_3.mouse_filter = Control.MOUSE_FILTER_IGNORE


	# ==========================================
	# CONNECT BUTTONS
	# ==========================================

	if not card_1.pressed.is_connected(_on_card_1_pressed):
		card_1.pressed.connect(_on_card_1_pressed)

	if not card_2.pressed.is_connected(_on_card_2_pressed):
		card_2.pressed.connect(_on_card_2_pressed)

	if not card_3.pressed.is_connected(_on_card_3_pressed):
		card_3.pressed.connect(_on_card_3_pressed)


# ==================================================
# CARD 1 - BERSERKER
# ==================================================

func _on_card_1_pressed() -> void:
	_choose_reward(
		0,
		damage_upgrade_1,
		0
	)


# ==================================================
# CARD 2 - BALANCED
# ==================================================

func _on_card_2_pressed() -> void:
	_choose_reward(
		1,
		damage_upgrade_2,
		health_upgrade_2
	)


# ==================================================
# CARD 3 - VITALITY
# ==================================================

func _on_card_3_pressed() -> void:
	_choose_reward(
		2,
		0.0,
		health_upgrade_3
	)


# ==================================================
# CHOOSE REWARD
# ==================================================

func _choose_reward(
	option_id: int,
	damage_bonus_percent: float,
	max_health_bonus: int
) -> void:
	if choice_made:
		return

	choice_made = true


	# ==========================================
	# DISABLE CARDS
	# ==========================================

	card_1.disabled = true
	card_2.disabled = true
	card_3.disabled = true


	# ==========================================
	# APPLY DAMAGE UPGRADE
	# ==========================================

	if damage_bonus_percent > 0.0:
		Global.upgrade_damage(
			damage_bonus_percent / 100.0
		)


	# ==========================================
	# APPLY HEALTH UPGRADE
	# ==========================================

	if max_health_bonus > 0:
		Global.upgrade_health(
			max_health_bonus
		)


	# ==========================================
	# NOTIFY OTHER SYSTEMS
	# ==========================================

	reward_selected.emit(
		option_id,
		damage_bonus_percent,
		max_health_bonus
	)


	# ==========================================
	# REFRESH PLAYER
	# ==========================================

	var player = get_tree().get_first_node_in_group(
		"player"
	)

	if player != null:
		if player.has_method("refresh_progression"):
			player.refresh_progression()


	# ==========================================
	# CLOSE UI
	# ==========================================

	queue_free()
