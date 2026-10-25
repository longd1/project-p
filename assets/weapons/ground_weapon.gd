extends Area2D

@export var weapon: Resource

@onready var sprite: Sprite2D = $Sprite2D

@onready var label: Label = $Label


var player = null


func _ready() -> void:
	update_sprite()
	label.hide()

	body_entered.connect(_on_body_entered)
	body_exited.connect(_on_body_exited)


func _process(_delta: float) -> void:
	if player != null and Input.is_action_just_pressed("interact"):
		swap_weapon()


func update_sprite() -> void:
	if weapon != null:
		sprite.frame = weapon.ground_sprite_frame
		update_label()


func swap_weapon() -> void:
	var old_weapon = player.current_weapon

	player.current_weapon = weapon
	weapon = old_weapon

	update_sprite()



func _on_body_entered(body: Node) -> void:
	if body.name == "Player":
		player = body
		label.show()



func _on_body_exited(body: Node) -> void:
	if body == player:
		player = null
		label.hide()

func update_label() -> void:
	if weapon == null:
		return
	label.text = (
		"[E] Swap: " + weapon.weapon_name
		+ "\nDamage: " + str(weapon.damage)
		+ "\nParry Duration: " + str(weapon.parry_duration)
		+ "\nKnockback: " + str(weapon.knockback)
		+ "\nBlock: " + str(int(weapon.block_damage_negation * 100)) + "%"
	)
