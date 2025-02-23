#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "../General/Sprite.h"
#include "../Physics/BasicMovementFPSlimit.h"
#include "../General/gpRender.h"
#include "lg_enviroment.h"
#include "../General/Call_Asset.h"

constexpr int PLAYER_WIDTH = 52;
constexpr int PLAYER_HEIGHT = 50;
constexpr int ZONE_WIDTH = 3840; 
constexpr int ZONE_HEIGHT = 2160;

 std::vector<std::pair<int, int>> randNumVector(){

	std::vector<int> coorX;
	std::vector<int> coorY;
	std::vector<std::pair<int, int>> coords;

	srand(time(0));
	
	for(int i = 0; i<10; i++){
		coorX.push_back((rand()%100));
	}
	
	for(int i = 0; i<10; i++){
		coorY.push_back((rand()%200));
	}

	coords.reserve(10);
	std::transform(coorX.begin(), coorX.end(), coorY.begin(), std::back_inserter(coords), 
		[](int a, int b){return std::make_pair(a, b);});

	for(int k = 0; k<10; k++){
	 	std::cout << coords[k].first << ", " << coords[k].second << endl;
	}

	 return coords;
}

void run_lg_enviro(gpRender gr){

	//Vector used to store all on screen entities

	std::vector<Sprite*> osSprite;

	Call_Asset test1;

	
	
	//Camera Initilization
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	bool fixed = true;
	
	//randNumVector();
	std::vector <std::pair<int, int>> randCoords = randNumVector();
	

	//Player Entity Initilizaiton
	SDL_Texture* tex = gr.loadImage("Assets/Objects/ship_player.png");
	SDL_Rect db = {SCREEN_WIDTH/2 - PLAYER_WIDTH/2,SCREEN_HEIGHT/2 - PLAYER_HEIGHT/2,PLAYER_WIDTH,PLAYER_HEIGHT};
	Ship playerent(db, tex, 0);
	osSprite.push_back(&playerent);


	//Red giant Initilzation-
	test1.testOutput(gr, osSprite, randCoords);	

	//osSprite.push_back(&starent);
	
	
	//Ship Cruiser initilization
	//SDL_Texture* tex3 = gr.loadImage("Assets/Objects/ship_cruiser_enemy.png");
	//SDL_Rect db3 = {400,300,225,300};
	//Sprite emyent(db3, tex3);

	//osSprite.push_back(&emyent);

	srand(time(0));
	SDL_Rect bgtile[100];
	std::vector<std::vector<SDL_Rect*> > bgzonelayer1( ZONE_WIDTH/20 , std::vector<SDL_Rect*> (ZONE_HEIGHT/20, 0));
	std::vector<std::vector<SDL_Rect*> > bgzonelayer2( ZONE_WIDTH/40 , std::vector<SDL_Rect*> (ZONE_HEIGHT/40, 0));
	std::vector<int> bggalaxies(4);

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			bgtile[x + 10*y].x = x * 40;
			bgtile[x + 10*y].y = y * 40;
			bgtile[x + 10*y].w = 40;
			bgtile[x + 10*y].h = 40;
		}
	}
	
	for (int x = 0; x < ZONE_WIDTH/20; x++) {
		for (int y = 0; y < ZONE_HEIGHT/20; y++) {
			bgzonelayer1[x][y] = &bgtile[rand() % 100];
			if ((x < ZONE_WIDTH/40) && (y < ZONE_HEIGHT/40)) {
				bgzonelayer2[x][y] = &bgtile[rand() % 100];
			}
		}
	}

	//random background galaxies
	bggalaxies[0] = rand() % (ZONE_WIDTH - 200);
	bggalaxies[1] = rand() % (ZONE_HEIGHT - 200);
	
	bggalaxies[2] = rand() % (ZONE_WIDTH - 200);
	bggalaxies[3] = rand() % (ZONE_HEIGHT - 200);

	SDL_Event e;
	bool gameon = true;
	int animation = 0;
	bool cycle;
	bool animate = false;
	Uint32 anim_last_time = SDL_GetTicks();

	//Game Loop
	while(gameon) {
		gr.setFrameStart(SDL_GetTicks());

		//Handles all incoming Key events
		while(SDL_PollEvent(&e)) {
			gameon = handleKeyEvents(e, playerent);	
			switch(e.key.keysym.sym) {
				case SDLK_w:
					if(e.type == SDL_KEYDOWN){
						animate = true;
					}
					else if (e.type == SDL_KEYUP){
						animate = false;
					}
					break;
			}
		}


		updatePosition(playerent, osSprite, ZONE_WIDTH, ZONE_HEIGHT);

		if (animate){
			if (SDL_GetTicks() - anim_last_time > 150) {
				if (animation == 0){
					cycle = true;
				}
				else if(animation == 3){
					cycle = false;
				}
				
				if (cycle){
					animation++;
				}
				else{
					animation--;
				}
				
				anim_last_time = SDL_GetTicks();
				playerent.setF(animation);
			}
		}
		else{
			animation = 0;
			playerent.setF(animation);
		}

		//Renders all renderable objects onto the screen

		camera.x = playerent.getX() - SCREEN_WIDTH/2 + PLAYER_WIDTH/2;
		camera.y = playerent.getY() - SCREEN_HEIGHT/2 + PLAYER_HEIGHT/2;
		
		if (camera.x < 0){
			camera.x = 0;
			fixed = true;
		}
		else if (camera.x + SCREEN_WIDTH > ZONE_WIDTH){
			camera.x = ZONE_WIDTH - SCREEN_WIDTH;
			fixed = true;
		}
		if (camera.y < 0){
			camera.y = 0;
			fixed = true;
		}
		else if (camera.y + SCREEN_HEIGHT > ZONE_HEIGHT){
			camera.y = ZONE_HEIGHT - SCREEN_HEIGHT;
			fixed = true;
		}
		gr.renderOnScreenEntity(osSprite, bggalaxies, bgzonelayer1, bgzonelayer2, camera, fixed);
		
	}
}
