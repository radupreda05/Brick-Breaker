				======================
				     Brick-Breaker
				======================

	
To run the program:
	Open Framework_EGC.sln from /Visual Studio and click on Debug Tab and
Start without Debugging. At the runtime a console will be shown and it will ask
the user if it wants to play the game with or without sound. After the answer
the game will start.
	To create this project I used Visual Studio 2017 (v141) with Windows
SDK Version 10.0.17134.0.
	
IMPLEMENTATION:
	Each object in the scene contains a position, a name, and the state of this
object (whether it is in the scene or not). Thus, the "Objects" class has the
purpose of defining these fields. Each object in the scene extends the
Objects class. To distinguish with what kind of object we are working (wall /
ball / brick etc) class Object contains an "id" field that is overwritten in
each builder.
	Initially, all objects are rendered at the point (0, 0)
(bottom left corner) and then translated to their positions. The position
of each object represents the position of the left corner of the squares or
rectangles, or in the case of the ball, its center.
	To determine the state of the game, an enum type variable is used which
is changed (if necessary) according to the events produced. As it happens in
the case of powerups.
	Each brick may or may not contain a powerup. If the ball hits a brick
containing a powerup, it is created in the scene through setting its existence.
The color of the bricks is random at each run it will be a different color.
	The intersection between 2 objects is calculated as follows: the distance
is calculated from the center of the object 1 to the object 2; check if the
distance is smaller than the "ray" of object 1; if it is smaller => intersection.
All intersections work on this principle. In case of intersection of the ball with
the wall it is checked the side where the brick is struck based on positions.
In the case of a wall-wall it is checked the type of wall hit and the velocity
vector is modified based on this type.
	Details about classes and their functionalities can be found in headers.

Power-ups:
	If the platform intersected with a blue powerup then it is rendered in the
scene two cannons. To shoot the cannons use the Left Click Button. Each cannon
contains a projectile, and in order to shoot the second time it is necessary
for the projectile to hit any object.
	If the platform intersected with a pink powerup then the ball will destroy
bricks without bouncing from them.