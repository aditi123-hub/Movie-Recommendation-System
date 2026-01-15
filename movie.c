#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "movie.h"
/* Utility */
void trim(char* str) {
    int i = 0, j = 0;
    while (isspace((unsigned char)str[i])) i++;
    while (str[i]) str[j++] = str[i++];
    while (j > 0 && isspace((unsigned char)str[j - 1])) j--;
    str[j] = '\0';
}
/* Genre parsing */
Genre parseGenre(const char* str) {
    if (!str) return UNKNOWN;
    char buf[50];
    strncpy(buf, str, sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';

    for (char* p = buf; *p; p++) *p = tolower(*p);

    if (strcmp(buf, "action") == 0) return ACTION;
    if (strcmp(buf, "comedy") == 0) return COMEDY;
    if (strcmp(buf, "drama") == 0) return DRAMA;
    if (strcmp(buf, "scifi") == 0 || strcmp(buf, "sci-fi") == 0) return SCIFI;
    if (strcmp(buf, "horror") == 0) return HORROR;
    return UNKNOWN;
}
const char* genreToString(Genre g) {
    switch (g) {
        case ACTION: return "Action";
        case COMEDY: return "Comedy";
        case DRAMA: return "Drama";
        case SCIFI: return "Sci-Fi";
        case HORROR: return "Horror";
        default: return "Unknown";
    }
}
/* File I/O */
void loadMovies(const char* filename, Movie** movies, int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        Movie* temp = realloc(*movies, (*count + 1) * sizeof(Movie));
        if (!temp) break;
        *movies = temp;

        Movie* m = &(*movies)[*count];

        trim(line);
        strcpy(m->title, line);

        fgets(line, sizeof(line), fp);
        trim(line);
        m->genre = parseGenre(line);

        fscanf(fp, "%f\n%d\n", &m->rating, &m->year);
        (*count)++;
    }
    fclose(fp);
}
void saveMovies(const char* filename, Movie* movies, int count) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s\n%s\n%.1f\n%d\n",
                movies[i].title,
                genreToString(movies[i].genre),
                movies[i].rating,
                movies[i].year);
    }
    fclose(fp);
}
/* Display */
void displayMovies(Movie* movies, int count) {
    for (int i = 0; i < count; i++) {
        printf("\nTitle : %s\nGenre : %s\nRating: %.1f\nYear  : %d\n",
               movies[i].title,
               genreToString(movies[i].genre),
               movies[i].rating,
               movies[i].year);
    }
}
/* Advanced Recommendation */
void recommendMoviesAdvanced(Movie* movies, int count, Genre prefGenre) {
    float score[count];

    for (int i = 0; i < count; i++) {
        score[i] = movies[i].rating * 0.7f;
        if (movies[i].genre == prefGenre && prefGenre != UNKNOWN)
            score[i] += 2.0f;
        if (movies[i].year >= 2015)
            score[i] += 1.0f;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (score[j] > score[i]) {
                float ts = score[i];
                score[i] = score[j];
                score[j] = ts;

                Movie tm = movies[i];
                movies[i] = movies[j];
                movies[j] = tm;
            }
        }
    }
    printf("\n--- Recommended Movies ---\n");
    for (int i = 0; i < count && i < 5; i++) {
        printf("Title: %s | Score: %.2f\n", movies[i].title, score[i]);
    }
}
/* Search */
void searchMovie(Movie* movies, int count, const char* keyword) {
    char key[100];
    strcpy(key, keyword);
    for (int i = 0; key[i]; i++) key[i] = tolower(key[i]);

    int found = 0;
    for (int i = 0; i < count; i++) {
        char title[100];
        strcpy(title, movies[i].title);
        for (int j = 0; title[j]; j++) title[j] = tolower(title[j]);

        if (strstr(title, key)) {
            printf("\n%s | %s | %.1f | %d",
                   movies[i].title,
                   genreToString(movies[i].genre),
                   movies[i].rating,
                   movies[i].year);
            found = 1;
        }
    }
    if (!found) printf("\nNo movie found.");
}
/* CRUD */
void addMovie(Movie** movies, int* count) {
    Movie* temp = realloc(*movies, (*count + 1) * sizeof(Movie));
    if (!temp) return;
    *movies = temp;

    Movie* m = &(*movies)[*count];

    printf("Title: ");
    fgets(m->title, sizeof(m->title), stdin);
    m->title[strcspn(m->title, "\n")] = '\0';

    char genreStr[50];
    printf("Genre: ");
    fgets(genreStr, sizeof(genreStr), stdin);
    genreStr[strcspn(genreStr, "\n")] = '\0';
    m->genre = parseGenre(genreStr);

    printf("Rating: ");
    scanf("%f", &m->rating);
    printf("Year: ");
    scanf("%d", &m->year);
    while (getchar() != '\n');

    (*count)++;
}

void deleteMovie(Movie* movies, int* count, const char* title) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(movies[i].title, title) == 0) {
            for (int j = i; j < *count - 1; j++)
                movies[j] = movies[j + 1];
            (*count)--;
            return;
        }
    }
}

/* Sorting */
int compareByRating(const void* a, const void* b) {
    float r1 = ((Movie*)a)->rating;
    float r2 = ((Movie*)b)->rating;
    return (r2 > r1) - (r2 < r1);
}

void sortByRating(Movie* movies, int count) {
    qsort(movies, count, sizeof(Movie), compareByRating);
}

