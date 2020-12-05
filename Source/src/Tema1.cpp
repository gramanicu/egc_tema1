#include "Tema1.hpp"
#include <vector>
#include <queue>
#include <math.h>

// An array with the game objects
std::vector<GameObject> gameObjects;

double Tema1::balloon_speed = 100;
double Tema1::shuriken_speed = 350;

/// <summary>
/// Clips a value to some bounds
/// </summary>
/// <param name="n">The value</param>
/// <param name="lower">The lower bound</param>
/// <param name="upper">The higher bound</param>
/// <returns>The clipped value</returns>
double clip(double n, double lower, double upper) {
	return std::max(lower, std::min(n, upper));
}

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

// Movement function for the shurikens
void m_func_shuriken(Physics::State& state, double time, double dt) {
	double lin_speed = Tema1::shuriken_speed;
	double ang_speed = lin_speed / 50.f;

	state.x.x -= lin_speed * dt;

	state.rot = ang_speed * time;
}

// Movement function for the balloons
void m_func_balloon(Physics::State& state, double time, double dt) {
	double amplitude = 0.75;
	double osc_factor = 3;
	double vert_speed = Tema1::balloon_speed;

	state.x.y +=  dt * vert_speed;
	state.x.x += amplitude * cos((time + state.drag_coef) * osc_factor);
}

void Tema1::spawnRedBalloon(glm::vec2 pos) {
	GameObject balloon(this, pos, GameObject_Types::RED_BALOON);
	balloon.MovementType(Physics::Constants::Motion_Type::FUNCTION);
	balloon.MovementFunction(m_func_balloon);
	balloon.getRigidBody().state.drag_coef = (double)(rand() % 100) / 100.0;
	balloon.getRigidBody().rotation_locked = true;
	gameObjects.push_back(balloon);
}

void Tema1::spawnYellowBalloon(glm::vec2 pos) {
	GameObject balloon(this, pos, GameObject_Types::YELLOW_BALOON);
	balloon.MovementType(Physics::Constants::Motion_Type::FUNCTION);
	balloon.MovementFunction(m_func_balloon);
	balloon.getRigidBody().state.drag_coef = (double)(rand() % 100) / 100.0;
	balloon.getRigidBody().rotation_locked = true;
	gameObjects.push_back(balloon);
}

void Tema1::spawnShuriken(glm::vec2 pos) {
	GameObject shuriken(this, pos, GameObject_Types::SHURIKEN);
	shuriken.MovementType(Physics::Constants::Motion_Type::FUNCTION);
	shuriken.MovementFunction(m_func_shuriken);
	gameObjects.push_back(shuriken);
}

void Tema1::popBalloon(GameObject *balloon) {
	if (balloon->getType() == GameObject_Types::RED_BALOON ||
		balloon->getType() == GameObject_Types::YELLOW_BALOON) {
		balloon->MovementType(Physics::Constants::Motion_Type::SIMULATED);
		balloon->getRigidBody().state.drag_coef = 0;
		balloon->getRigidBody().state.gravity_coef = 25.f;
		balloon->getRigidBody().state.scl = glm::vec2(0.35, 0.75);
	}
}

void Tema1::hitShuriken(GameObject* shuriken) {
	if (shuriken->getType() == GameObject_Types::SHURIKEN) {
		shuriken->MovementType(Physics::Constants::Motion_Type::SIMULATED);
		shuriken->getRigidBody().state.drag_coef = 0;
		shuriken->getRigidBody().state.gravity_coef = 100.f;
		shuriken->getRigidBody().state.scl = glm::vec2(0.95, 0.95);
	}
}

