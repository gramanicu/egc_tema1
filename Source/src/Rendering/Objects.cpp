#include <Core/Engine.h>

#include "./Objects.hpp"

Mesh* Objects::CreateLife(glm::vec3 center, float width) {
	double p_count = 100;
	double radius = width / 2;
	glm::vec3 l_color = COLOR_RED;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.emplace_back(center, l_color);
	indices.push_back(0);

	for (int i = 0; i < p_count; i++) {
		double angle = 360 / p_count * i;
		angle = RADIANS(angle);
		vertices.emplace_back(center + glm::vec3(radius * sin(angle), radius * cos(angle), 0), l_color);
		indices.emplace_back(i + 1);
	}
	indices.push_back(1);

	Mesh* life = new Mesh("life");
	life->SetDrawMode(GL_TRIANGLE_FAN);
	life->InitFromData(vertices, indices);
	return life;
}

Mesh* Objects::CreateSquare(glm::vec3 center, float length, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(-length / 2, length / 2, 0), color),
		VertexFormat(center + glm::vec3(length / 2, length / 2, 0), color),
		VertexFormat(center + glm::vec3(length / 2, -length / 2, 0), color),
		VertexFormat(center + glm::vec3(-length / 2, -length / 2, 0), color)
	};

	Mesh* square = new Mesh("square");
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Objects::CreateSquareLine(glm::vec3 center, float length, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(-length / 2, length / 2, 0), color),
		VertexFormat(center + glm::vec3(length / 2, length / 2, 0), color),
		VertexFormat(center + glm::vec3(length / 2, -length / 2, 0), color),
		VertexFormat(center + glm::vec3(-length / 2, -length / 2, 0), color)
	};

	Mesh* square1 = new Mesh("square_line");
	square1->SetDrawMode(GL_LINE_LOOP);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	square1->InitFromData(vertices, indices);
	return square1;
}

Mesh* Objects::CreateRope(glm::vec3 top, float height) {
	float step = height / 6;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(top, COLOR_WHITE),											// 0
		VertexFormat(top + glm::vec3(0, -step, 0), COLOR_WHITE),				// 1
		VertexFormat(top + glm::vec3(step, -2 * step, 0), COLOR_WHITE),			// 2
		VertexFormat(top + glm::vec3(-step, -3 * step, 0), COLOR_WHITE),		// 3
		VertexFormat(top + glm::vec3(step, -4 * step, 0),COLOR_WHITE),			// 4
		VertexFormat(top + glm::vec3(-step, -5 * step, 0), COLOR_WHITE)			// 5

	};

	std::vector<unsigned short> indices = {
		0, 1, 2, 3, 4, 5
	};

	Mesh* rope = new Mesh("rope");
	rope->SetDrawMode(GL_LINE_STRIP);
	rope->InitFromData(vertices, indices);
	return rope;
}

Mesh* Objects::CreateBalloon(glm::vec3 center, float height, glm::vec3 color)
{
	double x_rad = height / 3;
	double y_rad = height / 2;
	double p_count = 100;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.emplace_back(center, color);

	for (int i = 0; i < p_count; i++) {
		double arc = 2 * M_PI * i / p_count;

		vertices.emplace_back(center + glm::vec3(x_rad * sin(arc),y_rad * cos(arc), 0), color);
		indices.push_back(i + 1);
	}
	indices.push_back(1);

	std::string mesh_name = "_balloon";
	if (color == COLOR_RED) {
		mesh_name = "red" + mesh_name;
	}
	else if (color == COLOR_YELLOW) {
		mesh_name = "yellow" + mesh_name;
	}

	Mesh* bow = new Mesh(mesh_name);
	bow->SetDrawMode(GL_TRIANGLE_FAN);
	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Objects::CreateShuriken(glm::vec3 center, float width)
{
	// 1   2   3
	//
	// 8   0   4
	//
	// 7   6   5

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center, COLOR_BLACK),											// 0
		VertexFormat(center + glm::vec3(-width / 2, width / 2, 0), COLOR_WHITE),	// 1
		VertexFormat(center + glm::vec3(0, width / 2, 0), COLOR_WHITE),				// 2
		VertexFormat(center + glm::vec3(width / 2, width / 2, 0), COLOR_WHITE),		// 3
		VertexFormat(center + glm::vec3(width / 2, 0, 0), COLOR_WHITE),				// 4
		VertexFormat(center + glm::vec3(width / 2, -width / 2, 0), COLOR_WHITE),	// 5
		VertexFormat(center + glm::vec3(0, -width / 2, 0), COLOR_WHITE),			// 6
		VertexFormat(center + glm::vec3(-width / 2, -width / 2, 0), COLOR_WHITE),	// 7
		VertexFormat(center + glm::vec3(-width / 2, 0, 0), COLOR_WHITE)				// 8
	};

	Mesh* shuriken = new Mesh("shuriken");
	shuriken->SetDrawMode(GL_TRIANGLES);
	std::vector<unsigned short> indices = {
		0, 1, 2,
		0, 3, 4,
		0, 5, 6,
		0, 7, 8
	};


	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

Mesh* Objects::CreateArrow(glm::vec3 middle, float length)
{
	//		      3
	// 1     0    2  4
	//		      5

	double tip_ratio = .1f;
	double tip_height = length * tip_ratio / 2;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(middle, COLOR_WHITE),																		// 0
		VertexFormat(middle + glm::vec3(-length / 2, 0, 0), COLOR_WHITE),										// 1
		VertexFormat(middle + glm::vec3(length / 2 - length * tip_ratio, 0, 0), COLOR_WHITE),					// 2
		VertexFormat(middle + glm::vec3(length / 2 - length * tip_ratio, tip_height / 2, 0), COLOR_WHITE),		// 3
		VertexFormat(middle + glm::vec3(length / 2 , 0, 0), COLOR_WHITE),										// 4
		VertexFormat(middle + glm::vec3(length / 2 - length * tip_ratio, -tip_height / 2, 0), COLOR_WHITE)		// 5
	};

	Mesh* arrow = new Mesh("arrow");
	arrow->SetDrawMode(GL_LINE_LOOP);
	std::vector<unsigned short> indices = {
		0, 1, 0, 2, 3, 4, 5, 2
	};

	arrow->InitFromData(vertices, indices);
	return arrow;
}

Mesh* Objects::CreateBow(glm::vec3 middle, float height)
{
	double p_count = 100;						// The number of points used to draw the bow
	double t_angle = BOW_T_ANGLE;				// The total angle the bow uses
	double s_angle = (180 - t_angle) / 2;		// The angle the bow starts at

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	// The middle of the string
	vertices.emplace_back(middle, COLOR_WHITE);


	// Compute the radius of the circle that will draw the bow, knowing the "chord" length and the arc height
	double radius = height / (2 * sin((t_angle / 180 * M_PI) / 2));
	double arc_height = radius - 0.5f * sqrt(4 * radius * radius - (double)height * height);
	
	// The center of the circle that makes the bow
	glm::vec3 circ_centre = middle + glm::vec3(arc_height - radius, 0, 0);	

	for (int i = 0; i <= p_count; i++) {
		double angle_deg = s_angle + t_angle / p_count * i;
		double angle_rad = angle_deg / 180 * M_PI;

		vertices.emplace_back(circ_centre + glm::vec3(sin(angle_rad) * radius, cos(angle_rad) * radius, 0), COLOR_WHITE);
		indices.push_back(i);
	}
	indices.push_back(p_count + 1);

	Mesh* bow = new Mesh("bow");
	bow->SetDrawMode(GL_LINE_LOOP);
	bow->InitFromData(vertices, indices);
	return bow;
}
