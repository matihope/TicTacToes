extends Node2D

export(String, "X", "O", "X_preview", "O_preview", "Nothing") var tile_type = "Nothing" setget set_tile_type
var X_scene = preload("res://Scenes/X.tscn")
var O_scene = preload("res://Scenes/O.tscn")
var O_preview_scene = preload("res://Scenes/O_preview.tscn")
var X_preview_scene = preload("res://Scenes/X_preview.tscn")
var pos = [0, 0] setget set_pos
onready var sprites = $Sprites
var mouse_in = false
var applied = false

signal player_played(pos)

func set_tile_type(value):
	if is_instance_valid(sprites):
		if !applied:
			for ch in sprites.get_children():
				ch.queue_free()
			tile_type = value
		
			var n = null
			match value:
				"X":
					n = X_scene.instance()
				"O":
					n = O_scene.instance()
				"O_preview":
					n = O_preview_scene.instance()
				"X_preview":
					n = X_preview_scene.instance()
					
			if n != null:
				sprites.add_child(n)
	
	
func set_pos(value: Array) -> void:
	position.x = value[0] * 200
	position.y = value[1] * 200
	pos = value


func _on_Area2D_mouse_entered():
	if Globals.X_starts == Globals.player_starts:
		self.tile_type = "X_preview"
	else:
		self.tile_type = "O_preview"
	mouse_in = true

func _on_Area2D_mouse_exited():
	self.tile_type = "Nothing"
	mouse_in = false
	
func _unhandled_input(event: InputEvent):
	if !mouse_in or applied:
		return
		
	if event is InputEventMouseButton:
		if event.pressed:
			if Globals.player_starts == Globals.X_starts:
				self.tile_type = "X"
			else:
				self.tile_type = "O"
			applied = true
			emit_signal("player_played", pos)
			get_tree().set_input_as_handled()
		
