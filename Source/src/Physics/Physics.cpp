#include "Physics.hpp"

#include <iostream>

using namespace Physics;

double PhysixEngine::current_time = 0.f;

glm::vec2 PhysixEngine::compute_acceleration(const State& state, double t)
{
	// Drag = - drag_coef * V (this is simplified, as it would need to be divided
	// by the object mass to result an acceleration)
	glm::vec2 drag = glm::vec2(-state.v.x * state.drag_coef, 0);
	
	// Gravitational acceleration
	glm::vec2 grav = glm::vec2(0, -Constants::G_CONSTANT * state.gravity_coef);
	
	// The object acceleration
	return grav + drag;
}

Derivative PhysixEngine::evaluate(const State& initial, double t, float dt, const Derivative& d)
{
	State curr_state;
	curr_state.drag_coef = initial.drag_coef;
	curr_state.gravity_coef = initial.gravity_coef;
	
	// x(t+1) = x(t) + dx/dt 
	curr_state.x = initial.x + d.dx * dt;
	
	// v(t+1) = v(t) + dv/dt 
	curr_state.v = initial.v + d.dv * dt;

	Derivative output;
	output.dx = curr_state.v;
	output.dv = compute_acceleration(curr_state, t + dt);

	return output;
}

void PhysixEngine::integrate(RigidBody& body, double t, float dt, bool RK4)
{
	if (body.m_type == Constants::Motion_Type::SIMULATED) {
		if (RK4) {
			// Runge-Kutta 4th Order
			Derivative a, b, c, d;

			a = evaluate(body.state, t, 0.0f, Derivative());
			b = evaluate(body.state, t, dt * 0.5f, a);
			c = evaluate(body.state, t, dt * 0.5f, b);
			d = evaluate(body.state, t, dt, c);

			glm::vec2 dxdt = 1.0f / 6.0f * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
			glm::vec2 dvdt = 1.0f / 6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

			body.state.x += dxdt * dt;
			body.state.v += dvdt * dt;

			// Compute the rotation for the object
			if (!body.rotation_locked) {
				body.state.rot = atan2(body.state.v.y, body.state.v.x);
			}
		}
		else {
			// Semi-Implicit Euler
			Derivative d = evaluate(body.state, t, dt, Derivative());

			body.state.x += d.dx * dt;
			body.state.v += d.dv * dt;
		}
	}
	else {
		if (body.m_func != nullptr) {
			body.m_func(body.state, t, dt);
		}
	}
}

void PhysixEngine::UpdatePhysics(RigidBody &rigidBody ,const double deltaTime)
{
	if (rigidBody.physics_enabled) {
		PhysixEngine::integrate(rigidBody, current_time, deltaTime);
	}
}

void PhysixEngine::UpdateTime(const double deltaTime) {
	current_time += deltaTime;
}

void RigidBody::checkVelLimits()
{
	if (velocity_limit == -1) return;
	if (glm::length(state.v) > velocity_limit) {
		state.v = glm::normalize(state.v);
		state.v *= velocity_limit;
	}
}

RigidBody::RigidBody() : physics_enabled(true), rotation_locked(false), velocity_limit(2500.f), m_func(nullptr), m_type(Constants::Motion_Type::SIMULATED) {}

void RigidBody::addImpulse(glm::vec2 p)
{
	if (m_type == Constants::Motion_Type::SIMULATED) {
		this->state.v += p;

		checkVelLimits();
	}
}

void RigidBody::addImpulse(const double x, const double y)
{
	if (m_type == Constants::Motion_Type::SIMULATED) {
		this->state.v += glm::vec2(x, y);

		checkVelLimits();
	}
}