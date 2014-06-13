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
At input mode,we can input text.

*/
char buff[255];
int cur_line = 1;
int cur_col = 1;
const int min_line = 1;
const int min_col = 1;
const int max_line = 20;
const int max_col = 80;
char data[80][20];
char key,key1,key2;
int ret;
int condition = 1;
int condition1 = 1;
int iOpen;
struct termios oldt,newt,curt;


//mode 설정
enum MODES {
	COMMAND_MODE,
	INPUT_MODE
};

int main(int argc,char *argv[]){
//	FILE* stream;
//	stream = fopen("editor.txt","rt");//r인가?
//	while(!feof(stream))
//	{
//}

//	fscanf(stream,"%s",data);//여기 다시보기
	char buff[255];
	enum MODES mode = COMMAND_MODE;

/*	FILE * fp =fopen("editor.txt","wt");
	if(fp==NULL){
		puts("file open failure!");
		return -1;
	}
    //여기 수정해야함. data[][]말고 입력된 문자들로.
	fputs("data[][]",fp);
	fclose(fp);

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
 

	fputs("\033[2J",stdout);
	fputs("\033[1;1H",stdout);


	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= (~ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	while(1){
		 printf("Which mode do you want?\n");
 	     printf("Command mode : c(C) / Input mode : i(I) Exit : q(Q) \n");
		fflush(stdin);
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
		if(mode == COMMAND_MODE){
			switch(key){
				case 'i': 
				case 'I':
					mode = INPUT_MODE;
					tcgetattr(STDIN_FILENO,&curt);
					newt = curt;
					newt.c_lflag |= (ECHO);
					tcsetattr(STDIN_FILENO,TCSANOW,&newt);
					break;
				
				case 'q':
				case 'Q':
					condition = 0;//while문 탈출
					exit(0);//프로그램 종료
					break;
				
				case 'h':
				case 'H':
						cur_col--;;
						if(cur_col<min_col){
							if(cur_line == 1 && cur_line == 1){
								cur_col = min_col;
								cur_line = min_line;
							}
							else{
								cur_col=80;
								cur_line--;
								if(cur_line<min_line)cur_line  = min_line;
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
						if(cur_line == 20 &&cur_col ==80){
							cur_col = max_col;
							cur_line = max_line;
						}
						else{
							cur_col++;
							if(cur_col>max_col){
								cur_col = 1;
								cur_line++;
								if(cur_line>max_line)cur_line = max_line;
							}
						}
						break;
				}//switch종료
			//COMMAND_MODE종료

//		fflush(stdin);//출력버퍼내용을 모두 지워준다.

//		if(mode == COMMAND_MODE){
//				system("clear");//창화면을 지우는 clear 명령어 실행
//				iOpen = open("Editor.txt","r");
				//iOpen =open("Editor.txt",O_RDWR | O_CREAT,0644);
				//open()은 반환값이 int이므로 iOpen에 담아준다.
				//O_RDWR-읽기쓰기 가능
				//0_CREAT-파일이 없으면 생성
/*				if(iOpen){//open()을 정상수행했다면 '1'이 반환되므로
						tcgetattr(STDIN_FILENO,&oldt);
						newt = oldt;
						newt.c_lflag = ECHO;
						newt.c_lflag = ICANON;
						tcsetattr(STDIN_FILENO,TCSANOW,&newt);
						condition = 1;
						MovingCursor();
				}	
*/				
				/*key1 = getchar();	
				switch(key1){
						case 'i':
						case 'I':
							mode = INPUT_MODE;
							tcattr(STDIN_FILENO,&curt);
							newt = curt;
							newt.c_lflag |= (ECHO);
							tcsetattr(STDIN_FILENO,TCSANOW,&newt);
							break;
						
						case '
						
				*/
				// else{
	              //   printf("ERROR!\n");
    	            // exit(-1);//exit(-1);은 치명적인 에러
         	//	}

	}//COMMAND_MODE종료
	
		else if(mode == INPUT_MODE){
				switch(key){
				case 27:
					mode = COMMAND_MODE;
					tcgetattr(STDIN_FILENO,&curt);
					newt = curt;
					newt.c_lflag &= ~(ECHO);
					tcsetattr(STDIN_FILENO,TCSANOW,&newt);
					break;
				default:
					mode = INPUT_MODE;
					cur_col++;
					data[cur_col - 1][cur_line - 1] = key;
					if(cur_col> max_col){
						cur_col = 1;
						cur_line++;
						if(cur_line > max_line)cur_line = max_line;
					}
					break;
				}//switch종료
		}//INPUT_MODE종료
		sprintf(buff,"\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
        fputs(buff,stdout);
        sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
        fputs(buff,stdout);
	}//end while
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	return 0;
}
/*				
				system("clear");

				tcgetattr(STDIN_FILENO,&oldt);
				newt = oldt;
				newt.c_lflag = ECHO;
				newt.c_lflag &= ~ICANON;
				tcsetattr(STDIN_FILENO, TCSANOW , &newt);

	//			sprintf(buff,"\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
	//			fputs(buff,stdout);
	//			sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
	//			fputs(buff,stdout);
		        iOpen = open("editor.txt","w");
			//	iOpen = open("editor.txt",O_RDWR|O_CREAT,0644);

				if(iOpen>0){
					cur_line++;
                    text[cur_col - 1][cur_line - 1];

					if(cur_col>max_col){
						cur_col = 1;
						cur_line++;
						if(cur_line>max_line) cur_line = max_line;
					}
					fread(text,80,20,iOpen);
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
									tcsetattr(STDIN_FILENO,TCSANOW,&newt);
									exit(1);
									break;
				}
				fflush(stdin);
			}//inputmode종료
 
 	else {
			printf("Error!EXIT!\n");
			exit(-1);

		}
	}//while종료
		return 0;
}//main종료
int MovingCursor(){
			enum MODES mode = COMMAND_MODE;
			fputs("\033[2J",stdout);
			fputs("\033[1;1H",stdout);	

			tcgetattr(STDIN_FILENO,&oldt);
			newt = oldt;
			newt.c_lflag &= ~(ICANON | ECHO );
			tcsetattr (STDIN_FILENO,TCSANOW,&newt);

			sprintf(buff,"\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
			fputs(buff,stdout);
			sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
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
										cur_col=80;
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
						
								if(cur_line ==20 && cur_col == 80) {
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

			
		sprintf(buff, "\033[%3d;%3dH%3d:%3d",1,70,cur_line,cur_col);
		fputs(buff,stdout);
		sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
		fputs(buff,stdout);
	} //end while

	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
}//moving cursor종료
*/
