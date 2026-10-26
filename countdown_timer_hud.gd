extends CanvasLayer


# ==========================================
# SIGNALS
# ==========================================

signal time_up


# ==========================================
# SETTINGS
# ==========================================

@export var duration: float = 60.0
@export var auto_start: bool = true
@export var death_trigger_delay: float = 1.0


# ==========================================
# NODES
# ==========================================

@onready var countdown: Timer = $countdown
@onready var time_label: Label = $Screen/TimerUI/TimerPanel/TimeLabel
@onready var fail_video: VideoStreamPlayer = $Screen/FailVideo

@onready var tick_sound: AudioStreamPlayer = $TickSound



# ==========================================
# STATE
# ==========================================

var last_displayed_second: int = -1
var failure_started: bool = false


# ==========================================
# READY
# ==========================================

func _ready() -> void:
	$Screen.mouse_filter = Control.MOUSE_FILTER_IGNORE
	$Screen/TimerUI.mouse_filter = Control.MOUSE_FILTER_IGNORE
	$Screen/FailVideo.mouse_filter = Control.MOUSE_FILTER_IGNORE

	countdown.one_shot = true

	fail_video.hide()

	countdown.timeout.connect(_on_timeout)
	fail_video.finished.connect(_on_fail_video_finished)

	if auto_start:
		start_timer()


# ==========================================
# PROCESS
# ==========================================

func _process(_delta: float) -> void:
	if countdown.is_stopped():
		return

	update_display()


# ==========================================
# START TIMER
# ==========================================

func start_timer() -> void:
	failure_started = false

	last_displayed_second = int(ceil(duration))

	countdown.start(duration)

	update_display()


# ==========================================
# STOP TIMER
# ==========================================

func stop_timer() -> void:
	countdown.stop()
	tick_sound.stop()


# ==========================================
# RESET TIMER
# ==========================================

func reset_timer() -> void:
	countdown.stop()

	failure_started = false
	last_displayed_second = int(ceil(duration))

	countdown.start(duration)

	update_display()


# ==========================================
# DISPLAY
# ==========================================

func update_display() -> void:
	var total_seconds: int = int(ceil(countdown.time_left))

	var minutes: int = int(total_seconds / 60.0)
	var seconds: int = total_seconds % 60

	time_label.text = "%02d:%02d" % [
		minutes,
		seconds
	]

	if total_seconds != last_displayed_second:
		last_displayed_second = total_seconds

		if total_seconds > 0:
			tick_sound.play()


# ==========================================
# NORMAL TIMEOUT
# ==========================================

func _on_timeout() -> void:
	start_failure_sequence()


# ==========================================
# FORCE TIMEOUT
# Dùng khi quiz sai đủ 3 câu.
# ==========================================

func force_time_up() -> void:
	if failure_started:
		return

	countdown.stop()

	time_label.text = "00:00"
	last_displayed_second = 0

	tick_sound.stop()

	start_failure_sequence()


# ==========================================
# FAILURE SEQUENCE
# ==========================================

func start_failure_sequence() -> void:
	if failure_started:
		return

	failure_started = true

	countdown.stop()
	tick_sound.stop()

	time_label.text = "00:00"

	fail_video.show()
	fail_video.play()

	await get_tree().create_timer(death_trigger_delay).timeout

	time_up.emit()

# ==========================================
# FAILURE VIDEO FINISHED
# ==========================================

func _on_fail_video_finished() -> void:
	fail_video.hide()
