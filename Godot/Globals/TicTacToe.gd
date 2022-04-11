extends Node

const MAX_H = pow(3, 9)
const PRIME = 3
var boards = []

func find_most_optimal_move(board: Array) -> Array:
	var oes = 0
	var xes = 0
	for i in range(3):
		for j in range(3):
			if board[i][j] == 2:
				oes += 1
			elif board[i][j] == 1:
				xes += 1
	
	var best = [0, 0]
	var mini = 3
	for i in range(3):
		for j in range(3):
			if board[i][j] == 0:
				var board2 = board.duplicate(true)
				if oes > xes:
					board2[i][j] = 1
				else:
					board2[i][j] = 2
#				print(boards[Hash(board2)])
				if mini > boards[Hash(board2)]:
					mini = boards[Hash(board2)]
					best = [i, j]
#			else:
#				print('9')
	
	return best

func Hash(board: Array) -> int:
	var h = 0
	for i in range(3):
		for j in range(3):
			h = h * PRIME + board[i][j]
	return h
	
func UnHash(ha: int) -> Array:
	var board = []
	for i in range(3):
		board.push_back([])
		for _j in range(3):
			board[i].push_back(0)
	
	for i in range(3):
		for j in range(3):
			board[2 - i][2 - j] = ha % 3 
# warning-ignore:integer_division
			ha = ha / 3
			
	return board
	
func check_win(board: Array) -> int:
	for i in range(3):
		if board[i][0] != 0 and board[i][0] == board[i][1] and board[i][1] == board[i][2]:
			return 2
		
	for i in range(3):
		if board[0][i] != 0 and board[0][i] == board[1][i] and board[1][i] == board[2][i]:
			return 2
			
	if board[0][0] != 0 and board[0][0] == board[1][1] and board[1][1] == board[2][2]:
		return 2
		
	if board[2][0] != 0 and board[2][0] == board[1][1] and board[1][1] == board[0][2]:
		return 2
		
	for i in range(3):
		for j in range(3):
			if board[i][j] == 0:
				return 0
	return 1
	

func gen(board: Array) -> void:
#	print_board(board)
	var my_hash = Hash(board)
	if boards[my_hash] != -1:
		return
	var win = check_win(board)
	if win:
		boards[my_hash] = 2 - win
		return
	
	var oes = 0
	var xes = 0
	for i in range(3):
		for j in range(3):
			if board[i][j] == 2:
				oes += 1
			elif board[i][j] == 1:
				xes += 1
	
	var mini = 3
	for i in range(3):
		for j in range(3):
			if board[i][j] == 0:
				var board2 = board.duplicate(true)
				if oes > xes:
					board2[i][j] = 1
				else:
					board2[i][j] = 2
				gen(board2)
				mini = min(mini, boards[Hash(board2)])
	boards[my_hash] = 2 - mini
	
	
func print_board(board: Array):
	print(' -------')
	for i in range(3):
		var line = '| '
		for j in range(3):
			line += 'O' if board[i][j] == 2 else 'X' if board[i][j] == 1 else ' '
			line += ' '
		print(line, '|')
	print(' -------')

func _init():
	for _i in range(MAX_H):
		boards.push_back(-1)
	gen(UnHash(0))
