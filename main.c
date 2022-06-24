#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FILE "ball.bmp"
#define initW 500
#define initH 500
#define rW 50
#define rH 50
#define rMaxW (w - rW)
#define rMaxH (h - rH)
#define TICK 2
#define LOST 5 // Energy lost while colision with wall
#define GRAVITY 5 // Acceleration of gravity

#define PRESSX ev.button.x
#define PRESSY ev.button.y

#define MOTIONX ev.motion.x
#define MOTIONY ev.motion.y

#define BUTTONX ev.button.x
#define BUTTONY ev.button.y

int WinMain(void) {

	if (SDL_Init(SDL_INIT_EVERYTHING)) {

		fprintf(stderr, "SDL Init Error: %s\n", SDL_GetError());

		return 1;

	}

	SDL_Window *win = SDL_CreateWindow(
			"Foo",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			initW, initH, SDL_WINDOW_RESIZABLE
		);

	SDL_Renderer *rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	SDL_Rect rct = {
		.w = rW, .h = rH,
		.x = 500 / 2 - rW / 2, .y = initH - rH
	};

	SDL_Surface *simg = SDL_LoadBMP(FILE);

	if (simg == 0) {

		fprintf(stderr, "Could not read file %s\n", FILE);

		return 1;

	}

	SDL_Texture *tx = SDL_CreateTextureFromSurface(rnd, simg);

	SDL_Event ev;

	SDL_FreeSurface(simg);

	bool stop = false, holding = false;
	time_t t = time(NULL), c;
	size_t frames = 0, tick = 0;
	int w, h, speedX = 0, speedY = 0,
		offX, offY, lastX, lastY;

	for (;; tick = TICK == tick ? 0 : tick + 1) {

		while (SDL_PollEvent(&ev)) {

			switch (ev.type) {

				case SDL_QUIT:
					stop = true;
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (ev.button.button == SDL_BUTTON_LEFT &&
						PRESSX >= rct.x && PRESSX <= rct.x + rW &&
						PRESSY >= rct.y && PRESSY <= rct.y + rH) {

						holding = true;
						offX = BUTTONX - rct.x;
						offY = BUTTONY - rct.y;
						lastX = BUTTONX;
						lastY = BUTTONY;

					}
					break;

				case SDL_MOUSEBUTTONUP:

					if (ev.button.button == SDL_BUTTON_LEFT)
						holding = false;

					break;

				case SDL_MOUSEMOTION:

					if (holding) {

						speedX = MOTIONX - lastX;
						speedY = MOTIONY - lastY;

						lastX = MOTIONX;
						lastY = MOTIONY;

						rct.x = MOTIONX - offX;
						rct.y = MOTIONY - offY;

						if (rct.x < 0)
							rct.x = 0;
						else if (rct.x > rMaxW)
							rct.x = rMaxW;

						if (rct.y > rMaxH)
							rct.y = rMaxH;

					}

			}

		}

		if (stop)
			break;

		SDL_GetWindowSize(win, &w, &h);

		SDL_SetRenderDrawColor(rnd, 121, 143, 223, 1);

		SDL_RenderClear(rnd);

		SDL_RenderCopy(rnd, tx, NULL, &rct);

		SDL_RenderPresent(rnd);

		if (!holding) {

			if (speedX) {

				if (tick == TICK)
					speedX += speedX > 0 ? -1 : 1;

				if (speedX + rct.x < 0 || speedX + rct.x > rMaxW) {

					speedX += speedX > 0 ? -LOST : LOST;

					speedX = -speedX;

				}

			}

			if (rct.y + speedY + GRAVITY < rMaxH) {

				speedY += GRAVITY;

				rct.y += speedY;

			} else {

				rct.y = rMaxH;

				speedY = -speedY;

			}

			rct.x += speedX;

		}

		if (time(&c) - t == 1) {

			t = c;

			printf("\rWindow: %dx%d, FPS: %-8u", w, h, frames);

			frames = 0;

		} else
			frames++;

		SDL_Delay(14);

	}

	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;

}