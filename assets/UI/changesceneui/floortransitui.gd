extends CanvasLayer


# ==================================================
# UPGRADE SETTINGS
# Có thể chỉnh các giá trị này trong Inspector.
# ==================================================

@export_group("Card 1 - Berserker")
@export var damage_upgrade_1: float = 12.0

@export_group("Card 2 - Battle-Hardened")
@export var damage_upgrade_2: float = 8.0
@export var health_upgrade_2: int = 20

@export_group("Card 3 - Vitality")
@export var health_upgrade_3: int = 45

@export_group("Floor Bonus")
@export var bonus_potions: int = 2
@export var healing_bonus_per_floor: int = 20


# ==================================================
# STATE
# ==================================================

var choice_made: bool = false


# ==================================================
# UI NODES
# ==================================================

@onready var title_label: Label = $title
@onready var subtitle_label: Label = $subtitle

@onready var card_1: Button = $card1
@onready var card_2: Button = $card2
@onready var card_3: Button = $card3

@onready var card_title_1: Label = $card1/title1
@onready var card_content_1: Label = $card1/content1

@onready var card_title_2: Label = $card2/title2
@onready var card_content_2: Label = $card2/content2

@onready var card_title_3: Label = $card3/title3
@onready var card_content_3: Label = $card3/content3

@onready var bonus_label: Label = $bonus
@onready var bonus_content: Label = $bonus/bonuscontent


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	show()

	title_label.text = "CHOOSE YOUR REWARD"
	subtitle_label.text = "Choose one upgrade before descending deeper"

	card_title_1.text = (
		"⚔\nBERSERKER\n\n+%s%%"
		% _format_number(damage_upgrade_1)
	)
	card_content_1.text = "Damage"

	card_title_2.text = (
		"✦\nBATTLE-HARDENED\n\n+%s%%\n+%d"
		% [
			_format_number(damage_upgrade_2),
			health_upgrade_2
		]
	)
	card_content_2.text = "Damage\n\nMax HP"

	card_title_3.text = (
		"♥\nVITALITY\n\n+%d"
		% health_upgrade_3
	)
	card_content_3.text = "Max HP"

	bonus_label.text = "🎁 FLOOR BONUS"

	bonus_content.text = (
		"Gain %d healing potions, and healing "
		+ "effectiveness increases by %d HP after each floor."
	) % [
		bonus_potions,
		healing_bonus_per_floor
	]

	card_title_1.mouse_filter = Control.MOUSE_FILTER_IGNORE
	card_content_1.mouse_filter = Control.MOUSE_FILTER_IGNORE

	card_title_2.mouse_filter = Control.MOUSE_FILTER_IGNORE
	card_content_2.mouse_filter = Control.MOUSE_FILTER_IGNORE

	card_title_3.mouse_filter = Control.MOUSE_FILTER_IGNORE
	card_content_3.mouse_filter = Control.MOUSE_FILTER_IGNORE

	card_1.pressed.connect(_on_card_1_pressed)
	card_2.pressed.connect(_on_card_2_pressed)
	card_3.pressed.connect(_on_card_3_pressed)


# ==================================================
# CARD 1 - BERSERKER
# +12% DAMAGE
# ==================================================

func _on_card_1_pressed() -> void:
	_choose_reward(
		damage_upgrade_1,
		0
	)


# ==================================================
# CARD 2 - BATTLE-HARDENED
# +8% DAMAGE
# +20 MAX HP
# ==================================================

func _on_card_2_pressed() -> void:
	_choose_reward(
		damage_upgrade_2,
		health_upgrade_2
	)


# ==================================================
# CARD 3 - VITALITY
# +45 MAX HP
# ==================================================

func _on_card_3_pressed() -> void:
	_choose_reward(
		0.0,
		health_upgrade_3
	)


# ==================================================
# APPLY REWARD
# ==================================================

func _choose_reward(
	damage_bonus_percent: float,
	max_health_bonus: int
) -> void:
	if choice_made:
		return

	choice_made = true

	card_1.disabled = true
	card_2.disabled = true
	card_3.disabled = true


	# ==========================================
	# DAMAGE
	# ==========================================

	if damage_bonus_percent > 0.0:
		Global.upgrade_damage(
			damage_bonus_percent / 100.0
		)


	# ==========================================
	# MAX HEALTH
	# ==========================================

	if max_health_bonus > 0:
		Global.upgrade_health(
			max_health_bonus
		)


	# ==========================================
	# REFRESH CURRENT PLAYER
	# ==========================================

	var player = get_tree().get_first_node_in_group("player")

	if player != null:
		if player.has_method("refresh_progression"):
			player.refresh_progression()


	# ==========================================
	# CLOSE UI
	# ==========================================

	hide()
	queue_free()

# ==================================================
# HELPER
# ==================================================

func _format_number(value: float) -> String:
	if value == floorf(value):
		return str(int(value))

	return str(value)
