int write_dungeon(dungeon_t *d)
{
  char *home;
  char *filename;
  FILE *f;
  size_t len;
  uint32_t be32;
  uint16_t be16;

  if (!(home = getenv("HOME"))) {
    fprintf(stderr, "\"HOME\" is undefined.  Using working directory.\n");
    home = ".";
  }

  len = (strlen(home) + strlen(SAVE_DIR) + strlen(DUNGEON_SAVE_FILE) +
         1 /* The NULL terminator */                                 +
         2 /* The slashes */);

  filename = malloc(len * sizeof (*filename));
  sprintf(filename, "%s/%s/", home, SAVE_DIR);
  makedirectory(filename);
  strcat(filename, DUNGEON_SAVE_FILE);

  if (!(f = fopen(filename, "w"))) {
    perror(filename);
    free(filename);

    return 1;
  }
  free(filename);

  /* The semantic, which is 6 bytes, 0-5 */
  fwrite(DUNGEON_SAVE_SEMANTIC, 1, strlen(DUNGEON_SAVE_SEMANTIC), f);

  /* The version, 4 bytes, 6-9 */
  be32 = htobe32(DUNGEON_SAVE_VERSION);
  fwrite(&be32, sizeof (be32), 1, f);

  /* The size of the rest of the file, 4 bytes, 10-13 */
  be32 = htobe32(calculate_dungeon_size(d));
  fwrite(&be32, sizeof (be32), 1, f);

  /* The dungeon map, 61440 bytes, 14-61453 */
  write_dungeon_map(d, f);

  /* The room count, 2 bytes, 61454-61455 */
  be16 = htobe16(d->num_rooms);
  fwrite(&be16, sizeof (be16), 1, f);

  /* And the rooms, be16 * 4 bytes, 61456-end */
  write_rooms(d, f);

  fclose(f);

  return 0;
}

static int write_dungeon_map(dungeon_t *d, FILE *f)
{
  uint32_t x, y;
  uint8_t zero = 0;
  uint8_t non_zero = 1;
  uint32_t found_error;

  for (found_error = 0, y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      /* First byte is zero if wall, non-zero if floor */
      switch (mapxy(x, y)) {
      case ter_debug:
        if (!found_error) {
          found_error = 1;
          fprintf(stderr, "Bad cell in dungeon.  Output may be corrupt.\n");
        }
        /* Allow to fall through and write a wall cell. */
      case ter_wall:
      case ter_wall_no_room:
      case ter_wall_no_floor:
      case ter_wall_immutable:
        fwrite(&zero, sizeof (zero), 1, f);
        break;
      case ter_floor:
      case ter_floor_room:
      case ter_floor_hall:
      case ter_floor_tentative:
        fwrite(&non_zero, sizeof (non_zero), 1, f);
        break;
      default:
        if (!found_error) {
          found_error = 1;
          fprintf(stderr, "Bad cell in dungeon.  Output may be corrupt.\n");
        }
        fwrite(&zero, sizeof (zero), 1, f);
      }

      /* Second byte is non-zero if room, zero otherwise */
      switch (mapxy(x, y)) {
      case ter_debug:
      case ter_wall:
      case ter_wall_no_room:
      case ter_wall_no_floor:
      case ter_wall_immutable:
      case ter_floor:
        fwrite(&zero, sizeof (zero), 1, f);
        break;
      case ter_floor_room:
        fwrite(&non_zero, sizeof (non_zero), 1, f);
        break;
      case ter_floor_hall:
      case ter_floor_tentative:
      default:
        fwrite(&zero, sizeof (zero), 1, f);
      }

      /* Third byte is non-zero if a corridor, zero otherwise */
      switch (mapxy(x, y)) {
      case ter_debug:
      case ter_wall:
      case ter_wall_no_room:
      case ter_wall_no_floor:
      case ter_wall_immutable:
      case ter_floor:
      case ter_floor_room:
        fwrite(&zero, sizeof (zero), 1, f);
        break;
      case ter_floor_hall:
        fwrite(&non_zero, sizeof (non_zero), 1, f);
        break;
      case ter_floor_tentative:
      default:
        fwrite(&zero, sizeof (zero), 1, f);
      }

      /* And the fourth byte is the hardness */
      fwrite(&hardnessxy(x, y), sizeof (hardnessxy(x, y)), 1, f);
      
    }
  }

  return 0;
}

