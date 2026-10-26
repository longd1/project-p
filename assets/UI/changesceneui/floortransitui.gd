
extends CanvasLayer

# ==================================================
# SIGNALS
# ==================================================

signal reward_selected(
	option_id: int,
	damage_bonus_percent: float,
	max_health_bonus: int
)

signal floor_bonus_selected(
	potions: int,
	healing_bonus: int
)


# ==================================================
# NEXT FLOOR
# ==================================================

# Chọn scene Floor tiếp theo trong Inspector.
@export var next_floor_scene: PackedScene


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

@export_group("Floor Bonus")
@export var bonus_potions: int = 2
@export var healing_bonus_per_floor: int = 20


# ==================================================
# STATE
# ==================================================

var choice_made: bool = false


# ==================================================
# UI NODES
# Giữ đúng tên và cấu trúc trong scene của cậu.
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
# Không ghi đè text, position, size, màu hoặc style.
# ==================================================

func _ready() -> void:
	# Đảm bảo có thể bấm Button kể cả khi click lên Label.
	title_1.mouse_filter = Control.MOUSE_FILTER_IGNORE
	content_1.mouse_filter = Control.MOUSE_FILTER_IGNORE

	title_2.mouse_filter = Control.MOUSE_FILTER_IGNORE
	content_2.mouse_filter = Control.MOUSE_FILTER_IGNORE

	title_3.mouse_filter = Control.MOUSE_FILTER_IGNORE
	content_3.mouse_filter = Control.MOUSE_FILTER_IGNORE

	# Kết nối nút chọn card.
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
# CHOOSE REWARD AND CHANGE FLOOR
# ==================================================

func _choose_reward(
	option_id: int,
	damage_bonus_percent: float,
	max_health_bonus: int
) -> void:
	if choice_made:
		return

	if next_floor_scene == null:
		push_error(
			"FloorTransitionUI: Chưa gán Next Floor Scene "
			+ "trong Inspector."
		)
		return

	choice_made = true

	# Ngăn Player chọn nhiều hơn một card.
	card_1.disabled = true
	card_2.disabled = true
	card_3.disabled = true

	# Gửi phần thưởng đã chọn đến Floor hiện tại.
	reward_selected.emit(
		option_id,
		damage_bonus_percent,
		max_health_bonus
	)

	# Gửi phần thưởng cố định sau khi vượt qua Floor.
	floor_bonus_selected.emit(
		bonus_potions,
		healing_bonus_per_floor
	)

	# Chuyển sang Floor tiếp theo.
	var error: Error = get_tree().change_scene_to_packed(
		next_floor_scene
	)

	if error != OK:
		choice_made = false

		card_1.disabled = false
		card_2.disabled = false
		card_3.disabled = false

		push_error(
			"FloorTransitionUI: Không thể chuyển sang Floor tiếp theo."
		)
