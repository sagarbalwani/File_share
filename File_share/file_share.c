/* tcpclient.c */
//#include <curl/curl.h>
//#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include<fcntl.h>
#include <dirent.h>
int main(int argc,char *argv[])

{
    int pid,sock,clientsock;
    int  bytes_recieved; 
                char *check[1024],*check1[1024];
    int uflag=0,udpflag;
    char send_data[1024], clientsend_data[1024],recv_data[1024],clientrecv_data[1024];
    printf("enter 0 for UDP 1 otherwise");
    scanf("%d",&udpflag);
    getchar();
    if(argc==2) 
    {
        int connected, bytes_recieved , true = 1,addr_len,bytes_read;
        char send_data [1024] , recv_data[1024];
	
        struct sockaddr_in server_addr,client_addr;
        int sin_size;
	if (udpflag==1)
	{
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        {
            perror("Socket");
            exit(1);
        }
	}
	else
	{

        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
        {
            perror("Socket");
            exit(1);
        }
	}
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[1]));
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                == -1) {
            perror("Unable to bind");
            exit(1);
        }
	if (udpflag==1)
	{
        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }
        printf("\nTCPServer Waiting for client on port %s\n",argv[1]);
        fflush(stdout);
	}
	else
	{
		addr_len = sizeof(struct sockaddr);
		printf("\nUDPServer Waiting for client on port 5000\n");
			fflush(stdout);
		bytes_recieved = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr,&addr_len);
                recv_data[bytes_recieved] = '\0';
	}
	if (udpflag==1)
	{
      		 sin_size = sizeof(struct sockaddr_in);
        	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
        printf("\n I got a connection from (%s, %d)\n",
                inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
   	}
		char *command[1024];
        pid=fork();

            if (pid>0)
            {
                while(1)
                {
                    //                  printf("\n SEND (q or Q to quit) : ");
                    int i=0;
                    gets(send_data);
			printf("got data to send to client %s\n",send_data);

                    char copy[1024];
                    strcpy(copy,send_data);
                    char *str=strtok(copy,"  ");
                    while(str!=NULL)
                    {
                        check1[i]=str;
                        i++;
                        str=strtok(NULL," " );
                    }
                    check1[i]='\0';
                    if(i==2)
                    {
                        FILE *fpp=fopen("temp1.txt","w");
                        fprintf(fpp,"%s",check1[1]);
                        fclose(fpp);
                    }
                    if (strcasecmp(send_data , "q") == 0 || strcasecmp(send_data , "Q") == 0)
                    {
			if(udpflag==1)
			{
				printf("sending to client throgh tcp %s",send_data);
                        send(connected, send_data,strlen(send_data), 0); 
                        close(connected);
			}
			else
			{			sendto(sock,send_data, strlen(send_data), 0,
						(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
			}
			break;
                    }
                    /*else if(uflag==1 && strcasecmp(send_data ,"fileupload allow")==0)
                    {
                        
                    }*/
                    else
                    {
                       // printf("file = %s\n",send_data);
			    if(udpflag==1)
			    {
				    printf("sending to client throgh tcp %s\n",send_data);
                        	send(connected, send_data,strlen(send_data), 0);  
			    }
			    else
			    {  
				    printf("yepee sending %s\n",send_data);
				    sendto(sock, send_data, strlen(send_data), 0,
						    (struct sockaddr *)&client_addr, sizeof(struct sockaddr));

				    printf("yepee sending %s\n",send_data);
			    }
			    //                       printf("%d\n",sendd);
                    }
                }
            }
            else if(pid==0)
            {
                while(1)
                {
                    if (udpflag==1)
		    {
			bytes_recieved = recv(connected,recv_data,1024,0);
//
                    recv_data[bytes_recieved] = '\0';
		    }
		    else
		    {
			bytes_recieved = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr,&addr_len);
                    recv_data[bytes_recieved] = '\0';
		    printf("recv from the client %s\n",recv_data);
		    }
                    if (strcasecmp(recv_data , "q") == 0 || strcasecmp(recv_data , "Q") == 0)
                    {
			    if (udpflag==1)
			    {
                      	 	 close(connected);
			    }
                       		 break;
			    
                    }

                    else
                    {
                        int i=0;
                        char serverstring[1024];
                        strcpy(serverstring,recv_data);
                        char *str=strtok(recv_data,"  ");
                        while(str!=NULL)
                        {
                            command[i]=str;
                            i++;
                            str=strtok(NULL," " );
                        } 
                        command[i]='\0';
                        	//		printf("serverstring %s\n",serverstring);
                        	//		printf("command[0&1] %s %s \n",command[0],command[1]);
                        //	if(strcasecmp(command[1],"longlist")==0)
                   //     printf("pagal hai kya ??\n");
                        if(strcasecmp(command[0],"indexget")==0)
                        {
                            int bak=dup(1);
                            //                            printf("indexget mein aaya \n");
                            if(strcasecmp(command[1],"shortlist")==0)
                            {
                                int fd;
                                fd =open("shortlist",O_RDWR | O_CREAT , S_IRWXU , S_IRWXG , S_IRWXO );
                                dup2(fd,1);
                                system("ls --full-time -ht | tail -n +2 | tr -s \" \" | cut -d \" \" -f 6-7 |tr -d \"-\" | tr -d \":\" | cut -d \".\" -f 1 | tr -d \" \" " );
                                close(fd);
                                dup2(bak,1);
                                int c=0,d=0,j=0,flag=0;
                                char stamp[100],com[100],t;
                                FILE *fp;
                                fp=fopen("shortlist","r");

                                while(1)
                                {
                                    j=0;
                                    fscanf(fp,"%c",&t);
                                    while(t!='\n')
                                    {
                                        //  printf("he he %c",t);
                                        if(t==EOF)
                                        {
                                          //  printf("t==EOF in shortlist\n");
                                            flag=1;
                                            break;
                                        }
                                        stamp[j]=t;
                                        j++;
                                        fscanf(fp,"%c",&t);
                                    }
                                    if (flag==1)
                                        break;

                                    stamp[j]='\0';
                                    if(strcasecmp(stamp,command[3])>=0)
                                        d++;
                                    if(strcasecmp(stamp,command[2])>=0)
                                        c++;

                                    else
                                    {//printf("broke in else in shortlist\n");
                                        break;}
                                }
                                sprintf(com,"ls --full-time -ht | tail -n +2 | tr -s \" \" | cut -d \" \" -f 5-7,9 | head -%d | tail -%d",c,c-d);
      			if (udpflag==1)
				dup2(connected,1);
			else 
				dup2(sock,1);
                                system(com);
                                dup2(bak,1);
                                //printf("shortlist");
                            }
                            else if(strcasecmp(command[1],"longlist")==0)
                            {
                                     printf("longlist mein aaya \n");
      			if (udpflag==1)
                                dup2(connected,1);
			else 
				dup2(sock,1);
                                system("ls -lht |tr -s \" \" | cut -d \" \" -f 5-9");
                                dup2(bak,1);
                                //    printf("longlist mein aaya \n");
                            }
                            else if(strcasecmp(command[1],"regex")==0)
                            {
                                char b[100];
      			if (udpflag==1)
                                dup2(connected,1);
			else 
				dup2(sock,1);
                                //    dup2(clisock,1);
                                sprintf(b,"ls -lht %s |tr -s \" \" | cut -d \" \" -f 5,9",command[2]);
                                system(b);
                                dup2(bak,1);
                                //dup2(stdout,1);
                            }
                            close(bak);
                            //close(connected);

                        }
                        else if(strcasecmp(command[0],"filehash")==0)
                        {   
                            int bak=dup(1);
                            if(strcasecmp(command[1],"verify")==0)
                            {   
                                char b[100];
                                int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
                                dup2(fd,1);
                                sprintf(b,"md5sum %s" ,command[2]);
                                system(b);
                                sprintf(b,"ls -l %s | tr -s \" \" | cut -d \" \" -f 6-8 ",command[2]);
                                system(b);
                                close(fd);
                                // remove(anew);
                                dup2(bak,1);
      			if (udpflag==1)
                                dup2(connected,1);
			else 
				dup2(sock,1);
                                system("cat anew | tr \"\n\" \" \"");
                                remove("anew");
                                dup2(bak,1);
                            }   
                            else if(strcasecmp(command[1],"checkall")==0)
                            {
                                DIR * d;
                                char * dir_name = ".";

                                /* Open the current directory. */
                                char b[100];
                                d = opendir (dir_name);
                                char arr[100];

                                if (! d) {
                                    fprintf (stderr, "Cannot open directory '%s': %s\n",
                                            dir_name, strerror (errno));
                                    exit (EXIT_FAILURE);
                                }
                                while (1) 
                                {
                                    struct dirent * entry;

                                    entry = readdir (d);
                                    if (! entry) {
                                        break;
                                    }
                                    int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
                                    dup2(fd,1);
                                    dup2(fd,2);
                                    sprintf(b,"md5sum %s" ,entry->d_name);
                                    system(b);
                                    FILE *fp=fopen("anew","r");
                                    fscanf(fp,"%s",arr);
                                    //printf("arr = %s\n",arr);
                                    if(strcasecmp(arr,"md5sum:")!=0)
                                    {
                                        sprintf(b,"ls -l %s | tr -s \" \" | cut -d \" \" -f 6-8 ",entry->d_name);
                                        system(b);
                                    }
                                    close(fd);
                                    // remove(anew);
                                    dup2(bak,1);
                                    // system("cat anew");
                                    if(udpflag==1)
				    dup2(connected,1);
					else 
					dup2(sock,1);
                                    FILE *fw=fopen("anew","r");
                                    fscanf(fw,"%s",arr);
                                    if(strcasecmp(arr,"md5sum:")!=0)
                                        system("cat anew | tr \"\n\" \" \"");
                                    remove("anew");
                                    dup2(bak,1); 
                                    //    printf ("%s\n", entry->d_name);
                                }
                                /* Close thev directory. */
                                if (closedir (d)) {
                                    fprintf (stderr, "Could not close '%s': %s\n",
                                            dir_name, strerror (errno));
                                    exit (EXIT_FAILURE);
                                }  
                            }
                        }
                        else if(strcasecmp(command[0],"fileupload")==0)
                        {
                            if(strcasecmp(command[1],"allow")==0)
                            {
                                char tempo[1024];
                                FILE *fp1=fopen("temp1.txt","r");
                                fscanf(fp1,"%s",tempo);
                                int j;char c[1024];
                                c[0]='#';
                                c[1]='$';
                                c[2]='#';
                                c[3]=' ';
                                for(j=0;j<strlen(tempo);j++)
                                {
                                    c[j+4]=tempo[j];
                                }
                                for(;j<45;j++)
                                {
                                    c[j+4]=' ';
                                }
                                char b;
                                //j++;
                                int i=50;
                                int flag=1;
                                FILE *fp=fopen(tempo,"rb");
                                while(fscanf(fp,"%c",&b)!=EOF)
                                {
                                    if(flag==1)
                                    {
                                        c[49]='w';
                                    }
                                    else
                                        c[49]='a';
                                    if(i==1021)
                                    {	
                                        flag=0;
                                        //printf("hello\n");
					if(udpflag==1)
                                        send(connected,c,1024,0);
					else
					{	sendto(sock,c,1024, 0,
								(struct sockaddr *)&client_addr,sizeof(struct sockaddr));
					}
                                        //     c[0]='#';
                                        //   c[1]='$';
                                        // c[2]='#';
                                        // c[3]=' ';
                                        //	printf("aaa data: %s\n",c);
                                        i=50;
                                    }
                                    c[i]=b;
                                    i++;
                                }
                                c[i]='!';
                                c[i+1]='@';
                                c[i+2]='!';


                                //if (c[i]==EOF)
                                //    printf("c[136]= %churray clen=%d\n",c[136],strlen(c));
					if(udpflag==1)
                                send(connected,c,1024,0);
					else
					{	sendto(sock,c,1024, 0,
								(struct sockaddr *)&client_addr,sizeof(struct sockaddr));
					}
                                int bak=dup(1);
                                char d[100];
                                int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
                                dup2(fd,1);
                                sprintf(d,"md5sum %s" ,tempo);
                                system(d);
                                sprintf(d,"ls -lh %s | tr -s \" \" | cut -d \" \" -f 5-8 ",tempo);
                                system(d);
                                close(fd);
                                // remove(anew);
                                dup2(bak,1);
					if(udpflag==1)
                                dup2(connected,1);
					else
                                dup2(sock,1);

                                system("cat anew | tr \"\n\" \" \"");
                                remove("anew");
                                dup2(bak,1);
                                remove("temp1.txt");
                                fclose(fp1);
                            }
                            else if(strcasecmp(command[1],"deny")==0)
                            {

                            }
                            else
                                printf("Do you want to accept ? ");
                            //       uflag=1;
                        }

                        else if(strcmp(command[0],"#$#")==0)
                        {
                            // int len=atoi(command[1]);
                            char arr[10];
                            //printf("* = %s\n",command[1]);
                            sprintf(arr,"%c",serverstring[49]);
                          //  printf("aaarr = %s\n",arr);
                            FILE *fp=fopen(command[1],arr);
                            int i=50,flag=0;
                            //printf("bytes recv= %d strlen =%d \n",bytes_recieved,strlen(serverstring));
                    while(i<1021)
                    {
                        if(serverstring[i]=='!' && serverstring[i+1]=='@' && serverstring[i+2]=='!')
                        {   flag=1;
                            break;
                        }
                        fprintf(fp,"%c",serverstring[i]);
                        i++;
                    }

                    fclose(fp);
                }


/*
                        else if(strcmp(command[0],"#$#")==0)
                        {
                            // int len=atoi(command[1]);
                            char arr[10];
                            printf("* = %s\n",command[1]);
                            sprintf(arr,"%c",serverstring[49]);
                            printf("aaarr = %s\n",arr);
                            FILE *fp=fopen("hello",arr);
                            int i=50,flag=0;
                            printf("bytes recv= %d strlen =%d \n",bytes_recieved,strlen(serverstring));
                            while(i<1021)
                            {
                                if(serverstring[i]=='!' && serverstring[i+1]=='@' && serverstring[i+2]=='!')
                                {	flag=1;
                                    break;
                                }
                                fprintf(fp,"%c",serverstring[i]);
                                i++;
                            }

                            fclose(fp);
                            }
                            */
                        else if(strcasecmp(command[0],"filedownload")==0)
            {
                int j;char c[1024];
                c[0]='#';
                            c[1]='$';
                            c[2]='#';
                            c[3]=' ';
                            for(j=0;j<strlen(command[1]);j++)
                            {
                                c[j+4]=command[1][j];
                            }
                            for(;j<45;j++)
                            {
                                c[j+4]=' ';
                            }
                            char b;
                            //j++;
                            int i=50;
                            int flag=1;
                            FILE *fp=fopen(command[1],"r");
                            while(fscanf(fp,"%c",&b)!=EOF)
                            {
                                if(flag==1)
                                {
                                    c[49]='w';
                                }
                                else
                                    c[49]='a';
                                if(i==1021)
                                {	
                                    flag=0;
                                    //printf("hello\n");
				if (udpflag==1)
                                    send(connected,c,1024,0);
				else
				{
					sendto(sock,c,1024, 0,												(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
				}
					//     c[0]='#';
                                 //   c[1]='$';
                                   // c[2]='#';
                                   // c[3]=' ';
                                    //	printf("aaa data: %s\n",c);
                                    i=50;
                                }
                                c[i]=b;
                                i++;
                            }
                            c[i]='!';
                            c[i+1]='@';
                            c[i+2]='!';


                            //if (c[i]==EOF)
                        //    printf("c[136]= %churray clen=%d\n",c[136],strlen(c));
                       if (udpflag==1)
                                    send(connected,c,1024,0);
				else
				{
					sendto(sock,c,1024, 0,												(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
				}
                            int bak=dup(1);
                            char d[100];
                            int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
                            dup2(fd,1);
                            sprintf(d,"md5sum %s" ,command[1]);
                            system(d);
                            sprintf(d,"ls -lh %s | tr -s \" \" | cut -d \" \" -f 5-8 ",command[1]);
                            system(d);
                            close(fd);
                            // remove(anew);
                            dup2(bak,1);
			    if (udpflag==1)
                            dup2(connected,1);
				else
					dup2(sock,1);
                            system("cat anew | tr \"\n\" \" \"");
                            remove("anew");
                            dup2(bak,1);

                            // printf("hi%shi",c);
                            //      printf("%c & %d",c[76],strlen(c));
                        }
                        else
                        {
                            printf("RECIEVED DATA = %s \n" ,serverstring);
                            fflush(stdout);
                            //                           close(connected);
                            //                           break;
                        }
                    }
                    fflush(stdout);
                }
                exit(0);
            } 

    }
    else if(argc==3)
    {
	int addr_len=sizeof(struct sockaddr);
        struct sockaddr_in server_addr;  
        struct hostent *host;
        host = gethostbyname(argv[1]);
        //printf("%d",atoi(argv[2]));
	if(udpflag==1)
	{
        if ((clientsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
	}
	else
	{
	
        if ((clientsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
	}

	    server_addr.sin_family = AF_INET;     
	    server_addr.sin_port = htons(atoi(argv[2]));   
	    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	    bzero(&(server_addr.sin_zero),8); 
	if (udpflag==1)
	{
	    if (connect(clientsock, (struct sockaddr *)&server_addr,
				    sizeof(struct sockaddr)) == -1) 
	    {
		    perror("Connect");
		    exit(1);
	    }
	}
	else{

					sendto(clientsock,"sadadad",1024, 0,						    					(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	}

	pid=fork();
	if (pid==0)
	{
		while(1)
		{
			if (udpflag==1)
			{
				printf("1111recieving at client side %s\n",clientrecv_data);
				bytes_recieved=recv(clientsock,clientrecv_data,1024,0);
				clientrecv_data[bytes_recieved] = '\0';
			}
			else
			{
				bytes_recieved=recvfrom(clientsock,clientrecv_data,1024,0,
						(struct sockaddr *)&server_addr, &addr_len);
				clientrecv_data[bytes_recieved] = '\0';
				printf("recieving at client side %s\n",clientrecv_data);
			}
			if (strcasecmp(clientrecv_data , "q") == 0 || strcasecmp(clientrecv_data , "Q") == 0)
			{
				if(udpflag==1)
					close(clientsock);
				break;
			}
			else
			{
				int i=0;
				char *command[100],string[1024];
				//        printf("%s\n",check[1]);
				strcpy(string,clientrecv_data);
				char *str=strtok(clientrecv_data,"  ");
				while(str!=NULL)
				{
					command[i]=str;
					i++;
					str=strtok(NULL," " );
				} 
				command[i]='\0';
				printf("commands are %s %s\n",command[0],command[1]);
				if(strcasecmp(command[0],"indexget")==0)
				{
					printf("indexget mein aaya\n");
					int bak=dup(1);
					//  printf("%d",argc);
					if(strcasecmp(command[1],"shortlist")==0)
					{
						int fd;
						fd =open("shortlist",O_RDWR | O_CREAT , S_IRWXU , S_IRWXG , S_IRWXO );
						dup2(fd,1);
						system("ls --full-time -ht | tail -n +2 | tr -s \" \" | cut -d \" \" -f 6-7 |tr -d \"-\" | tr -d \":\" | cut -d \".\" -f 1 | tr -d \" \" " );
						close(fd);
						dup2(bak,1);
						int c=0,d=0,j=0,flag=0;
						char stamp[100],com[100],t;
						FILE *fp;
						fp=fopen("shortlist","r");

						while(1)
						{
							j=0;
							fscanf(fp,"%c",&t);
							while(t!='\n')
							{
								//  printf("he he %c",t);
								if(t==EOF)
								{
									flag=1;
									break;
								}
								stamp[j]=t;
								j++;
								fscanf(fp,"%c",&t);
							}
							if (flag==1)
								break;

							stamp[j]='\0';
							if(strcasecmp(stamp,command[3])>=0)
								d++;
							if(strcasecmp(stamp,command[2])>=0)
								c++;

							else
								break;
						}
						sprintf(com,"ls --full-time -ht | tail -n +2 | tr -s \" \" | cut -d \" \" -f 5-7,9 | head -%d | tail -%d",c,c-d);
						dup2(clientsock,1);
						system(com);
						dup2(bak,1);
						//printf("shortlist");
					}
					else if(strcasecmp(command[1],"longlist")==0)
					{
					printf("longlist mein aaya\n");
						dup2(clientsock,1);
						system("ls -lht |tr -s \" \" | cut -d \" \" -f 5-9");
						dup2(bak,1);
					}
					else if(strcasecmp(command[1],"regex")==0)
					{
						char b[100];
						dup2(clientsock,1);
						//    dup2(clisock,1);
						sprintf(b,"ls -lht %s |tr -s \" \" | cut -d \" \" -f 5,9",command[2]);
						system(b);
						dup2(bak,1);
						//dup2(stdout,1);
					}
					close(bak);
					//close(clientsock);
				}
				else if(strcasecmp(command[0],"filehash")==0)
				{
					int bak=dup(1);
					if(strcasecmp(command[1],"verify")==0)
					{
						char b[100];
						int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
						dup2(fd,1);
						sprintf(b,"md5sum %s" ,command[2]);
						system(b);
						sprintf(b,"ls -l %s | tr -s \" \" | cut -d \" \" -f 6-8 ",command[2]);
						system(b);
						close(fd);
						// remove(anew);
						dup2(bak,1);
						dup2(clientsock,1);

						system("cat anew | tr \"\n\" \" \"");
						remove("anew");
						dup2(bak,1);
					}
					else if(strcasecmp(command[1],"checkall")==0)
					{
						DIR * d;
						char * dir_name = ".";

						/* Open the current directory. */
						char b[100];
						d = opendir (dir_name);
						char arr[100];

						if (! d) {
                            fprintf (stderr, "Cannot open directory '%s': %s\n",
                                    dir_name, strerror (errno));
                            exit (EXIT_FAILURE);
                        }
                        while (1) 
                        {
                            struct dirent * entry;

                            entry = readdir (d);
                            if (! entry) {
                                break;
                            }
                            int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
                            dup2(fd,1);
                            dup2(fd,2);
                            sprintf(b,"md5sum %s" ,entry->d_name);
                            system(b);
                            FILE *fp=fopen("anew","r");
                            fscanf(fp,"%s",arr);
                            if(strcasecmp(arr,"md5sum:")!=0)
                            {
                                sprintf(b,"ls -l %s | tr -s \" \" | cut -d \" \" -f 6-8 ",entry->d_name);
                                system(b);
                            }
                            close(fd);
                            dup2(bak,1);
                            dup2(clientsock,1);
                            FILE *fw=fopen("anew","r");
                            fscanf(fw,"%s",arr);
                            if(strcasecmp(arr,"md5sum:")!=0)
                                system("cat anew | tr \"\n\" \" \"");
                            remove("anew");
                            dup2(bak,1); 
                        }
                        /* Close thev directory. */
                        if (closedir (d)) {
                            fprintf (stderr, "Could not close '%s': %s\n",
                                    dir_name, strerror (errno));
                            exit (EXIT_FAILURE);
                        }  
                    }


				}
				else if(strcasecmp(command[0],"fileupload")==0)
				{
					if(strcasecmp(command[1],"allow")==0)
					{
						char tempo[1024];
						FILE *fp1=fopen("temp.txt","r");
						fscanf(fp1,"%s",tempo);
						int j;char c[1024];
						c[0]='#';
						c[1]='$';
						c[2]='#';
						c[3]=' ';
						for(j=0;j<strlen(tempo);j++)
						{
							c[j+4]=tempo[j];
						}
						for(;j<45;j++)
						{
							c[j+4]=' ';
						}
						char b;
						//j++;
						int i=50;
						int flag=1;
						FILE *fp=fopen(tempo,"rb");
						while(fscanf(fp,"%c",&b)!=EOF)
						{
							if(flag==1)
							{
								c[49]='w';
							}
							else
								c[49]='a';
							if(i==1021)
							{	
								flag=0;
								//printf("hello\n");
								if (udpflag==1)
									send(clientsock,c,1024,0);
								else
								{	
									sendto(clientsock,c,1024, 0,						    					(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
								}
								//     c[0]='#';
								//   c[1]='$';
								// c[2]='#';
								// c[3]=' ';
								//	printf("aaa data: %s\n",c);
								i=50;
							}
							c[i]=b;
							i++;
						}
						c[i]='!';
						c[i+1]='@';
						c[i+2]='!';


						//if (c[i]==EOF)
						//    printf("c[136]= %churray clen=%d\n",c[136],strlen(c));
						if (udpflag==1)
							send(clientsock,c,1024,0);
						else
						{	
							sendto(clientsock,c,1024, 0,						    					(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
						}
						int bak=dup(1);
						char d[100];
						int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
						dup2(fd,1);
						sprintf(d,"md5sum %s" ,tempo);
						system(d);
						sprintf(d,"ls -lh %s | tr -s \" \" | cut -d \" \" -f 5-8 ",tempo);
						system(d);
						close(fd);
						// remove(anew);
						dup2(bak,1);
						dup2(clientsock,1);

						system("cat anew | tr \"\n\" \" \"");
						remove("anew");
						dup2(bak,1);
						remove("temp.txt");
						fclose(fp1);
					}
					else if(strcasecmp(command[1],"deny")==0)
					{

					}
					else
						printf("Do you want to accept ? ");
				}

				/* else if(strcasecmp(command[0],"fileupload")==0)
				   {
				   if(strcasecmp(command[1],"allow")==0)
				   {
				   char buffer[1024];
				   printf("gaya\n");
				   printf("%s\n",check[1]);
				   int j;
				   for(i=0;i<1024;i++)
				   buffer[i]=' ';
				   buffer[0]='#';
				   buffer[1]='$';
				   buffer[2]='#';
				   buffer[3]=' ';
				   for(j=0;j<strlen(check[1]);j++)
				   {
				   buffer[j+4]=check[1][j];
				   }
				   int flag=1;
				   printf("%s\n",check[1]);
				   int fd=open(check[1],O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
				   int read2=read(fd,&buffer[50],971);
				   printf("%d",read2);
				   while(read(fd,&buffer[50],971)!=0)
				   {
				   printf("enter");
				   if(flag==1)
				   {
				   buffer[49]='w';
				   flag=0;
				   }
				   else
				   buffer[49]='a';
				   printf("buf = %s\n",buffer);
				   send(clientsock,buffer,strlen(buffer),0);
				   }
				//     int readbytes=read(fd,&buffer[50],971);
				//   send(clientsock,buffer,strlen(buffer),0);
				}
				else
				{
				// char array[100];
				// strcpy(array,command[1]);
				}
				}*/
				else if(strcasecmp(command[0],"filedownload")==0)
				{
					int j;char c[1024];
					c[0]='#';
					c[1]='$';
					c[2]='#';
					c[3]=' ';
					for(j=0;j<strlen(command[1]);j++)
					{
						c[j+4]=command[1][j];
					}
					for(;j<45;j++)
					{
						c[j+4]=' ';
					}
					char b;
					//j++;
					int i=50;
					int flag=1;
					FILE *fp=fopen(command[1],"r");
					while(fscanf(fp,"%c",&b)!=EOF)
					{
						if(flag==1)
						{
							c[49]='w';
						}
						else
							c[49]='a';
						if(i==1021)
						{	
							c[1021]='\0';
							flag=0;
							//printf("hello\n");
							if (udpflag==1)
								send(clientsock,c,1024,0);
							else
							{	
								sendto(clientsock,c,1024, 0,						    					(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
							}
							//     c[0]='#';
							//   c[1]='$';
							// c[2]='#';
							// c[3]=' ';
							//	printf("data data: %s\n",c);
							i=50;
						}
						c[i]=b;
						i++;
					}
					c[i]='!';
					c[i+1]='@';
					c[i+2]='!';


					//if (c[i]==EOF)
					//printf("c[136]= %churray clen=%d\n",c[136],strlen(c));
					if (udpflag==1)
						send(clientsock,c,1024,0);
					else
					{	
						sendto(clientsock,c,1024, 0,						    					(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
					}
					int bak=dup(1);
					char d[100];
					int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
					dup2(fd,1);
					sprintf(d,"md5sum %s" ,command[1]);
					system(d);
					sprintf(d,"ls -lh %s | tr -s \" \" | cut -d \" \" -f 5-8 ",command[1]);
					system(d);
					close(fd);
					// remove(anew);
					dup2(bak,1);
					dup2(clientsock,1);

					system("cat anew | tr \"\n\" \" \"");
					remove("anew");
					dup2(bak,1);

					// printf("hi%shi",c);
					//      printf("%c & %d",c[76],strlen(c));
				}



				/*    else if(strcasecmp(command[0],"filedownload")==0)
				      {
				      int j;char c[1024];
				      c[0]='#';
				      c[1]='$';
				      c[2]='#';
				      c[3]=' ';
				      for(j=0;j<strlen(command[1]);j++)
				      {
				      c[j+4]=command[1][j];
				      }
				      for(;j<45;j++)
				      {
				      c[j+4]=' ';
				      }
				      char b;
				//j++;
				int i=50;
				int flag=1;
				FILE *fp=fopen(command[1],"r");
				while(fscanf(fp,"%c",&b)!=EOF)
				{
				if(flag==1)
				{
				c[49]='w';
				}
				else
				c[49]='a';
				if(i==1021)
				{	
				flag=0;
				printf("hello\n");
				send(clientsock,c,1024,0);
				c[0]='#';
				c[1]='$';
				c[2]='#';
				c[3]=' ';
				//	printf("sent bytes: %d , data: %s\n",x,c);
				i=50;
				}
				c[i]=b;
				i++;
				}
				c[i]='!';
				c[i+1]='@';
				c[i+2]='!';


				//if (c[i]==EOF)
				printf("c[136]= %churray clen=%d\n",c[136],strlen(c));

				send(clientsock,c,1024,0);
				int bak=dup(1);
				char d[100];
				int fd =open("anew", O_RDWR |O_CREAT, S_IRWXU , S_IRWXG , S_IRWXO);
				dup2(fd,1);
				sprintf(d,"md5sum %s" ,command[1]);
				system(d);
				sprintf(d,"ls -lh %s | tr -s \" \" | cut -d \" \" -f 5-8 ",command[1]);
				system(d);
				close(fd);
				// remove(anew);
				dup2(bak,1);
				dup2(clientsock,1);

				system("cat anew | tr \"\n\" \" \"");
				remove("anew");
				dup2(bak,1);
				//    printf("hi%shi",c);
				//      printf("%c & %d",c[76],strlen(c));
				}*/
				else if(strcmp(command[0],"#$#")==0)
				{
					// int len=atoi(command[1]);
					char arr[10];
					//printf("* = %s\n",command[1]);
					sprintf(arr,"%c",string[49]);
					//printf("aaarr = %s\n",arr);
					FILE *fp=fopen(command[1],arr);
					int i=50,flag=0;
					//printf("bytes recv= %d strlen =%d \n",bytes_recieved,strlen(string));
					while(i<1021)
					{
						if(string[i]=='!' && string[i+1]=='@' && string[i+2]=='!')
						{   flag=1;
							break;
						}
						fprintf(fp,"%c",string[i]);
						i++;
					}

					fclose(fp);
				}

				else
				{
					printf("Recieved data = %s \n" , string);

					fflush(stdout);
					//                          close(clientsock);
					//                            break;
				}
			}
			/*   else
			     printf("Recieved data = %s \n" , recv_data);
			     fflush(stdout);
			 */  
			fflush(stdout);
		}
		exit(0);

	}
	else if (pid>0)
	{
		while(1)
		{
			char copy[1024];
			int i=0;
			//				printf("\nSEND (q or Q to quit) : ");
			gets(clientsend_data);
			printf("got data to send %s\n",clientsend_data);
			strcpy(copy,clientsend_data);
			char *str=strtok(copy,"  ");
			while(str!=NULL)
			{
				check[i]=str;
				i++;
				str=strtok(NULL," " );
			}
			check[i]='\0';
			if(i==2)
			{
				FILE *fpp=fopen("temp.txt","w");
				fprintf(fpp,"%s",check[1]);
				fclose(fpp);
			}
			//     printf("ch[1]=%s\n",check[1]);

			if (strcasecmp(clientsend_data , "q") != 0 && strcasecmp(clientsend_data , "Q") != 0)
			{
				if(udpflag==1)
					send(clientsock,clientsend_data,strlen(clientsend_data), 0); 

				else
				{
					printf("sending data to server %s\n",clientsend_data);
					sendto(clientsock,clientsend_data, strlen(clientsend_data), 0,
							(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
				}
			}
			else
			{
				if(udpflag==1)
				{ send(clientsock,clientsend_data,strlen(clientsend_data), 0);   
					close(clientsock);
				}
				else
				{
					sendto(clientsock,clientsend_data, strlen(clientsend_data), 0,
							(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
				}
				break;
			}
		}
		exit(0);
	}
    }
    return 0;
}
