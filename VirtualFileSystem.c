#define _CRT_SECURE_NO_WARNINGS
#define MAXINODE 50
#define READ 0
#define write 1
#define MAXFILESIZE 1024
#define REGULAR 1
#define SPECIAL 2
#define START 0
#define CURRENT 1
#define END 2

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<io.h>
//#include<unistd.h>
typedef struct superblock
{
  int TotalInodes;  
  int FreeInodes;
}SUPERBLOCK,*SUPERBLOCK;

typedef struct inode;
{
  char filename[50];
  char inodenumber;
  int filesize;
  int fileactualsize;
  int filetype;
  char *buffer;
  int linkcount;
  int referencecount;
  int permission;
  struct inode *next;        /// pointer to next inode
}INODE,*PINODE,**PPINODE;

typedef struct filetable
{
  int readoffset;
  int writeoffset;
  int count;
  int mode;
  PINODE ptrinode;     /// ptr to inode 
}FILETABLE,*PFILETABLE;

typedef struct ufdt
{
  PFILETABLE ptrfiletable;       // array of pointer to file table
}UFDT;
 
UFDT UFDTarr[50];     /// object of the structure

SUPERBLOCK SUPERBLOCKobj;

PINODE head=NULL;


////FUNCTIONS /////////////////

/* 
   MAN COMMAND
   Function used to display Information about the command entered
   format: man command_name(the command whose info is required")
*/ 
void man(char *name)
{c
  if(name==NULL)
    return ;
  if(_stricmp(name,"create")==0)
  {
    printf("Description : Use to create new regular file \n");
    printf("Usage: create File_Name Permission \n"); 
  }
  else if(_stricmp(name,"read")==0)
  {
    printf("Description : Use to read data from regular file \n");
    printf("Usage: read File_name NO_of_Bytes_To_Read \n");    
  }
  else if(_stricmp(name,"write")==0)
  {
    printf("Description : Use to write data into regular file \n");
    printf("Usage: write File_name \n After this Enter the DATA that u want o write \n");    
  }
  else if(_stricmp(name,"ls")==0)
  {
    printf("Description : Use to lsit all the INFORMATION OF file \n");
    printf("Usage: ls \n");    
  }
  else if(_stricmp(naem,"stat")==0)
  {
    printf("Description : Use to display information of file \n");
    printf("Usage: stat File_name\n");    
  }
  else if(_stricmp(naem,"fstat")==0)
  {
    printf("Description : Use to display information of file \n");
    printf("Usage: stat File_Discriptor\n");    
  }
  else if(_stricmp(naem,"truncate")==0)
  {
    printf("Description : Use to remove data from the file \n");
    printf("Usage: truncate File_name \n");    
  }
  else if(_stricmp(naem,"open")==0)
  {
    printf("Description : Use to open existing file \n");
    printf("Usage: open File_name Mode \n");    
  }
  else if(_stricmp(naem,"close")==0)
  {
    printf("Description : Use to Close all opened files \n");
    printf("Usage: close File_name \n");    
  }
  else if(_stricmp(naem,"closeall")==0)
  {
    printf("Description : Use to Close all opened files\n);
    printf("Usage: closeall \n");    
  }
  else if(_stricmp(naem,"lseek")==0)
  {
    printf("Description : Use to change File offset \n");
    printf("Usage: lseek File_name ChangeINOffset StartPoint \n");    
  }
  else if(_stricmp(naem,"rm")==0)
  {
    printf("Description : Use to delete the file \n");
    printf("Usage: rm File_name \n");    
  }
  else
  {
    printf("ERROR: No manual Entry Available \n");
  }

} // end of man 

/*
  displayhelp() : Will dispay information about the all the commands in our VFS
*/
void displayhelp()
{
  printf("ls       : To LIST out all Files \n");
  printf("clear    : To clear console \n");
  printf("open     : To open the File \n");
  printf("close    : To close the File \n);
  printf("closeall : To close all opened files \n"); 
  printf("read     : To Read the contents from File \n");
  printf("write    : To Write the contents in the File \n);
  printf{"exit     : To terminate the File system \n");
  printf("stat     : To Display information of files using name \n);
  printf("fstat    : To Display Information of File using name \n");
  printf("truncate : To Remove all data form File \n"); 
  printf("rm       : To Delete the File \n");
}

/*
  getFDfrom name(char *name) : Used to get the File Discriptor by entering the name of the File 
  reurn value : fd(int)
*/
int getFDfromname(char *name)
{
  int i=0;  
  while(i<50)
  {
     if(UFDTarr[i].ptrfiletable != NULL)
     {
        if(_stricmp((UFDT[i].ptrfiletable->ptrinode->filename),name)==0)     /// input name is comapared with the name in the inode
          break;
     }
     i++;
  }
  if(i==50)
    return -1;  /// if failure returns -1
  else 
    return i;  /// return fd 
}

/*
  get_inode(char *name) :  use to get the required inode 
  return value : the address of the inode which we want 
*/
 
PINODE get_inode(char *name)
{
  PINODE temp=head; 
  if(name==NULL )
    return NULL; 
  while(temp!=NULL)
  {
    if(strcmp(naem,temp->filename)==0)
      break;
    temp->next;
  }
  return temp;
}


/*
   createDILB() : this function creates inode for the newly created file
*/

/// i=?

void createDILB()
{
  PINODE newn=NULL;
  PINODE temp=head;
  int i=0;

  while(i<=MAXINODE)
  {
    newn=(PINODE)malloc(sizeof(INODE)); 
    newn->linkcount=0;
    newn->referencecount =0;
    newn->filetype=0;
    newn->filesize=0;
    newn->next=NULL;
    newn->inodenumber=i;
    if(temp==NULL)
    {
      head=newn;
      temp=head;
    }
    else
    {
      temp->next=newn;
      temp=temp->next;
    }
    i++;
  }
}
    
/*
  initializesuperblock() - initialize the free and total inodes intitally (total inodes=50)
*/

void initializesuperblock()
{
  int i=0;
  while(i<50)
  {
    UFDTarr[i].ptrfiletable=NULL;
    i++;
  }
   
  SUPERBLOCKobj.totalinodes=MAXINODE;
  SUPERBLOCKobj.freeinode=MAXINODE;  
}


/*
   createfile() - 
   reutrn value - return the fd 
*/
int createfile(char *name,int permission )
{
  int i=0;
  PINODE temp=head;
  if((name==NULL) || (permission == 0 ) || (permission >3))
     return -1;
  if(SUPERBLOCKobj.freeinode ==0)
     return -2;
  if(get_inode(name)!=NULL)
     return -3;
  
 (SUPERBLOCKobj.freeinode)--;
 
  while(temp!=NULL)
  {
    if(temp->filetype==0)
       break;
    temp=temp->next;
  }
 
  while(i<50)
  {
    if(UFDTarr[i].ptrfiletable == NULL )
        break;
    i++;
  }
  
  UFDTarr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));
  if(UFDTarr[i].ptrfiletable==NULL)
     return -4;
  
  UFDTarr[i].ptrfiletable->count=1;
  UFDTarr[i].ptrfiletable->mode=permission;
  UFDTarr[i].ptrfiletable->readoffset=0;
  UFDTarr[i].ptrfiletable->writeoffset=0;

  UFDTarr[i].ptrfiletable->ptrinode= temp;

  strcpy_s(UFDTarr[i].ptrfiltable->ptrinode->filename,name);

  
  UFDTarr[i].ptrfiletable->ptrinode->filetype = REGULAR ;
  UFDTarr[i].ptrfiletable->ptrinode->referencecount =1 ;
  UFDTarr[i].ptrfiletable->ptrinode->linkcount = 1;
  UFDTarr[i].ptrfiletable->ptrinode->filesize = MAXFILESIZE ;
  UFDTarr[i].ptrfiletable->ptrinode->fileactualsize = 0 ;
  UFDTarr[i].ptrfiletable->ptrinode->permission = permission ;
  UFDTarr[i].ptrfiletable->ptrinode->buffer = (char *)malloc(MAXFILESIZE) ;

  memset(UFDTarr[i].ptrfiletable->ptrinode->buffer,0,1024) ;
  
  return i;   

}


/*
  rm_file(char* name) - decrease the link count by 1 ..........if link count becomes 0 the file gets deleted
*/

void  rm_file(char *name )
{
  int fd=0;
  fd =getFDfromname(name);
  if(fd==-1)
      return -1;

  (UFDTarr[fd].ptrfiletable->ptrinode->linkcount)--;
  
  if(UFDTarr[fd].ptrfiletable->prtinode->linkcount==0)
  {
    UFDTarr[fd].ptrfiletable->prtinode->filetype=0;
    free(UFDTarr[fd].ptrfiletable);
  }
  
  UFDTarr[fd].ptrfiletable=NULL;
  (SUPERBLOCKobj.freeinode)++;
}

int readfile(int fd,char *arr,int isize)
{
  int read_size=0;
  
  if(UFDTarr[fd].ptrfiletable==NULL)
     return -1;
  if(UFDTarr[fd].ptrfiletable->mode != READ && UFDTarr[fd].ptrfiletable->mode != READ+WRITE)
     return -2;
  if((UFDTarr[fd].ptrfiletable->ptrinode->permission != READ ) && 
     (UFDTarr[fd].ptrfiletable->ptrinode->permission != READ+WRITE))
     return -2;
 
  if((UFDTarr[fd].ptrfiletable->readoffset == (UFDTarr[fd].ptrfiletable->ptrinode->fileactualsize)
     return -3;    /// end of file

  if((UFDTarr[fd].ptrfiletable->ptrinode->filetype != REGULAR)
     return -4;     
  
  read_size= (UFDTarr[fd].ptrfiletable->ptrinode->fileactualsize) - (UFDTarr[fd].ptrfiletable->readoffset); //remaining bytes i have read
   
  /// if file conatins less count than that of user wants to read 
  if(read_size < isize )  
  {  
      strncpy(arr,(UFDTarr[fd].ptrfiletable->ptrinode->buffer) + (UFDTarr[fd].ptrfiletable->readoffset),read_size);
    
      /// changing the read offset after reading the file 
      UFDTarr[fd].ptrfiletable->readoffset = UFDTarr[fd].ptrfiletable->ptrinode->readoffset + read_size ;
      
  }
  else     //// isize
  {
      strncpy(arr,(UFDTarr[fd].ptrfiletable->ptrinode->buffer) + (UFDTarr[fd].ptrfiletable->readoffset),isize);
    
      /// changing the read offset after reading the file 
      UFDTarr[fd].ptrfiletable->readoffset = UFDTarr[fd].ptrfiletable->ptrinode->readoffset + isize ;
      
  }
  return isize;
}


/* 
  writefile()-

*/

int writefile(int fd,char *arr int isize)
{
  /// not open in write mode
  if(((UFDTarr[fd].ptrfiletable ->mode)!=WRITE) &&  ((UFDTarr[fd].ptrfiletable->mode) != READ+WRITE ))
     return -1;

  //// no permission(inode content)
  if( ((UFDTarr[fd].ptrfiletable->ptrinode->permission )!=WRITE) && ((UFDTarr[fd].ptrfiletable->ptrinode->permission)!=READ+WRITE) ) 
     return -1;

  // file is full 
  if((UFDTarr[fd].ptrfiletable->writeoffset) == MAXFILESIZE )
     return -2;
  
  /// it is not a regular file
  if(((UFDTarr[fd].ptrfiletable->ptrinode->filetype != REGULAR)
     return -3;
 
  /// write the content into the file 
  strncpy((UFDTarr[fd].ptrfiletable->ptrinode->buffer ) + (UFDTarr[fd].ptrfiletable->writeoffset),arr,isize);
  
  /// incease the writeoffset 
  (UFDTarr[fd].ptrfiletable->writeoffset)=(UFDTarr[fd].ptrfiletable->writeoffset) + isize;
  
  /// increace the file size
  (UFDTarr[fd].ptrfiletable->ptrinode->fileactualsize)=(UFDTarr[fd].ptrfiletable->ptrinode->fileactualsize) + isize;

  return isize;

}

/*
  
*/
int openfile(char *name,int mode)
{
  int i=0;
  PINODE temp =NULL;
  
  if(name==NULL) || mode<=0)
    return -1;
  
  temp=get_inode(name);
  if(temp==NULL)
    return -2;
  if(temp->permission < mode)
    return -3;
  
  while(i<50)
  {
    if(UFDTarr[i].ptrfiletable == NULL)
      break;
    i++;
  }

  UFDTarr[i[.ptrfilrable =(PTRFILETABLE)malloc(sizeof(FILETABLE));
  if(UFDTarr[i].ptrfiltable == NULL)
    return -1;
  UFDTarr[i].ptrfiltable->count=1;
  UFDTarr[i].ptrfiltable->mode = mode;
  
  if(mode==READ+WRITE)
  {
     UFDTarr[i].ptrfiltable->readoffset=0;
     UFDTarr[i].ptrfiltable->writeoffset=0;
  }
  else if(mode==READ)
  {
    UFDTarr[i].ptrfiltable->readoffset=0;
  }
  else if(mode==WRITE)
  {
    UFDTarr[i].ptrfiltable->writeoffset=0;
  }
  
  UFDTarr[i].ptrfiltable->ptrinode=temp;
  (UFDTarr[i].ptrfiltable->ptrinode->referencecount)++;
  return i;
 
}


/*

*/
void closefilebyFD(int fd)
{
  UFDTarr[fd].ptrfiltable->readoffset=0;
  UFDTarr[fd].ptrfiltable->writeoffset=0;
  (UFDTarr[fd].ptrfiltable->ptrinode->referencecount)--;
  
}


/*

*/
int closefilebyname(char *name)
{
  int i=0;
  i=getFDfromname(name);
  if(i==-1)
    return -1;
   
  UFDTarr[i].ptrfiltable->readoffset=0;
  UFDTarr[i].ptrfiltable->writeoffset=0;
  (UFDTarr[i].ptrfiltable->ptrinode->referencecount)--;
 
  return 0;
}


/*

*/
int closeallfile()
{
   int i=0;
   while(i<50);
   {
     if(UFDTarr[i].ptrfiltable != NULL)
     {
        UFDTarr[i].ptrfiltable->readoffset=0;
        UFDTarr[i].ptrfiltable->writeoffset=0;
        (UFDTarr[i].ptrfiltable->ptrinode->referencecount)--;
     }
     i++;
   }
}


/*

*/
int lseekfile(int fd,int size,int from)
{
  if((fd<0) || (fd>3))
    return -1;
  if(UFDTarr[fd].ptrfiltable == NULL )
    return -2;
  
  if((UFDTarr[fd].ptrfiltable->mode == READ || (UFDTarr[fd].ptrfiltable-mode == READ+WRITE) )
  {
     if(from==CURRENT)
     {
       if( ((UFDTarr[fd].ptrfiltable->readoffset)+size) > (UFDTarr[fd].ptrfiltable->ptrinode->fileactualsize))
       {  
         return -1;
       }
       if( ((UFDTarr[fd].ptrfiltable->readoffset)+size) < 0 )
         return -1;
        
       (UFDTarr[fd].ptrfiltable->readoffset) = (UFDTarr[fd].ptrfiltable->readoffset) + size;
     }
     else if(form==START)
     {
       if(size > (UFDTarr[fd].ptrfiltable->ptrinode->fileactualsize) )
          return -1;
       if(size<0)
         (UFDTarr[fd].ptrfiltable->readoffset) = size;
     }
     else if(form==END)
     {
       if( (UFDTarr[fd].ptrfiltable->ptrinode->fileactualsize) +size > MAXFILESIZE)
          return -1;
       if((UFDTarr[fd].ptrfiltable->readoffset)+size) <0 )
          return -1;
       
       (UFDTarr[fd].ptrfiltable->readoffset) = (UFDTarr[fd].ptrfiltable->ptrinode->fileactualsize) + size;
     }
  }  ///upper if
   
 // else if()
  //{ 
    

 // }
  

/*

*/
void is_file()
{
  PINODE temp=head;
  
  if(SUPERBLOCKobj.freeinode == MAXINODE)
  {
     printf("ERROR : There is no files \n");
     return; 
  }
 
  printf("\nFile Name \t Inode numer \t File size \t Link count \n);
  printf("------------------------------------------------------------\n");
  while(temp!=NULL)
  {
    if(temp->filetype!=0)
    {
       printf("%s\t\t%d\t\t%d\n",temp->filename,temp->inodenumber,temp->actualfilesize,temp->linkcount);
    }
    temp=temp-<next;
  }
  printf("-------------------------------------------------------------------------\n");

}


/*

*/

int fstat_file(int fd)
{
  PINODE temp=head;
  if(fd<0)
    return -1;
  if(((UFDTarr[fd].ptrfiltable == NULL )
    return -2;
  
  temp=UFDTarr[fd].ptrfiltable->ptrinode;
 
  printf("\n ------------statistical information about file --------\n);
  printf("File name : %s\n ",temp->filename);
  print("INODE number %d\n",temp->inodenumber);
  print("filesize %d\n",temp->filesize);
  print("actual file size %d\n",temp->fileactualsize);
  print("link count %d\n",temp->linkcount);
  print("refernece count %d\n",temp->referencecount);

  if(temp->permission ==1)
    print("file permssion : READ only \n);
  else if(temp->permission ==2)
    print("file permssion : WRITE only \n);
  else if(temp->permission ==3)
    print("file permssion : READ and WRITE \n);        
  printf("-------------------------------------------------\n\n");
 
  return 0;

}


/*

*/

int stat_file(char *name)
{
  PINODE temp=head;
  
  if(name==NULL)
    return -1;
  while(temp!=NULL)
  {
    if(strcmp(name,temp->filename) == 00
      break;
    temp=temp->next;
  }
  
  if(temp==NULL)
    return -2;
  
   printf("\n ------------statistical information about file --------\n);
  printf("File name : %s\n ",temp->filename);
  print("INODE number %d\n",temp->inodenumber);
  print("filesize %d\n",temp->filesize);
  print("actual file size %d\n",temp->fileactualsize);
  print("link count %d\n",temp->linkcount);
  print("reference count %d\n",temp->referencecount);

  if(temp->permission ==1)
    print("file permssion : READ only \n);
  else if(temp->permission ==2)
    print("file permssion : WRITE only \n);
  else if(temp->permission ==3)
    print("file permssion : READ and WRITE \n);        
  printf("-------------------------------------------------\n\n");
 
  return 0;
}


/*

*/
int truncate_file(char *name)
{
  int fd=getFDfromname(name);
  if(fd==-1)
   return -1; 
  memset(UFDTarr[fd].ptrfiletable->ptrinode->buffer,0,1024);
  UFDTarr[fd].ptrfiletable->readoffset=0;
  UFDTarr[fd].ptrfiletable->writeoffset=0;
  UFDTarr[fd].ptrfiletable->ptrinode-.fileactualsize=0;

}




int main()
{
  char *ptr=NULL;
  int ret=0,fd=0,count=0;
  char command[4][80], str[80],arr[1024];
  
  initializesuperblock();
  createDILB();
  
  while(1);
  {
    flush(stdin);
    strcpy_s(str,"");
    printf("NIKHIL VFS :> ");
    fgets(str,80,stdin);
    
    count=sscanf(str,"%s%s%s%s",command[0],command[1],command[2],command[3]);
    
    if(count==1)
    {
      if(_stricmp(command[0],"ls")==0)
      {
        is_file();  
      }
      else if(_stricmp(command[0],"closeall")==0)
      {
        closeallfile();
        printf("All Files are closed Successfully\n);
        continue;
      }
      else if(_stricmp(command[0],"clear")==0)
      {
        system("cls");
        continue;
      }
      else if(_stricmp(command[0],"help")==0)
      {
        displayhelp();
        continue;
      }
      else if(_stricmp(command[0],"exit")==0)
      {
        printf("Terminatin the Nikhil Virual File system");
        break;
      }
      else
      {
        printf("\nERROR: Command not found !!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
        continue;
      }
    }//end of count==1

    else if(count==2)
    {
     if(_stricmp(commnand[0],"stat")==0)
     { 
        ret=stat_file(command[1]);
        if(ret==-1) 
          printf("ERROR: Incorrect parameters \n");
        if(ret==-2)
          printf("ERROR: There is no such File\n");
        continue;
     }
     else if(_stricmp(command[0],"fsata")==0)
     {
       ret=fsa_file(atoi(command[1]));
        if(ret==-1) 
          printf("ERROR: Incorrect parameters \n");
        if(ret==-2)
          printf("ERROR: There is no such File\n");
        continue;
     }
     else if(_stricmp(command[0],"rm")==0)
     {  
       ret=rm_file(command[1]);
       if(ret==-1)
         printf("Ther is no such File");
       continue;
     }
     else if(_stricmp(command[0],"man")==0)
     {
       man(command[1]);
     }
     else if(_stricmp(command[0],"write")==0)  
     { 
       fd=getFDfromname(command[1]);
       if(fd==-1)
       {
          printf("error:Incorrect parameter \n");
          continue;
       }
       prinf("Enter the data :\n");
       scanf("%[^\n]",arr);
       
       ret=strlen(arr); 
       if(ret==0)
       {
          printf("Error:Incorretc parameter \n");
          continue;
       }
       
       ret=writefile(fd,arr,ret);   /// write the data into the file
       if(ret==-1)
          printf("ERROR: Permission denied\n");
       if(ret==-2)
          printf("ERROR: There is no sufficient memory to write \n");
       if(ret==-3) 
          printf("ERROR: It is not a regular file \n);
     }
     else if(_stricmp(command[0],"truncate")==0)       
     {
       ret=truncate_file(command[1]);
       if(ret==-1)
          printf("ERROR: Incorrect Parameter \n");
     }
     else
     {
        printf("\nERROR: Command Not Found !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11\n");
        continue;
     }
    }// count==2
    else if(count==3)
    {
      if(_stricmp(command[0],"create")==0)
      {
        ret=createfile(command[1],atio(command[2]));
        if(ret>=0)
           printf("FILE IS SUCCESSFULL CREATED WITH File descriptor: %d\n",ret);
        if(ret==-1)
           printf("ERROR: Incorret Parameters\n");
        if(ret==-2)
           printf("ERROR: Ther is no inodes\n");
        if(ret==-3)
           printf("ERROR: File Already Exist \n");
        if(ret==-4)
           printf("ERROR: Memry Allocation Failure\n");
        continue;
      }
      else if(_stricmp(command[0],"open")==0)
      {
        ret=openfile(command[1],atio(command[2]));
        if(ret>=0)
           printf("FILE IS SUCCESSFULL CREATED WITH File descriptor: %d\n",ret);
        if(ret==-1)
           printf("ERROR: Incorret Parameters\n");
        if(ret==-2)
           printf("ERROR: File not present\n");
        if(ret==-3)
           printf("ERROR: Permission Denied \n");
        continue;
      }
      else if(_stricmp(command[0],"read")==0)
      {
        fd=getFDfromname(command[1]);
        if(fd==-1)
        {
           prinf("Error: Incorrect parameter \n");
           continue;
        }
      
        ptr=(char *)malloc(sizeof(atoi(command[2])))+1);
        if(ptr==NULL)
        {
           printf("ERROR: Memory Allocation Failure ");
           continue;
        }
      
        ret=readfile(fd,ptr,atoi(command[2]); 

        if(ret==-1)
           printf("ERROR: File Not Exising\n");
        if(ret==-2)
          printf("ERROR: Permission denied\n");
        if(ret==-3)
          printf("ERROR: Reached at the End of the File  \n");
        if(ret==-4)
          printf("ERROR: It is not the Regular File\n");
        if(ret==0)
          printf("ERROR: File Empty \n");
        if(ret>=0)
        {
          _write(2,ptr,ret);
        }
        continue;
      }
      else
      {
        printf("ERROR : Command Not Found !!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        continue;
      } 
    }//count==3
 
    else if(count==4)
    {
      if(_stricmp(command[0],"lseek"==0)
      {
        fd=getFDfromname(command[1]);
        if(fd==-1)
        {
           printf("Error: Incorrect Parameter \n");
           continue;
        }
      
        ret=lseekfile(fd,atio(command[1]),atio(command[3]));

        if(ret==-1)
        {
           printf("ERRRO: Unable to Perform lseek\n");
        }
      }
      else
      {
        printf("\nERROR: Command NOt Found !!!!!!!!!!!\n");
        continue;
      }
    }
    else
    {
        printf("\nERROR : Command Not Found !!!!!!!!!\n ");
        continue;
    }
  }///while
  
  return 0;

}///main
