#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include<stdbool.h>
#include <dirent.h>
#include<sys/types.h>
#include<errno.h>

//builds a path string for a given directory and file name
void buildPath(char directory[],char file[],char* path){
	strcpy(path,directory);
	strcat(path,"/");
	strcat(path,file);
}

//returns true if it "file" is a file in the given directory
bool isFile(char *directory,char *file){
	struct stat fileStat;
	char path[400];
	buildPath(directory,file,path);
	if(stat(path,&fileStat)==0 && (S_ISREG(fileStat.st_mode))){
		return true;
	}
	return false;
}

//returns true if "directory" is a valid directory
bool isDirectory(char *directory){
	struct stat file;
	if(stat(directory,&file)==0 && (S_ISDIR(file.st_mode))){
		printf("Directory exists: %s\n",directory);
		return true;
	}
	
	printf("Error: Directory %s does not exist\n",directory);
	return false;
}

//goes through a given directory and adds files in the directory to fileList
void traverseDir(char *directory, char* fileList[]){
	DIR *dirPath = opendir(directory);
	struct dirent *dp;
	bool file;
	int i=0;
	while((dp = readdir(dirPath)) != NULL){
		if(isFile(directory,dp->d_name)){
			fileList[i]=dp->d_name;
			i++;
		}
	}
}


//prints array
void print(char* a[]){
	int i=0;
	while(a[i]!=0){
		printf("%s\n",a[i]);
		i++;
	}
}

//adds file that is in directoryA to directoryB
void addToDirectory(char *directoryA, char *directoryB, char *file){
	char pathA[400] ;
	buildPath(directoryA,file,pathA);
	char pathB[400] ;
   buildPath(directoryB,file,pathB);
	FILE *input;
	input = fopen(pathA,"r");

	FILE *output;
	output = fopen(pathB,"w");
	char line[100];

	int character;
	while((character=fgetc(input))!=EOF){
		fputc(character,output);
	}

	fclose(input);
	fclose(output);
}

//checks which file is the last updated and then replaces the outdated file
//with the most up to date one. So, if fileA was edited most recently, then
//fileB will be replaced by fileA
void checkLastUpdated(char dirA[], char dirB[], char fileA[], char fileB[]){
	char pathA[400];
	char pathB[400];
	buildPath(dirA,fileA,pathA);
	buildPath(dirB,fileB,pathB);

	struct stat fileStat;
	time_t t1,t2 = time(0);
   if(stat(pathA,&fileStat)==0){
		t1 = fileStat.st_mtime;
	}

	if(stat(pathB,&fileStat)==0){
		t2 = fileStat.st_mtime;
	}

	double seconds = t1-t2;
	if(seconds>0){
		printf("%s was most recently modified. Updating in %s\n",pathA,pathB);
		addToDirectory(dirA,dirB,fileA);
	}
	else{
      printf("%s was most recently modified. Updating in %s\n",pathB,pathA);	
		addToDirectory(dirB,dirA,fileA);
	}
}

//syncs the two directories by either adding files from A into B or
//replacing with the most recent file if a file is in both directories
void insertFromAtoB(char *directoryA, char *directoryB,  char* listA[],
		char* listB[]){
	int i=0;
	int j=0;
	bool elementInA=false;
	while(listA[i]!=0){
		while(listB[j]!=0){
			if(strcmp(listA[i],listB[j])==0){
				checkLastUpdated(directoryA,directoryB,listA[i],listB[j]);
				elementInA=true;
			}
			j++;
		}
		if(!elementInA){
		   printf("Adding %s to directory %s\n",listA[i],directoryB);
			addToDirectory(directoryA, directoryB, listA[i]);
			elementInA=false;
		}
		j=0;
		i++;
	}
}

//if a file is in B but not A, it will remove the file from B
void removeFromBnotInA(char *dirA, char *dirB, char* listA[], char* listB[]){
   int i=0;
   int j=0;
   bool elementInA=false;
   while(listB[i]!=0){
		while(listA[j]!=0){
			if(strcmp(listB[i],listA[j])==0){
				elementInA=true;
			}
			j++;
		}

      if(!elementInA){
			printf("Removing %s from directory %s\n",listB[i],dirB);
			char path[400];
			buildPath(dirB,listB[i],path);
			remove(path);
		}
		elementInA=false;
		j=0;
		i++;
	}
}


int main(int argc,char *argv[]){

	int flags=0;

	if(argc < 3){
		printf("Error: Must give two directories as arguments\n");
		return 1;
	}

	char dirA[100];
	strcpy(dirA,argv[1]);
	char dirB[100];
	strcpy(dirB,argv[2]);
	if(!isDirectory(dirA) || !isDirectory(dirB)){
		return 1;
	}
	printf("\n");
	char *fileList1[100]={0};
	char *fileList2[100]={0};
	traverseDir(dirA,fileList1);
//	print(fileList1);
	traverseDir(dirB,fileList2);
	//print(fileList2);
	insertFromAtoB(dirA,dirB,fileList1,fileList2);
	printf("\n");
	removeFromBnotInA(dirA,dirB,fileList1,fileList2);
}
