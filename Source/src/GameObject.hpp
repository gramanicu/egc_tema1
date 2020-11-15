#pragma once

#include "Tema1.hpp"
#include "./Physics/Physics.hpp"
#include "./Rendering/Objects.hpp"

#include <vector>

/// <summary>
/// This has something to do with ciclic dependencies
/// </summary>
class Tema1;

/// <summary>
/// Defines different types of objects (for rendering purposes mainly)
/// </summary>
enum class GameObject_Types{
	RED_BALOON,
	YELLOW_BALOON,
	ARROW,
	BOW,
	BOWARROW,
	SHURIKEN,
	SQUARE
};

namespace GameObject_Constants {
	const float squareSide = 100;
	const float arrowLength = 100;
	const float shurikenWidth = 50;
	const float balloonHeight = 75;
	const float bowHeight = 150;
	const float lifeWidth = 50;
}

class GameObject
{
private:
	/// <summary>
	/// The scene in which this object is placed.
	/// </summary>
	Tema1 *scene;

	/// <summary>
	/// The rigidbody attached to this object.
	/// </summary>
	Physics::RigidBody rigidbody;

	/// <summary>
	/// The type of this object.
	/// </summary>
	GameObject_Types type;

	/// <summary>
	/// If the meshes were initialised for the scene
	/// This should be teoretically linked to each scene, as they could have different meshes.
	/// </summary>
	static bool meshes_initialized;
public:
	/// <summary>
	/// If this object should still be taken in account for the score/lives
	/// </summary>
	bool scoring_system_active;

	/// <summary>
	/// Constructor for a gameobject
	/// </summary>
	/// <param name="scene">The scene in which the object is placed</param>
	/// <param name="pos">The position of the object</param>
	/// <param name="type">The type of object</param>
	GameObject(Tema1* scene, glm::vec2 pos, GameObject_Types type);

	/// <summary>
	/// The method used to render the game object
	/// </summary>
	void RenderObject();

	/// <summary>
	/// Makes the physics computations to update things like position, velocity, acceleration, etc.
	/// </summary>
	/// <param name="deltaTime">The delta time of the frame</param>
	void UpdatePhysics(const double deltaTime);

	/// <summary>
	/// Turns the physics simulations on for the object
	/// </summary>
	void EnablePhysics();

	/// <summary>
	/// Turns the physics simulations off for the object
	/// </summary>
	void DisablePhysics();

	/// <summary>
	/// Set the movement type for the rigidbody
	/// Simulated - use physics to determine the position
	/// Function - the movement is based on a time-dependant function
	/// </summary>
	/// <param name="type">The type of movement</param>
	void MovementType(Physics::Constants::Motion_Type type);

	/// <summary>
	/// Set the function that will be used for the function based movement
	/// </summary>
	/// <param name="f">The function that will be used. It will have a state, the total time and deltaTime as parameters</param>
	void MovementFunction(void (*f)(Physics::State& state, double time, double dt));

	/// <summary>
	/// Returns a reference to the rigidbody of the object
	/// </summary>
	/// <returns></returns>
	Physics::RigidBody& getRigidBody();

	/// <summary>
	/// Returns the object's type
	/// </summary>
	/// <returns>The type</returns>
	GameObject_Types getType() const;

	/// <summary>
	/// Set the object's type
	/// </summary>
	/// <param name="type">The type</param>
	void setType(GameObject_Types type);
};