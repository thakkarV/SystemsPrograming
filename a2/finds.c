// a2 : make a find clone . for extra credit, support regex matching

#include <stdlib.h> // printf
#include <string.h> // strcmp, strpbrk, strcpy, strcat
#include <unistd.h> // getopt, optopt, optarg
#include <stdbool.h> // bool support
#include <stdio.h> // file 
#include <dirent.h> // dir
#include <sys/stat.h> // stat, lstat

static const int max_path_length = 4096;

// color table for printing
#define normal_color  "\x1b[0m"
#define red           "\x1b[31m"
#define green         "\x1b[32m"
#define blue          "\x1b[34m"

// the file tree walk function that decends the directory hierarchy
int travers(const char * root, const char * match_string);

// parses the contents of regular files to find matches line by line
int parse_regular(const char * path, const char * match_string);

// check if the current head node of dir path has been seen already or not for symlink parsing
bool check_head_nodes(const char * path, size_t pathlen);

// declare global vars for cmd args
static const char const * flags = "p:f:ls:";
static bool pflag = false; // path flag
static bool fflag = false; // specifies [c|h|s] that parses either .c or .h file only
static bool sflag = false; // parse match string
static bool lflag = false; // parses symbolic links too if set

static char * pvalue = null; // path to the root of the parse tree
static size_t plength = 0;// size of abs_path in bytes
static char * fvalue = null; // stores the value for the -f flag as a string
static char * svalue = null; // the value of the s flag is the s string we match for in the parse tree
static size_t slength = 0;
static const char const * invalid_chars = "%&()#@!"; // chars that are illegal in the input mathc string

// add an array of pointers to head nodes already seen for symlink traversal
static char ** head_nodes = null;
static size_t len_head_nodes = 0;

int main(int argc, char * argv [])
{
	char cmd;
	char ** head_nodes = malloc(sizeof(char * ) * 1);

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
		printf("%s: path not specified.\n", argv[0]);
		exit(exit_failure);
	}

	if (!sflag || !slength)
	{
		printf("%s: match string not specified.\n", argv[0]);
		exit(exit_failure);
	}

	// check for illegal chars in the match string

	char * c =  null;
	if (c = strpbrk(svalue, invalid_chars))
	{
		printf("%s: invalid charecter(s) %c found in match string.\n", argv[0], c);
		exit(exit_failure);
	}

	// now check if fflag is true, then the vlaue of the flag is valid
	if (fflag)
	{
		if (!fvalue)
		{
			printf("no value for -f specified.\n");
			exit(exit_failure);
		}

		if ((*fvalue) != 'c' && (*fvalue) != 'h' && (*fvalue) != 's')
		{
			printf("invalid input arg %c for -f flag.", *fvalue);
			exit(exit_failure);
		}
	}

	// now traverse the path input
	int ret_val = traverse(pvalue, plength);

	// if we were traversing symlinks, free all the head nodes now
	if (head_nodes != null)
	{
		int i;
		for (i = 0; i < len_head_nodes; i++)
		{
			free(head_nodes[i]);
		}
	}

 	exit(exit_success);
}

// main os tree walker that recursively traverses the file tree, calling parser on all files
int traverse(const char * const path, size_t pathlen)
{
	struct stat statbuf;

	// we now lstat the file to tell if it is a link or not
	if (lstat(path, &statbuf) == -1)
	{
		printf("permission denied: could not open %s for reading.\n", path);
		return 1;
	}
	
	switch(statbuf.st_mode & s_ifmt)
	{
		case (s_ifreg):
		{
			// regular file

			// if flag -f is set, look only in files with extension specified
			if (fflag)
			{
				//
				if (path[pathlen - 2] == '.' && path[pathlen - 1] == *fvalue)
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
			
			break;
		}
		case (s_ifdir):
		{
			// directory
			// add this directory to the head nodes list if we have never seen this head node before
			// if we have seen this before, skip it
			if (lflag)
			{
				if (check_head_nodes(path, pathlen))
					return 0;
			}
			
			dir * directory = opendir(path);
			if (directory == null)
			{
				printf("could not open directory at %s for reading. \n", path);
				return 1;
			}
			else
			{
				struct dirent * dir_entry;
				// printf("%s%s%s: is a directory\n", green, path, normal_color);
				while ((dir_entry = readdir(directory)) != null) 
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
			break;
		}
		case (s_iflnk):
		{
			// symbloic link
			// only parse symlinks if sflag is true
			if (sflag)
			{
				int path_size;

				if (statbuf.st_size == 0)
					path_size = max_path_length;
				else
					path_size = statbuf.st_size;

				char * lnkpath = malloc(sizeof(char) * path_size);
				ssize_t size = readlink(path, lnkpath, path_size);

				traverse(lnkpath, size);

				free(lnkpath);
			}
			break;
		}
	}
}

int parse_regular(const char * path, const char * match_string)
{
	file * fptr;
	fptr = fopen(path, "r");

	// check for successful open
	if (fptr == null)
	{
		printf("could not open file for reading.\n");
		return 1;
	}

	// now start looking for matches
	int matchstr_len = strlen(match_string);
	// todo: make this able to read arbitrary length lines
	int bufsize = 1024;
	char * line_buffer = malloc(sizeof(char) * bufsize);
	char * start_of_match = null;
	bool found = false;

	while(!feof(fptr))
	{
		fgets(line_buffer, bufsize, fptr);
		
		if ((start_of_match = strstr(line_buffer, match_string)) != null)
		{
			// first we get the length of the first segment of the array
			size_t seg_1 = start_of_match - line_buffer;
			printf("%.*s", seg_1, line_buffer);
			printf("%s%.*s%s", red, matchstr_len, start_of_match, normal_color);
			start_of_match += matchstr_len;
			printf("%s", start_of_match);
		}
	}

	// print the path to the file if anything matched
	if (found)
		printf("%s\n", path);

	free(line_buffer);
	fclose(fptr);
	return 0;
}

bool check_head_nodes(const char * path, size_t pathlen)
{
	// first extract head node from path
	int len_node_name = strcspn(path, "/");
	char * node_name = malloc(len_node_name);
	node_name = memcpy(node_name, path, len_node_name);

	int i;
	for (i = 0; i < len_head_nodes; i++)
	{
		if (strcmp(node_name, head_nodes[i]) == 0)
		{
			free(node_name);
			return true;
		}
	}

	// head not seen, add to head nodes list
	len_head_nodes++;

	char ** pointer = realloc(head_nodes, len_head_nodes);

	if (pointer == null)
	{
		printf("realloc of head nodes failed.\n");
	}
	else
	{
		head_nodes = pointer;
		head_nodes[len_head_nodes - 1] = node_name;
	}

	return false;
}
