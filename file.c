#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include <pwd.h>
#include<grp.h>
#include<time.h>

//Colour codes for printing directories and sym links
#define COLOR_BLUE   "\033[1m\033[34m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_CYAN    "\033[36m"

int mysort(const struct dirent **a, const struct dirent **b)
{
	//printf("\nSorting");	
	struct stat sa;
	struct stat sb;	
	if (lstat((*a)->d_name, &sa) == -1) 
	{
		perror("Error in stat");
        }	

	if (lstat((*b)->d_name, &sb) == -1) 
	{
		perror("Error in stat");
        }

	const time_t timea = sa.st_mtime;
	
	const time_t timeb = sb.st_mtime;
		
	if(timea>timeb)
		return 1;
	else if(timea==timeb)
		return 0;
	return -1;
}
	
		
int myfilter(const struct dirent *entry)
{
	const char *num= entry->d_name;
	if(num[0]=='.')
		return 0;

	else
		return -1;


}
char *month(int num)
{
	char *arr;
	switch(num)
	{
		case 0: arr= "Jan";
			break;
		case 1: arr= "Feb";
			break;
		case 2: arr= "Mar";
			break;
		case 3: arr= "Apr";
			break;
		case 4: arr= "May";
			break;
		case 5: arr= "June";
			break;
		case 6: arr= "July";
			break;
		case 7: arr= "Aug";
			break;
		case 8: arr= "Sept";
			break;
		case 9: arr= "Oct";
			 break;
		case 10: arr= "Nov";
			 break;
		case 11: arr= "Dec";
	}
	return arr;
}
int funca(char *path)
{
	
	//Function to list ls -a
	//char path[]= "/home/ruchir";
	struct dirent **namelist;
	int n;
	struct stat sb;
	chdir(path);
	n = scandir(path, &namelist, NULL, alphasort);
	int count = 0;	
	if (n < 0)
		perror("scandir");
	else
	{
		while (n--) 
		{
			
			if (lstat(namelist[n]->d_name, &sb) == -1) 
			{
	               		perror("Error in stat");
        	        }
			mode_t mode = sb.st_mode;
			int flagdir=0,flagsym=0;
			//printf("Mode: %d\t",mode);
			switch (sb.st_mode & S_IFMT)
			{
			  
			   case S_IFDIR:   flagdir=1;  break;
				
			   case S_IFLNK:   flagsym=1;  break;
				
             		}
			
				
			//Name
			if(flagdir==1)
			{	
				printf(COLOR_BLUE);				
				printf("%-20s\t",namelist[n]->d_name);
				printf(COLOR_RESET);					
			}
			else if(flagsym==1)
			{	
				printf(COLOR_CYAN);				
				printf("%-20s\t",namelist[n]->d_name);
				printf(COLOR_RESET);					
				
			}

			else
				printf("%-20s\t", namelist[n]->d_name);					
			count++;
			if(count%6==0)
			{
				printf("\n");
				count=0;
			}
			free(namelist[n]);
		}
		free(namelist);
	}
	printf("\n");
	return 0;
}

