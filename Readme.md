# First EGC Homework - Bow & Arrow

The problem statement can be found [here](https://ocw.cs.pub.ro/courses/egc/teme/2020/01).
It is a 2D game written in C++, using OpenGL. The game objective is to shoot red balloons with arrow, while trying to not hit the yellow ones. At the same time, you have to dodge the shurikens, so they do not hit the bow (or shoot them with arrows).

## Table of Contents

- [First EGC Homework - Bow & Arrow](#first-egc-homework---bow--arrow)
  - [Table of Contents](#table-of-contents)
  - [Gameplay details](#gameplay-details)
    - [Game Difficulty](#game-difficulty)
    - [Physics](#physics)
  - [Implementation details](#implementation-details)
    - [GameObjects class](#gameobjects-class)
    - [Objects namespace](#objects-namespace)

## Gameplay details

In this section, I will present some small modifications/additions to the homework statement

### Game Difficulty

When the game starts, you can choose between 3 difficulty settings: Easy, Medium (default), Hard. By changing the difficulty, you affect:
    - number of lives
    - number of balloons and shurikens in the scene
    - the spawn delay for the balloons and shurikens
    - the reload time for the arrows
    - the speed of the balloons and shurikens

In the statement, it was specified that the player has 3 lives, but I considered I can increase/decrease the difficulty level by changing the number of lives.

### Physics

The movement in this game can be run in two modes: physics and function based (both managed by the physics system).

1. Physics mode - it will use forces and accelerations to compute (using a Runge-Kutta integrator) the position (and rotation) of the object. This is used for the shurikens and the balloons after they are hit and for the arrows. (This is my way of implementing an animation for them, as I also shrink them)
2. Function mode - a time-based function is used to compute the position and rotation of the objects. This is used for: the shuriken linear movement & the side to side updwards movement of the balloons.

Pretty much everything else was implemented exactly as it was specified.

## Implementation details

In this section, I will present a few details about the implementation/development.

### GameObjects class

Most of the objects found in the game (the bow, arrows, balloons and shurikens) are defined as GameObject, a class that contains the methods to compute their "transform"/state (using the physics engine), and to render them. The "Tema1" file exposes the render method and the meshes & shaders arrays, so that the rendering of the objects can occur.

Every frame, for every gameobject, the physics system performs the computations, and after that, they are rendered on the scene.

### Objects namespace

This namespace contains the methods that create all the different meshes used in the game. The ballon is actually created using two different meshes (as it was specified in the statement).

The most complex mesh (to create) was the bow mesh. The function takes two parameters:
    - the center of the bow (the place where the arrow "rests" on the string).
    - the height of the bow (length of the string)
From a geometry perspective, the string is a chord of a circle, and the bow body is an arc of the same circle. To compute the center of the circle that will be used to draw the mesh, we also need to know the angle of the arc (this is hardcoded in the function). Then, using two formulas (found [here](https://planetcalc.com/1421/)), we can compute the radius & the arc height, from where we can compute the center of the circle.

© 2020 Grama Nicolae, 332CA
