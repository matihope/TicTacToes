extends Node2D


var board = TicTacToe.UnHash(0)
var tile_scene = preload("res://Scenes/Tile.tscn")

func _ready():
	for i in range(3):
		for j in range(3):
			var n = tile_scene.instance()
			n.pos = [i, j]
			$Tiles.add_child(n)
			n.connect("player_played", self, "handle_player_move")
			
func handle_player_move(pos: Array) -> void:
	if Globals.X_turn:
		board[pos[1]][pos[0]] = 1
	else:
		board[pos[1]][pos[0]] = 2
		
	var CPU = TicTacToe.find_most_optimal_move(board)
	board[CPU[0]][CPU[1]] = 2
	for ch in $Tiles.get_children():
		if ch.pos == CPU:
			ch.tile_type = "O"
			ch.applied = true
	
