
extends CanvasLayer

signal challenge_finished(success: bool)

const TOTAL_QUESTIONS: int = 6
const REQUIRED_CORRECT: int = 4

const MAIN_MENU_PATH: String = \
    "res://assets/UI/main_menu_ui/main_menu_ui.tscn"

var correct_count: int = 0
var answered_count: int = 0
var current_question_index: int = -1
var challenge_ended: bool = false

var question_buttons: Array[Button] = []
var answer_buttons: Array[Button] = []
var answered_questions: Array[bool] = []

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


# Các node của bảng chọn câu hỏi.
@onready var main_panel: Control = $mainpanel
@onready var score_label: Label = $mainpanel/score

# Các node của bảng câu hỏi.
@onready var question_panel: Control = $questionpanel
@onready var question_label: Label = \
	$questionpanel/questionlabel

# Các node của bảng kết quả.
@onready var complete_panel: Control = $completepanel
@onready var result_title: Label = $completepanel/title
@onready var announcement_label: Label = \
	$completepanel/announcement/announceresult
@onready var result_score: Label = \
	$completepanel/announcement/resultscore
@onready var result_message: Label = \
	$completepanel/announcement/resultmessage
@onready var continue_button: Button = \
	$completepanel/continuebutton


func _ready() -> void:
	# Chỉ hiện bảng chọn câu hỏi lúc bắt đầu.
	main_panel.show()
	question_panel.hide()
	complete_panel.hide()

	# Kết nối sáu nút câu hỏi.
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

	# Kết nối bốn nút đáp án.
	for i in range(4):
		var button: Button = get_node(
			"questionpanel/ans%d" % (i + 1)
		) as Button

		answer_buttons.append(button)
		button.disabled = false
		button.pressed.connect(
			_on_answer_pressed.bind(i)
		)

	# Kết nối nút xác nhận kết quả.
	continue_button.pressed.connect(
		_on_continue_pressed
	)

	update_score()


func _on_question_pressed(index: int) -> void:
	if challenge_ended:
		return

	# Không cho trả lời lại câu đã chọn.
	if answered_questions[index]:
		return

	# Không chuyển câu khi câu hiện tại chưa được trả lời.
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


func _on_answer_pressed(answer_index: int) -> void:
	if challenge_ended:
		return

	if current_question_index == -1:
		return

	var index: int = current_question_index

	if answered_questions[index]:
		return

	var data: Dictionary = questions[index]

	# Cộng điểm nếu đáp án đúng.
	if answer_index == int(data["correct"]):
		correct_count += 1

	# Đánh dấu câu đã trả lời.
	answered_questions[index] = true
	answered_count += 1

	# Khóa câu đã trả lời để không thể làm lại.
	question_buttons[index].disabled = true

	current_question_index = -1

	question_panel.hide()
	update_score()

	# Sau câu thứ sáu, kiểm tra kết quả.
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

	# Ẩn giao diện làm câu hỏi.
	main_panel.hide()
	question_panel.hide()

	if correct_count >= REQUIRED_CORRECT:
		# Người chơi thắng: hiển thị bảng kết quả.
		show_success_panel()
	else:
		# Người chơi thua: quay về Main Menu.
		return_to_main_menu()


func show_success_panel() -> void:
	complete_panel.show()

	result_title.text = "TRIAL COMPLETED!"
	announcement_label.text = "YOUR RESULT"

	# Hiển thị số câu đúng thực tế.
	result_score.text = "%d / %d" % [
		correct_count,
		TOTAL_QUESTIONS
	]

	result_message.text = (
        "You passed the challenge!"
	)

	continue_button.text = "CONTINUE EXPLORING"


func _on_continue_pressed() -> void:
	if not challenge_ended:
		return

	# Chỉ cho tiếp tục khi người chơi đã thắng.
	if correct_count < REQUIRED_CORRECT:
		return

	# Báo cho puzzle_trigger rằng thử thách đã thành công.
	challenge_finished.emit(true)

	# Đóng giao diện thử thách.
	# Floor 4 vẫn được giữ nguyên.
	queue_free()


func return_to_main_menu() -> void:
	# Thông báo thất bại cho các node đang lắng nghe.
	challenge_finished.emit(false)

	# Chuyển thẳng về menu theo đường dẫn của project.
	var error: Error = get_tree().change_scene_to_file(
		MAIN_MENU_PATH
	)

	if error != OK:
		push_error(
            "Không thể mở Main Menu. Hãy kiểm tra đường dẫn: "
			+ MAIN_MENU_PATH
		)
