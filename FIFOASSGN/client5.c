#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include <ctype.h>
#include<sys/poll.h>
#include<string.h>

#define client5 "./client5"
#define serverpipe "./serverpipe"

int server_fd,read_fd;

void request_server()
{
  server_fd=open(serverpipe,O_WRONLY);
  write(server_fd,client5,strlen(client5)+1);
  close(server_fd);
}

void write_to_server(int sig)
{
  printf("write a message::");
  server_fd=open(serverpipe,O_WRONLY);
  char arr[80];fgets(arr,80,stdin);
  write(server_fd,arr,strlen(arr)+1);
  close(server_fd);
}

int main()
{
  mkfifo(serverpipe,0666);
  mkfifo(client5,0666);

  request_server();
  char buf[80];
  while(1)
  {
    signal(SIGINT,write_to_server);

    read_fd=open(client5,O_RDONLY);
    if(read(read_fd,buf,80)>0)
      printf("received: %s",buf);
    close(read_fd);
  }
}
