// A2 : Make a find clone . for extra credit, support regex matching

#include <stdlib.h> // printf
#include <string.h> // strcmp, strpbrk, strcpy, strcat
#include <unistd.h> // getopt, optopt, optarg
#include <stdbool.h> // bool support
#include <stdio.h> // FILE 
#include <dirent.h> // DIR
#include <sys/stat.h> // stat, lstat

#define NORMAL_COLOR  "\x1B[0m"
#define RED     "\x1B[31m"
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
static size_t plength = 0;// size of abs_path in bytes
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
static size_t abs_plength = 0;
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
		if (!fvalue)
		{
			printf("No value for -f specified.\n");
			exit(EXIT_FAILURE);
		}

		if (*fvalue != 'c' || *fvalue != 'h' || *fvalue != 'S')
		{
			printf("%s: Invalid input arg %c for -f flag.", argv[0], fvalue);
			exit(EXIT_FAILURE);
		}
	}

	// out starting dir is the path specified, so we set it to that
	abs_path = malloc(strlen(pvalue));
	strcpy(abs_path, pvalue);

	printf("Path suppied is %s\n", pvalue);
	printf("String to match is %s\n", svalue);

	// now traverse the path input
	int ret_val = traverse(pvalue, plength);

	num_total = num_symlinks + num_regular + num_dirs;

	printf("Number of regualr files = %d\n", num_regular);
	printf("Number of symlinks = %d\n", num_symlinks);
	printf("Number of directories = %d\n", num_dirs);
	printf("Total number of files = %d\n", num_total);

 	exit(EXIT_SUCCESS);
}

// main os tree walker that recursively traverses the file tree, calling parser on all files
int traverse(const char * const path, size_t pathlen)
{
	struct stat statbuf;

	if (stat(path, &statbuf) == -1)
	{
		printf("Permission Denied: Could not open %s for reading.\n", path);
		return 1;
	}
	
	switch(statbuf.st_mode & S_IFMT)
	{
		case (S_IFREG):
		{
			// TODO: parse files with specific extensions only
			if (fflag)
			{

			}
			// regular file
			printf("Reading file: %s\n", path);
			parse_regular(path, svalue);
			num_regular++;
			break;
		}
		case (S_IFDIR):
		{
			// directory
			DIR * directory = opendir(path);

			if (directory == NULL)
			{
				printf("Could not open directroy at %s for reading. \n", path);
				return 1;
			}
			else
			{
				struct dirent * dir_entry; 
				while ((dir_entry = readdir(directory)) != NULL) 
				{
					// if the dir entry is a symbolic link to self or parent, skip it
					if (strcmp(dir_entry-> d_name, ".") == 0 || strcmp(dir_entry-> d_name, "..") == 0)
						continue;

					// for all the contents of the file, construct the pathname to be passed to the parse function
					// we get the new path length as
					size_t dirent_pathlen =  pathlen + strlen(dir_entry-> d_name) + 2;
					char * dirent_path = malloc(dirent_pathlen);
					dirent_path[0] = '\0';
					strcat(dirent_path, path);
					
					if (dirent_path[pathlen - 1] != '/');
						strcat(dirent_path, "/");

					strcat(dirent_path, dir_entry-> d_name);

					// now recursively call traverse on all directory contents
					int retval = traverse(dirent_path, dirent_pathlen);

					free(dirent_path);
				}
				closedir(directory);
				num_dirs++;
			}
			break;
		}
		case (S_IFLNK):
		{
			// TODO: keep memory of nodes visited and do not visit again for symlinks
			// only parse symlinks if sflag is true
			if (sflag)
			{
				// lstat(path);
			}
			// symbloic link
			num_symlinks++;
			break;
		}
	}
}


int parse_regular(const char * path, const char * match_string)
{
	FILE * fptr;
	fptr = fopen(path, "r");

	// check for successful open
	if (fptr == NULL)
	{
		printf("Could not open file for reading.\n");
		return 1;
	}

	int matchstr_len = strlen(match_string);
	// TODO: make this able to read arbitrary length lines
	int bufsize = 512;
	char * line_buffer = malloc(sizeof(char) * bufsize);
	char * start_of_match = NULL;
	while(!feof(fptr))
	{
		fgets(line_buffer, bufsize, fptr);
		
		if ((start_of_match = strstr(line_buffer, match_string)) != NULL)
		{
			// first we get the length of the first segment of the array
			size_t seg_1 = start_of_match - line_buffer;
			printf("%.*s", seg_1, line_buffer);
			printf("%s%.*s%s", RED, matchstr_len, start_of_match, NORMAL_COLOR);
			start_of_match += matchstr_len;
			printf("%s\n", start_of_match);
		}
	}

	free(line_buffer);
	fclose(fptr);
	return 0;
}
