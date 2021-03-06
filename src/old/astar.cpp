#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "common.h"

#include <map>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define MAX_MAP_SIZE 1024*1024

bool first = false;
int items_added = 0;

int mapWidth = 512;
int mapHeight = 512;

int closed_size = 0;
POINT* closed_list[MAX_MAP_SIZE];
int lut[256];

//A Bigger map for later testing
int path[512][512];
//int path[20][20] = {
    //{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    //{0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    //{0,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    //{0,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0},
    //{0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    //{0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
    //{0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0},
    //{0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    //{0,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0},
    //{0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0},
    //{0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0},
    //{0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    //{0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,0},
    //{0,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0},
    //{0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    //{0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
    //{0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1},
    //{0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
    //{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
//};


int START_NODE = 0;
int END_NODE = 0;

int ALLOW_DIAGONAL = 0;

POINT* add_node(int id, int y, int x, POINT* parent) {
    POINT p;
    p.id = id;
    p.x = x;
    p.y = y;
    p.fcost = 0;
    p.hcost = 0;
    p.gcost = 0;
    p.parent = NULL;
    return points_add(&p);
}

void add_items(int sy=0, int sx=0, int dy=0, int dx=0) {
    START_NODE = (mapWidth * sy) + sx;
    END_NODE = (mapWidth * dy) + dx; 
    POINT *parent = NULL;

    int i,j;
    //printf("    ");
    //for (int i=0; i < mapWidth; i++) {
        //printf("%02d ",i);
    //}
    //printf("\n");

    //printf("    ");
    //for (int i=0; i < mapWidth; i++) {
        //printf("===",i);
    //}
    //printf("\n");

    for(i = 0; i < mapHeight; i++){
        //printf("%02d| ",i);
        for(j = 0; j < mapWidth; j++){

            int c_node = (i * mapWidth) + j;

            //if (c_node == START_NODE){
                //printf("SS ");
            //}else if(c_node == END_NODE){
                //printf("EE ");
            //}else{
                //printf("%i%i ", path[i][j], path[i][j]);
            //}

            parent = add_node(c_node, i, j, parent);
            items_added++;
        }
        //printf("\n");
    }
    //printf("\n");
}

void find_adj(int sy, int sx, int* successors){

    /* Representation of adjecent nodes
     * +-----+
     * |7|3|4|
     * |2|S|0|
     * |6|1|5|
     * +-----+
     */

    int start_node;
    for(int i=0; i<8; i++){

        POINT* s_node;
        start_node = 0;

        switch(i){
            case 0:
                start_node = (mapWidth * sy) + sx+1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
                break;

            case 1:
                start_node = (mapWidth * (sy + 1)) + sx;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 2:
                start_node = (mapWidth * sy) + sx - 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 3:
                start_node = (mapWidth * (sy - 1)) + sx;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 4:
                start_node = (mapWidth * (sy - 1)) + sx + 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 5:
                start_node = (mapWidth * (sy + 1)) + sx + 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 6:
                start_node = (mapWidth * (sy + 1)) + sx - 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;

            case 7:
                start_node = (mapWidth * (sy - 1)) + sx - 1;
                s_node = points_find(start_node);
                if(s_node)
                    successors[i] = start_node;
            break;
        }
    }
}

int manhatten(int sy, int sx, int dy, int dx) {
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return (2 * (tx + ty));
}

float chebyshev(int sy, int sx, int dy, int dx ) {
    float D = 1.41;
    int tx = abs(sx - dx);
    int ty = abs(sy - dy);
    return D * (tx + ty) + (D*2 - 2 * D) * MIN(tx, ty);
}

int test[200];
size_t cl = 0;

void create_path(POINT* p){
    int i = 0;
    //POINT* p = closed_find(id);
    while(p != NULL){
        int sy = p->y;
        int sx = p->x;
        path[sy][sx] = 'X';
        //printf("(%d,%d)\n", 
                //sy,sx, 
                //p->parent ? p->parent->x : 0,
                //p->parent ? p->parent->y : 0
        //);
        p = p->parent;

    };
}

void search(int sy, int sx, int dy, int dx) {
    int start_node = (mapWidth * sy) + sx;
    int END_NODE = (mapWidth * dy) + dx;

    POINT* s_node = points_find(start_node);
    printf("im searching for %d from %d\n", END_NODE, start_node);

    open_add(s_node);
    int successors[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    while(open_size() > 0){

        POINT* p = open_del();
        find_adj(p->y, p->x, successors);

        if (( mapWidth * p->y ) + p->x == END_NODE ){
            create_path(p);
            goto end;
        }
        closed_add(p);
        cl_print();

        int successor_count = 4;
        if (ALLOW_DIAGONAL)
            successor_count = 8;

        //printf("Lowest found was: %d%d fcost=%d parent=%d%d\n",
                //p->y, p->x,
                //p->fcost,
                //p->parent ? (p->parent->y) : 0,
                //p->parent ? (p->parent->x) : 0
        //);

        for(int i=0; i < successor_count; i++){
            POINT* successor;
            successor = points_find(successors[i]);

            if(!successor)
                continue;

            int sx = successor->x;
            int sy = successor->y;
            int gcost = successor->gcost;

            gcost += 1 * lut[path[sy][sx]];
            successor->fcost = manhatten(sy, sx, dy, dx) + gcost;

            successor->gcost = gcost;

            //printf("\tLowest found was: %d%d, fcost=%d parent=%d%d \n",
                    //successor->y, successor->x,
                    //successor->fcost,
                    //successor->parent ? (successor->parent->y) : 0,
                    //successor->parent ? (successor->parent->x) : 0
            //);

            POINT *tmp = open_search(successor->id);
            tmp = closed_find(successor->id);
            if (tmp) {

                if(tmp->fcost <= successor->fcost)
                    continue;
            }

            successor->parent = p;
            open_add(successor);
        }

        memset(successors, -1, sizeof(successors));

    }

end:
    open_destroy();
    //create_path(END_NODE);
    //printf("Finished :)\n");

}

void load_map(const char* mapfile){
    int idx = 0;
    FILE *fp;
    int ch;
    char mystring[100];
    fp=fopen(mapfile, "r");

    if (fp == NULL) {
        printf("Cannot open file '%s' : %s\n", mapfile, strerror(errno));
        return;
    }

    fgets (mystring , 100 , fp);
    fgets (mystring , 100 , fp);
    fgets (mystring , 100 , fp);
    fgets (mystring , 100 , fp);

    while( ( ch = fgetc(fp) ) != EOF ){
        if (ch == '.' || ch == '@') {
            path[idx / mapHeight][idx % mapWidth] = ch;
            ++idx;
        }
    }
}

int main() {
    memset(lut, 1, sizeof(int) * 200);
    lut['@'] = 999;
    lut['.'] = 1;

    load_map("maze512-16-0.map");
    add_items(3,3,145,69);
    search(3,3,145,69);
    points_destroy();

    for (int i=0; i<mapHeight; i++) {
        for (int j=0; j<mapWidth; j++ ){
            printf("%c", path[i][j]);
        }
        printf("\n");
    }
    
        
    return 0;
}
