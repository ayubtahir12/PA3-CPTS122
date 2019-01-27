#include "PA 3.h"

void printMenu(void)
{
	// used to show user all options in the digital music manager
	printf("Menu\n");
	printf("(1) Load (NOTE: Load must be completed before any other functions are executed.)\n");
	printf("(2) Store\n");
	printf("(3) Display\n");
	printf("(4) Insert\n");
	printf("(5) Delete\n");
	printf("(6) Edit\n");
	printf("(7) Sort\n");
	printf("(8) Rate\n");
	printf("(9) Play\n");
	printf("(10) Shuffle\n");
	printf("(11) Exit\n\n");
}

void clearStr(char str[100])
{
	strcpy(str, "");
}

Node* makeNode(Record data)
{
	Node *pMem = NULL; // will store address of our new node on the heap

	pMem = (Node *)malloc(sizeof(Node)); // allocate space on heap for a node and return its starting address
	pMem->data = data; // set data of new node to incoming data
	
	// set pointers to NULL so node doesn't point to anything
	pMem->pNext = NULL;
	pMem->pPrev = NULL;

	return pMem; // return address of node on the heap
}

bool insertAtFront(Node **pHead, Record data)
{
	bool success = false; // by default we have not succeeded in inserting our node at the front
	Node *pCur = NULL; // create a pointer to hold our new node's address

	pCur = makeNode(data); // will return the address of a new node storing our desired data

	if (*pHead == NULL) // if we have an empty list
	{
		*pHead = pCur; // store our new node as the head of the list
		success = true; // mark our operation as a success
	}
	else if (*pHead != NULL) // if items already exist in the list
	{
		pCur->pNext = *pHead; // set the next pointer of our new node to the start of the list
		(*pHead)->pPrev = pCur; // set the previous pointer of the start of the list to our new node
		*pHead = pCur; // set the head of the start of the list to our 
		success = true; // mark our operation as a success
	}
	return success; // return whether we are successful or not
}

int countList(Node **pHead)
{
	int count = 0; // will track how many nodes there are in a linked list
	Node *pCur = NULL;

	pCur = *pHead;

	while (pCur != NULL) // while we haven't reached the end of the list
	{
		count++; // increment count
		pCur = pCur->pNext; // move to next node
	}

	return count;
}

int convertInt(char *str)
{
	int num = atoi(str); // take the string and convert it into an integer

	return num;
}

int chooseOption(void)
{
	int option;

	do
	{
		system("cls");
		printMenu(); // print possible user options in the digital music manager
		printf("Please choose a menu option: ");
		scanf("%d", &option);
	} while ((option < 1) || (option > 11)); // make sure user selection will fall within the bounds of the options

	return option;
}

void command(FILE* infile, FILE* outfile, Node **pHead, int option)
{
	bool success = false;
	switch (option)
	{
		case 1: // (1) load
			success = load(infile, pHead);
			break;
		case 2: // (2) store
			success = store(outfile, pHead);
			break;
		case 3: // (3) display
			display(pHead);
			break;
		case 4: // (4) insert
			success = insert(pHead);
			break;
		case 5: // (5) delete
			success = delete(pHead);
			break;
		case 6: // (6) edit
			success = edit(pHead);
			break;
		case 7: // (7) sort
			success = sort(pHead);
			break;
		case 8: // (8) rate
			success = rate(pHead);
			break;
		case 9: // (9) play
			play(pHead);
			break;
		case 10: // (10) shuffle
			shuffle(pHead);
			break;
		case 11: // (11) exit
			success = store(outfile, pHead);
			break;
		default:
			printf("Something went wrong. :( Please try again.\n"); // option was not between 1 and 11. Check chooseOption()
			break;
	}
	if (success && option != 11) // if the operation chosen was completed successfully
	{
		printf("Operation completed successfully!\n");
		system("pause");
	}
	else if (option != 11)
	{
		printf("Something went wrong. :( Please try again.\n"); // if operation chosen was not completed successfully

	}
}

