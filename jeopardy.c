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
#include <gtk/gtk.h>
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
    //printf("Game Results:\n");
    //printf("Rank\tName\t\tScore\n");
    //for (int i = 0; i < num_players; i++) {
    //    printf("%d\t%s\t\t%d\n", i + 1, players[i].name, players[i].score);
   // }
   GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    char buffer[256];

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Game Results");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Game Results:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    for (int i = 0; i < num_players; i++) {
        snprintf(buffer, sizeof(buffer), "Rank %d: %s - %d points", i + 1, players[i].name, players[i].score);
        label = gtk_label_new(buffer);
        gtk_grid_attach(GTK_GRID(grid), label, 0, i + 1, 1, 1);
    }

    gtk_widget_show_all(window);
    gtk_main();
}

void on_category_button_clicked(GtkWidget *widget, gpointer data) {
    // Handle category button click
}

void on_value_button_clicked(GtkWidget *widget, gpointer data) {
    // Handle value button click
}

void create_game_interface() {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Jeopardy Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create category buttons
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        button = gtk_button_new_with_label(categories[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(on_category_button_clicked), NULL);
        gtk_grid_attach(GTK_GRID(grid), button, i, 0, 1, 1);
    }

    // Create value buttons
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < NUM_CATEGORIES; j++) {
            char value_label[16];
            snprintf(value_label, sizeof(value_label), "$%d", (i + 1) * 100);
            button = gtk_button_new_with_label(value_label);
            g_signal_connect(button, "clicked", G_CALLBACK(on_value_button_clicked), NULL);
            gtk_grid_attach(GTK_GRID(grid), button, j, i + 1, 1, 1);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();
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
    initialize_players(players, NUM_PLAYERS);
    create_game_interface();
    // Prompt for players names
    for (int i = 0; i < NUM_PLAYERS; i++){
        printf("Enter the name of player %d: ", i+1);
        scanf("%s", players[i].name);
        players[i].score = 0;
    }
    // initialize each of the players in the array
    // Perform an infinite loop getting command input from users until game ends
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        char category[MAX_LEN];
        int value;
        char answer[MAX_LEN];

        // Display categories and prompt for category and value
        display_categories();
        printf("Enter a category: ");
        scanf("%s", category);
        printf("Enter a value: ");
        scanf("%d", &value);

        // Display the question
        display_question(category, value);

        // Get the answer from the player
        printf("Enter your answer: ");
        scanf("%s", answer);

        // Check if the answer is correct
        if (valid_answer(category, value, answer)) {
            printf("Amazingly done, correct!\n");
            // Update the player's score
            printf("Enter the player's name: ");
            scanf("%s", buffer);
            update_score(players, NUM_PLAYERS, buffer, value);
        } else {
            printf("Incorrect...Sorry!\n");
        }

        // Check if all questions have been answered
        bool all_answered = true;
        for (int i = 0; i < NUM_QUESTIONS; i++) {
            if (!questions[i].answered) {
                all_answered = false;
                break;
            }
        }
        if (all_answered) {
            break;
        }
    }

    // Display the final results and exit
    show_results(players, NUM_PLAYERS);

    // Free the allocated memory at the end of the program
    free(players);

    return EXIT_SUCCESS;
    }
