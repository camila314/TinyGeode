# TinyGeode

Hot-reloadable UI scripting using [Tiny](https://github.com/goodpaul6/Tiny). Check out the example for how to incorporate.

# Docs

These are the functions you can use:

	Node(float, float): Node
		- Creates a CCNode at the given position
	Label(float, float, str): Node
		- Creates a CCLabelBMFont at the given position with the given text
	Sprite(float, float, str): Node
		- Creates a CCSprite at the given position with the given sprite frame name
	Button(Node, str): Node
		- Converts a CCSprite to a CCMenuItemSprite with the given callback
	Scale9Sprite(float, float, float, float, str): Node
		- Creates a CCScale9Sprite at the given position with the given size and sprite frame name

	tag(Node, int): void
		- Sets the tag of a node
	fetch(int): Node
		- Fetches a node by tag
	visible(Node, bool): void
		- Sets the visibility of a node
	pos(Node, float, float): void
		- Sets the position of a node
	xpos(Node, float): void
		- Sets the x position of a node
	ypos(Node, float): void
		- Sets the y position of a node
	move(Node, float, float): void
		- Moves a node by the given amount
	scale(Node, float): void
		- Sets the scale of a node
	scaleBy(Node, float): void
		- Scales a node by the given amount
	rotate(Node, float): void
		- Sets the rotation of a node
	rotateBy(Node, float): void
		- Rotates a node by the given amount
	z(Node, int): void
		- Sets the z order of a node
	child(Node, Node): void
		- Adds a child to a node
	runAction(Node, Action): void
		- Runs an action on a node
	color(Node, int, int, int): void
		- Sets the color of a node
		- Only works for CCRGBAProtocol nodes
	opacity(Node, int): void
		- Sets the opacity of a node
		- Only works for CCRGBAProtocol nodes
	text(Node, str): void
		- Sets the text of a node
		- Only works for CCLabelBMFont nodes
	font(Node, str): void
		- Sets the font of a node
		- Only works for CCLabelBMFont nodes

	terminate(): void
		- Destroys the TinyNode
	self_menu(): Node
		- Returns the menu of the TinyNode
	self(): Node
		- Returns the TinyNode itself
	popup(str, str, str): void
		- Creates a popup with a title, message, and button text
	prompt(str, str, str): void
		- Creates a prompt with a title, placeholder, and callback
		- The callback takes the name of a function

	Sequence(Action, Action): Action
		- Creates a CCSequence with two actions
	Delay(float): Action
		- Creates a CCDelayTime action
	MoveTo(float, float, float): Action
		- Creates a CCMoveTo action, takes time, x, and y
	MoveBy(float, float, float): Action
		- Creates a CCMoveBy action, takes time, x, and y
	ScaleTo(float, float): Action
		- Creates a CCScaleTo action, takes time and scale
	ScaleBy(float, float): Action
		- Creates a CCScaleBy action, takes time and scale
	RotateTo(float, float): Action
		- Creates a CCRotateTo action, takes time and angle
	RotateBy(float, float): Action
		- Creates a CCRotateBy action, takes time and angle
	FadeTo(float, float): Action
		- Creates a CCFadeTo action, takes time and opacity
	FadeBy(float, float): Action
		- Creates a CCFadeTo action, takes time and opacity