bool load(FILE *infile, Node **pHead)
{
	bool success = false;
	Record songData;
	char temp[100];

	while (!feof(infile)) // while we still have data to read in
	{
		fgets(temp, 100, infile); // read in one line of the string and store it into our temp string
		if (temp[0] == '"') // if the first char in the string is " i.e. if the artist uses their full name
		{
			// tokenize on second " then add all values into Record
			strcpy(songData.artist, strtok(temp, "\""));
			strcpy(songData.album, strtok(NULL, ","));
			strcpy(songData.title, strtok(NULL, ","));
			strcpy(songData.genre, strtok(NULL, ","));
			songData.length.minutes = atoi(strtok(NULL, ":"));
			songData.length.seconds = atoi(strtok(NULL, ","));
			songData.plays = atoi(strtok(NULL, ","));
			songData.rating = atoi(strtok(NULL, ","));
		}
		else if (temp[0] != '\n' && temp[0] != '\0') // if the line is holding values (used to account for empty lines)
		{
			// add all values into Record
			strcpy(songData.artist, strtok(temp, ","));
			strcpy(songData.album, strtok(NULL, ","));
			strcpy(songData.title, strtok(NULL, ","));
			strcpy(songData.genre, strtok(NULL, ","));
			songData.length.minutes = atoi(strtok(NULL, ":"));
			songData.length.seconds = atoi(strtok(NULL, ","));
			songData.plays = atoi(strtok(NULL, ","));
			songData.rating = atoi(strtok(NULL, ","));
		}
		// add data to front of linked list
		if (temp[0] != '\n' && temp[0] != '\0') success = insertAtFront(pHead, songData);

		// reset string to null to avoid errors when reading in new lines
		clearStr(temp);
	}

	return success;
}

bool store(FILE *outfile, Node **pHead)
{
	bool success = false;
	Node *pCur = NULL;

	pCur = *pHead;

	outfile = fopen("musicPlayList.csv", "w");
	while (pCur != NULL)
	{
		fprintf(outfile, "\"%s\",%s,%s,%s,%d:%d,%d,%d\n", pCur->data.artist, pCur->data.album, pCur->data.title, pCur->data.genre,
			pCur->data.length.minutes, pCur->data.length.seconds, pCur->data.plays, pCur->data.rating);
		success = true;

		pCur = pCur->pNext;
	}
	fclose(outfile);

	return success;
}

void display(Node **pHead)
{
	int option = 0;
	Node *pCur = NULL;
	char artist[50];

	pCur = *pHead;

	do
	{
		system("cls");
		printf("(1) Print all records.\n");
		printf("(2) Print all records that match an artist.\n\n");
		printf("Please pick an option: ");
		scanf("%d", &option);
	} while (option < 1 || option > 2);

	if (option == 1)
	{
		system("cls");
		while (pCur != NULL)
		{
			printf("%s\n%s\n%s\n%s\n%d:%d\n%d\n%d\n", pCur->data.artist, pCur->data.album, pCur->data.title, pCur->data.genre,
				pCur->data.length.minutes, pCur->data.length.seconds, pCur->data.plays, pCur->data.rating);
			printf("-----------------------------------------------------------\n");

			pCur = pCur->pNext;
		}
	}
	else if (option == 2)
	{
		printf("\nPlease type in an artist \n(Note: artists with a first and last name will need to be inputted with 'LastName, FirstName' format): ");
		scanf(" %[^\n]s", &artist);

		system("cls");

		while (pCur != NULL)
		{
			if (strcmp(pCur->data.artist, artist) == 0)
			{
				printf("%s, %s, %s, %s, %d:%d, %d, %d\n", pCur->data.artist, pCur->data.album, pCur->data.title, pCur->data.genre,
					pCur->data.length.minutes, pCur->data.length.seconds, pCur->data.plays, pCur->data.rating);
			}
			pCur = pCur->pNext;
		}
	}
	system("pause");
}

bool edit(Node **pHead)
{
	char song[50], edit[50];
	Node *pCur = NULL;
	int attribute, edit1, edit2;
	bool success = false;

	pCur = *pHead;

	system("cls");
	printf("Enter the name of the song you would like to edit: ");
	scanf(" %[^\n]s", &song); // for some reason, this is the only way I can scan in the file. found on stack overflow

	while (pCur != NULL && strcmp(pCur->data.title, song) != 0)
	// while the song given and the song in the node don't match, and while we haven't reached the end of the list
	{
		pCur = pCur->pNext;
	}
	if (pCur != NULL) // if we didn't reach the end of the list i.e. we found a song that matches the song given to us by the user
	{
		do {
			system("cls");
			printf("Which attribute would you like to edit?\n");
			printf("(1) Artist\n");
			printf("(2) Album\n");
			printf("(3) Title\n");
			printf("(4) Genre\n");
			printf("(5) Length\n");
			printf("(6) Plays\n");
			printf("(7) Rating\n");
			scanf("%d", &attribute);
		} while (attribute < 1 || attribute > 8); // while the selected choice falls outside the bounds of the given options

		switch (attribute)
		{
		case 1: // (1) artist
			printf("Enter the new artist name: ");
			scanf(" %[^\n]s", &edit);
			strcpy(pCur->data.artist, edit); // place new artist name into the song record
			success = true;
			break;
		case 2: // (2) album
			printf("Enter the new album name: ");
			scanf(" %[^\n]s", &edit);
			strcpy(pCur->data.album, edit);
			success = true;
			break;
		case 3: // (3) title
			printf("Enter the new title: ");
			scanf(" %[^\n]s", &edit);
			strcpy(pCur->data.title, edit);
			success = true;
			break;
		case 4: // (4) genre
			printf("Enter the new genre name: ");
			scanf(" %[^\n]s", &edit);
			strcpy(pCur->data.genre, edit);
			success = true;
			break;
		case 5: // (5) length
			printf("Enter the new minutes of song (will enter seconds next): ");
			scanf("%d", &edit1);
			printf("Enter the new seconds of song: ");
			scanf("%d", &edit2);
			pCur->data.length.minutes = edit;
			pCur->data.length.seconds = edit2;
			success = true;
			break;
		case 6: // (6) plays
			printf("Enter the new amount of plays: ");
			scanf("%d", &edit1);
			pCur->data.plays = edit;
			success = true;
			break;
		case 7: // (7) rating
			printf("Enter the new rating: ");
			scanf("%d", &edit1);
			pCur->data.rating = edit;
			success = true;
			break;
		}
	}
	return success;
}

