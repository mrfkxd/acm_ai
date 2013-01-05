#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Board  Dimensionss
#define WIDTH 25
#define HEIGHT 15 

//Direction
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// Action Types
#define MOVE 1
#define ATTACK 2
#define BUILD_RECRUIT 3
#define BUILD_VETERAN 4
#define BUILD_WALL 5

//Boolean Values use in place of 1 or 0
#define FALSE 0
#define TRUE 1

//Objects Types
#define FACTORY 1
#define RECRUIT 2
#define VETERAN 3
#define WALL 4
#define COIN 5
#define FLAG 6
#define SQUARE 7

typedef struct object {
	int id; // Unique object_id
	int type; // Value of 1-7 (based on defined constants above)
	int x; // X coordinate
	int y; // Y coordinate
	int team; // Team_id of the team the object belongs to (if applicable)
	int health; // Health of object (if applicable)
	int flag; // The object_id of the flag this object is holding (if applicable)
} Object;


Object *objects;
int objects_num = 2;
int main() {
	objects = malloc(sizeof(Object)*objects_num);
	objects[0].id = 1;
	objects[0].type = SQUARE;
	objects[0].x = 3;
	objects[0].y = 5;

	objects[1].id = 2;
	objects[1].type = RECRUIT;
	objects[1].x = 3;
	objects[1].y = 4;
	
	printf("Before move: x: %d, y: %d\n", objects[1].x, objects[1].y);
	move(&objects[1], 3, 5);
	printf("After move: x: %d, y: %d\n", objects[1].x, objects[1].y);
}

int move(Object *obj, int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return FALSE;
    }
    if (abs(obj->x - x) > 1 || abs(obj->y - y) > 1 || (abs(obj->x -x) == 1 && abs(obj->y - y) == 1)) {
		return FALSE;
    }

    int i, square_id = 0;
	for (i = 0; i < objects_num; i++) {
		if (objects[i].type == SQUARE && objects[i].x == x && objects[i].y == y) {
			square_id = objects[i].id;
		}
	}

	if (square_id) {
		obj->x = x;
		obj->y = y;
		//log_move(obj->id, MOVE, square_id);
		return TRUE;
    }
	
    return FALSE;
}
