#ifndef PA3_H
#define PA3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

typedef struct duration
{
	int minutes;
	int seconds;
} Duration;

typedef struct record
{
	char artist[50];
	char album[50];
	char title[35];
	char genre[25];
	Duration length;
	int plays;
	int rating;
} Record;

typedef struct node
{
	struct node *pPrev;
	Record data;
	struct node *pNext;
} Node;


void printMenu(void);

Node* makeNode(Record data);

bool insertAtFront(Node **pHead, Record data);

int chooseOption(void);

int convertInt(char *str);

void command(FILE* infile, FILE* outfile, Node **pHead, int option);

bool load(FILE *infile, Node **pHead);

bool store(FILE *outfile, Node **pHead);

void display(Node **pHead);

bool edit(Node **pHead);

bool rate(Node **pHead);

void play(Node **pHead);

bool insert(Node **pHead);

void clearStr(char str[100]);

bool sort(Node **pHead);

bool sortArtist(Node **pHead);

bool sortAlbum(Node **pHead);

bool sortRating(Node **pHead);

bool sortPlays(Node **pHead);

int countList(Node **pHead);

bool delete(Node **pHead);

void shuffle(Node **pHead);

#endif PA3_H

