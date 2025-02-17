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
#include "questions.h"
#include <time.h>

// Define the questions array
question questions[NUM_QUESTIONS];


// Initialize the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    char *questions_data[NUM_QUESTIONS][3] = {
        {"programming", "What language is used for system programming?", "C"},
        {"programming", "What language is primarily used for web development?", "JavaScript"},
        {"programming", "Which programming paradigm focuses on objects?", "Object-Oriented"},
        {"programming", "What does HTML stand for?", "HyperText Markup Language"},
        {"algorithms", "What sorting algorithm has an average complexity of O(n log n)?", "Merge Sort"},
        {"algorithms", "What data structure follows FIFO order?", "Queue"},
        {"algorithms", "What algorithm is used to find the shortest path in a graph?", "Dijkstra's Algorithm"},
        {"algorithms", "What is the worst-case complexity of QuickSort?", "O(n^2)"},
        {"databases", "What SQL command is used to retrieve data?", "SELECT"},
        {"databases", "What type of database is MongoDB?", "NoSQL"},
        {"databases", "Which normalization form removes partial dependencies?", "2NF"},
        {"databases", "What does ACID stand for in databases?", "Atomicity, Consistency, Isolation, Durability"}
    };

    int values [] = {100, 200, 300, 400}; //possible values for the types of questions
    int index = 0; //index to track the current question that is being initialized

    //  Add categories, questions, answers, and values to the questions array
    for (int i = 0; i < NUM_CATEGORIES; i++){
        for (int j = 0; j < 4; j++){ // Each category has 4 questions
            strcpy(questions[index].category, questions_data[index][0]); // Assign category
            strcpy(questions[index].question, questions_data[index][1]); // Assign question
            strcpy(questions[index].answer, questions_data[index][2]); // Assign correct answer
            questions[index].value = values[j]; // Assign point value
            questions[index].answered = false; // Mark question as unanswered 
            index++;
        }
    }

}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    // print categories and dollar values for each unanswered question in questions array
    printf("Categories and Available Questions are: \n");
    for (int i = 0; i < NUM_QUESTIONS; i++){
        if (!questions[i].answered) {
            printf("%s - $%d\n", questions[i].category, questions[i].value);
        }
    }
}

// Display the question for the category and dollar value
void display_question(char *category, int value)
{
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (!questions[i].answered && strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            printf("Question: %s\n", questions[i].question);
            return;
        }
    }
    printf("No question found for %s at $%d or it's already answered.\n", category, value);
}

// Return true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (!questions[i].answered && strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return strcasecmp(questions[i].answer, answer) == 0;
        }
    }
    return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return questions[i].answered;
        }
    }
    return false;
}