bool rate(Node **pHead)
{
	bool success = false;
	char song[50]; // will store name of song
	int edit; // will store new rating
	Node *pCur = NULL;

	pCur = *pHead;

	printf("Enter the name of the song you would like to rate: ");
	scanf(" %[^\n]s", &song);

	while (pCur != NULL && strcmp(pCur->data.title, song) != 0) // traverse through the list until either we find the song to rate or reach the end of the list
	{
		pCur = pCur->pNext;
	}

	if (pCur != NULL) // if we haven't reached the end of the list
	{
		do
		{
			system("cls");
			printf("Enter the new rating: ");
			scanf("%d", &edit);
		} while (edit < 1 || edit > 5);
		pCur->data.rating = edit; // update the rating of the song
		success = true;
	}
	

	return success;
}

void play(Node **pHead)
{
	Node *pCur = NULL;
	char song[50];

	pCur = *pHead;


	printf("Enter the name of the song you would like to play: ");
	scanf(" %[^\n]s", &song);

	while (pCur != NULL && strcmp(pCur->data.title, song) != 0) // traverse through list until either song is found or end of list
	{
		pCur = pCur->pNext;
	}

	while (pCur != NULL) // while there are more songs to play
	{
		system("cls");
		printf("Now Playing\n"); // "plays" song
		printf("%s - %s - %s", pCur->data.title, pCur->data.artist, pCur->data.album);
		Sleep(2000);

		pCur = pCur->pNext; // moves to next song in the list
	}

}

bool insert(Node **pHead)
{
	bool success = false;
	Record song;

	/*char artist[50];
	char album[50];
	char title[35];
	char genre[25];
	Duration length;
	int plays;
	int rating;*/

	printf("Adding a new song: \n\n");
	Sleep(500);

	printf("Enter the song's artist: ");
	scanf(" %[^\n]s", &song.artist);
	printf("Enter the song's : ");
	scanf(" %[^\n]s", &song.album);
	printf("Enter a new song name: ");
	scanf(" %[^\n]s", &song.title);
	printf("Enter a new song genre: ");
	scanf(" %[^\n]s", &song.genre);

	do {
		printf("Enter the song's length (minutes): ");
		scanf("%d", &song.length.minutes);
	} while (song.length.minutes <= 0); // length can't be less than 0

	do {
		printf("Enter the song's length (seconds): ");
		scanf("%d", &song.length.seconds);
	} while (song.length.seconds <= 0 && song.length.seconds > 60); // seconds can't be less than 0

	do {
		printf("Enter the song's amount of plays: ");
		scanf("%d", &song.plays);
	} while (song.plays <= 0); // can't have a negative amount of plays

	do {
		printf("Enter the song's rating (between 1 and 5): ");
		scanf("%d", &song.rating);
	} while (song.rating <= 0 || song.rating > 5); // must be between 1 and 5

	success = insertAtFront(pHead, song); // add new song to front of list

	return success;
}

bool sort(Node **pHead)
{
	bool success = FALSE;
	int option = 0;
	

	do {
		system("cls");
		printf("SORT OPTIONS:\n");
		printf("(1) Sort based on artist (A-Z)\n");
		printf("(2) Sort based on album title (A-Z)\n");
		printf("(3) Sort based on rating (1-5)\n");
		printf("(4) Sort based on times played (largest-smallest)\n");
		scanf("%d", &option);
	} while (option < 1 || option > 4);

	switch (option)
	{
		case 1: // sort on artist
			success = sortArtist(pHead);
			break;
		case 2: // sort on album
			success = sortAlbum(pHead);
			break;
		case 3: // sort on rating
			success = sortRating(pHead);
			break;
		case 4: // sort on plays
			success = sortPlays(pHead);
			break;
		default:
			printf("Something went wrong. :( Please try again.\n");
	}

	return success;
}

