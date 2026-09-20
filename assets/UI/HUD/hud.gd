extends CanvasLayer

@onready var player = $"../../Player"
@onready var hp_background = $hp_background
@onready var hp_bar = $hp_bar

const BASE_MAX_HEALTH := 100.0

const BASE_BACKGROUND_WIDTH := 175.0
const BASE_HEALTHBAR_WIDTH := 169.0

const HEALTH_LERP_SPEED := 10.0


func _ready() -> void:
	pass


func _process(delta: float) -> void:
	if not is_instance_valid(player):
		return

	var max_health = player.get_max_health()
	var current_health = player.get_health()



	var max_health_ratio = float(max_health) / BASE_MAX_HEALTH

	var background_width = BASE_BACKGROUND_WIDTH * max_health_ratio
	var max_healthbar_width = BASE_HEALTHBAR_WIDTH * max_health_ratio

	hp_background.size.x = background_width


	var health_ratio = float(current_health) / float(max_health)
	health_ratio = clamp(health_ratio, 0.0, 1.0)

	var target_healthbar_width = max_healthbar_width * health_ratio


	var lerp_weight = clamp(HEALTH_LERP_SPEED * delta, 0.0, 1.0)

	hp_bar.size.x = lerp(hp_bar.size.x, target_healthbar_width, lerp_weight)
