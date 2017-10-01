// A2 : Make a find clone . for extra credit, support regex matching

#include <stdlib.h> // printf
#include <string.h> // strcmp, strpbrk, strcpy, strcat
#include <unistd.h> // getopt, optopt, optarg
#include <stdbool.h> // bool support
#include <stdio.h> // FILE 
#include <dirent.h> // DIR
#include <sys/stat.h> // stat, lstat

static const int max_path_length = 4096;

// color table for printing
#define NORMAL_COLOR  "\x1B[0m"
#define RED           "\x1B[31m"
#define GREEN         "\x1B[32m"
#define BLUE          "\x1B[34m"

// the file tree walk function that decends the directory hierarchy
int travers(const char * root, const char * match_string);

// parses the contents of regular files to find matches line by line
int parse_regular(const char * path, const char * match_string);

// check if the current head node of dir path has been seen already or not for symlink parsing
bool check_seen_links(const char * path, size_t pathlen);

// declare global vars for cmd args
static const char const * flags = "p:f:ls:";
static bool pflag = false; // path flag
static bool fflag = false; // specifies [c|h|S] that parses either .c or .h file only
static bool sflag = false; // parse match string
static bool lflag = false; // parses symbolic links too if set

static char * pvalue = NULL; // path to the root of the parse tree
static size_t plength = 0;// size of abs_path in bytes
static char * fvalue = NULL; // stores the value for the -f flag as a string
static char * svalue = NULL; // the value of the s flag is the s string we match for in the parse tree
static size_t slength = 0;
static const char const * invalid_chars = "%&#@!"; // chars that are illegal in the input mathc string

// add an array of pointers to head nodes already seen for symlink traversal
static char ** seen_links = NULL;
static size_t len_seen_links = 0;

int main(int argc, char * argv [])
{
	char cmd;
	char ** seen_links = malloc(sizeof(char * ) * 1);

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
				printf("unknown predicate '%c'\n", optopt);
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
		printf("Path not specified.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (!sflag || !slength)
	{
		printf("Match string not specified.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// check for illegal chars in the match string
	char * c =  NULL;
	if (c = strpbrk(svalue, invalid_chars))
	{
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

		if ((*fvalue) != 'c' && (*fvalue) != 'h' && (*fvalue) != 'S')
		{
			printf("Invalid input arg %c for -f flag.", *fvalue);
			exit(EXIT_FAILURE);
		}
	}

	// now traverse the path input
	int ret_val = traverse(pvalue, plength);

	// if we were traversing symlinks, free all the head nodes now
	if (seen_links != NULL)
	{
		int i;
		for (i = 0; i < len_seen_links; i++)
		{
			free(seen_links[i]);
		}
	}

 	exit(EXIT_SUCCESS);
}

// main os tree walker that recursively traverses the file tree, calling parser on all files
int traverse(const char * const path, size_t pathlen)
{
	struct stat statbuf;

	// we now lstat the file to tell if it is a link or not
	if (lflag)
	{
		if (stat(path, &statbuf) == -1)
		{
			printf("'%s': No such file or directory\n", path);
			return 1;
		}
	}
	else
	{
		if (lstat(path, &statbuf) == -1)
		{
			printf("'%s': No such file or directory\n", path);
			return 1;
		}
	}

	
	switch(statbuf.st_mode & S_IFMT)
	{
		case (S_IFREG):
		{
			// regular file

			// if flag -f is set, look only in files with extension specified
			if (fflag)
			{
				//
				if (path[pathlen - 3] == '.' && path[pathlen - 2] == *fvalue)
				{
					parse_regular(path, svalue);
				}
				else
				{
					return 0;
				}
			}
			else
			{
				parse_regular(path, svalue);
			}
		} break;
		case (S_IFDIR):
		{
			// directory
			// add this directory to the head nodes list if we have never seen this head node before
			// if we have seen this before, skip it
			if (lflag)
			{
				if (check_seen_links(path, pathlen))
					return 0;
			}	

			DIR * directory = opendir(path);
			if (directory == NULL)
			{
				printf("Could not open directory at %s for reading. \n", path);
				return 1;
			}
			else
			{
				struct dirent * dir_entry;
				// printf("%s%s%s: Is a directory\n", GREEN, path, NORMAL_COLOR);
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
			}
		} break;
		case (S_IFLNK):
		{
			// symbloic link
			// only parse symlinks if sflag is true
			if (sflag)
			{
				if (check_seen_links(path, pathlen))
					return 0;

				int path_size;

				if (statbuf.st_size == 0)
					path_size = max_path_length;
				else
					path_size = statbuf.st_size + 1;

				char * lnkpath = malloc(sizeof(char) * path_size);
				ssize_t size = readlink(path, lnkpath, path_size);
				lnkpath[size] = '\0';

				traverse(lnkpath, size);

				free(lnkpath);
			}
		} break;
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

	// now start looking for matches
	int matchstr_len = strlen(match_string);
	// TODO: make this able to read arbitrary length lines
	int bufsize = 1024;
	char * line_buffer = malloc(sizeof(char) * bufsize);
	char * start_of_match = NULL;
	bool found = false;

	while(fgets(line_buffer, bufsize, fptr))
	{
		
		if ((start_of_match = strstr(line_buffer, match_string)) != NULL)
		{
			// first we get the length of the first segment of the array
			size_t seg_1 = start_of_match - line_buffer;
			printf("%.*s", seg_1, line_buffer);
			printf("%s%.*s%s", RED, matchstr_len, start_of_match, NORMAL_COLOR);
			start_of_match += matchstr_len;
			printf("%s", start_of_match);

			// now check if the line has a \n at end or not
			char * newlinecheck = strchr(start_of_match, '\n');

			if (!newlinecheck)
				printf("\n");

			found = true;
		}
	}

	// print the path to the file if anything matched
	if (found)
		printf("%s\n", path);

	free(line_buffer);
	fclose(fptr);
	return 0;
}

bool check_seen_links(const char * path, size_t pathlen)
{

	// first make a copy of the path to the link
	char * copy_of_path = malloc(pathlen);

	copy_of_path = strcpy(copy_of_path, path);

	// head not seen, add to head nodes list
	int i;
	for (i = 0; i < len_seen_links; i++)
	{
		if (strcmp(seen_links[i], copy_of_path) == 0)
		{
			free(copy_of_path);
			return true;
		}
	}

	// the link is new, add it to the list
	len_seen_links++;

	char ** pointer = realloc(seen_links, len_seen_links);

	if (pointer == NULL)
	{
		printf("Realloc of head nodes failed.\n");
	}
	else
	{
		seen_links = pointer;
		seen_links[len_seen_links - 1] = copy_of_path;
	}

	return false;
}
