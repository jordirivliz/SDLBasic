/*
 * Screen.cpp
 *
 *  Created on: Sep 28, 2020
 *      Author: jordirivliz
 */

#include "Screen.h"

namespace jrl {

Screen::Screen() :
		m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(
		NULL) {
}

bool Screen::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		//cout << "SDL Init failed" << endl;
		return false;
	}

	// CREATE BLANK WINDOW
	m_window = SDL_CreateWindow("Particle Fire Explosion",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
			SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	// Check if the window was created.
	if (m_window == NULL) {
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Check if the Renderer was not created
	if (m_renderer == NULL) {
		//cout << "Could not create renderer" << endl;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	// Check if the Texture was not created
	if (m_texture == NULL) {
		//cout << "Could not create texture" << endl;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

	// Very Important function to set memory
	memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); // 0xFF = 255
	memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); // 0xFF = 255

	return true;
}

void Screen::boxBlur() {
	// Swap the buffers so pixel is in m_buffer2. We are drawing to m_buffer1.
	Uint32 *tmp = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = tmp;

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {

			/*
			 * 0 0 0
			 * 0 1 0
			 * 0 0 0
			 */

			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			for (int row = -1; row <= 1; row++) {
				for (int col = -1; col <= 1; col++) {
					int currentX = x + col;
					int currentY = y + row;

					if (currentX >= 0 && currentX < SCREEN_WIDTH
							&& currentY >= 0 && currentY < SCREEN_HEIGHT) {
						Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];

						// Get RGB components
						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;

					}
				}
			}

			Uint8 red = redTotal / 9;
			Uint8 green = greenTotal / 9;
			Uint8 blue = blueTotal / 9;

			setPixel(x, y, red, green, blue);
		}
	}
}

// Do not need it anymore
//void Screen::clear() {
//	memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
//}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

// Check if the pixel is out of the boundaries.
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return;
	}

	Uint32 color = 0;

// Order is Red Green Blue Alpha
	color += red;
	color = color << 8;
	color += green;
	color = color << 8;
	color += blue;
	color = color << 8;
	color += 0xFF;

// Set the pixel in the right position
	m_buffer1[(y * SCREEN_WIDTH) + x] = color;

// Set all the screen to one color
//	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
//		// Set each pixel to a concrete color
//		m_buffer[i] = 0xFF0000FF; -> RED
//		m_buffer[i] = 0x00FF00FF; -> GREEN
//		m_buffer[i] = 0x0000FFFF; -> BLUE
//		m_buffer[i] = 0xFFFF00FF; // -> YELLOW
//	}
}

void Screen::update() {
	SDL_UpdateTexture(m_texture, NULL, m_buffer1,
			SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}

bool Screen::processEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void Screen::close() {
// Delete the buffer
	delete[] m_buffer1;
	delete[] m_buffer2;

// Destroy Texture and Renderer
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);

// Destroy Window after it has been created and used.
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

} /* namespace jrl */
