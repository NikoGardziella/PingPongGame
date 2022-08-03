/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 11:41:02 by ngardzie          #+#    #+#             */
/*   Updated: 2022/06/22 11:41:04 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../SDL2-2.0.22/include/SDL.h"
#include  "../includes/pong.h"

int served = 0;
SDL_Window *window;

void UpdateScore(int player, int points, t_Player *player1, t_Player *player2)
{
	served = 0;
	if (player == 1)
		player1->score += points;
	if(player == 2)
		player2->score += points;

		char *fmt = "Player %d | Player 2: %d";
		int len = snprintf(NULL, 0, fmt, player1->score, player2->score);
		char buf[len + 1];
		snprintf(buf,len + 1, fmt, player1->score, player2->score);
		SDL_SetWindowTitle(window, buf);
}

t_Player MakePlayer(t_Player *Player)
{
	Player->yPosition = HEIGHT / 2;
	Player->score = 0;
	return (*Player);
}

void UpdatePlayer(float elapsed, t_Player *Player1, t_Player *Player2, t_Ball *ball, SDL_Rect ballRect)
{
	const Uint8 *keyboardState;
	float PLAYER_MOVE_SPEED = 300;	
	keyboardState = SDL_GetKeyboardState(NULL);

	if(keyboardState[SDL_SCANCODE_SPACE])
		served = 1;

	if(keyboardState[SDL_SCANCODE_W] && Player1->yPosition > PLAYER_HEIGHT / 2)
		Player1->yPosition -= PLAYER_MOVE_SPEED * elapsed;
	if(keyboardState[SDL_SCANCODE_S] && Player1->yPosition <= HEIGHT - PLAYER_HEIGHT / 2)
		Player1->yPosition += PLAYER_MOVE_SPEED * elapsed;
	if(keyboardState[SDL_SCANCODE_UP] && Player2->yPosition > PLAYER_HEIGHT / 2)
		Player2->yPosition -= PLAYER_MOVE_SPEED * elapsed;
	if(keyboardState[SDL_SCANCODE_DOWN] && Player2->yPosition <= HEIGHT - PLAYER_HEIGHT / 2)
		Player2->yPosition += PLAYER_MOVE_SPEED * elapsed;
	
	SDL_Rect Player1Rect = {
		.x = PLAYER_MARGIN,
		.y = (int)(Player1->yPosition) - PLAYER_HEIGHT / 2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT
	};

	if(SDL_HasIntersection(&ballRect, &Player1Rect))
		ball->xSpeed = fabs(ball->xSpeed);  // ball goes right when hit

	SDL_Rect Player2Rect = {
		.x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
		.y = (int)(Player2->yPosition) - PLAYER_HEIGHT / 2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT
	};

	if(SDL_HasIntersection(&ballRect, &Player2Rect))
		ball->xSpeed = -fabs(ball->xSpeed);
}

void RenderPlayer(SDL_Renderer *renderer, t_Player *Player1, t_Player *Player2)
{
	
	SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	SDL_Rect Player1Rect = {
		.x = PLAYER_MARGIN,
		.y = (int)(Player1->yPosition) - PLAYER_HEIGHT / 2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT
	};
	SDL_RenderFillRect(renderer, &Player1Rect);

	SDL_SetRenderDrawColor(renderer, 0,0,255,255);
	SDL_Rect Player2Rect = {
		.x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
		.y = (int)(Player2->yPosition) - PLAYER_HEIGHT / 2,
		.w = PLAYER_WIDTH,
		.h = PLAYER_HEIGHT
	};
	SDL_RenderFillRect(renderer, &Player2Rect);
}

void Updateball(t_Ball *ball, float elapsed, t_Player *Player1, t_Player *Player2)
{
	if(!served)
	{
		ball->x = WIDTH / 2;
		ball->y = HEIGHT / 2;
		return ;
	}
	ball->x += ball->xSpeed * elapsed;
	ball->y += ball->ySpeed * elapsed;

	if(ball->x < 5)
		UpdateScore(2,100, Player1, Player2);
	if(ball->x > WIDTH - 5)
		UpdateScore(1,100, Player1, Player2);
	if(ball->y < 5)
		ball->ySpeed = fabs(ball->ySpeed);
	if(ball->y > HEIGHT - 5)
		ball->ySpeed = -fabs(ball->ySpeed);
}

SDL_Rect RenderBall(const t_Ball *ball, SDL_Renderer *renderer)
{

	int size = ball->size;
	int halfSize = size / 2;
	SDL_Rect rect = {
		.x = ball->x - halfSize,
		.y = ball->y - halfSize,
		.w = size,
		.h = size
	};
	SDL_SetRenderDrawColor(renderer, 255,255,255,255);
	SDL_RenderFillRect(renderer, &rect);

	return (rect);
}

t_Ball Makeball(int size, t_Ball *ball)
{
	const float Speed = 120;
	ball->x = WIDTH / 2 - size / 2;
	ball->y = HEIGHT / 2 - size / 2;
	ball->xSpeed = Speed;
	ball->ySpeed = Speed;
	ball->size = size;
	return (*ball);
}

void Update(float elapsed, SDL_Renderer *renderer, t_Ball *ball, t_Player *Player1, t_Player *Player2)
{
	SDL_Rect ballRect;
	
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);

	Updateball(ball, elapsed, Player1, Player2);
	ballRect = RenderBall(ball, renderer);

	UpdatePlayer(elapsed, Player1, Player2, ball, ballRect);
	RenderPlayer(renderer, Player1, Player2);

	SDL_RenderPresent(renderer);
}

void ShutDown(SDL_Renderer *renderer, SDL_Window *window)
{
	if(renderer)
		SDL_DestroyRenderer(renderer);
	if(window)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

int main()
{
	t_Ball *ball;
	int BALL_SIZE = 10;
	int quitBool = 0;
	t_Player *Player1;
	t_Player *Player2;

	Player1 = (t_Player*)malloc(sizeof(t_Player));
	Player2 = (t_Player*)malloc(sizeof(t_Player));
	ball = (t_Ball*)malloc(sizeof(t_Ball));
	Makeball(BALL_SIZE, ball);
	MakePlayer(Player1);
	MakePlayer(Player2);
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		fprintf(stderr, "Failed to initialize SDL: %s\n",SDL_GetError());
	window = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // 2nd value to choose  graphic card. -1 for default. FLAG use GPU to render instead of CPU. VSYNC syncs frames to monitors hz.
	SDL_Event event;
	Uint32 lastTick = SDL_GetTicks();
	while(!quitBool)
	{
		Uint32 curTick = SDL_GetTicks();
		Uint32 diff = curTick - lastTick;
		float elapsed = diff / 1000.0f;
		lastTick = curTick;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				quitBool = 1;
			Update(elapsed, renderer, ball, Player1, Player2);

		}
		Update(elapsed, renderer, ball, Player1, Player2);	
	}
	SDL_Quit();
	return (0);
}