void Tema1::updateBow(GameObject* bow, double dt) {
	if (bow->getType() == GameObject_Types::BOWARROW || bow->getType() == GameObject_Types::BOW) {
		// Bow rotation
		glm::vec2 mouse = glm::vec2(window->GetCursorPosition().x, window->GetResolution().y - window->GetCursorPosition().y);

		glm::vec2 dir = mouse - bow->getRigidBody().state.x;
		dir = glm::normalize(dir);

		double angle = atan2(dir.y, dir.x) - atan2(0, 1);
		angle = clip(angle, rot_lim_l, rot_lim_h);
		bow->getRigidBody().state.rot = angle;

		bow_angle = angle;

		// Bow movement
		if (window->KeyHold(GLFW_KEY_W)) {
			bow->getRigidBody().state.x.y += bow_speed * dt;
			if (bow->getRigidBody().state.x.y > (double)window->GetResolution().y - GameObject_Constants::bowHeight / 2) {
				bow->getRigidBody().state.x.y = (double)window->GetResolution().y - GameObject_Constants::bowHeight / 2;
			}
		}
		else if (window->KeyHold(GLFW_KEY_S)) {
			bow->getRigidBody().state.x.y -= bow_speed * dt;
			if (bow->getRigidBody().state.x.y < GameObject_Constants::bowHeight / 2) {
				bow->getRigidBody().state.x.y = GameObject_Constants::bowHeight / 2;
			}
		}

		// Arrow reload
		if (reload_t_remaining < 0.f) {
			bow_shot = false;
			bow->setType(GameObject_Types::BOWARROW);
		}

		// Collisions checking
		// If the bow is rotated, the shuriken can still "hit", even if the bow is not hit
		// This happens because OF COURSE (XD), reality, there is a bowman behind the bot, who will be hit
		auto& go = std::begin(gameObjects);
		while (go != std::end(gameObjects)) {
			GameObject_Types type = go->getType();

			if (type == GameObject_Types::SHURIKEN && go->scoring_system_active) {
				double b_height = GameObject_Constants::bowHeight;
				glm::vec2 b_center = bow->getRigidBody().state.x - glm::vec2(b_height / 6, 0);

				bool collided = is_collision_cc(b_center, go->getRigidBody().state.x, b_height / 2, GameObject_Constants::shurikenWidth / 2);

				if (collided) {
					go->scoring_system_active = false;
					hitShuriken(&(*go));
					lives--;
				}
			}

			go++;
		}
	}
}

/// <summary>
/// Check if the Point-Circle collision is happening
/// </summary>
/// <param name="p">The point</param>
/// <param name="c">The center of the circle</param>
/// <param name="radius">The radius of the circle</param>
/// <returns>If a collision is happening</returns>
bool Tema1::is_collision_pc(const glm::vec2& p, const glm::vec2& c, double radius) {
	double dist_pow_2 = ((double)p.x - c.x) * ((double)p.x - c.x) + ((double)p.y - c.y) * ((double)p.y - c.y);

	return dist_pow_2 < radius * radius;
}

/// <summary>
/// Check if the Circle-Circle collision is happening
/// </summary>
/// <param name="c1">The first circle</param>
/// <param name="c2">The second circle</param>
/// <param name="radius1">The first circle radius</param>
/// <param name="radius2">The second circle radius</param>
/// <returns></returns>
bool Tema1::is_collision_cc(const glm::vec2& c1, const glm::vec2& c2, double radius1, double radius2) {
	double dist_pow_2 = ((double)c1.x - c2.x) * ((double)c1.x - c2.x) + ((double)c1.y - c2.y) * ((double)c1.y - c2.y);
	double radius_sum = radius1 + radius2;

	return dist_pow_2 < radius_sum* radius_sum;
}

void Tema1::updateArrow(GameObject* arrow, double dt) {
	if (arrow->getType() == GameObject_Types::ARROW) {
		// Compute the arroy point position
		double angle = arrow->getRigidBody().state.rot;
		double offset = GameObject_Constants::arrowLength / 2;
		glm::vec2 a_point = arrow->getRigidBody().state.x + glm::vec2(offset * cos(angle), offset * sin(angle));

		// Collision checking
		auto& go = std::begin(gameObjects);
		while (go != std::end(gameObjects)) {
			GameObject_Types type = go->getType();
			bool collided;

			if((type == GameObject_Types::RED_BALOON || type == GameObject_Types::YELLOW_BALOON) && go->scoring_system_active) {
				collided = is_collision_pc(a_point, go->getRigidBody().state.x, GameObject_Constants::balloonHeight / 2);
				if (collided) {
					go->scoring_system_active = false;
					popBalloon(&(*go));
					if (type == GameObject_Types::RED_BALOON) {
						score += score_red;
					}
					else {
						score += score_yellow;
					}
				}
			}
			else if(type == GameObject_Types::SHURIKEN && go->scoring_system_active) {
				collided = is_collision_pc(a_point, go->getRigidBody().state.x, GameObject_Constants::shurikenWidth / 2);
				if (collided) {
					go->scoring_system_active = false;
					hitShuriken(&(*go));
					score += score_shuriken;

					// Make arrow drop
					arrow->getRigidBody().state.v *= 0.05f;
				}
			}

			go++;
		}
	}
}

