#include<stdio.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>

/* simple line editor */
/* when it begins, starts with command mode
At command mode,we can move cursor.
initial cursor position is 1.1

cursor moves left,up,down,right by 'h','j','k','l' key input
*/
/*
typedef struct ListNodeType ListNode;
struct ListNodeType
{
	int data;
	ListNode* link;
};

typedef struct LinkedListType LinkedList;
struct LinkedListType
{
	int currentElement;
	ListNode headerNode;
};

LinkedList* createLinkedList()
{
	LinkedList* pList;
	pList = (LinkedList*)malloc(sizeof(LinkedList));
	if(pList !=NULL)
	{
		pList->currentElement=0;
		pList->headerNode.link = NULL;
	}
	return pList;
}
void addNode(LinkedList* pList, ListNode* newNode)
{
	ListNode* preNode;
	ListNode* pNewNode;

 	preNode = &(pList->headerNode);
	pNewNode = newNode;
	if(pList->headerNode.link == NULL)
	{
		pNewNode->link = preNode->link;
		preNode->link = pNewNode;
	}
	else
	{
		while(preNode->link != NULL)
		{
			preNode = preNode-> link;
		}
		preNode-> link = pNewNode;
	}
}
*/
char buff[255];
int cur_line = 1;
int cur_col = 1;
const int min_line = 1;
const int min_col = 1;
const int max_line = 20;
const int max_col = 80;
char data[80][20];
char key;
int ret;
struct termios oldt,curt,newt;

//mode 설정
enum MODES {
	COMMAND_MODE,
	INPUT_MODE,
	DELETE_MODE,
	STORE_MODE,
	BRING_MODE
};

int main(int argc,char *argv[])
{
//	int n = 0;
//	FILE* stream;
//	LinkedList* pList;
//	ListNode *Node;
//	stream = fopen("editor.txt","rt");//r인가?
//	pList = createLinkedList();
//	while(!feof(stream))
//	{
//		Node=(ListNode*)malloc(sizeof(ListNode));
//}

//	fscanf(stream,"%s",data);//여기 다시보기
	enum MODES mode = COMMAND_MODE;

//	int key,ret;
	

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
 	/*some initialization */
	fputs("\033[2J",stdout);
	fputs("\033[1;1H",stdout);

		
//	struct termios oldt,curt,newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	/*now, we get input*/
	while(1){
	
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
				mode = INPUT_MODE;
				tcgetattr(STDIN_FILENO, & curt);
				newt = curt;
				newt.c_lflag |= (ECHO);
				tcsetattr(STDIN_FILENO,TCSANOW,&newt);
				break;
			case 'd':
				mode = DELETE_MODE;
      		    tcgetattr(STDIN_FILENO, & curt);
                newt = curt;
                newt.c_lflag |= (ECHO);
                tcsetattr(STDIN_FILENO,TCSANOW,&newt);
                break;
			case 's':
				mode = STORE_MODE;
     			tcgetattr(STDIN_FILENO, & curt);
                newt = curt;
                newt.c_lflag |= (ECHO);
                tcsetattr(STDIN_FILENO,TCSANOW,&newt);
                break;
			case 'b':
				mode = BRING_MODE;
     			tcgetattr(STDIN_FILENO, & curt);
                newt = curt;
                newt.c_lflag |= (ECHO);
                tcsetattr(STDIN_FILENO,TCSANOW,&newt);
                break;
			case 'h':
				cur_col--;
				if(cur_col<min_col){
					if(cur_line ==1 && cur_line ==1 ){
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
				cur_line++;
				if(cur_line>max_line) cur_line = max_line;
				break;
			case 'j':
				cur_line--;
				if(cur_line<min_line) cur_line = min_line;
				break;
			case 'l':
			
					if(cur_line == 20 && cur_col == 80) {
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
			}//end switch
			
			//end COMMADN_MODE
		}
		else if(mode==INPUT_MODE){
			switch(key){
			case 27: //escape key code
				mode = COMMAND_MODE;
				tcgetattr(STDIN_FILENO,&curt);
				newt = curt;
				newt.c_lflag &= ~(ECHO);
				tcsetattr(STDIN_FILENO,TCSANOW,&newt);
				break;
			default:
				mode = INPUT_MODE;
				//calibrate cursor
				cur_col++;
				//save contents
				data[cur_col - 1][cur_line - 1] = key;
				if (cur_col > max_col){
					cur_col = 1;
					cur_line++;
					if(cur_line > max_line) cur_line = max_line;
				}
				break;
			} //end switch
			//end INPUT_MODE
		}
		else if(mode==DELETE_MODE){
			switch(key){
			case '27':
				mode = COMMAND_MODE;
				tcgetattr(STDIN_FILENO,&curt);
				newt = curt;
				newt.c_lflag &= ~(ECHO);
				tcsetattr(STDIN_FILENO,TCSANOW,&newt);
				break;
			default:
				mode = DELETE_MODE;
				cur_col--;
			//	NULL = data[cur_col +1][cur_line +1];
				if(cur_col<min_col){
					cur_col=80;
					cur_line--;
					if(cur_line < min_line) cur_line = min_line;
				}
				break;
			}//end switch
			//end DELETE_MODE
		}
		else if(mode==STORE_MODE){
			 switch(key){
             case '27':
                 mode = COMMAND_MODE;
                 tcgetattr(STDIN_FILENO,&curt);
                 newt = curt;
                 newt.c_lflag &= ~(ECHO);
                 tcsetattr(STDIN_FILENO,TCSANOW,&newt);
                 break;
			 default:
				mode= STORE_MODE;
			}//end switch
			//end STORE_MODE
		}
				//여기에 이제 STOTR모드 관련된거 쓰면됨
		else if(mode==BRING_MODE){
			switch(key){
            case '27':
                  mode = COMMAND_MODE;
                  tcgetattr(STDIN_FILENO,&curt);
                  newt = curt;
                  newt.c_lflag &= ~(ECHO);
                  tcsetattr(STDIN_FILENO,TCSANOW,&newt);
                  break;
            default:
				mode= BRING_MODE;
				//BRING 모드 관련된거 쓰기

			}//end switch
			//end BRING_MODE
		}
		sprintf(buff, "\033[%d;%dH%3d:%3d" ,1,70,cur_line,cur_col);
		fputs(buff,stdout);
		sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
		fputs(buff,stdout);
	} //end while

	/*return termio back */
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);

	return 0;
}

