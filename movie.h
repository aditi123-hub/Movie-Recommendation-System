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

void loadMovies(const char* filename, Movie** movies, int* count);
void saveMovies(const char* filename, Movie* movies, int count);
void displayMovies(Movie* movies, int count);
void recommendMoviesFiltered(Movie* movies, int count, Genre genreFilter, float minRating, int minYear);
void sortByRating(Movie* movies, int count);
Genre parseGenre(const char* str);
const char* genreToString(Genre g);
void trim(char* str);

#endif
