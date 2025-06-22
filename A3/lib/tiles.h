#ifndef TILES_H
#define TILES_H 1

#define SUBTILE_W 16.0
#define SUBTILE_H 16.0

#define SUBTILE_GRASS_W 16.0
#define SUBTILE_GRASS_H 22.0

#define SUBTILE_GRASS_1_PATH "assets/tiles/grass_1.png"
#define SUBTILE_GRASS_2_PATH "assets/tiles/grass_2.png"
#define SUBTILE_GRASS_3_PATH "assets/tiles/grass_3.png"
#define SUBTILE_GRASS_L_PATH "assets/tiles/grass_l.png"
#define SUBTILE_GRASS_R_PATH "assets/tiles/grass_r.png"

#define SUBTILE_DIRT_W 16.0
#define SUBTILE_DIRT_H 16.0

#define SUBTILE_DIRT_PATH "assets/tiles/dirt.png"
#define SUBTILE_DIRT_L_PATH "assets/tiles/dirt_l.png"
#define SUBTILE_DIRT_R_PATH "assets/tiles/dirt_r.png"

typedef enum SUBTILE_ID
{
  SUBTILE_GRASS_1 = 0,
  SUBTILE_GRASS_2,
  SUBTILE_GRASS_3,
  SUBTILE_GRASS_L,
  SUBTILE_GRASS_R,
  SUBTILE_DIRT,
  SUBTILE_DIRT_L,
  SUBTILE_DIRT_R,
  SUBTILES_NUM,
  SUBTILE_NODRAW
} SUBTILE_ID;

typedef enum TILE_TYPE
{
  TILE_GRASS = 0,
  TILE_DIRT,
  TILES_NUM,
  TILE_NODRAW
} TILE_TYPE;

void tiles_init ();

void tiles_destroy ();

void tile_draw (TILE_TYPE type, float x, float y, float w, float h);

#endif /* tiles.h */
