#include "PA 3.h"

int count = 0;

int main(int argc, char argv[])
{
	srand((unsigned int)(time NULL));

	int option = 0;
	FILE *infile = NULL;
	FILE *outfile = NULL;
	Node *pHead = NULL;

	infile = fopen("music.csv", "r");

	while (option != 11)
	{
		option = chooseOption();
		command(infile, outfile, &pHead, option);
	}

	fclose(infile);

	return 0;
}