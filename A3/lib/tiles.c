#include <allegro5/allegro5.h>

#include "common.h"
#include "tiles.h"

static ALLEGRO_BITMAP *subtiles[SUBTILES_NUM];

#define load(X) ensure (subtiles[X] = al_load_bitmap (X##_PATH))

void
tiles_init ()
{
  load (SUBTILE_GRASS_1);
  load (SUBTILE_GRASS_2);
  load (SUBTILE_GRASS_3);
  load (SUBTILE_GRASS_L);
  load (SUBTILE_GRASS_R);
  load (SUBTILE_DIRT);
  load (SUBTILE_DIRT_L);
  load (SUBTILE_DIRT_R);
}

#undef load

void
tiles_destroy ()
{
  for (int i = 0; i < TILES_NUM; i++)
    al_destroy_bitmap (subtiles[i]);
}

static void
draw_dirt (float x, float y, float w, float h)
{
  SUBTILE_ID id;
  int cols, rows;
  int col, row;
  float draw_x, draw_y;

  cols = (int)(w / SUBTILE_DIRT_W);
  rows = (int)(h / SUBTILE_DIRT_H);

  for (row = 0; row < rows; row++)
    {
      draw_y = y + row * SUBTILE_DIRT_H;

      for (col = 0; col < cols; col++)
        {
          draw_x = x + col * SUBTILE_DIRT_W;

          if (col == 0)
            id = SUBTILE_DIRT_L;

          else if (col == cols - 1)
            id = SUBTILE_DIRT_R;

          else
            id = SUBTILE_DIRT;

          al_draw_bitmap (subtiles[id], draw_x, draw_y, 0);
        }
    }
}

static void
draw_grass (float x, float y, float w, float h)
{
  SUBTILE_ID id;
  int cols, rows;
  int col, row;
  float draw_x, draw_y;
  int variant;
  float dirt_y, dirt_h;

  cols = (int)(w / SUBTILE_GRASS_W);
  rows = (int)(h / SUBTILE_GRASS_H);

  row = 0;
  draw_y = y + row * SUBTILE_GRASS_H;

  for (col = 0; col < cols; col++)
    {
      draw_x = x + col * SUBTILE_GRASS_W;

      if (col == 0)
        id = SUBTILE_GRASS_L;

      else if (col == cols - 1)
        id = SUBTILE_GRASS_R;

      else
        {
          variant = (col - 1) % 3;

          switch (variant)
            {
            case 0:
              id = SUBTILE_GRASS_1;
              break;

            case 1:
              id = SUBTILE_GRASS_2;
              break;

            default:
              id = SUBTILE_GRASS_3;
              break;
            }
        }

      al_draw_bitmap (subtiles[id], draw_x, draw_y, 0);
    }

  if (rows > 1)
    {
      dirt_y = y + SUBTILE_GRASS_H;
      dirt_h = (rows - 1) * SUBTILE_DIRT_H;

      draw_dirt (x, dirt_y, w, dirt_h);
    }
}

void
tile_draw (TILE_TYPE type, float x, float y, float w, float h)
{
  switch (type)
    {
    case TILE_GRASS:
      draw_grass (x, y, w, h);
      break;

    case TILE_DIRT:
      draw_dirt (x, y, w, h);
      break;

    default:
      break;
    }
}
