#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include <Core/Engine.h>
#include <Component/SimpleScene.h>

#include "Physics/Transform2D.hpp"
#include "GameObject.hpp"

/// <summary>
/// This has something to do with ciclic dependencies
/// </summary>
class GameObject;

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

	// Exposes the scene mesh list
	void AddMeshToScene(Mesh *mesh);

	// Exposes the render mesh method
	void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat3 &modelMatrix);

	std::unordered_map<std::string, Mesh *> &getMeshes();
	std::unordered_map<std::string, Shader *> &getShaders();

	// Stop the thread for a specified amount of miliseconds
	static void sleepMilis(int timeMs);

	// Public game variables
	static double shuriken_speed;
	static double baloon_speed;

private:
	// The total time elapsed
	double total_time = 0.f;


	/// Bow shooting related variables
	// Reload mechanic
	double reload_time = 1.f;
	double reload_t_remaining = 0.f;
	bool bow_shot = false;
	
	// Arrow shooting mechanic
	double drawback_time = 1.5f;
	double drawback_s_time = 0.f;
	const double max_impulse = 1500.f;
	bool is_drawing_back = false;

	// The rotation angle limits for the bow
	const double rot_lim_l = -1.f;
	const double rot_lim_h = 1.f;

	// The movement speed of the bow
	const double bow_speed = 150.f;
	double bow_angle = 0;

	/// Game variables
	bool game_started = false;
	int lives = 3;
	int score = 0;
	int max_shurikens = 2;
	int max_baloons = 4;
	int count_shurikens = 0;
	int count_baloons = 0;
	double spawn_delay = 1;
	double last_spawn_t = 0;
	double score_red = 50;
	double score_yellow = -50;
	double score_shuriken = 200;

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void RenderUI();
	void DifficultySelect();
	void ObjectSpawner();
	void CheckGameEnd();

	void spawnRedBaloon(glm::vec2 pos);
	void spawnYellowBaloon(glm::vec2 pos);
	void spawnShuriken(glm::vec2 pos);
	void popBaloon(GameObject *baloon);
	void hitShuriken(GameObject* shuriken);
	void updateBow(GameObject* bow, double dt);
	void updateArrow(GameObject* arrow, double dt);
	double forceLevel();
	bool is_collision_pc(const glm::vec2& p, const glm::vec2& c, double radius);
	bool is_collision_cc(const glm::vec2& c1, const glm::vec2& c2, double radius1, double radius2);
};
