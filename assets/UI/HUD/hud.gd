extends CanvasLayer

# =========================================
# HEALTH BAR CONFIGURATION
# =========================================

# Width of the HP bar when Max HP = 100
const BASE_HEALTHBAR_WIDTH := 268.0

# Reference Max HP
const BASE_MAX_HEALTH := 100.0

# Allows us to adjust how quickly the bar grows
# when Max HP increases.
# 1.0 = directly proportional
# 0.8 = grows a little slower
# 1.2 = grows a little faster
const HEALTHBAR_SCALE := 1.0

# =========================================
# REFERENCES
# =========================================

@onready var player = $"../John"
@onready var hp_background = $hp_background
@onready var hp_bar = $hp_bar

# =========================================
# UPDATE HUD
# =========================================

func _process(delta: float) -> void:

	if not is_instance_valid(player):
		return

	var max_health = player.get_max_health()

	# Prevent division by zero
	if max_health <= 0:
		return

	var current_health = player.get_health()

	# =========================================
	# CALCULATE MAXIMUM HEALTH BAR WIDTH
	# =========================================

	var max_health_ratio = float(max_health) / BASE_MAX_HEALTH

	var background_width = (
		BASE_HEALTHBAR_WIDTH
		* max_health_ratio
		* HEALTHBAR_SCALE
	)

	# Update background width
	hp_background.size.x = background_width

	# =========================================
	# CALCULATE CURRENT HEALTH
	# =========================================

	var health_ratio = float(current_health) / float(max_health)

	health_ratio = clamp(
		health_ratio,
		0.0,
		1.0
	)

	# =========================================
	# CALCULATE CURRENT HP BAR WIDTH
	# =========================================

	var current_health_width = (
		background_width
		* health_ratio
	)

	# =========================================
	# SMOOTH HP BAR ANIMATION
	# =========================================

	hp_bar.size.x = lerp(
		hp_bar.size.x,
		current_health_width,
		10.0 * delta
	)
