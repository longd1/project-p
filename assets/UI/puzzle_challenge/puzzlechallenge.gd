extends CanvasLayer


# ==================================================
# SIGNALS
# ==================================================

signal challenge_finished(success: bool)


# ==================================================
# SETTINGS
# ==================================================

const TOTAL_QUESTIONS: int = 6
const REQUIRED_CORRECT: int = 4
const MAX_WRONG: int = 3

const MAIN_MENU_PATH: String = \
	"res://assets/UI/main_menu_ui/main_menu_ui.tscn"

@export var feedback_duration: float = 0.8


# ==================================================
# STATE
# ==================================================

var correct_count: int = 0
var wrong_count: int = 0
var answered_count: int = 0

var current_question_index: int = -1

var challenge_ended: bool = false


var question_buttons: Array[Button] = []
var answer_buttons: Array[Button] = []
var answered_questions: Array[bool] = []


# ==================================================
# QUESTIONS
# ==================================================

var questions: Array[Dictionary] = [
	{
		"question": "Which language is commonly used to create web pages?",
		"answers": [
			"HTML",
			"SQL",
			"C",
			"Assembly"
		],
		"correct": 0
	},
	{
		"question": "Which component executes program instructions?",
		"answers": [
			"Monitor",
			"CPU",
			"Keyboard",
			"Printer"
		],
		"correct": 1
	},
	{
		"question": "What does RAM provide?",
		"answers": [
			"Temporary memory",
			"Permanent printing",
			"Internet access",
			"File compression"
		],
		"correct": 0
	},
	{
		"question": "Which one is a database query language?",
		"answers": [
			"CSS",
			"HTML",
			"SQL",
			"PNG"
		],
		"correct": 2
	},
	{
		"question": "Which number system uses only 0 and 1?",
		"answers": [
			"Decimal",
			"Binary",
			"Octal",
			"Hexadecimal"
		],
		"correct": 1
	},
	{
		"question": "What does OOP stand for?",
		"answers": [
			"Object-Oriented Programming",
			"Open Online Platform",
			"Operating Output Process",
			"Object Operation Protocol"
		],
		"correct": 0
	}
]


# ==================================================
# MAIN PANEL
# ==================================================

@onready var main_panel: Control = $mainpanel
@onready var score_label: Label = $mainpanel/score


# ==================================================
# QUESTION PANEL
# ==================================================

@onready var question_panel: Control = $questionpanel

@onready var question_label: Label = \
	$questionpanel/questionlabel


# ==================================================
# COMPLETE PANEL
# ==================================================

@onready var complete_panel: Control = $completepanel

@onready var result_title: Label = \
	$completepanel/title

@onready var announcement_label: Label = \
	$completepanel/announcement/announceresult

@onready var result_score: Label = \
	$completepanel/announcement/resultscore

@onready var result_message: Label = \
	$completepanel/announcement/resultmessage

@onready var continue_button: Button = \
	$completepanel/continuebutton


# ==================================================
# ANSWER FEEDBACK
# ==================================================

@onready var feedback_panel: Control = $feedback

@onready var correct_icon: TextureRect = \
	$feedback/correct_icon

@onready var wrong_icon: TextureRect = \
	$feedback/wrong_icon

@onready var correct_sound: AudioStreamPlayer = \
	$feedback/correct_sound

@onready var wrong_sound: AudioStreamPlayer = \
	$feedback/wrong_sound


# ==================================================
# TIMER
# ==================================================

@onready var countdown_timer = get_node("CountdownTimer")


# ==================================================
# DEATH HANDLER
# ==================================================

@onready var death_handler = \
	get_tree().get_first_node_in_group("death_handler")


# ==================================================
# READY
# ==================================================

func _ready() -> void:
	main_panel.show()
	question_panel.hide()
	complete_panel.hide()

	feedback_panel.hide()
	correct_icon.hide()
	wrong_icon.hide()


	# ==========================================
	# QUESTION BUTTONS
	# ==========================================

	for i in range(TOTAL_QUESTIONS):
		var button: Button = get_node(
			"mainpanel/questions/q%d" % (i + 1)
		) as Button

		question_buttons.append(button)
		answered_questions.append(false)

		button.text = str(i + 1)
		button.disabled = false

		button.pressed.connect(
			_on_question_pressed.bind(i)
		)


	# ==========================================
	# ANSWER BUTTONS
	# ==========================================

	for i in range(4):
		var button: Button = get_node(
			"questionpanel/ans%d" % (i + 1)
		) as Button

		answer_buttons.append(button)

		button.disabled = false

		button.pressed.connect(
			_on_answer_pressed.bind(i)
		)


	# ==========================================
	# CONTINUE BUTTON
	# ==========================================

	continue_button.pressed.connect(
		_on_continue_pressed
	)


	# ==========================================
	# TIMER
	# ==========================================

	countdown_timer.time_up.connect(
		_on_timer_time_up
	)


	update_score()


# ==================================================
# SELECT QUESTION
# ==================================================

