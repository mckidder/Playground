/********************************************************************
  File name:     songs.h
  Last updated:  November 2018.
  Description:   A program that generates an ideal playlist. 
                 Functions generating a random song list.
*********************************************************************/

#ifndef SONGS_H
#define SONGS_H

struct Song {
    int title1, title2, title3;
    float att1, att2;
};

// Creates a random song with title and attributes
void random_song(Song &theSong);

// Creates a random song title out of 3 words
void random_title(char song[]);

// Returns a real number between 0 and 1
float random_attribute();

// Generates n songs names and their attributes.
void generate_songs(Song *songs, int n);

// Computes the distance between the songs at the two indexes based on
// the arrays of attributes.
float distance_song(Song &song1, Song &song2);

// writes out the song on a line
void output_song(Song &theSong);

#endif
