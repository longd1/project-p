
extends Area2D


# ==================================================
# WEAPON
# ==================================================

@export var weapon: Resource


# ==================================================
# NODES
# ==================================================

@onready var sprite: Sprite2D = $Sprite2D
@onready var label: Label = $Label


# ==================================================
# STATE
# ==================================================

var player = null


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	update_sprite()

	label.hide()

	body_entered.connect(
		_on_body_entered
	)

	body_exited.connect(
		_on_body_exited
	)


# ==================================================
# PROCESS
# ==================================================

func _process(_delta: float) -> void:
	if player == null:
		return

	if Input.is_action_just_pressed("interact"):
		swap_weapon()


# ==================================================
# SETUP WEAPON
# Dùng khi spawn weapon runtime từ boss/miniboss.
# ==================================================

func setup_weapon(
	new_weapon: Resource
) -> void:
	if new_weapon == null:
		push_error(
			"GroundWeapon: new_weapon is null."
		)
		return

	weapon = new_weapon

	# Nếu node đã _ready rồi thì update luôn.
	# Nếu chưa add_child thì _ready() sẽ update sau.
	if is_node_ready():
		update_sprite()


# ==================================================
# UPDATE VISUAL
# ==================================================

func update_sprite() -> void:
	if weapon == null:
		return

	sprite.frame = weapon.ground_sprite_frame

	update_label()


# ==================================================
# SWAP WEAPON
# ==================================================

func swap_weapon() -> void:
	if player == null:
		return

	if weapon == null:
		return


	# ==========================================
	# SAVE OLD WEAPON
	# ==========================================

	var old_weapon = \
		player.get_current_weapon()


	# ==========================================
	# EQUIP NEW WEAPON
	#
	# Gọi method thay vì gán trực tiếp property
	# để Player cập nhật cả Global.
	# ==========================================

	player.set_current_weapon(
		weapon
	)


	# ==========================================
	# PUT OLD WEAPON ON THE GROUND
	# ==========================================

	weapon = old_weapon


	# ==========================================
	# UPDATE GROUND WEAPON
	# ==========================================

	update_sprite()


# ==================================================
# PLAYER ENTER
# ==================================================

func _on_body_entered(
	body: Node
) -> void:
	if body is Player:
		player = body
		label.show()


# ==================================================
# PLAYER EXIT
# ==================================================

func _on_body_exited(
	body: Node
) -> void:
	if body == player:
		player = null
		label.hide()


# ==================================================
# UPDATE LABEL
# ==================================================

func update_label() -> void:
	if weapon == null:
		label.text = ""
		return

	label.text = (
		"[E] Swap: " + weapon.weapon_name
		+ "\nDamage: " + str(weapon.damage)
		+ "\nParry Duration: " + str(weapon.parry_duration)
		+ "\nKnockback: " + str(weapon.knockback)
		+ "\nBlock: "
		+ str(
			int(
				weapon.block_damage_negation * 100
			)
		)
		+ "%"
	)
