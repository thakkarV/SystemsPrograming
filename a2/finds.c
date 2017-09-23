// A2 : Make a find clone . for extra credit, support regex matching

#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for cmd abool
#include <stdbool.h> // bool support

// the file tree walk function that decends the directory hierarchy
static int file_tree_walk(const char * root, parse_file * parser);

// recursively calls itself from FTW and calls the parser function on each file
static int dopath(parse_file * parser);

// the main parse subroutiune that takes in an indivifual file and then parses it with string s as reference
typedef int parse_file(const char * path, const struct stat * buf, int type, const char * match_string);


// declare global vars for cmd args
static const char * flags = "p:f::l::s:";
static bool pflag = false; // path flag
static bool fflag = false; // specifies [c|h|S] that parses either .c or .h file only
static bool sflag = false; // parse match string
static bool lflag = false; // parses symbolic links too if set

static char * pvalue = NULL; // path to the root of the parse tree
static char * fvalue = NULL; // stores the value for the -f flag as a string
static char * match_string = NULL; // the value of the s flag is the s string we mathc for in the parse tree

// declare global vars for walker
static long num_regular = 0;
static long num_dirs = 0;
static long num_symlinks = 0;
static long num_total = 0;

static parse_file parser;

// define files types as const ints
static const int FTW_FILE = 1; // readable file
static const int FTW_DIR = 2; // openable dir
static const int FTW_NO_DIR = 3; // dir with no read permission
static const int FTW_NO_STAT = 4; // file that we cannot stat

static char * abs_path; // the path to the file/dir currently being parsed
static size_t path_len;// size of abs_path in bytes

int main(int argc, char * argv [])
{
	int ret_val = 0;

	// first get all command line arguments to form parse rules
	while ((cmd = getopt(argc, argv, flags)) != -1)
	{

		switch (cmd)
		{
			case 'p':
			{
				pflag = true;
				pvalue = optarg;
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
				mathc_string = optarg;
				break;
			}
			case '?':
			{
				if (isprint(optopt))
				{
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				}
				else
				{
					fprintf(stderr, "Unknown option charecter '\\x%x'.\n", optopt);
				}
				return 1;
			}
			default :
			{
				abort();
			}
		}
	}

	ret_val = file_tree_walk(pvalue, parser);

	num_total = num_symlinks + num_regular + num_dirs;

	printf("Number of regualr files = %d", num_regular);
	printf("Number of symlinks = %d", num_symlinks);
	printf("Number of directories = %d", num_dirs);
	printf("Total number of files = %d", num_total);

 	exit(ret_val);
}

// main os tree walker that recursively traverses the file tree, calling parser on all files
static int file_tree_walk(const char * root, parse_file * parser)
{

}

// function that recursively calls itself for each dir, and calls parser for each file or folder
static int dopath(parse_file * parser)
{

}

// main parser subroutine that parses through all difference types of files and calls the appropriate parse funtion
static int parse_file(const char * path, const struct stat * statbuf, int type)
{
	switch (type)
	{
		case FTW_FILE:
		{
			switch(statbuf->st_mode & S_IFMT) // bitwise and so we get the file type as the switch param
			{
				case S_IFREG:
				{
					parse_regualr(const char * path, const char * match_string);
					num_regular++;
					break;
				}
				case S_IFLNK:
				{
					parser_symlink(const char * path, const char * match_string);
					num_symlinks++;
					break;
				}
				default :
				{
					printf("Unsupported file format");
				}
			}
		}
		case FTW_DIR:
		{
			num_dirs++;
			break;
		}
		case FTW_NO_STAT:
		{
			err_ret()
			break;
		}
	}
}
