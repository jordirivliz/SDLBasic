/*
 * Swarm.h
 *
 *  Created on: Sep 28, 2020
 *      Author: jordirivliz
 */

#ifndef SWARM_H_
#define SWARM_H_

#include "Particles.h"

namespace jrl {

class Swarm {
public:
	const static int NPARTICLES = 5000;
private:
	Particles *m_pParticles;
	int lastTime;

public:
	Swarm();
	virtual ~Swarm();
	void update(int elapsed);

	const Particles * const getParticles() {
		return m_pParticles;
	}
};

} /* namespace jrl */

#endif /* SWARM_H_ */
