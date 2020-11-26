/*
 * Particles.cpp
 *
 *  Created on: Sep 28, 2020
 *      Author: jordirivliz
 */

#include "Particles.h"
#include <stdlib.h>
#include <math.h>

namespace jrl {

Particles::Particles() {
	init();
}

Particles::~Particles() {
	// TODO Auto-generated destructor stub
}

void Particles::init() {
	// Get random value between -1 and 1 to originate particles
	// m_x = (2.0 * rand()) / RAND_MAX - 1;
	// m_y = (2.0 * rand()) / RAND_MAX - 1;

	// m_xspeed = 0.001 * (((2.0 * rand()) / RAND_MAX) - 1); // Multiply by double to get a double
	// m_yspeed = 0.001 * (((2.0 * rand()) / RAND_MAX) - 1);

	m_x = 0;
	m_y = 0;

	// Angle from 0 to  2π
	m_direction = (2 * M_PI * rand()) / RAND_MAX;

	// Declare a constant speed;
	m_speed = (0.04 * rand()) / RAND_MAX;

	// Average distance between particles increases.
	m_speed *= m_speed; // m_speed = m_speed * m_speed;

}

void Particles::update(int interval) {

//	m_x += m_xspeed;
//	m_y += m_yspeed;
//
//	// If particle reaches a extreme set it equal to its negative velocity
//	if (m_x < -1.0 || m_x >= 1.0) {
//		m_xspeed = -m_xspeed;
//	}
//
//	if (m_y < -1.0 || m_y >= 1.0) {
//		m_yspeed = -m_yspeed;
//	}

	// Make particles curl around
	m_direction += interval * 0.0004;

	// Determine x and y velocities with the angle and speed
	double x_speed = m_speed * cos(m_direction);
	double y_speed = m_speed * sin(m_direction);

	// Interval ensures constant speed in all systems that run this project.
	m_x += x_speed * interval;
	m_y += y_speed * interval;

	// Reset Particles when they reach the corners of the screen.
	if (m_x < -1.0 || m_x >= 1.0 || m_y < -1.0 || m_y >= 1.0) {
		init();
	}

	// Reset Particles Randomly
	if(rand() < RAND_MAX/100) {
		init();
	}

}

} /* namespace jrl */
