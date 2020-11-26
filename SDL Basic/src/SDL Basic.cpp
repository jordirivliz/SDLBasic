//============================================================================
// Name        : SDL.cpp
// Author      : Jordi Rivera
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SDL.h>
#include "Screen.h"
#include "Swarm.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace jrl;

int main() {

	// Seeds our random number generator.
	srand(time(NULL));

	Screen screen;

	if (screen.init() == false) {
		cout << "Error initializing SDL" << endl;
	}

	Swarm swarm;

	// GAME LOOP
	while (true) {
		// Update particles

		// Draw particles
		// Time that has elapsed between iterations
		int elapsed = SDL_GetTicks();

		// Now we are going to blur the screen
		// screen.clear();

		swarm.update(elapsed);

		// Use sin function to create a range of colors.
		unsigned char green = (1 + sin(elapsed * 0.0001)) * 128;
		unsigned char red = (1 + sin(elapsed * 0.0002)) * 128;
		unsigned char blue = (1 + sin(elapsed * 0.0003)) * 128;

		// Loop through all particles and plot them in the screen
		const Particles *const pParticles = swarm.getParticles();
		for (int i = 0; i < Swarm::NPARTICLES; i++) {

			Particles particle = pParticles[i];
			int x = (particle.m_x + 1) * Screen::SCREEN_WIDTH / 2;
			// int y = (particle.m_y + 1) * Screen::SCREEN_HEIGHT / 2;
			int y = particle.m_y * Screen::SCREEN_WIDTH / 2 + Screen::SCREEN_HEIGHT / 2;

			// Display particles in screen.
			screen.setPixel(x, y, red, green, blue);
		}

		/*
		 // Use sin function to create a range of colors.
		 unsigned char green = (1 + sin(elapsed * 0.0001)) * 128;
		 unsigned char red = (1 + sin(elapsed * 0.0002)) * 128;
		 unsigned char blue = (1 + sin(elapsed * 0.0003)) * 128;


		 for (int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
		 for (int x = 0; x < Screen::SCREEN_WIDTH; x++) {
		 screen.setPixel(x, y, red, green, blue);
		 }
		 }
		 */

		// Blur the screen.
		screen.boxBlur();

		// Draw the screen
		screen.update();

		// Check for messages/events
		if (screen.processEvents() == false) {
			break;
		}
	}
	screen.close();

	return 0;
}
