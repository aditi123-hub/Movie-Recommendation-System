#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "movie.h"

void trim(char* str) {
    int i = 0, j = 0;
    while (isspace((unsigned char)str[i])) i++;
    while (str[i]) str[j++] = str[i++];
    while (j > 0 && isspace((unsigned char)str[j - 1])) j--;
    str[j] = '\0';
}

Genre parseGenre(const char* str) {
    if (!str) return UNKNOWN;
    char buffer[50];
    strncpy(buffer, str, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    for (char* p = buffer; *p; ++p) *p = tolower(*p);

    if (strcmp(buffer, "action") == 0) return ACTION;
    if (strcmp(buffer, "comedy") == 0) return COMEDY;
    if (strcmp(buffer, "drama") == 0) return DRAMA;
    if (strcmp(buffer, "scifi") == 0 || strcmp(buffer, "sci-fi") == 0) return SCIFI;
    if (strcmp(buffer, "horror") == 0) return HORROR;
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

void loadMovies(const char* filename, Movie** movies, int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file for reading");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        Movie* temp = realloc(*movies, (*count + 1) * sizeof(Movie));
        if (!temp) {
            fprintf(stderr, "Memory allocation failed.\n");
            fclose(fp);
            return;
        }
        *movies = temp;
        Movie* m = &(*movies)[*count];

        trim(line);
        strncpy(m->title, line, sizeof(m->title));
        m->title[sizeof(m->title) - 1] = '\0';

        if (!fgets(line, sizeof(line), fp)) break;
        trim(line);
        m->genre = parseGenre(line);

        if (fscanf(fp, "%f\n%d\n", &m->rating, &m->year) != 2) break;

        (*count)++;
    }

    fclose(fp);
}

void saveMovies(const char* filename, Movie* movies, int count) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s\n%s\n%.1f\n%d\n",
                movies[i].title, genreToString(movies[i].genre),
                movies[i].rating, movies[i].year);
    }

    fclose(fp);
}

void displayMovies(Movie* movies, int count) {
    printf("\n--- Movie List ---\n");
    for (int i = 0; i < count; i++) {
        printf("\nTitle : %s\nGenre : %s\nRating: %.1f\nYear  : %d\n",
               movies[i].title, genreToString(movies[i].genre),
               movies[i].rating, movies[i].year);
    }
}

void recommendMoviesFiltered(Movie* movies, int count, Genre genreFilter, float minRating, int minYear) {
    printf("\nRecommended Movies:\n");
    int found = 0;

    for (int i = 0; i < count; i++) {
        if ((genreFilter == UNKNOWN || movies[i].genre == genreFilter) &&
            (minRating == 0.0f || movies[i].rating >= minRating) &&
            (minYear == 0 || movies[i].year >= minYear)) {

            printf("Title: %s | Genre: %s | Rating: %.1f | Year: %d\n",
                   movies[i].title, genreToString(movies[i].genre),
                   movies[i].rating, movies[i].year);
            found = 1;
        }
    }

    if (!found) {
        printf("No movies found matching the criteria.\n");
    }
}

int compareByRating(const void* a, const void* b) {
    float ra = ((Movie*)a)->rating;
    float rb = ((Movie*)b)->rating;
    return (rb > ra) - (rb < ra); // Descending
}

void sortByRating(Movie* movies, int count) {
    qsort(movies, count, sizeof(Movie), compareByRating);
}
