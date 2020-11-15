#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

#define COLOR_RED glm::vec3(1, 0, 0)
#define COLOR_BLUE glm::vec3(0, 0, 1)
#define COLOR_GREEN glm::vec3(0, 1, 0)
#define COLOR_YELLOW glm::vec3(1, 1, 0)
#define COLOR_CYAN glm::vec3(0, 1, 1)
#define COLOR_MAGENTA glm::vec3(1, 0, 1)
#define COLOR_WHITE glm::vec3(1, 1, 1)
#define COLOR_BLACK glm::vec3(0, 0, 0)

// The angle of a circle the bow uses
#define BOW_T_ANGLE 120

namespace Objects
{
	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(glm::vec3 center, float length, glm::vec3 color);

	// Create empty(no fill) square with given bottom left corner, length and color
	Mesh* CreateSquareLine(glm::vec3 center, float length, glm::vec3 color);

	/// <summary>
	/// Create a balloon.
	/// </summary>
	/// <param name="center">The center of the balloon(without the string)</param>
	/// <param name="height">The height of the balloon (without the string)</param>
	/// <param name="color">The color of the balloon</param>
	/// <returns>The balloon mesh</returns>
	Mesh* CreateBalloon(glm::vec3 center, float height, glm::vec3 color);

	/// <summary>
	/// Create a shuriken.
	/// </summary>
	/// <param name="center">The center of the shuriken</param>
	/// <param name="width">The width of the shuriken</param>
	/// <returns>The shuriken mesh</returns>
	Mesh* CreateShuriken(glm::vec3 center, float width);

	/// <summary>
	/// Create an arrow.
	/// </summary>
	/// <param name="middle">The middle of the arrow</param>
	/// <param name="length">The lenght of the arrow</param>
	/// <returns>The arrow mesh</returns>
	Mesh* CreateArrow(glm::vec3 middle, float length);

	/// <summary>
	/// Create an bow
	/// </summary>
	/// <param name="tip">The tip of the bow (where the arrow would rest on an actual bow)</param>
	/// <param name="height">The height of the bow</param>
	/// <returns>The bow mesh</returns>
	Mesh* CreateBow(glm::vec3 tip, float height);

	/// <summary>
	/// Create a string that will be attached to the bow
	/// </summary>
	Mesh* CreateRope(glm::vec3 top, float height);

	/// <summary>
	/// Draw a life (the lifes remaining for the player)
	/// </summary>
	/// <param name="center">The center</param>
	/// <param name="width">The width</param>
	/// <returns>The life mesh</returns>
	Mesh* CreateLife(glm::vec3 center, float width);
}

