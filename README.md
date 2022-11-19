# LIFAMI

1 - BY BROILLET Virgile IN 2021

2 - Application is a Brick Breaker, the main racket is dirigible with left arrows right.
The goal is to break the entire bricks, at level 2 (after 8 bricks broken) a still brick appears at random coordinates. If the ball hits that brick from below, the ball bounces and gains speed; if the ball hits that brick from above, it is teleported to random x coordinates.

3 - For the broken brick side, when the ball hits the main racquet, its speed is reversed which makes it bounce. When it hits a broken brick, the hitting brick is teleported out of the game, obviously, if the ball comes out of the field of vision, from below, the game is lost, otherwise they bounce off the walls.

4 - I did not use references based on the rules of brick-breaking: 
http://juliette.hernando.free.fr/scratch/pong3.pdf

5-   // The goal is to make a brick break, with different level.
	// The first step will be to destroy 8 bricks, then a still brick will appear, making the game more complex.
	//update 29/04 Modification of the stationary lighter init

	//update 06/05/2022 Modified init, trying to color the bricks black (before they disappear) when the ball hits it.

	//update 13/05 functional puzzle, setting up the menu, rules, a button reload and play + creation of the immobile brick that appears only after having already broken 8 bricks.
	//Placement of obstacle brick collisions.