void funcal(int flag,char *path)
{
	// Function to list using ls -al and ls -l options. If flag is one, display all (-a) if flag is 2 ls-lt
	struct dirent **namelist;
	int n;
	struct stat sb; 

	chdir(path);
	if(flag==1) //ls -al
		n = scandir(path, &namelist, NULL, alphasort);
	else if(flag==2)
	{
		// ls -lt
		n = scandir(path,&namelist, myfilter, mysort);
	}
	else if(flag==3) //ls -l
		n = scandir(path, &namelist, myfilter, alphasort);
	
	
		
	if (n < 0)
		perror("scandir");
	else
	{
		//counting
		long long int sum=0;
		while(n--)
		{
			if (lstat(namelist[n]->d_name, &sb) == -1) 
			{
			       		perror("Error in stat");
			}
			else
			{
				int count = sb.st_blocks;		
				sum = sum + count;
			}
		}

		printf("total %lld\n",sum/2);

		//initializing again		
		if(flag==1) //ls -al
		n = scandir(".", &namelist, NULL, alphasort);
		else if(flag==2)
		{
			// ls -lt
			n = scandir(".",&namelist, myfilter, mysort);
		}
		else if(flag==3) //ls -l
			n = scandir(".", &namelist, myfilter, alphasort);
	
		while (n--) 
		{
			
			 if (lstat(namelist[n]->d_name, &sb) == -1) 
			{
	               		perror("Error in stat");
        	         }
			else
			{
				char out[4];out[0]='d';
				int flagsym=0,flagdir=0;
				//For Mode				
				mode_t mode = sb.st_mode;
				//printf("Mode: %d\t",mode);
				switch (sb.st_mode & S_IFMT)
				 {
				    case S_IFBLK:  printf("b");               break;
				    case S_IFCHR:  printf("c");               break;
				    case S_IFDIR:  printf("d");   flagdir=1;  break;
				    case S_IFIFO:  printf("p");               break;
				    case S_IFLNK:  printf("l");   flagsym=1;  break;
				    case S_IFREG:  printf("-");               break;
				    case S_IFSOCK: printf("s");               break;
				    default:       printf("?\n");             break;
             			    }
				printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
				printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
				printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
				printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
				printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
				printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
				printf( (sb.st_mode & S_IROTH) ? "r" : "-");
				printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
				printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
				
				//No of links				
				printf("\t%4ld",(long)sb.st_nlink);
				
				//User Details
				uid_t uid =     sb.st_uid;
				struct passwd *pwd= getpwuid(uid);
				printf(" %6s",pwd->pw_name);
				
				//Group Details
				gid_t  gid=   sb.st_gid;				
				struct group *g = getgrgid(gid);
				printf(" %6s",g->gr_name);
				
				//File Size
				printf("\t %8lld",(long long )sb.st_size);
				
				//Time
				const time_t *time = &sb.st_atime;
				struct tm *t = localtime(time);				    
				printf("\t%s %d %2d:%2d", month(t->tm_mon),t->tm_mday,t->tm_hour,t->tm_min);

				//Name
				if(flagdir==1)
				{	
					printf(COLOR_BLUE);				
					printf("\t%-20s\n",namelist[n]->d_name);
					printf(COLOR_RESET);					
				}
				else if(flagsym==1)
				{	
					printf(COLOR_CYAN);
					char linkname[102];
					//printf("\n\nIts a link..!!");
					int r = readlink(namelist[n]->d_name, linkname, sizeof(linkname)-1);
					if(r==-1)
						perror("\nError in else");
					else
					{	
						printf("\t%s -> ",namelist[n]->d_name);
						printf(COLOR_BLUE);
						printf("%s\n",linkname);
					}					
					printf(COLOR_RESET);				
				}

				else
					printf("\t%-20s\n", namelist[n]->d_name);					
			}
			free(namelist[n]);
		}
		free(namelist);
	}
	printf("\n");
	
}


