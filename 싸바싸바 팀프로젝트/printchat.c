#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <signal.h>
#define  MAXLINE 256
#define MAXSHM 1024

int chat_shmid, bag_shmid;
char *chatlog, *bag;

void intHandler(){
	shmctl(chat_shmid,IPC_RMID,NULL);
	shmctl(bag_shmid, IPC_RMID, NULL);	
	exit(1);
}

int main(){
	int chatread;
	char roomname[20];
	char code[20]; //
	char hg_ans[20];
	memset(roomname, 0, sizeof(roomname));

	signal(SIGINT, intHandler);
	while(1){
		printf("\033[2J\033[H");
		printf("호스트(1) 게스트(0) : ");
		memset(hg_ans, 0, 20);
		fgets(hg_ans, 20, stdin);
		if(!strcmp(hg_ans, "1\n")||!strcmp(hg_ans, "0\n")){
			break;
		}
	}
	printf("팀 이름을 임력하세요 : ");
	fgets(roomname, sizeof(roomname), stdin);
	printf("방 코드를 입력하세요 : "); //
	fgets(code, sizeof(code), stdin);//
	char cpipe[20] = "./chat";
	strncat(cpipe, hg_ans, strlen(hg_ans));
	strncat(cpipe, roomname, strlen(roomname));
	strncat(cpipe, code, 1); //
	printf("cpipe : %s\n", cpipe);

	chatread = open(cpipe, O_RDONLY);

	if(chatread == -1) {
        perror("잘못된 코드와 방 이름입니다."); 
    }
	printf("파이프 열림\n");

	read(chatread, &chat_shmid, sizeof(chat_shmid));
	if(chat_shmid == -1){
		perror("공유메모리 가져오기 실패");
        exit(5);
    }
	read(chatread, &bag_shmid, sizeof(bag_shmid));
	if(bag_shmid == -1){
        perror("공유메모리 가져오기 실패");
        exit(5);
    }

	printf("%d\n", chat_shmid);
	chatlog = shmat(chat_shmid, NULL, 0);
	bag = shmat(bag_shmid, NULL, 0);

	while(1){
		printf("\033[2J\033[H"); //창 지우기
		printf("작은 가방 : ");
		if(bag[0] == '0'){
			printf("스테이크\n");
		}
		else if(bag[0] == '1'){
			printf("쇠젓가락\n");
		}
		else if(bag[0] == '2'){
			printf("찬장 열쇠\n");
		}
		else if(bag[0] == '3'){
			printf("고무장갑\n");
		}
		else{
			printf("\n");
		}
		printf("----------------------------\n");
		printf("%s", chatlog);
		usleep(500000);
	}	
}

