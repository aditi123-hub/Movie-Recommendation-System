#ifndef MOVIE_H
#define MOVIE_H
typedef enum {
    UNKNOWN, ACTION, COMEDY, DRAMA, SCIFI, HORROR
} Genre;
typedef struct {
    char title[100];
    Genre genre;
    float rating;
    int year;
} Movie;
/* Core functions */
void loadMovies(const char* filename, Movie** movies, int* count);
void saveMovies(const char* filename, Movie* movies, int count);
void displayMovies(Movie* movies, int count);
/* Recommendation */
void recommendMoviesAdvanced(Movie* movies, int count, Genre prefGenre);
/* Utilities */
Genre parseGenre(const char* str);
const char* genreToString(Genre g);
void trim(char* str);
/* Search */
void searchMovie(Movie* movies, int count, const char* keyword);
/* CRUD */
void addMovie(Movie** movies, int* count);
void deleteMovie(Movie* movies, int* count, const char* title);
/* Sorting */
void sortByRating(Movie* movies, int count);
#endif
