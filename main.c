#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s movies.txt\n", argv[0]);
        return 1;
    }

    Movie* movies = NULL;
    int count = 0;
    loadMovies(argv[1], &movies, &count);

    int choice;
    while (1) {
        printf("\n1.Display\n2.Recommend\n3.Top Rated\n4.Search\n5.Add\n6.Delete\n7.Save & Exit\nChoice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1)
            displayMovies(movies, count);
        else if (choice == 2) {
            char g[50];
            printf("Preferred genre: ");
            fgets(g, sizeof(g), stdin);
            g[strcspn(g, "\n")] = '\0';
            recommendMoviesAdvanced(movies, count, parseGenre(g));
        }
        else if (choice == 3) {
            sortByRating(movies, count);
            displayMovies(movies, count < 5 ? count : 5);
        }
        else if (choice == 4) {
            char key[100];
            printf("Search: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = '\0';
            searchMovie(movies, count, key);
        }
        else if (choice == 5)
            addMovie(&movies, &count);
        else if (choice == 6) {
            char t[100];
            printf("Title to delete: ");
            fgets(t, sizeof(t), stdin);
            t[strcspn(t, "\n")] = '\0';
            deleteMovie(movies, &count, t);
        }
        else if (choice == 7) {
            saveMovies(argv[1], movies, count);
            free(movies);
            break;
        }
    }
    return 0;
}
