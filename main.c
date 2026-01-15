#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

void getFilters(Genre* genreFilter, float* minRating, int* minYear) {
    char genreInput[50];
    printf("Enter preferred genre(Sci-Fi/Drama/Action/Comedy/Horror)(or press Enter to skip): ");
    fgets(genreInput, sizeof(genreInput), stdin);
    genreInput[strcspn(genreInput, "\n")] = '\0';
    trim(genreInput);

    *genreFilter = strlen(genreInput) == 0 ? UNKNOWN : parseGenre(genreInput);

    printf("Enter minimum rating (or 0 to skip): ");
    scanf("%f", minRating);

    printf("Enter minimum year (or 0 to skip): ");
    scanf("%d", minYear);

    while (getchar() != '\n'); // clear input buffer
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <movies_file>\n", argv[0]);
        return 1;
    }

    Movie* movies = NULL;
    int count = 0;
    loadMovies(argv[1], &movies, &count);

    int choice;
    while (1) {
        printf("\n==== Movie Recommendation System ====\n");
        printf("1. Display All Movies\n");
        printf("2. Recommend Movies \n");
        printf("3. Show Top Rated Movies\n");
        printf("4. Save and Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1) {
            displayMovies(movies, count);
        } else if (choice == 2) {
            Genre genreFilter;
            float minRating;
            int minYear;
            getFilters(&genreFilter, &minRating, &minYear);
            recommendMoviesFiltered(movies, count, genreFilter, minRating, minYear);
        } else if (choice == 3) {
    sortByRating(movies, count);
    int topN = count < 5 ? count : 5;
    printf("\n--- Top %d Rated Movies ---\n", topN);
    for (int i = 0; i < topN; i++) {
        printf("\nTitle : %s\nGenre : %s\nRating: %.1f\nYear  : %d\n",
               movies[i].title, genreToString(movies[i].genre),
               movies[i].rating, movies[i].year);
    }
}
         else if (choice == 4) {
            saveMovies(argv[1], movies, count);
            free(movies);
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