void funcalt(char *path)
{
	//This function displays the output in ls -alt form	
	struct dirent **namelist;
	int n;
	struct stat sb;
	chdir(path);
	n = scandir(path, &namelist, NULL, alphasort);
	
	
		
	if (n < 0)
		perror("scandir");
	else
	{
		
		//counting
		long long int sum=0;
		while(n--)
		{
			if (lstat(namelist[n]->d_name, &sb) == -1) 
			{
			       		perror("Error in stat");
			}
			else
			{
				int count = sb.st_blocks;		
				sum = sum + count;
			}
		}

		printf("total %lld\n",sum/2);

		// Scanning again
		n = scandir(".", &namelist, NULL, alphasort);


		while (n--) 
		{
			
			 if (lstat(namelist[n]->d_name, &sb) == -1) 
			{
	               		perror("Error in stat");
        	         }
			else
			{
				char out[4];out[0]='d';
				int flagsym=0,flagdir=0;
				//For Mode				
				mode_t mode = sb.st_mode;
				//printf("Mode: %d\t",mode);
				switch (sb.st_mode & S_IFMT)
				 {
				    case S_IFBLK:  printf("b");               break;
				    case S_IFCHR:  printf("c");               break;
				    case S_IFDIR:  printf("d");   flagdir=1;  break;
				    case S_IFIFO:  printf("p");               break;
				    case S_IFLNK:  printf("l");   flagsym=1;  break;
				    case S_IFREG:  printf("-");               break;
				    case S_IFSOCK: printf("s");               break;
				    default:       printf("?\n");             break;
             			    }
				printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
				printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
				printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
				printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
				printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
				printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
				printf( (sb.st_mode & S_IROTH) ? "r" : "-");
				printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
				printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
				
				//No of links				
				printf("\t%4ld",(long)sb.st_nlink);
				
				//User Details
				uid_t uid =     sb.st_uid;
				struct passwd *pwd= getpwuid(uid);
				printf(" %6s",pwd->pw_name);
				
				//Group Details
				gid_t  gid=   sb.st_gid;				
				struct group *g = getgrgid(gid);
				printf(" %6s",g->gr_name);
				
				//File Size
				printf("\t %8lld",(long long )sb.st_size);
				
				//Time
				const time_t *time = &sb.st_atime;
				struct tm *t = localtime(time);				    
				printf("\t%s %d %2d:%2d", month(t->tm_mon),t->tm_mday,t->tm_hour,t->tm_min);

				//Name
				if(flagdir==1)
				{	
					printf(COLOR_BLUE);				
					printf("\t%-20s\n",namelist[n]->d_name);
					printf(COLOR_RESET);					
				}
				else if(flagsym==1)
				{	printf(COLOR_CYAN);
					char linkname[100];
					//printf("\n\nIts a link..!!");
					int r = readlink(namelist[n]->d_name, linkname, sizeof(linkname)-1);
					if(r==-1)
						perror("\nError in else");
					else
					{
						printf("\t%s -> ",namelist[n]->d_name);
						printf(COLOR_BLUE);
						printf("%s\n",linkname);
					}						
					printf(COLOR_RESET);				
				}

				else
					printf("\t%-20s\n", namelist[n]->d_name);					
			}
			free(namelist[n]);
		}
		free(namelist);
	}
	printf("\n");
	
	
}
void funct(int flag, char *path)
{
	//This function displays the output in ls -t(if flag=1) and ls -at form	
	struct dirent **namelist;
	int n;
	chdir(path);
	struct stat sb;
	if (flag==1)
		n = scandir(path, &namelist,myfilter, mysort);
	else
		n = scandir(path, &namelist,NULL, mysort);
		
	int count = 0;	
	if (n < 0)
		perror("scandir");
	else
	{
	
				
		while (n--) 
		{
			
			if (lstat(namelist[n]->d_name, &sb) == -1) 
			{
	               		perror("Error in stat");
        	        }
			mode_t mode = sb.st_mode;
			int flagdir=0,flagsym=0;
			//printf("Mode: %d\t",mode);
			switch (sb.st_mode & S_IFMT)
			{
			  
			   case S_IFDIR:   flagdir=1;  break;
				
			   case S_IFLNK:   flagsym=1;  break;
				
             		}
			
				
			//Name
			if(flagdir==1)
			{	
				printf(COLOR_BLUE);				
				printf("%-20s\t",namelist[n]->d_name);
				printf(COLOR_RESET);					
			}
			

			else
				printf("%-20s\t", namelist[n]->d_name);					
			count++;
			if(count%6==0)
			{
				printf("\n");
				count=0;
			}
			free(namelist[n]);
		}
		free(namelist);
	}
	printf("\n");
}

int whatis(char var)
{
	if(var=='l')
		return 1;
	if(var=='a')
		return 2;
	if(var=='t')
		return 3;
	else
		return 4;
}		