double Tema1::forceLevel() {
	return (MIN(total_time - drawback_s_time, drawback_time));
}

void Tema1::Init()
{
	srand(time(NULL));

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	GameObject bow(this, glm::vec2(25, window->GetResolution().y / 2), GameObject_Types::BOWARROW);
	bow.DisablePhysics();
	gameObjects.push_back(bow);

	// Setup difficulty
	DifficultySelect();
}

void Tema1::sleepMilis(int timeMs)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(timeMs));
}

void Tema1::AddMeshToScene(Mesh *mesh)
{
	AddMeshToList(mesh);
}

void Tema1::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat3 &modelMatrix)
{
	this->RenderMesh2D(mesh, shader, modelMatrix);
}

void Tema1::ObjectSpawner() {
	// If it can spawn a new object
	if (spawn_delay < total_time - last_spawn_t) {
		// Spawn balloons
		if (count_balloons < max_balloons) {
			last_spawn_t = total_time;
			count_balloons++;

			int spawn_bounds_l = window->GetResolution().x / 6;
			int spawn_bounds_r = window->GetResolution().x / 12;

			GameObject_Types type = (rand() % 2 == 0) ? GameObject_Types::RED_BALOON : GameObject_Types::YELLOW_BALOON;
			int x = (rand() % (window->GetResolution().x - spawn_bounds_r - spawn_bounds_l)) + spawn_bounds_l;
			if (type == GameObject_Types::RED_BALOON) {
				spawnRedBalloon(glm::vec2(x, -25));
			}
			else {
				spawnYellowBalloon(glm::vec2(x, -25));
			}
			return;
		}

		// Spawn shurikens
		if (count_shurikens < max_shurikens) {
			last_spawn_t = total_time;
			count_shurikens++;

			int spawn_bounds_t = window->GetResolution().y / 10;
			int spawn_bounds_b = window->GetResolution().y / 10;

			int y = rand() % (window->GetResolution().y - spawn_bounds_t - spawn_bounds_b) + spawn_bounds_b;
			spawnShuriken(glm::vec2(window->GetResolution().x + 25, y));
			return;
		}
	}
}

void Tema1::RenderUI() {
	// Render power bar
	if (is_drawing_back) {
		double fraction = forceLevel() / drawback_time;

		glm::mat3 mMatrix = glm::mat3(1);
		mMatrix *= Transform2D::Translate(4 * fraction, 10);
		mMatrix *= Transform2D::Scale(4 * fraction, 0.2);
		RenderMesh2D(meshes["square"], shaders["VertexColor"], mMatrix);

		mMatrix = glm::mat3(1);
		mMatrix *= Transform2D::Translate(4, 10);
		mMatrix *= Transform2D::Scale(4, 0.2);
		RenderMesh2D(meshes["square_line"], shaders["VertexColor"], mMatrix);
	}

	// Render remaining lives
	glm::vec2 center(0, window->GetResolution().y - GameObject_Constants::lifeWidth);
	for (int i = 0; i < lives; ++i) {
		center += glm::vec2(GameObject_Constants::lifeWidth * 1.25f, 0);
		glm::mat3 mMatrix = glm::mat3(1);
		mMatrix *= Transform2D::Translate(center.x, center.y);
		RenderMesh2D(meshes["life"], shaders["VertexColor"], mMatrix);
	}

	// Print score
	std::cout << "Current score is : " << "                   " << '\r' << std::flush;
	std::cout << "Current score is : " << score << '\r' << std::flush;
}

void Tema1::CheckGameEnd() {
	if (lives == 0) {
		std::cout << "\n --- Press ENTER to exit --- \n\n";
		(void)getchar();
		exit(0);
	}
}

