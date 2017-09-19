// A2 : Make a find clone . for extra credit, support regex matching

#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for cmd abool
#include <stdbool.h> // bool support

int main(int argc, char * argv [])
{
	// set up flag variables
	const char * flags = "p:f::l::s:";

	bool pflag = false; // path flag
	bool fflag = false; // specifies [c|h|S] that parses either .c or .h file only
	bool sflag = false; // parse match string
	bool lflag = false; // parses symbolic links too if set

	char * pvalue = NULL; // path to the root of the parse tree
	char * fvalue = NULL; // stores the value for the -f flag as a string
	char * svalue = NULL; // the value of the s flag is the s string we mathc for in the parse tree

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
				svalue = optarg;
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

	
 	return 0;
}