int main(int args, char *argv[])
{
	
	int chcount=0;
	char *path;
	path = ".";
	int array[10];	
	int i,flagl=0,flaga=0,flagt=0,countp=0;
	for(i=0;i<10;i++)
		array[i]=0;	
	for(i=1;i<args;i++)
	{
		//printf("\n in loop");
		char *var = argv[i];
		//printf("/n var is %s",var);
		int len = strlen(var);
		char *last;
		
		if(strstr(var,"/"))
		{
			path = var;
			array[countp] = i;
 			countp++;
			//printf("\n\n!!! Here, path =%s",path);
			//printf("%s: \n",var);
		}
		else
		{
			DIR *dirptr = opendir(var);
			if(dirptr!=NULL)
			{
				
				array[countp] = i;
 				countp++;
				//printf("%s: \n",var);
			}
			else
			{	
		
			
				int j,flag=0;
				for(j=0;j<len;j++)
				{
					char c = var[j]; 
					//printf("\n in loop 2 char is %c",c);
					if(c=='-')
					{
						flag=1; //printf("\n read -");
					}
					else
					{
				
						if(flag==0)
						{
							printf("Invalid input");
													
							return 0;						
						}						
						int num = whatis(c);
						if(num==1)
							flagl=1;
						else if(num==2)
							flaga=1;
						else if(num==3)
							flagt=1;
						else if(num==4)
						{
							printf("\n Invalid input, Not a directory or flag !! Exiting\n");
							return 0;			
						}
					}
				}
			}
		}
	
	}
	if(countp==0)
	{
		//Call for simple
		if((flaga==1)&&(flagl==1)&&(flagt==1))
		{
			funcalt(path);
		}	
		if(flagt==1)
		{
			if((flaga==1)&&(flagl==0))	
			{
				//printf("\n\nPrinting for ls -at\n\n");		
				funct(2,path); //ls -at
			}
			else if((flaga==0)&&(flagl==0))
			{
				//printf("\n\nPrinting for ls -t\n\n");			
				funct(1,path); //ls -t
			}
		}
		if(flaga==1)
		{
			if((flagl==1)&&(flagt==0))	
			{
				//printf("\n\nPrinting for ls -al\n\n");
				funcal(1,path); //ls -al
			}				
			else if((flagl==0)&&(flagt==0))		
			{
				//printf("\n\nPrinting for ls -a\n\n");			
				funca(path); //ls -a
			}
			
		}
	
		if(flagl==1)
		{	
			if((flagt==1)&&(flaga==0))
			{
				//printf("\n\nPrinting for ls-lt");
				funcal(2,path);//ls -lt
			}
			else if((flagt==0)&&(flaga==0))		
			{
				//printf("\n\nPrinting for ls-l");
				funcal(3,path); //ls -l	
			}
		}
		if((flagl==0)&&(flagt==0)&&(flaga==0))
			funct(1,path);
	}	

	// Calling functions for complex tasks
	for(i=0;i<countp;i++)
	{
		path= argv[array[i]];
		printf("\n%s: \n",path);		
		if((flaga==1)&&(flagl==1)&&(flagt==1))
		{
			funcalt(path);
		}	
		if(flagt==1)
		{
			if((flaga==1)&&(flagl==0))	
			{
				//printf("\n\nPrinting for ls -at\n\n");		
				funct(2,path); //ls -at
			}
			else if((flaga==0)&&(flagl==0))
			{
				//printf("\n\nPrinting for ls -t\n\n");			
				funct(1,path); //ls -t
			}
		}
		if(flaga==1)
		{
			if((flagl==1)&&(flagt==0))	
			{
				//printf("\n\nPrinting for ls -al\n\n");
				funcal(1,path); //ls -al
			}				
			else if((flagl==0)&&(flagt==0))		
			{
				//printf("\n\nPrinting for ls -a\n\n");			
				funca(path); //ls -a
			}
			
		}
	
		if(flagl==1)
		{	
			if((flagt==1)&&(flaga==0))
			{
				//printf("\n\nPrinting for ls-lt");
				funcal(2,path);//ls -lt
			}
			else if((flagt==0)&&(flaga==0))		
			{
				//printf("\n\nPrinting for ls-l");
				funcal(3,path); //ls -l	
			}
		}
	}

}



