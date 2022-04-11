extends Node2D

var board = null  # get an empty board
var tile_scene = preload("res://Scenes/Tile.tscn")

func _ready():
	init_game()
	
func init_game() -> void:
	for ch in $Tiles.get_children():
		ch.queue_free()
	
	board = TicTacToe.UnHash(0)
	for i in range(3):
		for j in range(3):
			var n = tile_scene.instance()
			n.pos = [i, j]
			$Tiles.add_child(n)
			n.connect("player_played", self, "handle_player_move")
			
	if not Globals.player_starts:
		make_cpu_move([randi() % 3, randi() % 3])
			
func make_cpu_move(CPU: Array = []) -> void:
	if len(CPU) == 0:
		CPU = TicTacToe.find_most_optimal_move(board)
	if Globals.player_starts:
		board[CPU[0]][CPU[1]] = 1
	else:
		board[CPU[0]][CPU[1]] = 2
		
#	TicTacToe.print_board(board)
	for ch in $Tiles.get_children():
		if ch.pos == [CPU[1], CPU[0]]:
			if Globals.X_starts == Globals.player_starts:
				ch.tile_type = "O"
			else:
				ch.tile_type = "X"
			ch.applied = true

func handle_player_move(pos: Array) -> void:
	if Globals.player_starts:
		board[pos[1]][pos[0]] = 2
	else:
		board[pos[1]][pos[0]] = 1
	
	make_cpu_move()
	
func _unhandled_key_input(event: InputEventKey):
	if event.is_action_pressed("Restart"):
		init_game()
		get_tree().set_input_as_handled()
