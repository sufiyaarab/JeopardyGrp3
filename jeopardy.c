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

 // Define the color palette
 #define COLOR_BLUE "#BAEDFD"
 #define COLOR_GREEN "#CBE9C5"
 #define COLOR_PINK "#FDEDF5"
 #define COLOR_GREY "#9F8C88"
 #define COLOR_LAVENDER "#EDD1FF" 
 
 void refresh_board();

 // Function to apply background color to widgets
 void apply_background_color(GtkWidget *widget, const char *color) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, 
        g_strdup_printf("* { background-color: %s; }", color), -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
 }


 // This function adds a font style to widgets
 void apply_font_style(GtkWidget *widget, const gchar *font) {
    PangoFontDescription *font_desc = pango_font_description_from_string(font);
    gtk_widget_override_font(widget, font_desc);
    pango_font_description_free(font_desc);
 }

 // Cute button with rounded corners, soft colors, and outline
 void apply_cute_button_style(GtkWidget *widget, const char *bg_color, const char *outline_color) {
    apply_background_color(widget, bg_color);
    gtk_widget_override_font(widget, pango_font_description_from_string("Poppins 14"));
    gtk_button_set_relief(GTK_BUTTON(widget), GTK_RELIEF_NONE);  // Flat style
    gtk_widget_set_size_request(widget, 150, 50);  // Larger buttons for easy clicking

    // Rounded corners and outline
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, 
        g_strdup_printf("* { border-radius: 25px; border: 2px solid %s; }", outline_color), -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
 }

 // Function to create a cute label with bubble-like styling
 GtkWidget* create_cute_label_with_bubble(const char *text, const char *bg_color, const char *outline_color) {
    GtkWidget *label = gtk_label_new(text);
    apply_font_style(label, "Poppins 16");
    apply_background_color(label, bg_color);  // Light green background
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);  // Center text horizontally

    // Apply bubble-like style with rounded corners and outline
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, 
        g_strdup_printf("* { border-radius: 15px; border: 2px solid %s; padding: 10px; }", outline_color), -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(label),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    return label;
 }
 
 int compare_scores(const void *a, const void *b) {
     return ((player *)b)->score - ((player *)a)->score;
 }
 
 void show_final_rankings() {
    GtkWidget *window, *grid, *title_label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Final Rankings");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 350);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);  // Add margin

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 15);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Styled title label
    title_label = gtk_label_new("🏆 Final Rankings 🏆");
    apply_font_style(title_label, "Poppins Bold 18");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);

    qsort(players, NUM_PLAYERS, sizeof(player), compare_scores);

    // Loop through players and display rankings
    for (int i = 0; i < NUM_PLAYERS; i++) {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "%d. %s - %d points", i + 1, players[i].name, players[i].score);
        
        GtkWidget *ranking_label = create_cute_label_with_bubble(buffer, COLOR_LAVENDER, COLOR_GREY);
        gtk_widget_set_margin_bottom(ranking_label, 10);  // Space between each rank
        gtk_grid_attach(GTK_GRID(grid), ranking_label, 0, i + 1, 1, 1);
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
         gtk_label_set_text(GTK_LABEL(widgets[0]), "<span foreground=\"green\">Correct!</span>");
     } else {
         gtk_label_set_text(GTK_LABEL(widgets[0]), "Incorrect. Try again next time!");
         gtk_label_set_text(GTK_LABEL(widgets[0]), "<span foreground=\"red\">Incorrect. Try again next time!</span>");
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
    GtkWidget *window, *grid, *label, *entry, *button, *feedback_label;
    GtkWidget **widgets = malloc(3 * sizeof(GtkWidget *));

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Answer the Question");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 250);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);  // Add padding

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Stylish question label
    label = gtk_label_new(q->question);
    gtk_widget_set_size_request(label, 450, 60);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_label_set_yalign(GTK_LABEL(label), 0.5);
    apply_font_style(label, "Poppins Bold 16");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    // Styled Entry Box
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter your answer...");
    gtk_widget_set_size_request(entry, 400, 40);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 2, 1);

    // Feedback Label
    feedback_label = gtk_label_new("");
    apply_font_style(feedback_label, "Poppins 14");
    gtk_grid_attach(GTK_GRID(grid), feedback_label, 0, 3, 2, 1);

    widgets[0] = feedback_label;
    widgets[1] = entry;
    widgets[2] = (GtkWidget *)q;

    // Stylish Button
    button = gtk_button_new_with_label("Submit Answer");
    apply_cute_button_style(button, COLOR_BLUE, COLOR_GREY);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(check_answer), widgets);

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
 
     // Display current player with a cute font
     char buffer[512];
     snprintf(buffer, sizeof(buffer), "Current Player: %s", players[current_player].name);
     player_label = create_cute_label_with_bubble(buffer, COLOR_PINK, COLOR_GREY);
     gtk_grid_attach(GTK_GRID(main_grid), player_label, 0, 0, 3, 1);
 
     const char *categories[] = {"Programming", "Algorithms", "Databases"};
     for (int i = 0; i < NUM_CATEGORIES; i++) {
         GtkWidget *category_label = create_cute_label_with_bubble(categories[i], COLOR_PINK, COLOR_GREY);
         gtk_widget_set_margin_start(category_label, 10);  // Margin for spacing
         gtk_widget_set_margin_end(category_label, 10);
         gtk_grid_attach(GTK_GRID(main_grid), category_label, i, 1, 1, 1);
     }
 
     int values[] = {100, 200, 300, 400}; 
     for (int row = 0; row < 4; row++) {
         for (int col = 0; col < NUM_CATEGORIES; col++) {
             int index = row * NUM_CATEGORIES + col;
             GtkWidget *button;
             char label[32];
             snprintf(label, sizeof(label), "$%d", values[row]);
 
             button = gtk_button_new_with_label(label);
             const char *button_color = (col == 2) ? COLOR_LAVENDER : (col % 2 == 0 ? COLOR_GREEN : COLOR_BLUE); 
             apply_cute_button_style(button, button_color, COLOR_GREY);

             // Add spacing between buttons
             gtk_grid_set_row_spacing(GTK_GRID(main_grid), 10);
             gtk_grid_set_column_spacing(GTK_GRID(main_grid), 10);
 
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
     apply_background_color(main_window, "#FFF9F8"); 
     g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
     main_grid = gtk_grid_new();
     gtk_container_add(GTK_CONTAINER(main_window), main_grid);

     // Center the "Jeopardy" title
     GtkWidget *title_label = gtk_label_new("Jeopardy");
     gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);  // Center text horizontally
     gtk_label_set_yalign(GTK_LABEL(title_label), 0.5);  // Center vertically
     gtk_widget_override_font(title_label, pango_font_description_from_string("Poppins 30"));
     apply_background_color(title_label, COLOR_PINK);  // Light green background
     gtk_grid_attach(GTK_GRID(main_grid), title_label, 0, 0, 3, 1);
 
     refresh_board();
 
     gtk_widget_show_all(main_window);
     gtk_main();
 
     return 0;
 }
 