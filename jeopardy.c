/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, 
 * * Yara Mahmoud
 * Zainab Ahmed
 * Rushda Khan
 * Sufiya Arab
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Put global environment variables here

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens) {
    // Tokenize the answer to retrieve the answer and check if it is correct

}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players, int num_players){
    // Display the final results for each player including the name and final score
    printf("Game Results:\n");
    printf("Rank\tName\t\tScore\n");
    for (int i = 0; i < num_players; i++) {
        printf("%d\t%s\t\t%d\n", i + 1, players[i].name, players[i].score);
    }
}

void display_game_intro() {
    printf("***Welcome to Jeopardy!***\n");
    printf("You will be given a board of categories and questions.\n");
    printf("You will be asked to select a category and a question value.\n");
    printf("You will then be asked to provide an answer in the form of a question.\n");
    printf("You must answer in the form of a question.\n");
    printf("For example, 'What is the capital of Canada?'\n");
    printf("You will be awarded points based on the question value if you answer correctly.\n");
    printf("The game ends when all questions have been answered.\n");
    printf("The player with the most points wins!\n");
}



int main(int argc, char *argv[])
{
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    player *players = malloc(sizeof(player) * NUM_PLAYERS);
    if (players == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    player players[NUM_PLAYERS];
    
    // Input buffer and and commands

    char buffer[BUFFER_LEN] = { 0 };

    // Display the game introduction and initialize the questions
    display_game_intro();
    initialize_game();
    
    // Prompt for players names
    for (int i = 0; i < NUM_PLAYERS; i++){
        printf("Enter the name of player %d: ", i+1);
        scanf("%s", players[i].name);
        players[i].score = 0;
    }
    // initialize each of the players in the array
    free(players);

    // Perform an infinite loop getting command input from users until game ends
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        //Call functions from the questions and players source files
        
        // Execute the game until all questions are answered

        // Display the final results and exit
    }
    return EXIT_SUCCESS;
}
