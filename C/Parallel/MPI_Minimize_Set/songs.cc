/********************************************************************
  File name:     songs.cc
  Last updated:  November 2018.
  Description:   A program that generates an ideal playlist. 
                 Functions generating a random song list.
*********************************************************************/

#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;
#include "songs.h"

// Taken from the Fall Out Boy albums Folie a Deux and Mania
const int nr_words = 61, word_max_length = 15;
char words[nr_words][word_max_length] = 
{"Stay", "Frosty", "Royal", "Milk", "Tea", "Last", "Real", "Ones", 
 "Hold", "Me", "Tight", "Don't", "Wilson", "Expensive", "Mistakes",
 "Church", "Heaven", "Gate", "Champion", "Sunshine", "Riptide",
 "Young", "Menace", "Bishops","Knife", "Trick", "Disloyal", "Order", 
 "Water", "Buffaloes", "I", "Don't", "Care", "She's", "My",  "Winona",
 "America's", "Suite", "Hearts", "Headfirst", "Slide", "Into", "Coopers", 
 "Town", "Bad", "Bet", "Shipped", "Gold", "Standard", "Coffee's", 
 "Closers", "What", "Catch", "Donnie", "Tiffany", "Dollar", "Nose", "Bleed", 
 "West", "Coast", "Smoker"};

// Creates a random song with title and attributes
void random_song(Song &theSong)
{
    theSong.title1 = rand() % nr_words;
    theSong.title2 = rand() % nr_words;
    theSong.title3 = rand() % nr_words;
    theSong.att1 = random_attribute();
    theSong.att2 = random_attribute();
}

// Returns a real number between 0 and 1
float random_attribute() 
{
    return float(rand()) / RAND_MAX;
}

// Generates n songs names and their attributes.
void generate_songs(Song *songs, int n)
{
    for (int i=0; i<n; i++) 
        random_song(songs[i]);
}

// Computes the distance between the songs at the two indexes based on
// the arrays of attributes.
float distance_song(Song &song1, Song &song2)
{
    return sqrt((song1.att1 - song2.att1)*(song1.att1 - song2.att1) +
                (song1.att2 - song2.att2)*(song1.att2 - song2.att2));
}

// writes out the song on a line
void output_song(Song &theSong)
{
    cout << words[theSong.title1] << ' ' << words[theSong.title2] << ' '
         << words[theSong.title3] << " (" << theSong.att1 
         << ", " << theSong.att2 << ")" << endl;
}
