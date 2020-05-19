#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include "UnixLs.h"


//List out the contents of the directory with the il option or li option specified
//Reference: https://stackoverflow.com/questions/20450556/linux-ls-l-implementation-using-file-descriptors-in-c-c
//time reference: https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811
//formatting reference for time: https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
void lsilOption(char * directory)
{

        DIR * directoryPointer;
        struct dirent * entryPointer;
        struct stat theStat;
        struct passwd * pw;
        struct group * gr;
        char fullPath[PATH_MAX];

	time_t timer;
        char timeBuffer[TIME_BUFFER_MAX];
        struct tm *timeInfo;

	//These variables will be used for determining the format width when printing out the inode number, nlink number and size
	int iNodeDigitCount = 0;
	int maxiNodeDigitCount = 0;

	int digitCountSize = 0;
        int maxDigitCountSize = 0;

        int digitCountNLink = 0;
        int maxDigitCountNLink = 0;

	pw = getpwuid(geteuid());


	//The first time we open up the directory, we want to only determine the max digits of the biggest inode number
	directoryPointer = opendir(directory);

	if(directoryPointer != NULL)
	{
		   
                while (entryPointer = readdir(directoryPointer))
                {
			//Ignore any files that begin with . or ..
                        if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
                        {
                                continue;
                        }

                        strcat(fullPath,directory);
                        strcat(fullPath, "/");
                        strcat(fullPath, entryPointer->d_name);
                        stat(fullPath, &theStat);

                        char iNodeString[50];
                        sprintf(iNodeString, "%ld", theStat.st_ino);
                        iNodeDigitCount = 0;
                        for(int i = 0; i < strlen(iNodeString); i++)
                        {
                                iNodeDigitCount++;
                        }

			if(iNodeDigitCount > maxiNodeDigitCount)
			{
				maxiNodeDigitCount = iNodeDigitCount;
			}

			char sizeString[50];
                        sprintf(sizeString, "%ld", theStat.st_size);
                        digitCountSize = 0;

                        for(int i = 0; i < strlen(sizeString); i++)
                        {
                                digitCountSize++;
                        }

                        if(digitCountSize > maxDigitCountSize)
                        {
                                maxDigitCountSize = digitCountSize;
                        }

                        char NLinkString[50];
                        sprintf(NLinkString, "%ld", theStat.st_nlink);
                        digitCountNLink = 0;
                        for(int i = 0; i < strlen(NLinkString); i++)
                        {
                                digitCountNLink++;
                        }

                        if(digitCountNLink > maxDigitCountNLink)
                        {
                                maxDigitCountNLink = digitCountNLink;
                        }


                        strcpy(fullPath, "");

                }
		closedir(directoryPointer);
	}

	//The second time we open up the directory, we gather the entries and print out their information 
        directoryPointer = opendir(directory);

        if(directoryPointer != NULL)
        {
		
                while (entryPointer = readdir(directoryPointer))
                {

			//Ignore any files beginning with .. or .
                        if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
                        {
                                continue;
                        }

			//Concatenate the directory string onto the buffer we have declared
                        strcat(fullPath,directory);
			strcat(fullPath, "/");
                        strcat(fullPath, entryPointer->d_name);
                        stat(fullPath, &theStat);
			
			//Get the group number
			gr = getgrgid(theStat.st_gid);

			//Get the time the file was last modified
                        timer = theStat.st_mtime;
                        timeInfo = localtime  ( &timer );
                        strftime(timeBuffer, TIME_BUFFER_MAX, "%b %2d %4Y %2H:%2M", timeInfo);

			//Determine the permissions of the file and print out the inode number
			printf("%*ld ",maxiNodeDigitCount, theStat.st_ino);
                        printf( (S_ISDIR(theStat.st_mode)) ? "d"  : "-");
                        printf( (theStat.st_mode & S_IRUSR) ? "r" : "-");
                        printf( (theStat.st_mode & S_IWUSR) ? "w" : "-");
                        printf( (theStat.st_mode & S_IXUSR) ? "x" : "-");
                        printf( (theStat.st_mode & S_IRGRP) ? "r" : "-");
                        printf( (theStat.st_mode & S_IWGRP) ? "w" : "-");
                        printf( (theStat.st_mode & S_IXGRP) ? "x" : "-");
                        printf( (theStat.st_mode & S_IROTH) ? "r" : "-");
                        printf( (theStat.st_mode & S_IWOTH) ? "w" : "-");
                        printf( (theStat.st_mode & S_IXOTH) ? "x" : "-");


			//Print out the entry information 
			printf(" %*ld %s %s %*ld %s %s\n",maxDigitCountNLink, theStat.st_nlink, pw->pw_name, gr->gr_name, maxDigitCountSize, theStat.st_size, timeBuffer, entryPointer->d_name);
                        
			strcpy(fullPath, "");
                }

                closedir(directoryPointer);
        }
}





