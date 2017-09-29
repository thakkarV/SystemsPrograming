#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

void show_dir_content(char * path)
{
	FILE * f;

	DIR * d = opendir(path); 
	
	if (d==NULL)
		return; 
	
	struct dirent * dir; 

	while ((dir = readdir(d)) != NULL) 
	{
		if(dir-> d_type != DT_DIR) // if it is not a directory
		{
			printf("%s%s\n",BLUE, dir->d_name);
		}
		else if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory { // dir -> d_type == DT_DIR
		{
			printf("%s%s\n",GREEN, dir->d_name); 

		}
	}

	closedir(d); // close the directory
}


int main(int argc, char **argv)
{

	printf("%s\n", NORMAL_COLOR);


	show_dir_content(argv[1]);

	printf("%s\n", NORMAL_COLOR);
	return(0);
}

