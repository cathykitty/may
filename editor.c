#include<stdio.h>
#include<termios.h>
#include<unistd.h>

/* simple line editor */
/* when it begins, starts with command mode
At command mode,we can move cursor.
initial cursor position is 1.1

cursor moves left,down,up,right by 'h','j','k','l' key input
*/

enum MODE {
	COMMAND_MODE,
	INPUT_MODE
};

int main(int argc,char *argv[])
{
	char buff[255];
	enum MODE mode = COMMAND_MODE;

	int cur_line = 1;
	int cur_col = 1;
 	const int min_line = 1;
	const int min_col = 1;
	const int max_line = 20;
	const int max_col = 80;
	int key,ret;
	char data[80][20];

 	/*some initialization */
	fputs("\033[2J",stdout);
	fputs("\033[1;1H",stdout);

	struct termios oldt,curt,newt;

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
			case 'h':
				cur_col--;
				if(cur_col<min_col) cur_col = min_col;
				break;
			case 'j':
				cur_line++;
				if(cur_line>max_line) cur_line = max_line;
				break;
			case 'k':
				cur_line--;
				if(cur_line<min_line) cur_line = min_line;
				break;
			case 'l':
				cur_col++;
				if(cur_col> max_col){
					cur_col = 1;
					cur_line++;
					if(cur_line>max_line) cur_line = max_line;
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
		
		sprintf(buff, "\033[%d;%dH%3d:%3d",1,70,cur_line,cur_col);
		fputs(buff,stdout);
		sprintf(buff,"\033[%d;%dH",cur_line,cur_col);
		fputs(buff,stdout);
	} //end while

	/*return termio back */
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);

	return 0;
}

