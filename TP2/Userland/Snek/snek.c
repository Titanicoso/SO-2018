#include "snek.h"

int main(int argc, char *argv[]) {
	player_t player;
	tail_t tail;
	snack_t snack; 

	//Initialize using malloc? Or leave it on stack?

	int playerAlive = 1;
	char inputKey;

	player.sprite = 'X';

	tail.sprite = '.';
	tail.length = 0;
	tail.youngestElemIndex = 0;
	tail.oldestElemIndex = 0;

	snack.sprite = 'o';
	snack.position.x = 10;
	snack.position.y = 10;

	while (playerAlive){
		handleInput(&inputKey, &player);
		update(&player, &tail, &snack, &playerAlive);
		draw(&player, &tail, &snack);
	}

	return 0;
}

void handleInput(char* inputKey, player_t* player){
	*inputKey = getchar();
	switch(*inputKey){
		case 'w':
			(*player).speed.x = 0;
			(*player).speed.y = 1;
			break;
		case 's':
			(*player).speed.x = 0;
			(*player).speed.y = -1;
			break;
		case 'd':
			(*player).speed.x = 1;
			(*player).speed.y = 0;
			break;
		case 'a':
			(*player).speed.x = -1;
			(*player).speed.y = 0;
	}	
}

void update(player_t* player, tail_t* tail, snack_t* snack, int* playerAlive){
	updatePlayer(player);
	updateTail(player, tail);
	checkCollisionWithSnack(player, tail, snack);
	checkCollisionWithTail(player, tail, playerAlive);
}

void draw(player_t* player, tail_t* tail, snack_t* snack){
	int80(2,0,0,0); //Clear Screen syscall
	putcharatpos((*player).sprite, (*player).position.x, (*player).position.y);
	for (int i = (*tail).oldestElemIndex; i < (*tail).length; i++){
		putcharatpos((*tail).sprite, (*tail).tailPositions[i].x, (*tail).tailPositions[i].y);
	}
	putcharatpos((*snack).sprite, (*snack).position.x, (*snack).position.y);
}

void updatePlayer(player_t* player){
	(*player).position.x += (*player).speed.y;
	(*player).position.x += (*player).speed.y;
}

void updateTail(player_t* player, tail_t* tail){
	(*tail).tailPositions[(*tail).youngestElemIndex].x = (*player).position.x;
	(*tail).tailPositions[(*tail).youngestElemIndex].y = (*player).position.y;
	(*tail).youngestElemIndex++;
	(*tail).oldestElemIndex++;
	if ((*tail).youngestElemIndex >= TAIL_LENGTH_LIMIT)
		(*tail).youngestElemIndex = 0;
	if ((*tail).oldestElemIndex >= TAIL_LENGTH_LIMIT)
		(*tail).oldestElemIndex = 0;
}

void checkCollisionWithSnack(player_t* player, tail_t* tail, snack_t* snack){
	if ((*player).position.x == (*snack).position.x && (*player).position.y == (*snack).position.y){
		(*tail).length++;
		(*tail).oldestElemIndex--;
		(*snack).position.x = getSeconds() % GAME_WIDTH;
		(*snack).position.y = getSeconds() % GAME_HEIGHT;
	}
}

void checkCollisionWithTail(player_t* player, tail_t* tail, int* playerAlive){
	for (int i = (*tail).oldestElemIndex; i < (*tail).length; i++){
		if ((*tail).tailPositions[i].x == (*player).position.x && 
			(*tail).tailPositions[i].y == (*player).position.y)
			*playerAlive = 0;
	}
}