//List out the contents of the directory with the l option specified
//Reference: https://stackoverflow.com/questions/20450556/linux-ls-l-implementation-using-file-descriptors-in-c-c
//time reference: https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811
//https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
void lslOption(char * directory)
{

	DIR * directoryPointer;
	struct dirent * entryPointer;
	struct stat theStat;
	struct passwd * pw;
	struct group * gr;
	char fullPath[PATH_MAX];

	time_t timer;
        char timeBuffer[TIME_BUFFER_MAX];
        struct tm *timeInfo;

	//these variables will be used to determine the format width
	int digitCountSize = 0;
	int maxDigitCountSize = 0;

	int digitCountNLink = 0;
	int maxDigitCountNLink = 0;

	pw = getpwuid(geteuid());

	
	//The first time we open up the directory, we want to only determine the max digits of the biggest entry size
	//as well as the max digits of the biggest NLink number
        directoryPointer = opendir(directory);

        if(directoryPointer != NULL)
        {

                while (entryPointer = readdir(directoryPointer))
                {
			//Ignore any files that begin with . or ..
                        if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
                        {
                                continue;
                        }

                        strcat(fullPath,directory);
                        strcat(fullPath, "/");
                        strcat(fullPath, entryPointer->d_name);
                        stat(fullPath, &theStat);


                        char sizeString[50];
                        sprintf(sizeString, "%ld", theStat.st_size);
                        digitCountSize = 0;
			
                        for(int i = 0; i < strlen(sizeString); i++)
                        {
                                digitCountSize++;
                        }

                        if(digitCountSize > maxDigitCountSize)
                        {
                                maxDigitCountSize = digitCountSize;
                        }

			char NLinkString[50];
			sprintf(NLinkString, "%ld", theStat.st_nlink);
			digitCountNLink = 0;
			for(int i = 0; i < strlen(NLinkString); i++)
                        {
                                digitCountNLink++;
                        }

                        if(digitCountNLink > maxDigitCountNLink)
                        {
                                maxDigitCountNLink = digitCountNLink;
                        }

                        strcpy(fullPath, "");

                }
                closedir(directoryPointer);
        }


	//Open the directory so we can list out the files inside
	directoryPointer = opendir(directory);

	if(directoryPointer != NULL)
	{
		while (entryPointer = readdir(directoryPointer))
		{
			//Ignore any files that begin with . or ..
			if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
                        {
                                continue;
                        }
			
			//Concatenate the directory as a string onto a buffer we have declared
			strcat(fullPath,directory);
			strcat(fullPath, "/");
			strcat(fullPath, entryPointer->d_name);
			stat(fullPath, &theStat);

			//Get the group number
			gr = getgrgid(theStat.st_gid);	

			//Get the time the directory entry was last modified
			timer = theStat.st_mtime;
                        timeInfo = localtime  ( &timer );
                        strftime(timeBuffer, TIME_BUFFER_MAX, "%b %2d %4Y %2H:%2M", timeInfo);

			//Determine the permissions of the entry
			printf( (S_ISDIR(theStat.st_mode)) ? "d"  : "-");
			printf( (theStat.st_mode & S_IRUSR) ? "r" : "-");
			printf( (theStat.st_mode & S_IWUSR) ? "w" : "-");
			printf( (theStat.st_mode & S_IXUSR) ? "x" : "-");
			printf( (theStat.st_mode & S_IRGRP) ? "r" : "-");
			printf( (theStat.st_mode & S_IWGRP) ? "w" : "-");
			printf( (theStat.st_mode & S_IXGRP) ? "x" : "-");
			printf( (theStat.st_mode & S_IROTH) ? "r" : "-");
			printf( (theStat.st_mode & S_IWOTH) ? "w" : "-");
			printf( (theStat.st_mode & S_IXOTH) ? "x" : "-");

			//Print out the entry's information
			printf(" %*ld %s %s %*ld %s %s\n",maxDigitCountNLink, theStat.st_nlink, pw->pw_name, gr->gr_name,  maxDigitCountSize, theStat.st_size, timeBuffer, entryPointer->d_name);
	
			strcpy(fullPath, "");
		}

		closedir(directoryPointer);



	}
}

//List out the contents of the directory with the i option specified
//Reference for getting inode number: https://stackoverflow.com/questions/9480568/find-inode-number-of-a-file-using-c-code
void lsiOption(char * directory)
{
	DIR * directoryPointer;
	struct dirent * entryPointer;
	struct stat theStat;
	char fullPath[PATH_MAX];

	//These variables will be used for determining the format width when printing out the inode number
        int iNodeDigitCount = 0;
        int maxiNodeDigitCount;

	//The first time we open up the directory, we want to only determine the max digits of the biggest inode number
        directoryPointer = opendir(directory);

        if(directoryPointer != NULL)
        {

                while (entryPointer = readdir(directoryPointer))
                {
			//Ignore any files that begin with . or ..
                        if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
                        {
                                continue;
                        }

                        strcat(fullPath,directory);
                        strcat(fullPath, "/");
                        strcat(fullPath, entryPointer->d_name);
                        stat(fullPath, &theStat);

                        char iNodeString[50];
                        sprintf(iNodeString, "%ld", theStat.st_ino);
                        iNodeDigitCount = 0;
                        for(int i = 0; i < strlen(iNodeString); i++)
                        {
                                iNodeDigitCount++;
                        }

                        if(iNodeDigitCount > maxiNodeDigitCount)
                        {
                                maxiNodeDigitCount = iNodeDigitCount;
                        }

                        strcpy(fullPath, "");

                }
                closedir(directoryPointer);
        }

	//Open the directory so we can list the entries inside it
	directoryPointer = opendir(directory);
	if (directoryPointer != NULL)
	{
		while (entryPointer = readdir(directoryPointer))
		{
			//Ignore any entries beginning with . or ..
			if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
			{
				continue;
			}


			//Concatenate the specified directory path into the buffer so we can get the entry information
			strcat(fullPath,directory);
			strcat(fullPath,"/");
			strcat(fullPath, entryPointer->d_name);
			
			stat(fullPath, &theStat);
			

			//Print out the result
			printf("%*ld %s  \n", maxiNodeDigitCount, theStat.st_ino, entryPointer->d_name);

			strcpy(fullPath, "");
			
		}
		

		closedir(directoryPointer);
	}
	else
	{
		perror("could not open the directory");
	}


}

