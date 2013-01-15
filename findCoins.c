#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Board  Dimensionss
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
typedef struct {
	int xPos, yPos;
} Queue;
typedef struct {
	int type; //0 non-obstical, 1 coin, 2 obstical
	int position;
} Maze;

Object *objects;
int objects_num = 26;
int front = 0, back = 0;

Object *find_coins(int unit_id, int *size);
Object copy_object(int unit_id);

int main() {
	objects = malloc(sizeof(Object)*objects_num);
    int size, i, x, y;
	srand(time(NULL));
	
	int visited[HEIGHT][WIDTH];
	memset(visited, 0, sizeof(int) * HEIGHT * WIDTH);
	
	x = rand() % HEIGHT;
	y = rand() % WIDTH;
	visited[x][y] = 1;
	objects[0].x = x;
	objects[0].y = y;
	objects[0].team = 0;
	objects[0].type = RECRUIT;
	objects[0].id = x * WIDTH + y;
	
	for (i=1;i<=25;) {
		x = rand() % HEIGHT;
		y = rand() % WIDTH;
		if (visited[x][y]) continue;
		visited[x][y] = 1;
		objects[i].x = x;
		objects[i].y = y;
		objects[i].type = COIN;
		objects[i++].id = x * WIDTH + y;
	}
	printf("\nCurrent position (x) (y): %d %d\n", objects[0].x, objects[0].y);
	Object *temp = find_coins(0, &size);
    for (i=0;i<size;i++)
        printf("\nPosition #%d (x) (y): %d %d", i+1, temp[i].x, temp[i].y);
	printf("\n\n");
	return 0;
}

Object copy_object(int unit_id) {
	Object ptr;
	memcpy(&ptr, &objects[unit_id], sizeof(Object));
	return ptr;
}

void queue(Queue *queue, int x, int y) {
	queue[back].xPos = x;
	queue[back++].yPos = y;
}
Queue pop (Queue *queue) {
	return queue[front++];
}

Object *find_coins(int unit_id, int *size) {
	Queue list[HEIGHT * WIDTH], ptr;	//queue
	Maze board[HEIGHT][WIDTH];	//board
	int visited[HEIGHT][WIDTH];
	int temp[objects_num], right = 0; //position queue
	memset(board, 0, sizeof(Maze) * HEIGHT * WIDTH);
	memset(visited, 0, sizeof(int) * HEIGHT * WIDTH);
	int i, type, x, y;
		
	for (i=0;i<objects_num;i++) {
		switch(objects[i].type) {
			case 1:
			case 4:	type = 2;
					break;
			case 2:
			case 3:
				if (objects[i].team != objects[unit_id].team)
					type = 2;
				else 
					type = 0;
				break;
			case 5:	type = 1;
					break;
			default:
					type = 0;
					break;
		}
		board[objects[i].x][objects[i].y].type = type;
		board[objects[i].x][objects[i].y].position = i;
	}
	
	visited[objects[unit_id].x][objects[unit_id].y] = 1;
	queue(list, objects[unit_id].x, objects[unit_id].y);
	while (front != back) {
		ptr = pop(list);
		x = ptr.xPos;
		y = ptr.yPos;

		if (board[x][y].type == 1)	//if a coin add to the new list.
			temp[right++] = board[x][y].position;

		if (x + 1 < HEIGHT && board[x + 1][y].type != 2 && !visited[x + 1][y]) {
			visited[x + 1][y] = 1;
			queue(list, x + 1, y);
		}
		if (x > 0 && board[x - 1][y].type != 2 && !visited[x - 1][y]) {
			visited[x - 1][y] = 1;
			queue(list, x - 1, y);
		}
		if (y + 1 < WIDTH && board[x][y + 1].type != 2 && !visited[x][y + 1]) {
			visited[x][y + 1] = 1;
			queue(list, x, y + 1);
		}
		if (y > 0 && board[x][y - 1].type != 2 && !visited[x][y - 1]) {
			visited[x][y - 1] = 1;
			queue(list, x, y - 1);
		}
	}	
	*size = right;
	Object *retObject = malloc(right * sizeof(Object));
		if (!retObject) return NULL;
	for (i=0;i<right;i++)
		retObject[i] = copy_object(temp[i]);
	return retObject;
}
