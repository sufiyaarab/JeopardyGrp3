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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "players.h"
#include <time.h>

//initialize the players with a name and score of 0
void initialize_players(player *players, int num_players){
    for (int i = 0; i < num_players; i++){
        printf("Enter the name of player %d: ", i+1);
        scanf("%s", players[i].name);
        players[i].score = 0;
    }
}

// Returns true if the player name matches one of the existing players
bool player_exists(player *players, int num_players, char *name)
{
    for (int i = 0; i < num_players; i++){
        if (strcmp(players[i].name, name) == 0){
            return true;
        }
    }
    return false;
}

// Go through the list of players and update the score for the 
// player given their name
void update_score(player *players, int num_players, char *name, int score)
{
    for (int i = 0; i < num_players; i++){
        if (strcmp(players[i].name, name) == 0){
            players[i].score += score;
            return;
        }
    }
}

// Print the scores of each player
void print_players(player *players, int num_players)
{
    printf("Player Scores:\n");
    for (int i = 0; i < num_players; i++){
        printf("Player %s has a score of %d\n", players[i].name, players[i].score);
    }
}