//Lists all the items in the specified parameter
//Reference: https://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
void listDirectory(char * directory)
{
	DIR * directoryPointer;
	struct dirent * entryPointer;
	directoryPointer = opendir(directory);

	if (directoryPointer != NULL)
	{
		while (entryPointer = readdir(directoryPointer))
		{
			if(strcmp(entryPointer->d_name, ".") == 0 || strcmp(entryPointer->d_name, "..") == 0 || entryPointer->d_name[0] == '.')
			{
				continue;
			}
			printf("%s  \n",entryPointer->d_name);
		}
		
		closedir(directoryPointer);
	}	
	else
	{
		perror("Could not open the directory");
	}

}



int main(int argc, char *argv[])
{
	char path[PATH_MAX];
	char option[50] = "-";
	int optionCount = 0;

	//Set boolean flags which will determine which options are being specified
	bool iOption = false;
	bool lOption = false;

	getcwd(path, sizeof(path));
	

	
	//No options specified, just list the current directory
	if(argv[1] == NULL && argc == 1)
	{
		listDirectory(path);
	}
	//List a specified directory with no options
	else if(argc == 2 && argv[1][0] != '-')
	{
		strcpy(path, argv[1]);
		listDirectory(path);

	}
	//Single argument option (list current directory with options
	else if(argc == 2 && argv[1][0] == '-')
	{
		if(argv[1][1] == 'i' && !argv[1][2])
		{
			lsiOption(path);
		}
		else if(argv[1][1] == 'l' && !argv[1][2])
		{
			lslOption(path);
		}	
		else if(argv[1][1] == 'i' && argv[1][2] == 'l')
		{
			lsilOption(path);
		}
		else if(argv[1][1] == 'l' && argv[1][2] == 'i')
		{
			lsilOption(path);
		}
	}	
	//More than one argument
	else if (argc > 2)
	{

		
	 	//Determine the option for the command
		for(int i = 1; i < argc; i++)
		{
			if(argv[i][0] == '-')
			{
				optionCount++;
				strcat(option,&argv[i][1]);
			}
		}
		//Set option flags based on the command option
		for(int i = 1; i < strlen(option); i++)
		{
			if(option[i] == 'l')
			{
				lOption = true;
			}
			else if(option[i] == 'i')
			{
				iOption = true;
			}
		}
	
	
	
		//Only options specified in the command
		if(optionCount + 1 == argc)
		{
			if( strlen(option) > 1 && iOption && !lOption)
			{
				lsiOption(path);
			}
			else if( strlen(option) > 1 && lOption && !iOption)
			{
				lslOption(path);
			}
			else if( strlen(option) > 1 && iOption && lOption)
			{
				lsilOption(path);
			}
		}
	
		//List the directories specified in the command with respect to the options	
		for(int i = 1 + optionCount; i < argc; i++)
		{
			//No options specified
			if(strlen(option) == 1)
			{				
				printf("%s:\n",argv[i]);
				listDirectory(argv[i]);
			}	
			
			//i option specified
			else if( strlen(option) > 1 && iOption && !lOption )
			{
				
				if(optionCount + 1 == argc-1)
				{
					lsiOption(argv[i]);
				}
				else
				{
					printf("%s:\n",argv[i]);
					lsiOption(argv[i]);
				}


				if(i != argc-1)
				{
					printf("\n");
				}

				
			}
			//l option specified
			else if( strlen(option) > 1 && lOption && !iOption)
			{

				
				if(optionCount + 1 == argc-1)
				{
					lslOption(argv[i]);
				}
				else
				{
					printf("%s:\n",argv[i]);
					lslOption(argv[i]);
				}

				if(i != argc-1)
				{
					printf("\n");
				}

			}
			//Both l and i options specified
			else if( strlen(option) > 1 && lOption && iOption)
			{

				
				if(optionCount + 1 == argc-1)
				{
					lsilOption(argv[i]);
				}
				else
				{
					printf("%s:\n",argv[i]);
					lsilOption(argv[i]);
				}


				if(i != argc-1)
				{
					printf("\n");
				}
	
			}	
		}

	}


	return 0;


}
