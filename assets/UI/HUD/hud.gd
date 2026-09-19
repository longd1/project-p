extends CanvasLayer

var base_healthbar_width = 268.0
var base_max_health = 100.0

@onready var player = $"../John"
@onready var hp_background = $hp_background
@onready var hp_bar = $hp_bar


func _process(delta: float) -> void:
	if is_instance_valid(player):
		var max_health_ratio = float(player.get_max_health()) / base_max_health
		var background_width = base_healthbar_width * max_health_ratio

		hp_background.size.x = background_width

		var health_ratio = float(player.get_health()) / float(player.get_max_health())
		health_ratio = clamp(health_ratio, 0.0, 1.0)

		var current_health_width = background_width * health_ratio

		hp_bar.size.x = lerp(
			hp_bar.size.x,
			current_health_width,
			10.0 * delta
		)
