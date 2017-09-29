// A2 : Make a find clone . for extra credit, support regex matching

#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for cmd args
#include <stdbool.h> // bool support
#include <stdio.h> // FILE 
#include <dirent.h> // DIR

#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

// the file tree walk function that decends the directory hierarchy
static int travers(const char * root, const char * match_string);

// declare global vars for cmd args
static const char const * flags = "p:f::l::s:";
static bool pflag = false; // path flag
static bool fflag = false; // specifies [c|h|S] that parses either .c or .h file only
static bool sflag = false; // parse match string
static bool lflag = false; // parses symbolic links too if set

static char * pvalue = NULL; // path to the root of the parse tree
static char * fvalue = NULL; // stores the value for the -f flag as a string
static char * svalue = NULL; // the value of the s flag is the s string we match for in the parse tree
static const char const * invalid_chars = "%&()#@!"; // chars that are illegal in the input mathc string

// declare global vars for walker
static long num_regular = 0;
static long num_dirs = 0;
static long num_symlinks = 0;
static long num_total = 0;

// static parse_file parser;

// define files types as const ints
static const int FTW_FILE = 1; // readable file
static const int FTW_DIR = 2; // openable dir
static const int FTW_NO_DIR = 3; // dir with no read permission
static const int FTW_NO_STAT = 4; // file that we cannot stat

static char * abs_path = NULL; // the path to the file/dir currently being parsed
static size_t plength = 0;// size of abs_path in bytes
static size_t slength = 0;

int main(int argc, char * argv [])
{
	char cmd;
	
	// first get all command line arguments to form parse rules
	while ((cmd = getopt(argc, argv, flags)) != -1)
	{

		switch (cmd)
		{
			case 'p':
			{
				pflag = true;
				pvalue = optarg;
				plength = strlen(optarg);
				break;
			}
			case 'f':
			{
				fflag = true;
				fvalue = optarg;
				break;
			}
			case 'l':
			{
				lflag = true;
				break;
			}
			case 's':
			{
				sflag = true;
				svalue = optarg;
				slength = strlen(svalue);
				break;
			}
			case '?':
			{
				printf("Unknown option '-%c'.\n", optopt);
				return 1;
			}
			default :
			{
				abort();
			}
		}
	}

	// first make sure we got both the path and the match string
	if (!pflag || !plength)
	{
		printf("%s: Path not specified.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (!sflag || !slength)
	{
		printf("%s: Match string not specified.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// check for illegal chars in the match string

	char * c =  NULL;
	if (c = strpbrk(svalue, invalid_chars))
	{
		printf("%s: Invalid charecter(s) %c found in match string.\n", argv[0], c);
		exit(EXIT_FAILURE);
	}

	// now check if fflag is true, then the vlaue of the flag is valid
	if (fflag)
	{
		if (*fvalue != 'c' || *fvalue != 'h' || *fvalue != 'S')
		{
			printf("%s: Invalid input arg %c for -f flag.", argv[0], fvalue);
			exit(EXIT_FAILURE);
		}
	}

	// ret_val = file_tree_walk(pvalue, 	parser);

	printf("Path suppied is %s\n", pvalue);
	printf("String to match is %s\n", svalue);
	num_total = num_symlinks + num_regular + num_dirs;

	printf("Number of regualr files = %d", num_regular);
	printf("Number of symlinks = %d", num_symlinks);
	printf("Number of directories = %d", num_dirs);
	printf("Total number of files = %d", num_total);

 	exit(EXIT_SUCCESS);
}

// main os tree walker that recursively traverses the file tree, calling parser on all files
// static int traverse(const char * path, const char * match_string)
// {

// }

// // main parser subroutine that parses through all difference types of files and calls the appropriate parse funtion
// static int parse(const char * path, const struct stat * statbuf, int type)
// {
// 	switch (type)
// 	{
// 		case FTW_FILE:
// 		{
// 			switch(statbuf->st_mode & S_IFMT) // bitwise and so we get the file type as the switch param
// 			{
// 				case S_IFREG:
// 				{
// 					parse_regualr(path, match_string);
// 					num_regular++;
// 					break;
// 				}
// 				case S_IFLNK:
// 				{
// 					parser_symlink(path, match_string);
// 					num_symlinks++;
// 					break;
// 				}
// 				default :
// 				{
// 					printf("Unsupported file format");
// 				}
// 			}
// 		}
// 		case FTW_DIR:
// 		{
// 			num_dirs++;
// 			break;
// 		}
// 		case FTW_NO_STAT:
// 		{
// 			return 1;
// 			break;
// 		}
// 	}
// }

int parse_regualr(const char * path, const char * match_string)
{
	FILE * fptr;
	fptr = fopen(path, "r");

	char * line_buffer = malloc(sizeof(char) * 512);

	while(!feof(fptr))
	{
		fgets(line_buffer, 256, fptr);
		printf("%s\n", line_buffer);
	}

	free(line_buffer);
	fclose(fptr);
	return 0;
}
