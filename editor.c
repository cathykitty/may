#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
/*  editor */
/*
At command mode,we can move cursor.
initial cursor position is 1.1
cursor moves left,up,down,right by 'h','j','k','l' key input
*/
char buff[255];
int cur_line = 1;
int cur_col = 1;
const int min_line = 1;
const int min_col = 1;
const int max_line = 50;
const int max_col = 100;
char text[100][50];
char key,key1,key2;
int ret;
int condition = 1;
int condition1 =1;
int iOpen;
struct termios oldt,newt;


//mode 설정
enum MODES {
	COMMAND_MODE,
	INPUT_MODE,
	DELETE_MODE,
	STORE_MODE,
	BRING_MODE
};

int main(int argc,char *argv[]){
//	FILE* stream;
//	stream = fopen("editor.txt","rt");//r인가?
//	while(!feof(stream))
//	{
//}

//	fscanf(stream,"%s",data);//여기 다시보기
	enum MODES mode = COMMAND_MODE;

/*	FILE * fp =fopen("editor.txt","wt");
	if(fp==NULL){
		puts("file open failure!");
		return -1;
	}
    //여기 수정해야함. data[][]말고 입력된 문자들로.
	fputs("data[][]",fp);
	fclose(fp);

	//Bring stored data
	int ch,i;
	//파일로부터 데이터 어떻게 읽어오지?
	FILE *fw = fopen("editor.txt","rt");
	if(fw==NULL){
		puts("file open failure!");
		return -1;
	}
	for(i=0;i<100 ;i++)
	{
		ch=fgetc(fw);
		printf("%c \n",ch);
	}
	fclose(fw);		
*/		
 

/*	fputs("\033[2J",stdout);*/

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= (~ICANON);
	newt.c_lflag &= (~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	printf("Which mode do you want?\n");
	printf("Command mode : c / Input mode : i \n");
 
	/*now, we get input*/
	while(condition){
	
		key = getchar();
		ret = feof(stdin);
		if (ret!= 0){
			printf("EOF\n");
			return 0;
		}
		ret = ferror(stdin);
		if(ret!=0){
			printf("ERROR getchar() by %d\n",ret);
			return 0;
		}
		
		switch(key)
			{

				case 'c':
				case 'C':
					mode = COMMAND_MODE;
					break;
			
				case 'i': 
				case 'I':
					mode = INPUT_MODE;
					break;
	
				case 'd':
				case 'D':
					mode = DELETE_MODE;
					break;

				case 's':
				case 'S':
					mode = STORE_MODE;
					break;

				case 'b':
				case 'B':
					mode = BRING_MODE;
					break;

				case 'q':
				case 'Q':
					condition = 0;//while문 탈출
					exit(0);//프로그램 종료
					break;
			}//switch종료

		fflush(stdin);//출력버퍼내용을 모두 지워준다.
		if(mode == COMMAND_MODE){
				system("clear");//창화면을 지우는 clear 명령어 실행
				iOpen =open("Editor.txt",O_RDWR | O_CREAT,0644);
				//open()은 반환값이 int이므로 iOpen에 담아준다.
				//O_RDWE-읽기쓰기 가능
				//0_CREAT-파일이 없으면 생성
				if(iOpen>0){//open()을 정상수행했다면 '1'이 반환되므FH
						tcgetattr(STDIN_FILENO,&oldt);
						newt = oldt;
						newt.c_lflag = ECHO;
						newt.c_lflag = ICANON;
						tcsetattr(STDIN_FILENO,TCSANOW,&newt);
						condition = 1;
						MovingCursor();			
				}
				 else{
	                 printf("ERROR!\n");
    	             exit(-1);//exit(-1);은 치명적인 에러
         		}

		}//COMMAND_MODE종료
	
		else if(mode == INPUT_MODE){
				system("clear");

				tcgetattr(STDIN_FILENO,&oldt);
				newt = oldt;
				newt.c_lflag = ECHO;
				newt.c_lflag &= ~ICANON;
				tcsetattr(STDIN_FILENO, TCSANOW , &newt);

				sprintf(buff,"\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
				fputs(buff,stdout);
				sprintf(buff,"*INPUT_MODE*\033[%d;%dH",cur_line,cur_col);
				fputs(buff,stdout);
			
				iOpen = open("editor.txt",O_RDWR|O_CREAT,0644);

				if(iOpen>0){
					cur_col++;
                    text[cur_col - 1][cur_line - 1];

					if(cur_col>max_col){
						cur_col = 1;
						cur_line++;
						if(cur_line>max_line) cur_line = max_line;
					}
					fread(text,100,50,iOpen);
					printf("%s\n",text);
				}
				scanf("%s",&text);
				fclose(iOpen);
				printf("Save\n");
				
				tcgetattr(STDIN_FILENO,&oldt);
				newt = oldt;
				newt.c_lflag &= ~ECHO;
				tcsetattr(STDIN_FILENO,TCSANOW,&newt);
				
	
				//else{
				//		printf("ERROR!\n");
				//	}

				key2 = getchar();
				switch(key2){
							case 'c':
							case 'C':
					
									mode = COMMAND_MODE;
									break;
							case 'q':
							case 'Q':
									tcgetattr(STDIN_FILENO,&oldt);
									newt = oldt;
									newt.c_lflag = ECHO;
									tcsetatr(STDIN_FILENO,TCSANOW,&newt);
									exit(1);
									break;
				}
				fflush(stdin);
			}//inputmode종료
 
 		/*else {
			printf("Error!EXIT!\n");
			exit(-1);

		}*/
	}//while종료
		return 0;
}//main종료
void MovingCursor(){
			enum MODES mode = COMMAND_MODE;
			fputs("\033[2J",stdout);
			fputs("\033[1;1H",stdout);	

			tcgetattr(STDIN_FILENO,&oldt);
			newt = oldt;
			newt.c_lflag &= ~(ICANON | ECHO );
			tcsetattr (STDIN_FILENO,TCSANOW,&newt);

			sprintf(buff,"\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
			fputs(buff,stdout);
			sprintf(buff,"*COMMAND_MODE*\033[%d;%dH",cur_line,cur_col);
			fputs(buff,stdout);

			fflush(stdin);
			while(condition1){
			key1 = getchar();

			if(mode == COMMAND_MODE){

				switch(key1){
						case 'h':
						case 'H':

								cur_col--;
								if(cur_col<min_col){
									if(cur_line == 1 && cur_line == 1 ){
										cur_col = min_col;
										cur_line = min_line; 
									}						
						
									else{
										cur_col=100;
										cur_line--;
										if(cur_line<min_line) cur_line = min_line;	
										}
								}
								break;

						case 'k':
						case 'K':

								cur_line++;
								if(cur_line>max_line) cur_line = max_line;
								break;

						case 'j':
						case 'J':
			
								cur_line--;
								if(cur_line<min_line) cur_line = min_line;
								break;

						case 'l':
						case 'L':
						
								if(cur_line == 50 && cur_col == 100) {
									cur_col= max_col;
                    				cur_line = max_line;
								}
								else{
									cur_col++;
									if(cur_col>max_col){
										cur_col = 1;
										cur_line++;
									if(cur_line>max_line) cur_line = max_line;
									}
								}
								break;

						case 'i':
						case 'I':
					
								condition1 = 0;
								mode = INPUT_MODE;
								break;
					
						case 'q':
						case 'Q':
						
								exit(1);
								break;
				}// switch종료
				}//commandmode종료
			
					sprintf(buff,"\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
					fputs(buff,stdout);
					sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
					fputs(buff,stdout);
	
					tcsetattr(STDIN_FILENO,TCSANOW,&oldt);

			
		sprintf(buff, "\033[%d;%dH%3d:%3d" ,1,70,cur_line,cur_col);
		fputs(buff,stdout);
		sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
		fputs(buff,stdout);
	} //end while

	/*return termio back */
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
}//moving cursor종료
