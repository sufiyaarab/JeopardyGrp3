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
#include <strings.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

#define NUM_PLAYERS 4
#define MAX_SCORE 1000

player players[NUM_PLAYERS];
int current_player = 0;
GtkWidget *main_window, *main_grid, *player_label;
GtkWidget *progress_bars[NUM_PLAYERS];

// Function Prototypes
void refresh_board();
void apply_cute_button_style(GtkWidget *widget, const char *bg_color, const char *outline_color);
GtkWidget *create_cute_label_with_bubble(const char *text, const char *bg_color, const char *outline_color);
void display_question_window(question *q);
void update_progress_bar();

// Preset Colour palette
#define COLOR_BLUE "#BAEDFD"
#define COLOR_GREEN "#CBE9C5"
#define COLOR_PINK "#FDEDF5"
#define COLOR_GREY "#9F8C88"
#define COLOR_LAVENDER "#EDD1FF"

void on_question_button_clicked(GtkWidget *widget, gpointer data)
{
    question *q = (question *)data;
    display_question_window(q);
}
void update_progress_bar()
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        // normalize the score between 0 and 1 based on MAX_SCORE
        double progress = (double)players[i].score / MAX_SCORE;
        if (progress > 1.0)
            progress = 1.0;
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bars[i]), progress);
    }
}
// Refreshes board when changes are made
void refresh_board()
{
    gtk_widget_destroy(main_grid);
    main_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(main_window), main_grid);

    // Display the current player
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "Current Player: %s", players[current_player].name);
    player_label = create_cute_label_with_bubble(buffer, COLOR_PINK, COLOR_GREY);
    gtk_widget_set_margin_top(player_label, 20); // Add space from the top
    gtk_grid_attach(GTK_GRID(main_grid), player_label, 0, 0, 3, 1);

    const char *categories[] = {"Programming", "Algorithms", "Databases"};
    for (int i = 0; i < NUM_CATEGORIES; i++)
    {
        GtkWidget *category_label = create_cute_label_with_bubble(categories[i], COLOR_PINK, COLOR_GREY);
        gtk_widget_set_margin_start(category_label, 10);
        gtk_widget_set_margin_end(category_label, 10);
        gtk_grid_attach(GTK_GRID(main_grid), category_label, i, 1, 1, 1);
    }

    int values[] = {100, 200, 300, 400};
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < NUM_CATEGORIES; col++)
        {
            int index = col * 4 + row;
            GtkWidget *button;
            char label[32];
            snprintf(label, sizeof(label), "$%d", values[row]);

            button = gtk_button_new_with_label(label);
            const char *button_color = (col == 2) ? COLOR_LAVENDER : (col % 2 == 0 ? COLOR_GREEN : COLOR_BLUE);
            apply_cute_button_style(button, button_color, COLOR_GREY);

            gtk_grid_set_row_spacing(GTK_GRID(main_grid), 10);
            gtk_grid_set_column_spacing(GTK_GRID(main_grid), 10);

            if (questions[index].answered)
            {
                gtk_widget_set_sensitive(button, FALSE);
                gtk_widget_set_opacity(button, 0.5); // Optional: visually indicate disabled buttons
            }
            else
            {
                g_signal_connect(button, "clicked", G_CALLBACK(on_question_button_clicked), &questions[index]);
            }

            gtk_grid_attach(GTK_GRID(main_grid), button, col, row + 2, 1, 1);
        }
    }
    // Create a container box to hold all player boxes with an outline
    GtkWidget *player_container_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(player_container_box), 5);
    gtk_widget_set_name(player_container_box, "player-container"); // For styling the border

    // Create and display progress bars, names, and scores for each player
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        // Create a box for each player (vertical)
        GtkWidget *player_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

        // Add a border around the player box
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,
                                        g_strdup_printf("* { border: 2px solid %s; padding: 10px; border-radius: 10px; }", COLOR_GREY), -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(player_box),
                                       GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        // Create a label for the player's name
        GtkWidget *name_label = create_cute_label_with_bubble(players[i].name, COLOR_PINK, COLOR_GREY);
        gtk_box_pack_start(GTK_BOX(player_box), name_label, FALSE, FALSE, 0);

        // Create a label for the player's score
        char score_text[100];
        snprintf(score_text, sizeof(score_text), "Score: %d", players[i].score);
        GtkWidget *score_label = create_cute_label_with_bubble(score_text, COLOR_GREEN, COLOR_GREY);
        gtk_box_pack_start(GTK_BOX(player_box), score_label, FALSE, FALSE, 0);

        // Create the player's progress bar
        GtkWidget *progress_bar = gtk_progress_bar_new();
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0); // Max score normalized to 1.0
        gtk_box_pack_start(GTK_BOX(player_box), progress_bar, FALSE, FALSE, 0);

        // Add the player box to the container box
        gtk_box_pack_start(GTK_BOX(player_container_box), player_box, FALSE, FALSE, 0);

        // Store the reference to the progress bar
        progress_bars[i] = progress_bar;
    }

    // Attach the player container box to the grid
    gtk_grid_attach(GTK_GRID(main_grid), player_container_box, 3, 2, 1, 4); // Adjust position as needed

    update_progress_bar();

    gtk_widget_show_all(main_window);
}

void apply_background_color(GtkWidget *widget, const char *color)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    g_strdup_printf("* { background-color: %s; }", color), -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// Applys preset consistent font style
void apply_font_style(GtkWidget *widget, const gchar *font)
{
    PangoFontDescription *font_desc = pango_font_description_from_string(font);
    gtk_widget_override_font(widget, font_desc);
    pango_font_description_free(font_desc);
}

