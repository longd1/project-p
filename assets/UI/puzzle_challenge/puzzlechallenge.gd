
extends CanvasLayer

signal challenge_finished(success: bool)

const TOTAL_QUESTIONS: int = 9
const REQUIRED_CORRECT: int = 6

var correct_count: int = 0
var answered_count: int = 0
var challenge_ended: bool = false

var current_question_index: int = -1

var question_buttons: Array[Button] = []
var answer_buttons: Array[Button] = []
var answered_questions: Array[bool] = []

@onready var main_panel = $mainpanel
@onready var question_panel = $questionpanel
@onready var complete_panel = $completepanel

@onready var question_label: Label = $questionpanel/question
@onready var score_label: Label = $mainpanel/score


# Mỗi câu hỏi gồm:
# question: nội dung câu hỏi
# answers: 4 phương án trả lời
# correct: vị trí đáp án đúng (0, 1, 2 hoặc 3)

var questions: Array[Dictionary] = [
	{
		"question": "Which language is commonly used to create web pages?",
		"answers": ["HTML", "SQL", "C", "Assembly"],
		"correct": 0
	},
	{
		"question": "Which component executes program instructions?",
		"answers": ["Monitor", "CPU", "Keyboard", "Printer"],
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
		"answers": ["CSS", "HTML", "SQL", "PNG"],
		"correct": 2
	},
	{
		"question": "Which number system uses only 0 and 1?",
		"answers": ["Decimal", "Binary", "Octal", "Hexadecimal"],
		"correct": 1
	},
	{
		"question": "Which device connects a computer to a network?",
		"answers": ["Router", "Speaker", "Scanner", "Projector"],
		"correct": 0
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
	},
	{
		"question": "Which data type represents True or False?",
		"answers": ["String", "Integer", "Boolean", "Float"],
		"correct": 2
	},
	{
		"question": "Which structure follows FIFO?",
		"answers": ["Stack", "Queue", "Tree", "Graph"],
		"correct": 1
	}
]


func _ready() -> void:
	question_panel.hide()
	complete_panel.hide()

	# Lưu 9 nút câu hỏi theo thứ tự.
	for i in range(TOTAL_QUESTIONS):
		var button := get_node(
			"mainpanel/questions/q%d" % (i + 1)
		) as Button

		question_buttons.append(button)
		answered_questions.append(false)

		button.pressed.connect(_on_question_pressed.bind(i))

	# Lưu 4 nút trả lời.
	for i in range(4):
		var button := get_node(
			"questionpanel/ans%d" % (i + 1)
		) as Button

		answer_buttons.append(button)
		button.pressed.connect(_on_answer_pressed.bind(i))

	update_score()


func _on_question_pressed(index: int) -> void:
	if challenge_ended:
		return

	# Không cho mở lại câu đã trả lời.
	if answered_questions[index]:
		return

	current_question_index = index

	var data: Dictionary = questions[index]

	question_label.text = data["question"]

	var options: Array = data["answers"]

	for i in range(4):
		answer_buttons[i].text = str(options[i])
		answer_buttons[i].disabled = false

	question_panel.show()


func _on_answer_pressed(answer_index: int) -> void:
	if challenge_ended:
		return

	if current_question_index < 0:
		return

	var index := current_question_index

	if answered_questions[index]:
		return

	var data: Dictionary = questions[index]

	# Kiểm tra đáp án.
	if answer_index == int(data["correct"]):
		correct_count += 1

	answered_questions[index] = true
	answered_count += 1

	# Đánh dấu câu đã trả lời.
	question_buttons[index].disabled = true

	question_panel.hide()

	current_question_index = -1

	update_score()

	# Đã trả lời đủ 9 câu thì kết thúc thử thách.
	if answered_count >= TOTAL_QUESTIONS:
		finish_challenge()


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


func finish_challenge() -> void:
	if challenge_ended:
		return

	if answered_count < TOTAL_QUESTIONS:
		return

	challenge_ended = true

	var success := correct_count >= REQUIRED_CORRECT

	# Báo kết quả cho puzzle_trigger ở Floor 4.
	challenge_finished.emit(success)

	if success:
		# Đóng giao diện thử thách.
		queue_free()
