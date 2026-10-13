extends Node

var rng := RandomNumberGenerator.new()


func _ready() -> void:
	rng.randomize()


func random_int(min_value: int, max_value: int) -> int:
	return rng.randi_range(min_value, max_value)


func random_float() -> float:
	return rng.randf()


func chance(probability: float) -> bool:
	return rng.randf() < probability
