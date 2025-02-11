/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015,
 * Yara Mahmoud
 * Zainab Ahmed
 * Rushda Khan
 * Sufiya Arab
 * All rights reserved.
 *
 */
#ifndef PLAYERS_H_
#define PLAYERS_H_

#include <stdbool.h>

#define MAX_LEN 256
#define MAX_PLAYERS 4

// Player struct for each player
typedef struct {
    char name[MAX_LEN];
    int score;
} player;

// Returns true if the player name matches one of the existing players
extern bool player_exists(player *players, int num_players, char *name);

// Go through the list of players and update the score for them 
extern void update_score(player *players, int num_players, char *name, int score);

// Initialize all of the players
extern void initialize_players(player *players, int num_players);

// Print each player and their score
extern void print_players(player *players, int num_players);

#endif /* PLAYERS_H_ */
