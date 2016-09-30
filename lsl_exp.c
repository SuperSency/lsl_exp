#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>

#define _S_IFMT 0170000
#define _S_IFDIR 0040000
#define _S_IFCHR 0020000
#define _S_IFBLK 0060000
#define _S_IFREG 0100000
#define _S_IFIFO 0010000
#define _S_IFLNK 0120000
#define _S_IFSOCK 0140000

void printID(const struct stat info)
{
  struct passwd *pwd;
  struct group *grp;
  pwd=getpwuid(info.st_gid);
  grp=getgrgid(info.st_gid);
  printf(" %5s",pwd->pw_name);
  printf(" %5s",grp->gr_name);
}

void printPer(struct stat buff)
{
   unsigned int i = 0,mask=0700;
   static char *perm[]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};
   mask=0700;
   if((buff.st_mode&_S_IFMT)==_S_IFDIR){
     printf("d");
   }else if((buff.st_mode&_S_IFMT)==_S_IFCHR){
     printf("c");
   }else if((buff.st_mode&_S_IFMT)==_S_IFBLK){
     printf("b");
   }else if((buff.st_mode&_S_IFMT)==_S_IFREG){
     printf("-");
   }else if((buff.st_mode&_S_IFMT)==_S_IFIFO){
     printf("p");
   }else if((buff.st_mode&_S_IFMT)==_S_IFLNK){
     printf("l");
   }else if((buff.st_mode&_S_IFMT)==_S_IFSOCK){
     printf("s");
   }      
   for(i=3;i;--i)
   {
     printf("%3s",perm[(buff.st_mode&mask)>>(i-1)*3]);
     mask>>=3;
   }
}

void printTime(const struct stat info)
{
   char time[30];
   memset(time,'\0',30);
   strftime(time,sizeof(time),"%b %d %H:%M",localtime(&info.st_mtime));
   printf("%s",time);
}
int main(int argc,char *argv[])
{
   DIR *dir;
   struct dirent *ptr;
   struct stat buff;
   char *filename;
   char *dirname=".";
   if(argc!=1)
   {
     dirname=argv[1];
   }
   if((dir=opendir(dirname))==NULL)
   {
        perror("OPEN");
        exit(EXIT_FAILURE);
   }
   while((ptr=readdir(dir))!=NULL)
   {
       sprintf(filename,"%s/%s",dirname,ptr->d_name);
       if((stat(filename,&buff))==-1)
       {
          perror("FIALED");
          exit(EXIT_FAILURE);
       }else if(ptr->d_name[0]!='.') 
       {
         printPer(buff);
         printf(" %d",buff.st_nlink);
         printID(buff);
         printf(" %d ",buff.st_size);
         printTime(buff);
         printf(" %s\n",ptr->d_name);
      }
    }
   return 0;   
}