func _on_question_pressed(index: int) -> void:
	if challenge_ended:
		return

	if answered_questions[index]:
		return

	if current_question_index != -1:
		return

	current_question_index = index

	var data: Dictionary = questions[index]

	question_label.text = data["question"]

	var options: Array = data["answers"]

	for i in range(4):
		answer_buttons[i].text = str(options[i])
		answer_buttons[i].disabled = false

	question_panel.show()


# ==================================================
# SELECT ANSWER
# ==================================================

func _on_answer_pressed(answer_index: int) -> void:
	if challenge_ended:
		return

	if current_question_index == -1:
		return

	var index: int = current_question_index

	if answered_questions[index]:
		return

	var data: Dictionary = questions[index]

	var is_correct: bool = (
		answer_index == int(data["correct"])
	)


	# ==========================================
	# SCORE
	# ==========================================

	if is_correct:
		correct_count += 1
	else:
		wrong_count += 1


	answered_questions[index] = true
	answered_count += 1

	question_buttons[index].disabled = true


	# ==========================================
	# LOCK ANSWERS
	# ==========================================

	for button in answer_buttons:
		button.disabled = true


	current_question_index = -1

	question_panel.hide()


	# ==========================================
	# SHOW CORRECT / WRONG
	# ==========================================

	await show_answer_feedback(is_correct)


	# ==========================================
	# UPDATE SCORE
	# ==========================================

	update_score()


	# ==========================================
	# 3 WRONG ANSWERS = FAIL
	# ==========================================

	if wrong_count >= MAX_WRONG:
		challenge_ended = true

		countdown_timer.force_time_up()

		return


	# ==========================================
	# ALL QUESTIONS ANSWERED
	# ==========================================

	if answered_count >= TOTAL_QUESTIONS:
		finish_challenge()


# ==================================================
# ANSWER FEEDBACK
# ==================================================

func show_answer_feedback(is_correct: bool) -> void:
	correct_icon.hide()
	wrong_icon.hide()

	if is_correct:
		correct_icon.show()
		correct_sound.play()

	else:
		wrong_icon.show()
		wrong_sound.play()


	feedback_panel.show()


	await get_tree().create_timer(
		feedback_duration
	).timeout


	feedback_panel.hide()

	correct_icon.hide()
	wrong_icon.hide()


# ==================================================
# UPDATE SCORE
# ==================================================

func update_score() -> void:
	score_label.text = (
		"Correct: %d/%d    Answered: %d/%d"
		% [
			correct_count,
			TOTAL_QUESTIONS,
			answered_count,
			TOTAL_QUESTIONS
		]
	)


# ==================================================
# FINISH CHALLENGE
# ==================================================

func finish_challenge() -> void:
	if challenge_ended:
		return

	if answered_count < TOTAL_QUESTIONS:
		return


	challenge_ended = true


	main_panel.hide()
	question_panel.hide()
	feedback_panel.hide()


	# ==========================================
	# SUCCESS
	# ==========================================

	if correct_count >= REQUIRED_CORRECT:
		show_success_panel()

		return


	# ==========================================
	# FAILURE
	# ==========================================

	countdown_timer.force_time_up()


# ==================================================
# SUCCESS PANEL
# ==================================================

func show_success_panel() -> void:
	countdown_timer.stop_timer()

	complete_panel.show()


	result_title.text = "TRIAL COMPLETED!"

	announcement_label.text = "YOUR RESULT"


	result_score.text = "%d / %d" % [
		correct_count,
		TOTAL_QUESTIONS
	]


	result_message.text = (
		"You passed the challenge!"
	)


	continue_button.text = "CONTINUE EXPLORING"


# ==================================================
# TIMER FAILURE
# Được gọi SAU KHI video fail của timer kết thúc.
# ==================================================

func _on_timer_time_up() -> void:
	if(correct_count < 4):
		challenge_ended = true


		main_panel.hide()
		question_panel.hide()
		complete_panel.hide()
		feedback_panel.hide()


	# Báo challenge thất bại.
		challenge_finished.emit(false)

	# ==========================================
	# START DEATH SEQUENCE
	# ==========================================

		if death_handler != null:
			death_handler.trigger_death_sequence()
		else:
			push_error("PuzzleChallenge: Không tìm thấy death_handler.")
	else:
		show_success_panel()


# ==================================================
# CONTINUE
# ==================================================

func _on_continue_pressed() -> void:
	if not challenge_ended:
		return

	if correct_count < REQUIRED_CORRECT:
		return


	challenge_finished.emit(true)

	queue_free()


# ==================================================
# RETURN TO MAIN MENU
# ==================================================

func return_to_main_menu() -> void:
	challenge_finished.emit(false)


	var error: Error = get_tree().change_scene_to_file(
		MAIN_MENU_PATH
	)


	if error != OK:
		push_error(
			"Không thể mở Main Menu. Hãy kiểm tra đường dẫn: "
			+ MAIN_MENU_PATH
		)
