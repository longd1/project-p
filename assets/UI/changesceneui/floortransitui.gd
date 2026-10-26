
extends CanvasLayer

# ==================================================
# SIGNALS
# ==================================================

# option_id: 0 = +12% damage
# option_id: 1 = +8% damage, +20 Max HP
# option_id: 2 = +45 Max HP
signal reward_selected(
	option_id: int,
	damage_bonus_percent: float,
	max_health_bonus: int
)

# Phần thưởng cố định sau mỗi Floor.
signal floor_bonus_selected(
	potions: int,
	healing_bonus: int
)


# ==================================================
# NEXT FLOOR
# ==================================================

# Chọn Floor tiếp theo trong Inspector.
@export var next_floor_scene: PackedScene


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
# Đúng theo file floortransitui.tscn của cậu.
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
	# Nội dung tiêu đề.
	title_label.text = "CHOOSE YOUR REWARD"
	subtitle_label.text = (
		"Choose one upgrade before descending deeper"
	)

	# Nội dung ba lựa chọn.
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

	# Phần thưởng cố định sau mỗi Floor.
	bonus_label.text = "🎁 FLOOR BONUS"

	bonus_content.text = (
		"Gain %d healing potions, and healing "
		+ "effectiveness increases by %d HP after each floor."
	) % [
		bonus_potions,
		healing_bonus_per_floor
	]

	# Cho phép click xuyên qua các Label nằm trong Button.
	card_title_1.mouse_filter = Control.MOUSE_FILTER_IGNORE
	card_content_1.mouse_filter = Control.MOUSE_FILTER_IGNORE

	card_title_2.mouse_filter = Control.MOUSE_FILTER_IGNORE
	card_content_2.mouse_filter = Control.MOUSE_FILTER_IGNORE

	card_title_3.mouse_filter = Control.MOUSE_FILTER_IGNORE
	card_content_3.mouse_filter = Control.MOUSE_FILTER_IGNORE

	# Kết nối sự kiện chọn card.
	card_1.pressed.connect(_on_card_1_pressed)
	card_2.pressed.connect(_on_card_2_pressed)
	card_3.pressed.connect(_on_card_3_pressed)


# ==================================================
# CARD BUTTONS
# ==================================================

func _on_card_1_pressed() -> void:
	_choose_reward(
		0,
		damage_upgrade_1,
		0
	)


func _on_card_2_pressed() -> void:
	_choose_reward(
		1,
		damage_upgrade_2,
		health_upgrade_2
	)


func _on_card_3_pressed() -> void:
	_choose_reward(
		2,
		0.0,
		health_upgrade_3
	)


# ==================================================
# APPLY SELECTION AND TRANSITION
# ==================================================

func _choose_reward(
	option_id: int,
	damage_bonus_percent: float,
	max_health_bonus: int
) -> void:
	if choice_made:
		return

	# Không chuyển Floor nếu chưa cấu hình scene tiếp theo.
	if next_floor_scene == null:
		push_error(
			"FloorTransitionUI: Chưa gán Next Floor Scene "
			+ "trong Inspector."
		)
		return

	choice_made = true

	# Khóa các card để tránh chọn nhiều lần.
	card_1.disabled = true
	card_2.disabled = true
	card_3.disabled = true

	# Gửi thông tin nâng cấp cho Floor đang chạy.
	reward_selected.emit(
		option_id,
		damage_bonus_percent,
		max_health_bonus
	)

	# Gửi phần thưởng cố định của Floor.
	floor_bonus_selected.emit(
		bonus_potions,
		healing_bonus_per_floor
	)

	# Chuyển sang Floor tiếp theo.
	var error: Error = get_tree().change_scene_to_packed(
		next_floor_scene
	)

	if error != OK:
		push_error(
			"FloorTransitionUI: Không thể chuyển sang Floor tiếp theo."
		)


# ==================================================
# HELPER
# ==================================================

func _format_number(value: float) -> String:
	if value == floorf(value):
		return str(int(value))

	return str(value)
