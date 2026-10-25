extends CharacterBody2D

@export var roll_speed: float = 180.0
@export var push_force: float = 500.0

@export var shake_amount: float = 3.0
@export var shake_speed: float = 35.0

@export var rotation_speed: float = 4.0

@export var wait_time: float = 1.5
@export var shake_time: float = 1.5

@onready var sprite: Sprite2D = $Sprite2D

var timer: float = 0.0

var start_position: Vector2
var sprite_start_position: Vector2


func _ready() -> void:
	motion_mode = CharacterBody2D.MOTION_MODE_FLOATING

	start_position = global_position
	sprite_start_position = sprite.position


func _physics_process(delta: float) -> void:
	timer += delta


	# ==========================================
	# WAIT
	# 0 - 1.5s
	# ==========================================

	if timer < wait_time:
		velocity = Vector2.ZERO
		sprite.position = sprite_start_position
		return


	# ==========================================
	# SHAKE
	# 1.5 - 3s
	# ==========================================

	if timer < wait_time + shake_time:
		velocity = Vector2.ZERO

		sprite.position.x = (
			sprite_start_position.x
			+ sin(timer * shake_speed) * shake_amount
		)

		return


	# ==========================================
	# ROLL
	# AFTER 3s
	# ==========================================

	sprite.position = sprite_start_position

	velocity = Vector2(0, roll_speed)

	# Chỉ xoay hình, KHÔNG xoay CharacterBody2D.
	sprite.rotation += rotation_speed * delta

	move_and_slide()


	# ==========================================
	# PUSH ACTORS
	# ==========================================

	for i in range(get_slide_collision_count()):
		var collision := get_slide_collision(i)
		var body := collision.get_collider()

		if body == null:
			continue

		if body.has_method("apply_external_knockback"):
			var push_direction := velocity.normalized()

			body.apply_external_knockback(
				push_direction * push_force
			)


# ==========================================
# RESET
# ==========================================

func reset_rock() -> void:
	timer = 0.0

	global_position = start_position
	velocity = Vector2.ZERO

	sprite.position = sprite_start_position
	sprite.rotation = 0.0
