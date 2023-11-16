#ifndef PROG2002_CONSTANTS_H
#define PROG2002_CONSTANTS_H

/// Size of chess board
const int BOARD_SIZE = 8;

/// How many rows of pieces each team has
const int TEAM_ROWS = 2;

/// Total amount of board pieces there will be
const int BOARD_PIECES = BOARD_SIZE * TEAM_ROWS * 2; // 2 teams

/// Size of each chess piece
const float PIECE_SCALE = 1.3f / (float) BOARD_SIZE;

const float CAMERA_SENSITIVITY = 1.75f;
const float ZOOM_SENSITIVITY = 1.f;
const float MIN_ZOOM = 0.6f;
const float MAX_ZOOM = 1.5f;

#endif //PROG2002_CONSTANTS_H