int write_rooms(dungeon_t *d, FILE *f)
{
  uint32_t i;

  for (i = 0; i < d->num_rooms; i++) {
    /* write order is xpos, ypos, width, height */
    fwrite(&d->rooms[i].position[dim_x], 1, 1, f);
    fwrite(&d->rooms[i].position[dim_y], 1, 1, f);
    fwrite(&d->rooms[i].size[dim_x], 1, 1, f);
    fwrite(&d->rooms[i].size[dim_y], 1, 1, f);
  }

  return 0;
}

static int read_dungeon_map(dungeon_t *d, FILE *f)
{
  uint32_t x, y;
  uint8_t open, room, corr;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      fread(&open, sizeof (open), 1, f);
      fread(&room, sizeof (room), 1, f);
      fread(&corr, sizeof (corr), 1, f);
      fread(&hardnessxy(x, y), sizeof (hardnessxy(x, y)), 1, f);
      if (room) {
        mapxy(x, y) = ter_floor_room;
      } else if (corr) {
        mapxy(x, y) = ter_floor_hall;
      } else if (y == 0 || y == DUNGEON_Y - 1 ||
                 x == 0 || x == DUNGEON_X - 1) {
        mapxy(x, y) = ter_wall_immutable;
      } else {
        mapxy(x, y) = ter_wall;
      }
    }
  }

  return 0;
}

static int read_rooms(dungeon_t *d, FILE *f)
{
  uint32_t i;

  for (i = 0; i < d->num_rooms; i++) {
    fread(&d->rooms[i].position[dim_x], 1, 1, f);
    fread(&d->rooms[i].position[dim_y], 1, 1, f);
    fread(&d->rooms[i].size[dim_x], 1, 1, f);
    fread(&d->rooms[i].size[dim_y], 1, 1, f);
  }

  return 0;
}

int read_dungeon(dungeon_t *d, char *file)
{
  char semantic[6];
  uint32_t be32;
  uint16_t be16;
  FILE *f;
  char *home;
  size_t len;
  char *filename;
  struct stat buf;

  if (!file) {
    if (!(home = getenv("HOME"))) {
      fprintf(stderr, "\"HOME\" is undefined.  Using working directory.\n");
      home = ".";
    }

    len = (strlen(home) + strlen(SAVE_DIR) + strlen(DUNGEON_SAVE_FILE) +
           1 /* The NULL terminator */                                 +
           2 /* The slashes */);

    filename = malloc(len * sizeof (*filename));
    sprintf(filename, "%s/%s/%s", home, SAVE_DIR, DUNGEON_SAVE_FILE);

    if (!(f = fopen(filename, "r"))) {
      perror(filename);
      free(filename);
      exit(-1);
    }

    if (stat(filename, &buf)) {
      perror(filename);
      exit(-1);
    }

    free(filename);
  } else {
    if (!(f = fopen(file, "r"))) {
      perror(file);
      exit(-1);
    }
    if (stat(file, &buf)) {
      perror(file);
      exit(-1);
    }

  }

  d->num_rooms = 0;
  /* 16 is the 14 byte header + the two byte room count */
  if (buf.st_size < 16 + calculate_dungeon_size(d)) {
    fprintf(stderr, "Dungeon appears to be truncated.\n");
    exit(-1);
  }

  fread(semantic, sizeof(semantic), 1, f);
  if (strncmp(semantic, DUNGEON_SAVE_SEMANTIC, 6)) {
    fprintf(stderr, "Not an RLG229 save file.\n");
    exit(-1);
  }
  fread(&be32, sizeof (be32), 1, f);
  if (be32toh(be32) != 0) { /* Since we expect zero, be32toh() is a no-op. */
    fprintf(stderr, "File version mismatch.\n");
    exit(-1);
  }
  fread(&be32, sizeof (be32), 1, f);
  if (buf.st_size - 14 != be32toh(be32)) {
    fprintf(stderr, "File size mismatch.\n");
    exit(-1);
  }
  read_dungeon_map(d, f);
  fread(&be16, sizeof (be16), 1, f);
  d->num_rooms = be16toh(be16);
  if (buf.st_size != 14 + calculate_dungeon_size(d)) {
    fprintf(stderr, "Incorrect file size.\n");
    exit(-1);
  }
  read_rooms(d, f);

  fclose(f);

  return 0;
}
