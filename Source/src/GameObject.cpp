#include "GameObject.hpp"

#define Squared(x) ((x) * (x))

bool GameObject::meshes_initialized = false;

GameObject::GameObject(Tema1* scene, glm::vec2 pos, GameObject_Types type) : scene(scene), type(type), scoring_system_active(true)
{
	using namespace GameObject_Constants;
	this->scene = scene;

	if (!meshes_initialized) {
		meshes_initialized = true;

		glm::vec3 center = glm::vec3(0, 0, 0);

		Mesh* square = Objects::CreateSquare(center, squareSide, COLOR_GREEN);
		this->scene->AddMeshToScene(square);

		Mesh* square_line = Objects::CreateSquareLine(center, squareSide, COLOR_GREEN);
		this->scene->AddMeshToScene(square_line);

		Mesh* life = Objects::CreateLife(center, lifeWidth);
		this->scene->AddMeshToScene(life);

		Mesh* shuriken = Objects::CreateShuriken(center, shurikenWidth);
		this->scene->AddMeshToScene(shuriken);

		Mesh* arrow = Objects::CreateArrow(center, arrowLength);
		this->scene->AddMeshToScene(arrow); 

		Mesh* bow = Objects::CreateBow(center, bowHeight);
		this->scene->AddMeshToScene(bow);

		Mesh* rope = Objects::CreateRope(center, balloonHeight / 2);
		this->scene->AddMeshToScene(rope);

		Mesh* red_balloon = Objects::CreateBalloon(center, balloonHeight, COLOR_RED);
		this->scene->AddMeshToScene(red_balloon);

		Mesh* yellow_balloon = Objects::CreateBalloon(center, balloonHeight, COLOR_YELLOW);
		this->scene->AddMeshToScene(yellow_balloon);
	}

	rigidbody.state.x = pos;
}

void GameObject::RenderObject()
{
	glm::mat3 mMatrix = glm::mat3(1);

	double x = rigidbody.state.x.x;
	double y = rigidbody.state.x.y;

	double r = rigidbody.state.rot;

	double sx = rigidbody.state.scl.x;
	double sy = rigidbody.state.scl.y;

	mMatrix *= Transform2D::Translate(x, y);
	mMatrix *= Transform2D::Rotate(r);
	mMatrix *= Transform2D::Scale(sx, sy);

	switch (type) {
	case GameObject_Types::SHURIKEN: {
		scene->RenderMesh(scene->getMeshes()["shuriken"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	case GameObject_Types::SQUARE: {
		scene->RenderMesh(scene->getMeshes()["square"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	case GameObject_Types::ARROW: {
		scene->RenderMesh(scene->getMeshes()["arrow"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	case GameObject_Types::BOW: {
		scene->RenderMesh(scene->getMeshes()["bow"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	case GameObject_Types::BOWARROW: {
		scene->RenderMesh(scene->getMeshes()["bow"], scene->getShaders()["VertexColor"], mMatrix);

		mMatrix *= Transform2D::Translate(GameObject_Constants::arrowLength / 2, 0);
		scene->RenderMesh(scene->getMeshes()["arrow"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	case GameObject_Types::YELLOW_BALOON: {
		scene->RenderMesh(scene->getMeshes()["yellow_balloon"], scene->getShaders()["VertexColor"], mMatrix);
		mMatrix *= Transform2D::Translate(0, -GameObject_Constants::balloonHeight / 2);
		scene->RenderMesh(scene->getMeshes()["rope"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	case GameObject_Types::RED_BALOON: {
		scene->RenderMesh(scene->getMeshes()["red_balloon"], scene->getShaders()["VertexColor"], mMatrix);
		mMatrix *= Transform2D::Translate(0, -GameObject_Constants::balloonHeight / 2);
		scene->RenderMesh(scene->getMeshes()["rope"], scene->getShaders()["VertexColor"], mMatrix);
	} break;
	}
}

void GameObject::UpdatePhysics(const double deltaTime)
{
	Physics::PhysixEngine::UpdatePhysics(rigidbody, deltaTime);
}

void GameObject::EnablePhysics()
{
	rigidbody.physics_enabled = true;
}

void GameObject::DisablePhysics()
{
	rigidbody.physics_enabled = false;
}

void GameObject::MovementType(Physics::Constants::Motion_Type type)
{
	rigidbody.m_type = type;
}

void GameObject::MovementFunction(void(*f)(Physics::State& state, double time, double dt))
{
	rigidbody.m_func = f;
}

Physics::RigidBody& GameObject::getRigidBody()
{
	return rigidbody;
}

GameObject_Types GameObject::getType() const
{
	return type;
}

void GameObject::setType(GameObject_Types type)
{
	this->type = type;
}
