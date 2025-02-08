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

 #include <gtk/gtk.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include <strings.h>  // Added for strcasecmp
 #include "questions.h"
 #include "players.h"
 #include "jeopardy.h"
 
 #define NUM_PLAYERS 4
 
 player players[NUM_PLAYERS];
 int current_player = 0;
 GtkWidget *main_window, *main_grid, *player_label;
 
 void refresh_board();
 
 int compare_scores(const void *a, const void *b) {
     return ((player *)b)->score - ((player *)a)->score;
 }
 
 void show_final_rankings() {
     GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_window_set_title(GTK_WINDOW(window), "Final Rankings");
     gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
     GtkWidget *grid = gtk_grid_new();
     gtk_container_add(GTK_CONTAINER(window), grid);
 
     gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Final Rankings:"), 0, 0, 1, 1);
 
     qsort(players, NUM_PLAYERS, sizeof(player), compare_scores);
 
     for (int i = 0; i < NUM_PLAYERS; i++) {
         char buffer[512];  // Increased buffer size
         snprintf(buffer, sizeof(buffer), "%d. %s - %d points", i + 1, players[i].name, players[i].score);
         gtk_grid_attach(GTK_GRID(grid), gtk_label_new(buffer), 0, i + 1, 1, 1);
     }
 
     gtk_widget_show_all(window);
 }
 
 bool all_questions_answered() {
     for (int i = 0; i < NUM_QUESTIONS; i++) {
         if (!questions[i].answered) return false;
     }
     return true;
 }
 
 void check_answer(GtkWidget *widget, gpointer data) {
     GtkWidget **widgets = (GtkWidget **)data;
     const gchar *answer = gtk_entry_get_text(GTK_ENTRY(widgets[1]));
     question *q = (question *)widgets[2];
 
     char trimmed_answer[256];
     sscanf(answer, " %255[^\n]", trimmed_answer);  // Fixed sscanf format
 
     if (strcasecmp(q->answer, trimmed_answer) == 0) {  // Case-insensitive comparison
         update_score(players, NUM_PLAYERS, players[current_player].name, q->value);
         gtk_label_set_text(GTK_LABEL(widgets[0]), "Correct!");
     } else {
         gtk_label_set_text(GTK_LABEL(widgets[0]), "Incorrect. Try again next time!");
     }
 
     q->answered = true;
     gtk_widget_hide(GTK_WIDGET(gtk_widget_get_toplevel(widget)));
 
     if (all_questions_answered()) {
         show_final_rankings();
     } else {
         current_player = (current_player + 1) % NUM_PLAYERS;
         refresh_board();
     }
 }
 
 void display_question_window(question *q) {
     GtkWidget *window, *grid, *label, *entry, *button;
     GtkWidget **widgets = malloc(3 * sizeof(GtkWidget *));
 
     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_window_set_title(GTK_WINDOW(window), "Answer the Question");
     gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
 
     grid = gtk_grid_new();
     gtk_container_add(GTK_CONTAINER(window), grid);
 
     label = gtk_label_new(q->question);
     gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);
 
     entry = gtk_entry_new();
     gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 2, 1);
 
     GtkWidget *feedback_label = gtk_label_new("");
     gtk_grid_attach(GTK_GRID(grid), feedback_label, 0, 3, 2, 1);
 
     widgets[0] = feedback_label;
     widgets[1] = entry;
     widgets[2] = (GtkWidget *)q;
 
     button = gtk_button_new_with_label("Submit Answer");
     g_signal_connect(button, "clicked", G_CALLBACK(check_answer), widgets);
     gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);
 
     gtk_widget_show_all(window);
 }
 
 void on_value_button_clicked(GtkWidget *widget, gpointer data) {
     (void)widget;
     question *q = (question *)data;
 
     if (q->answered) return;
 
     display_question_window(q);
 }
 
 void refresh_board() {
     gtk_widget_destroy(main_grid);
     main_grid = gtk_grid_new();
     gtk_container_add(GTK_CONTAINER(main_window), main_grid);
 
     char buffer[512];  // Increased buffer size
     snprintf(buffer, sizeof(buffer), "Current Player: %s", players[current_player].name);
     player_label = gtk_label_new(buffer);
     gtk_grid_attach(GTK_GRID(main_grid), player_label, 0, 0, 3, 1);
 
     const char *categories[] = {"programming", "algorithms", "databases"};
     for (int i = 0; i < NUM_CATEGORIES; i++) {
         GtkWidget *category_label = gtk_label_new(categories[i]);
         gtk_grid_attach(GTK_GRID(main_grid), category_label, i, 1, 1, 1);
     }
 
     for (int row = 0; row < 4; row++) {
         for (int col = 0; col < NUM_CATEGORIES; col++) {
             int index = row * NUM_CATEGORIES + col;
             GtkWidget *button;
             char label[32];
             snprintf(label, sizeof(label), "$%d", questions[index].value);
 
             button = gtk_button_new_with_label(label);
 
             if (questions[index].answered) {
                 gtk_widget_set_sensitive(button, FALSE);
             } else {
                 g_signal_connect(button, "clicked", G_CALLBACK(on_value_button_clicked), &questions[index]);
             }
 
             gtk_grid_attach(GTK_GRID(main_grid), button, col, row + 2, 1, 1);
         }
     }
 
     gtk_widget_show_all(main_window);
 }
 
 int main(int argc, char *argv[]) {
     gtk_init(&argc, &argv);
 
     initialize_game();
     initialize_players(players, NUM_PLAYERS);
 
     main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_window_set_title(GTK_WINDOW(main_window), "Jeopardy Game");
     gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
     g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
     main_grid = gtk_grid_new();
     gtk_container_add(GTK_CONTAINER(main_window), main_grid);
 
     refresh_board();
 
     gtk_widget_show_all(main_window);
     gtk_main();
 
     return 0;
 }
 