// Cute button style applied for consistency
void apply_cute_button_style(GtkWidget *widget, const char *bg_color, const char *outline_color)
{
    apply_background_color(widget, bg_color);
    gtk_widget_override_font(widget, pango_font_description_from_string("Poppins 14"));
    gtk_button_set_relief(GTK_BUTTON(widget), GTK_RELIEF_NONE);
    gtk_widget_set_size_request(widget, 150, 50);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    g_strdup_printf("* { border-radius: 25px; border: 2px solid %s; }", outline_color), -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget),
                                   GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// Cute lables applied for consistency
GtkWidget *create_cute_label_with_bubble(const char *text, const char *bg_color, const char *outline_color)
{
    GtkWidget *label = gtk_label_new(text);
    apply_font_style(label, "Poppins 16");
    apply_background_color(label, bg_color);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    g_strdup_printf("* { border-radius: 15px; border: 2px solid %s; padding: 10px; }", outline_color), -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(label),
                                   GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    return label;
}

int compare_scores(const void *a, const void *b)
{
    return ((player *)b)->score - ((player *)a)->score;
}

void show_final_rankings()
{
    GtkWidget *window, *grid, *title_label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Final Rankings");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 350);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 15);
    gtk_container_add(GTK_CONTAINER(window), grid);

    title_label = gtk_label_new("\xf0\x9f\x8f\x86 Final Rankings \xf0\x9f\x8f\x86");
    apply_font_style(title_label, "Poppins Bold 18");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 1, 1);

    qsort(players, NUM_PLAYERS, sizeof(player), compare_scores);

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "%d. %s - %d points", i + 1, players[i].name, players[i].score);

        GtkWidget *ranking_label = create_cute_label_with_bubble(buffer, COLOR_LAVENDER, COLOR_GREY);
        gtk_widget_set_margin_bottom(ranking_label, 10);
        gtk_grid_attach(GTK_GRID(grid), ranking_label, 0, i + 1, 1, 1);
    }

    gtk_widget_show_all(window);
}

bool all_questions_answered()
{
    for (int i = 0; i < NUM_QUESTIONS; i++)
    {
        if (!questions[i].answered)
            return false;
    }
    return true;
}

// ✅ Updated check_answer Function
void check_answer(GtkWidget *widget, gpointer data)
{
    GtkWidget **widgets = (GtkWidget **)data;
    const gchar *answer = gtk_entry_get_text(GTK_ENTRY(widgets[1]));
    question *q = (question *)widgets[2];

    if (strlen(answer) == 0)
    {
        gtk_label_set_markup(GTK_LABEL(widgets[0]), "<span foreground='orange'>Please enter an answer!</span>");
        return;
    }

    // Disable the submit button to prevent multiple clicks
    gtk_widget_set_sensitive(widget, FALSE);

    char trimmed_answer[256];
    sscanf(answer, " %255[^\n]", trimmed_answer);

    if (strcasecmp(q->answer, trimmed_answer) == 0)
    {
        players[current_player].score += q->value;
        gtk_label_set_markup(GTK_LABEL(widgets[0]), "<span foreground='green'> Correct! You're going to crush those technical interviews!</span>");
    }
    else
    {
        gtk_label_set_markup(GTK_LABEL(widgets[0]), "<span foreground='red'>Incorrect...sorry :(</span>");
    }

    q->answered = true;

    // Debugging Output
    printf("Player %s answered. Correct: %s | Score: %d\n",
           players[current_player].name,
           (strcasecmp(q->answer, trimmed_answer) == 0) ? "Yes" : "No",
           players[current_player].score);

    update_progress_bar();

    // Add a delay before closing the window
    GtkWidget *window = gtk_widget_get_toplevel(widget);

    // Close the window after 1.5 seconds (1500 milliseconds)
    g_timeout_add(2500, (GSourceFunc)gtk_widget_destroy, window);

    // Refresh the board (this can stay immediate as it's non-intrusive)
    refresh_board();

    if (all_questions_answered())
    {
        show_final_rankings();
    }
    else
    {
        current_player = (current_player + 1) % NUM_PLAYERS;
        refresh_board();
    }
}

void display_question_window(question *q)
{
    GtkWidget *window, *grid, *label, *entry, *button, *feedback_label;
    GtkWidget **widgets = malloc(3 * sizeof(GtkWidget *));

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Answer the Question");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 250);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new(q->question);
    gtk_widget_set_size_request(label, 450, 60);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_label_set_yalign(GTK_LABEL(label), 0.5);
    apply_font_style(label, "Poppins Bold 16");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter your answer...");
    gtk_widget_set_size_request(entry, 400, 40);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 2, 1);

    feedback_label = gtk_label_new("");
    apply_font_style(feedback_label, "Poppins 14");
    gtk_grid_attach(GTK_GRID(grid), feedback_label, 0, 3, 2, 1);

    widgets[0] = feedback_label;
    widgets[1] = entry;
    widgets[2] = (GtkWidget *)q;

    button = gtk_button_new_with_label("Submit Answer");
    apply_cute_button_style(button, COLOR_BLUE, COLOR_GREY);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(check_answer), widgets);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[])
{
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

    GtkWidget *title_label = gtk_label_new("Jeopardy");
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.5);
    gtk_label_set_yalign(GTK_LABEL(title_label), 0.5);
    gtk_widget_override_font(title_label, pango_font_description_from_string("Poppins 30"));
    apply_background_color(title_label, COLOR_PINK);
    gtk_grid_attach(GTK_GRID(main_grid), title_label, 0, 0, 3, 1);

    refresh_board();

    gtk_widget_show_all(main_window);
    gtk_main();

    return 0;
}
