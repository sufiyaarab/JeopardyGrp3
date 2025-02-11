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
        printf("Enter the name of player %d: ", i+1); //read player name from input
        scanf("%s", players[i].name);
        players[i].score = 0; //set initial score
    }
}

// Function to check if a player exists in the list. It will return true if a player with the given name is found, false otherwise
bool player_exists(player *players, int num_players, char *name)
{
    for (int i = 0; i < num_players; i++){
        if (strcmp(players[i].name, name) == 0){
            return true;
        }
    }
    return false;
}

// Go through the list of players and update the score for them by adding a given score
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