bool sortArtist(Node **pHead)
{
	bool success = false;
	int count = 0;
	Record temp;
	Node *pCur = NULL;
	Node *pComp = NULL;

	count = countList(pHead);

	for (int i = 0; i < count - 1; i++)
	{
		pCur = *pHead;
		pComp = pCur->pNext;

		while (pComp != NULL)
		{
			if (strcmp(pCur->data.artist, pComp->data.artist) > 0)
			{
				temp = pCur->data;
				pCur->data = pComp->data;
				pComp->data = temp;
				
				success = true;
			}
			pCur = pCur->pNext;
			pComp = pComp->pNext;
		}
	}
	
	return success;
}

bool sortAlbum(Node **pHead)
{
	bool success = false;
	int count = 0;
	Record temp;
	Node *pCur = NULL;
	Node *pComp = NULL;

	count = countList(pHead);

	for (int i = 0; i < count - 1; i++)
	{
		pCur = *pHead;
		pComp = pCur->pNext;

		while (pComp != NULL)
		{
			if (strcmp(pCur->data.album, pComp->data.album) > 0)
			{
				temp = pCur->data;
				pCur->data = pComp->data;
				pComp->data = temp;

				success = true;
			}
			pCur = pCur->pNext;
			pComp = pComp->pNext;
		}
	}

	return success;
}

bool sortRating(Node **pHead)
{
	bool success = false;
	int count = 0;
	Record temp;
	Node *pCur = NULL;
	Node *pComp = NULL;

	count = countList(pHead);

	for (int i = 0; i < count - 1; i++)
	{
		pCur = *pHead;
		pComp = pCur->pNext;

		while (pComp != NULL)
		{
			if (pCur->data.rating > pComp->data.rating)
			{
				temp = pCur->data;
				pCur->data = pComp->data;
				pComp->data = temp;

				success = true;
			}
			pCur = pCur->pNext;
			pComp = pComp->pNext;
		}
	}

	return success;
}

bool sortPlays(Node **pHead)
{
	bool success = false;
	int count = 0;
	Record temp;
	Node *pCur = NULL;
	Node *pComp = NULL;

	count = countList(pHead);

	for (int i = 0; i < count - 1; i++)
	{
		pCur = *pHead;
		pComp = pCur->pNext;

		while (pComp != NULL)
		{
			if (pCur->data.plays < pComp->data.plays)
			{
				temp = pCur->data;
				pCur->data = pComp->data;
				pComp->data = temp;

				success = true;
			}
			pCur = pCur->pNext;
			pComp = pComp->pNext;
		}
	}

	return success;
}

bool delete(Node **pHead)
{
	bool success = false;
	char song[100];
	Node *pCur = NULL;

	pCur = *pHead;

	system("cls");
	printf("Enter the name of the song you would like to delete: ");
	scanf(" %[^\n]s", &song);

	while (pCur != NULL && strcmp(pCur->data.title, song) != 0)
	{
		pCur = pCur->pNext;
	}

	if (pCur != NULL)
	{
		if (pCur->pPrev == NULL)
		{
			*pHead = pCur->pNext;
			(*pHead)->pNext = NULL;
			free(pCur);
			success = true;
		}
		else if (pCur->pNext == NULL)
		{
			pCur->pPrev->pNext = NULL;
			free(pCur);
			success = true;
		}
		else
		{
			pCur->pPrev->pNext = pCur->pNext;
			pCur->pNext->pPrev = pCur->pPrev;
			free(pCur);
			success = true;
		}
	}

	return success;
}

void shuffle(Node **pHead)
{
	int *randomContainer = NULL;
	int count = countList(pHead);
	Node *pCur = NULL;

	pCur = *pHead;

	randomContainer = (int *)malloc((sizeof(int) * count)); //allocate enough memory for space for as many songs there are in the library

	
	for (int i = 0; i < count; i++) randomContainer[i] = i; //iterate through the array and initialize the values

	//shuffle the array
	for (int j = 0; j < count; j++) { //shuffle the array

		int temp = randomContainer[j];
		int randomIndex = rand() % count;

		randomContainer[j] = randomContainer[randomIndex];
		randomContainer[randomIndex] = temp;

	}

	for (int k = 0; k < count; k++)
	{
		pCur = *pHead; // reset node to front of list

		system("cls");

		for (int m = 0; m < randomContainer[k]; m++) pCur = pCur->pNext;

		printf("Now Playing\n"); // "plays" song
		printf("%s - %s - %s", pCur->data.title, pCur->data.artist, pCur->data.album);
		Sleep(2000);
	}
	printf("\nEnd of playlist.\n");
	system("pause");
}