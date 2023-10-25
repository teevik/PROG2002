#ifndef PROG2002_CONSTANTS_H
#define PROG2002_CONSTANTS_H

const int BOARD_SIZE = 8;
const int TEAM_ROWS = 2;
const int BOARD_PIECES = BOARD_SIZE * TEAM_ROWS * 2; // 2 teams
const float PIECE_SCALE = 1. / (float) BOARD_SIZE;

const float CAMERA_SENSITIVITY = 1.75f;
const float ZOOM_SENSITIVITY = 1.f;
const float MIN_ZOOM = 0.6f;
const float MAX_ZOOM = 1.5f;

#endif //PROG2002_CONSTANTS_H
