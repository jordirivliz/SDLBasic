/*
 * Particles.h
 *
 *  Created on: Sep 28, 2020
 *      Author: jordirivliz
 */

#ifndef PARTICLES_H_
#define PARTICLES_H_

namespace jrl {

struct Particles {

	double m_x;
	double m_y;

private:
	// double m_xspeed;
	// double m_yspeed;
	double m_speed;
	double m_direction;

private:
	void init();

public:
	Particles();
	virtual ~Particles();
	void update(int interval);

};

} /* namespace jrl */

#endif /* PARTICLES_H_ */