void Tema1::DifficultySelect() {
	int dif;
	std::cout << "Welcome to 'Bow & Arrow'. Select a difficulty by typing it's corresponding number\n";
	std::cout << "1. Easy\n";
	std::cout << "2. Medium (default)\n";
	std::cout << "3. Hard\n";
	std::cout << "Your choice : "; std::cin >> dif;

	switch (dif) {
	case 1: {
		lives = 4;
		max_shurikens = 1;
		max_balloons = 6;
		reload_time = 0.5f;
		balloon_speed = 75;
	} break;
	case 3: {
		lives = 2;
		max_balloons = 4;
		max_shurikens = 6;
		spawn_delay = 0.5f;
		reload_time = 1.5f;
		shuriken_speed = 750;
		balloon_speed = 125;
	} break;
	default: {
		lives = 3;
		max_shurikens = 3;
		max_balloons = 4;
		spawn_delay = 0.75f;
		shuriken_speed = 500;
	} break;
	}

	std::cout << "\nPress 'space' (in the game window) to start the game\n\n";
}

std::unordered_map<std::string, Mesh *> &Tema1::getMeshes()
{
	return this->meshes;
}

std::unordered_map<std::string, Shader *> &Tema1::getShaders()
{
	return this->shaders;
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	if (game_started) {
		total_time += deltaTimeSeconds;
		CheckGameEnd();

		RenderUI();
		ObjectSpawner();

		Physics::PhysixEngine::UpdateTime(deltaTimeSeconds);

		// Update reload time
		reload_t_remaining -= deltaTimeSeconds;

		auto& go = std::begin(gameObjects);
		while (go != std::end(gameObjects)) {
			updateBow(&(*go), deltaTimeSeconds);
			updateArrow(&(*go), deltaTimeSeconds);

			go->UpdatePhysics(deltaTimeSeconds);
			go->RenderObject();

			// Delete  out of bounds object
			glm::vec2 pos = go->getRigidBody().state.x;

			int e_limit = 25;
			if (pos.x > window->GetResolution().x + e_limit
				|| pos.x < -e_limit
				|| pos.y > window->GetResolution().y + e_limit
				|| pos.y < -e_limit) {
				GameObject_Types type = go->getType();

				if (type == GameObject_Types::RED_BALOON || type == GameObject_Types::YELLOW_BALOON) {
					count_balloons--;
				}
				else if (type == GameObject_Types::SHURIKEN) {
					count_shurikens--;
				}

				go = gameObjects.erase(go);
			}
			else {
				go++;
			}
		}
	}
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
	if (!game_started) {
		if (key == GLFW_KEY_SPACE) {
			game_started = true;
			std::cout << "-- Game started -- \n\n";
		}
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (game_started) {
		// Draw back the arrow
		if (button == 1 && !bow_shot) {
			drawback_s_time = total_time;
			is_drawing_back = true;
		}
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (game_started) {
		// Shoot arrow
		if (button == 1 && !bow_shot && is_drawing_back) {
			glm::vec2 arrowSpawn = glm::vec2(0, 0);
			double angle = 0;

			auto& go = std::begin(gameObjects);
			while (go != std::end(gameObjects)) {
				if (go->getType() == GameObject_Types::BOWARROW) {
					arrowSpawn = go->getRigidBody().state.x;

					angle = go->getRigidBody().state.rot;
					go->setType(GameObject_Types::BOW);
					break;
				}

				go++;
			}


			GameObject arrow(this, arrowSpawn, GameObject_Types::ARROW);
			arrow.MovementType(Physics::Constants::Motion_Type::SIMULATED);
			arrow.getRigidBody().state.drag_coef = 0.25;
			arrow.getRigidBody().state.gravity_coef = 17.5;
			arrow.getRigidBody().state.rot = bow_angle;

			double impulse_magnitude = max_impulse / drawback_time * forceLevel();

			glm::vec2 impulse = glm::vec2(cos(angle), sin(angle));
			impulse = glm::normalize(impulse);
			impulse *= impulse_magnitude;

			arrow.getRigidBody().addImpulse(impulse);

			bow_shot = true;
			reload_t_remaining = reload_time;
			is_drawing_back = false;

			gameObjects.push_back(arrow);
		}
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
