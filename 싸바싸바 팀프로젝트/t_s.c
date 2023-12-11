#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAXLINE 256
#define ROOMMAX 10

typedef struct ROOM{
	char code[MAXLINE];
	char roomname[MAXLINE];
}ROOM;

typedef struct rank {
	int record;
	char name[20];
}rank;

int main(int argc, char *argv[]) {
	int rd, wr;
	ROOM room[10]; //방 정보를 담을 문자열 배열
 	int roomindex = 0;

	unlink("./fromserver");
	unlink("./fromclient");

	if (mkfifo("./fromserver", 0666) == -1) {
         perror("serverpipe");
         exit(1);
    }

	if (mkfifo("./fromclient", 0666) == -1) {
         perror("clientpipe");
         exit(2);
    }

	char msg[MAXLINE];
	wr = open("./fromserver", O_WRONLY);
	rd = open("./fromclient", O_RDONLY);
	if (wr == -1 || rd == -1) {
		perror("open");
		exit(3);
	}
/*	
	FILE *fp; //랭크 텍스트 파일
	if (argc != 2) {
		fprintf(stderr, "파일 입력 오류\n");
		return 1;
    }
	
	fp = fopen(argv[1], "rb+");

	if(fp == NULL) {
		fprintf(stderr, "파일 열기 오류\n");
		return 2;
	}

	rank rec;
	rank filerec;
	rank temp;
	
	char emptyline[50];
	for (int i = 0; i < 50; i++) {
		emptyline[i] = ' ';
	}
*/
	int pid = fork();
	if(pid == -1)
	{
		perror("자식프로세스 생성 실패");
		exit(4);	
	}
	if(pid == 0) {
		printf("자식 시작\n");


		while(1){
			char hg_ans[20];
			char code[20];
			memset(hg_ans, 0, sizeof(hg_ans));
			memset(code, 0, sizeof(code));
			int n = 0;
			while(n == 0){
				n = read(rd, hg_ans, 1); //보낸 사람이 호스트인지 게스트인지 확인
			}
			printf("게스트 호스트: %s\n", hg_ans);
			if(!strcmp(hg_ans, "1")) //호스트라면
			{	
				printf("호스트 정보 : \n");
				read(rd, room[roomindex%ROOMMAX].code, 1); //방 코드와
				printf("호스트 방 코드 : %c\n", room[roomindex%ROOMMAX].code[0]);
				memset(room[roomindex%ROOMMAX].roomname, 0, MAXLINE);
				read(rd, room[roomindex%ROOMMAX].roomname, MAXLINE); //방 이름 저장
				printf("호스트 방 이름 : %s\n", room[roomindex%ROOMMAX].roomname);
				roomindex++;
			}
			else if(!strcmp(hg_ans, "0"))
			{
				int codecheck = 0;
				printf("게스트 정보 : \n");
				read(rd, code, MAXLINE); //게스트에게 방 코드를 받고
				printf("게스트 입력 코드 : %s\n", code);
				for(int i = 0; i < ROOMMAX; i++) {
					if(room[i].code[0] == code[0]) //코드로 방을 찾았다면
					{	
						printf("게스트 방 이름 : %s\n", room[i].roomname);
						write(wr, room[i].roomname, MAXLINE);
						memset(room[i].roomname, 0, sizeof(room[i].roomname));
						memset(room[i].code, 0, sizeof(room[i].code));
						codecheck = 1;
					}
				}
				if(!codecheck) //코드가 맞지 않다면
				{	
					write(wr, "\0", 1);
				}
			} /*
			else if(!strcmp(hg_ans, "2")){ //만약 보낸 것이 랭크라면
				char playtime[20];
				memset(rec.name, 0, sizeof(rec.name));
				memset(playtime, 0, sizeof(playtime));
				read(rd, rec.name, sizeof(rec.name)); //빙 이름 읽기
				read(rd, playtime, sizeof(playtime)); //점수 읽기
				rec.record = atoi(playtime); //점수 정수로 변환
				printf("랭크저장 %s : %d\n", rec.name, rec.record);

				int rankexist = 1;
				int rankstart = 0;
				fseek(fp, 0, SEEK_SET);
				while (rankexist > 0) {

					rankexist = fread(&filerec, sizeof(filerec), 1, fp);
					if (filerec.record > rec.record && rankexist > 0) { //본인보다 큰 랭크를 발견했다면 정렬
						fseek(fp, rankstart * sizeof(filerec), SEEK_SET);

						temp.record = filerec.record;
						memset(temp.name, 0, sizeof(temp.name));
						strcpy(temp.name, filerec.name);

						fprintf(fp, "%s", emptyline);
						fseek(fp, rankstart * sizeof(filerec), SEEK_SET);
						fwrite(&rec, sizeof(rec), 1, fp);

						rec.record = temp.record;
						memset(rec.name, 0, sizeof(rec.name));
						strcpy(rec.name, temp.name);
					}
					if (rankexist == 0) {

						fwrite(&rec, sizeof(rec), 1, fp);
					}
					rankstart++;
				}

			} */
		}
	}
	wait(NULL);

}

