# include "iGraphics.h"
#include <windows.h>
#include <mmsystem.h>
#include <time.h>
#include <unistd.h>

#define sw 1024
#define sh 600
/*
	function iDraw() is called again and again by the system.

*/
int chng_ind;
int del_ind=-1;
int taskcount=0;
int x=255,y=255,z=255;
int bgi=0;
int ringingtaskindex=0;
bool musicstate=false;
typedef struct{
    int dd;
    int mm;
    int yy;
    int h;
    int mi;
    int sec;
}systime;
systime pc;
FILE *fp;
FILE *fp2;

typedef struct{
    char taskname[100];
    int year;
    int day;
    int month;
    int hour;
    int minute;
    int taskstate=0;
}task;
task tasklist[10];
typedef struct{
    int oldbgi;
    int oldmsi;
    int oldst;
    int oldx,oldy,oldz;
}restoreold;
restoreold restore;

int i=0,j=0;
int st=0;
char tempdate[50];
char temptime[50];
char tempo[10];
char *tmp=tempdate;
char *tmt=temptime;
int counterfordate=0;
int counterfortime=0;

char bg[5][100]={"","pxfuel.bmp","pxfuel2.bmp","pxfuel3.bmp"};
char music[5][100]={"","Rick Astley - Never Gonna Give You Up (Official Music Video)-dQw4w9WgXcQ","timeup","caseclosed"};
int musici=1;

void frontpage();
void AddRemoveTask();
void ChangeRingtone();
void ChangeBackground();
void Settings();
void Credits();
void inputTask();
void saveToFile();
void getFileSize();
void removeTask();
void tasknamelist();
void remindtimelist();
void biostimefetch();
void ringtone();
void taskshow();
void alarm();
void Alarmoff();
void statusshow();
void savesettings();
void fetchsettings();
void push();
void delete_everything();
void ringtonechk();
void saveandquit();
void biostimeprint();
void changeschedule();

void iDraw() {
	//place your drawing codes here
	iClear();
	if(st==0){
        frontpage();
	}
	if(st==1){
        inputTask();
	}
	if(st==2){
        removeTask();
	}
	if(st==3){
        ChangeRingtone();
	}
	if(st==4){
        ChangeBackground();
	}
	if(st==5){
        Credits();
	}
	if(st==6){
        inputTask();
	}
	if(st==7){
        taskshow();
	}
	if(st==8){
        inputTask();
	}
	if(st==9){
        Alarmoff();
	}
	if(st==10){
        changeschedule();
	}
	if(st==11){
        changeschedule();
	}
	if(st==12){
        changeschedule();
	}
}
void changeschedule()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,500,"Enter task index(1-10) below: ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,450,tempo,GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,400,"Enter New Date: (format-dd/mm/yyyy)",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,350,tmp,GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,300,"Enter New Time: (format(24 hour clock)-hh:mm)", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,250,temptime,GLUT_BITMAP_TIMES_ROMAN_24);
}
void delete_everything()
{
    remove("tasks.bin");
    remove("oldset.bin");
    exit(0);
}


void statusshow()
{
    for(int f=0;f<taskcount;f++){
        iFilledCircle(120,463-50*(f+1),2);
        if(tasklist[f].taskstate==0 ){
            iText(700,460-50*(f+1),"Not Done",GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else {
            iText(700,460-50*(f+1),"Done",GLUT_BITMAP_TIMES_ROMAN_24);
        }
    }
}

void alarm()
{
    biostimefetch();
    for(int i=0;i<taskcount;i++){
        if(pc.h==tasklist[i].hour && pc.mi==tasklist[i].minute && pc.dd==tasklist[i].day && pc.mm==tasklist[i].month && pc.yy==tasklist[i].year && pc.sec==0 && tasklist[i].taskstate==0){
            musicstate=true;
            tasklist[i].taskstate=1;
            ringingtaskindex=i;
            st=9;
            ringtone();
            break;
        }
    }

}
void Alarmoff()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(325,450, "Turn off Reminder for:", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(325,400, " -> 5 minutes", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(325,350, " -> 10 minutes", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(325,300, " -> Indefinitely", GLUT_BITMAP_TIMES_ROMAN_24);
}

void taskshow()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,500,"Tasks you've added: ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(195,450,"Title", GLUT_BITMAP_TIMES_ROMAN_24);
    iLine(193,440,243,440);
    iText(400,450,"Remind at", GLUT_BITMAP_TIMES_ROMAN_24);
    iLine(400,440,500,440);
    iText(700,450,"Status", GLUT_BITMAP_TIMES_ROMAN_24);
    iLine(700,440,760,440);
    iLine (380,480,380,0);
    iLine(680,480,680,0);

    tasknamelist();
    remindtimelist();
    statusshow();
}

void frontpage()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iRectangle(374,450,270,40);
    iText(425,460,"ToDo & !ToDo", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,360,"1. Add Task", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,310,"2. Remove Task", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,260,"3. Task List", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,210,"4. Change Ringtone", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,160,"5. Change Background", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,110,"6. Credits", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(740,20,"Delete Everything & Exit!!",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(10,20,"Press End to Save & Exit", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,60,"7. Change Task Time",GLUT_BITMAP_TIMES_ROMAN_24);
    biostimeprint();

}

void Credits()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,360,"1. Me", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,310,"2. My Laptop", GLUT_BITMAP_TIMES_ROMAN_24);
    //iText(400,260,"0. Back", GLUT_BITMAP_TIMES_ROMAN_24);
}
void ChangeRingtone()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,360,"1. NGGYU(Default)", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,310,"2. Surprise", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,260,"3. Case Closed Theme", GLUT_BITMAP_TIMES_ROMAN_24);
}
void ChangeBackground()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,360,"1. White ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,310,"2. Tanjiro", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(400,260,"3. Dark", GLUT_BITMAP_TIMES_ROMAN_24 );
    iText(400,210,"4. Komi-San ", GLUT_BITMAP_TIMES_ROMAN_24);
}
void saveToFile()
{
    fp = fopen("tasks.bin", "w");
    for (int p = 0; p < taskcount; p++)
    {
        fwrite(&tasklist[p], sizeof(task), 1, fp);
    }
    fclose(fp);
}
void getFileSize()
{
    fseek(fp, 0L, SEEK_END);
    unsigned int long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    i = size / sizeof(task);
    taskcount=i;
}

void fetchtask()
{
    fp=fopen("tasks.bin","r");
    if(fp){
        getFileSize();
        for(int p=0;p<i;p++){
            fread(&tasklist[p], sizeof(task),1,fp);
        }
        fclose(fp);
    }
}
void savesettings()
{
    fp2 = fopen("oldset.bin", "w");
    fwrite(&restore, sizeof(restoreold), 1, fp2);
    fclose(fp2);
}
void fetchsettings()
{
    fp2=fopen("oldset.bin","r");
    if(fp2){
        fread(&restore, sizeof(restoreold),1,fp2);
        bgi=restore.oldbgi;
        musici=restore.oldmsi;
        st=restore.oldst;
        x=restore.oldx;
        y=restore.oldy;
        z=restore.oldz;
        fclose(fp2);
    }
}

void inputTask()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,500,"Enter Task Below: ", GLUT_BITMAP_TIMES_ROMAN_24);
    if(i!=-1){
        iText(200,450,tasklist[i].taskname,GLUT_BITMAP_TIMES_ROMAN_24);
    }
    iText(200,400,"Enter Date: (format-dd/mm/yyyy)",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,350,tempdate,GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,300,"Enter Time: (format(24 hour clock)-hh:mm)", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,250,temptime,GLUT_BITMAP_TIMES_ROMAN_24);
}
void removeTask()
{
    if(bgi) iShowBMP(0,0,bg[bgi]);
    iSetColor(x,y,z);
    iText(30,560,"Back",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(200,500,"Click the task you want to remove: ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(195,450,"Title", GLUT_BITMAP_TIMES_ROMAN_24);
    iLine(193,440,243,440);
    iText(400,450,"Remind at", GLUT_BITMAP_TIMES_ROMAN_24);
    iLine(400,440,500,440);
    iText(700,450,"Status", GLUT_BITMAP_TIMES_ROMAN_24);
    iLine(700,440,760,440);
    iLine (380,480,380,0);
    iLine(680,480,680,0);

    tasknamelist();
    remindtimelist();
    statusshow();

}
void tasknamelist()
{
    for(int f=0;f<taskcount;f++){
        char bruh[3];
        itoa(f+1,bruh,10);
        iText(100,460-50*(f+1),bruh,GLUT_BITMAP_TIMES_ROMAN_24);
        iFilledCircle(120,463-50*(f+1),2);
        iText(130,460-50*(f+1),tasklist[f].taskname,GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
void remindtimelist()
{
    for (int f=0;f<taskcount;f++){
        char lol[11][5][5];
        itoa(tasklist[f].day,lol[f][0],10);
        itoa(tasklist[f].month,lol[f][1],10);
        itoa(tasklist[f].year,lol[f][2],10);
        itoa(tasklist[f].hour,lol[f][3],10);
        itoa(tasklist[f].minute,lol[f][4],10);
        for(int s=3;s<5;s++){
            if(lol[f][s][1]=='\0'){
                char tmp2=lol[f][s][0];
                lol[f][s][0]='0';
                lol[f][s][1]=tmp2;
                lol[f][s][2]='\0';
            }
        }
        for(int s=0;s<2;s++){
            if(lol[f][s][1]=='\0'){
                char tmp2=lol[f][s][0];
                lol[f][s][0]='0';
                lol[f][s][1]=tmp2;
                lol[f][s][2]='\0';
            }
        }

        iText (405,460-(f+1)*50,lol[f][0],GLUT_BITMAP_TIMES_ROMAN_24);
        iText (435,460-(f+1)*50,"/",GLUT_BITMAP_TIMES_ROMAN_24);
        iText (445,460-(f+1)*50,lol[f][1],GLUT_BITMAP_TIMES_ROMAN_24);
        iText (470,460-(f+1)*50,"/",GLUT_BITMAP_TIMES_ROMAN_24);
        iText (480,460-(f+1)*50,lol[f][2],GLUT_BITMAP_TIMES_ROMAN_24);
        iText (530,460-(f+1)*50,",",GLUT_BITMAP_TIMES_ROMAN_24);
        iText (540,460-(f+1)*50,lol[f][3],GLUT_BITMAP_TIMES_ROMAN_24);
        iText (565,460-(f+1)*50,":",GLUT_BITMAP_TIMES_ROMAN_24);
        iText (575,460-(f+1)*50,lol[f][4],GLUT_BITMAP_TIMES_ROMAN_24);

    }
}
void push()
{
    for(int s=0;s<(taskcount);s++){
        if(del_ind!=-1){
            tasklist[del_ind+s]=tasklist[del_ind+s+1];
            tasklist[del_ind+s+1].taskname[0]='\0';
        }
    }

}

void saveandquit()
{
    saveToFile();
    restore.oldbgi=bgi;
    restore.oldmsi=musici;
    restore.oldst=st;
    restore.oldx=x;
    restore.oldy=y;
    restore.oldz=z;
    savesettings();
    exit(0);
}
void biostimeprint()
{
    char c[10];
    itoa(pc.dd,c,10);
    iText(850,570,c,GLUT_BITMAP_TIMES_ROMAN_24);
    iText(880,570,"/",GLUT_BITMAP_TIMES_ROMAN_24);
    itoa(pc.mm,c,10);
    iText(890,570,c,GLUT_BITMAP_TIMES_ROMAN_24);
    itoa(pc.dd,c,10);
    iText(920,570,"/",GLUT_BITMAP_TIMES_ROMAN_24);
    itoa(pc.yy,c,10);
    iText(930,570,c,GLUT_BITMAP_TIMES_ROMAN_24);
    itoa(pc.h,c,10);
    iText(880,540,c,GLUT_BITMAP_TIMES_ROMAN_24);
    iText(905,540,":",GLUT_BITMAP_TIMES_ROMAN_24);
    itoa(pc.mi,c,10);
    iText(910,540,c,GLUT_BITMAP_TIMES_ROMAN_24);
    iText(935,540,":",GLUT_BITMAP_TIMES_ROMAN_24);
    itoa(pc.sec,c,10);
    iText(940,540,c,GLUT_BITMAP_TIMES_ROMAN_24);

}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //place your codes here
        if(st==0){
            if(mx>=400 && mx<=524 && my>=360 && my<=381){
                st=1;
            }
            if(mx>=400 && mx<=564 && my>=310 && my<=331){
                st=2;
            }
            if(mx>=400 && mx<=594 && my>=260 && my<=281){
                st=7;
            }
            if(mx>=400 && mx<=594 && my>=210 && my<=231){
                st=3;
            }
            if(mx>=400 && mx<=624 && my>=160 && my<=181){
                st=4;
            }
            if(mx>=400 && mx<=494 && my>=110 && my<=131){
                st=5;
            }
            if(mx>=740 && mx<=1000 && my>=20 && my<=43){
                delete_everything();
            }
            if(mx>=0 && mx<=300 && my>=20 && my<=43){
                saveandquit();
            }
            if(mx>400 && mx<=600 && my>=60 && my<=83){
                st=10;
            }
        }
        if (mx>=30 && mx<=80 && my>=560 && my<=581 && st!=0){
            st=0;
        }
        if (st==4){
            if(mx>=400 && mx<=600 && my>=360 && my<=381){
                x=0;
                y=0;
                z=0;
                bgi=1;
            }
            if(mx>=400 && mx<=700 && my>=310 && my<=331){
                x=255;
                y=255;
                z=255;
                bgi=2;
            }
            if(mx>=400 && mx<=475 && my>=260 && my<=281){
                x=255;
                y=255;
                z=255;
                bgi=0;
            }
            if(mx>=400 && mx<=535 && my>=210 && my<=231){
                x=0;
                y=0;
                z=0;
                bgi=3;
            }
        }
        if(st==3){
            if(mx>=400 && mx<=600 && my>=360 && my<=381){
                musicstate=true;
                musici=1;
                ringtone();
            }
            if(mx>=400 && mx<=700 && my>=310 && my<=331){
                musicstate=true;
                musici=2;
                ringtone();
            }
            if(mx>=400 && mx<=700 && my>=260 && my<=281){
                musicstate=true;
                musici=3;
                ringtone();
            }
        }
        if(st==9){

            if(mx>=360 && mx<=465 && my>=399 && my<=424){
                tasklist[ringingtaskindex].minute+=5;
                if(tasklist[ringingtaskindex].minute>=60){
                    tasklist[ringingtaskindex].hour+=1;
                    tasklist[ringingtaskindex].minute%=60;
                }
                PlaySound("No - Sound Effect",NULL, SND_ASYNC);
                tasklist[ringingtaskindex].taskstate=0;
                st=0;

                }
                if(mx>=310 && mx<=465 && my>=349 && my<=374){
                    tasklist[ringingtaskindex].minute+=10;
                    if(tasklist[ringingtaskindex].minute>=60){
                        tasklist[ringingtaskindex].hour+=1;
                        tasklist[ringingtaskindex].minute%=60;
                    }
                    PlaySound("No - Sound Effect",NULL, SND_ASYNC);
                    tasklist[ringingtaskindex].taskstate=0;
                    st=0;
                }
                if(mx>=310 && mx<=465 && my>=299 && my<=324){
                    PlaySound("No - Sound Effect",NULL, SND_ASYNC);
                    tasklist[ringingtaskindex].taskstate=1;
                    st=0;
                }
        }
        if (st==2){

            if(mx>=125 && mx<=370 && my>=410&& my<=431){
                del_ind=0;
                push();
                taskcount--;
                i--;
            }
            if(mx>=125 && mx<=370 && my>=360&& my<=381){
                del_ind=1;
                push();
                taskcount--;
                i--;
            }
            if(mx>=125 && mx<=370 && my>=310&& my<=331){
                del_ind=2;
                taskcount--;
                i--;
                push();
            }
            if(mx>=125 && mx<=370 && my>=260 && my<=281){
                del_ind=3;
                taskcount--;
                i--;
                push();
            }
            if(mx>=125 && mx<=370 && my>=210&& my<=231){
                del_ind=4;
                taskcount--;
                i--;
                push();
            }
            if(mx>=125 && mx<=370 && my>=160 && my<=181){
                del_ind=5;
                taskcount--;
                i--;
                push();
            }
            if(mx>=125 && mx<=370 && my>=110 && my<=131){
                del_ind=6;
                taskcount--;
                i--;
                push();
            }
            if(mx>=125 && mx<=370 && my>=60 && my<=81){
                del_ind=7;
                taskcount--;
                i--;
                push();
            }
            if(mx>=125 && mx<=370 && my>=10 && my<=31){
                del_ind=8;
                taskcount--;
                i--;
                push();
            }

        }

    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        static int countclick=0;
        for(int f=0;f<taskcount;f++){
            if(mx>=700 && mx<=850 && my>=460-50*(f+1) && my<=483-50*(f+1)){
                countclick=(countclick+1)%2;
            }
        }

        //place your codes here
        printf("%d\n",countclick);
        if(st==7 && countclick==0){
            for(int f=0;f<taskcount;f++){
                if(mx>=700 && mx<=850 && my>=460-50*(f+1) && my<=483-50*(f+1)){
                    tasklist[f].taskstate=!tasklist[f].taskstate;
                }
            }
        }
    }

}
void ringtone()
{
    if(musicstate ){
        PlaySound(music[musici],NULL, SND_LOOP | SND_ASYNC);
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/

void iKeyboard(unsigned char key) {
	/*if (key == 'q') {
		exit(0);
	}*/
	//place your codes for other keys here
	if(st==1){
        if( key == '\b'){
            j--;
            if(j<0) j=0;
            tasklist[i].taskname[j]='\0';

        }
        if (key != '\b') {
            tasklist[i].taskname[j]=key;
            tasklist[i].taskname[j+1]='\0';
            j++;
        }

        if (key == '\r'){
            st=6;
            j=0;
            key=' ';
        }
	}
	if(st==6 && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9'|| key=='\r' || key=='\b' || key=='/' ) ){

        if( key == '\b'){
            j--;
            if(j<0) j=0;
            tempdate[j]='\0';
        }
        if (key != '\b' ) {
            tempdate[j]=key;
            j++;
        }
        if (key == '/' || key=='\r'){
            tempdate[j]='\0';
            tasklist[i].day=atoi(tempdate);
            printf("%d\n",tasklist[i].day);
            tasklist[i].month=atoi(tmp+3);
            printf("%d\n",tasklist[i].month);
            tasklist[i].year=atoi(tmp+6);
            printf("%d\n",tasklist[i].year);

            //}
            if(key=='\r'){
                st=8;
                j=0;
                key=' ';
            }

        }

	}

	if(st==8 && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9'|| key=='\r' || key=='\b' || key==':')){
        if( key == '\b'){
            j--;
            if(j<0) j=0;
            temptime[j]='\0';

        }
        if (key != '\b' ) {
            temptime[j]=key;
            j++;
        }
        if ((key == ':' || key=='\r')){
            temptime[j]='\0';
            tasklist[i].hour=atoi(temptime);
            printf("%d\n",tasklist[i].hour);
            tasklist[i].minute=atoi(tmt+3);
            printf("%d\n",tasklist[i].minute);
            if(key=='\r'){
                for(int k=0;k<50;k++){
                    tempdate[k]='\0';
                    temptime[k]='\0';
                }
                taskcount++;
                i++;
                st=1;
                j=0;
            }

        }

    }
    if(st==10 && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9'|| key=='\r' || key=='\b')){
        if( key == '\b'){
            j--;
            if(j<0) j=0;
            tempo[j]='\0';
        }
        if (key != '\b' ) {
            tempo[j]=key;
            j++;
        }
        if (key=='\r'){
            tempo[j]='\0';
            chng_ind=atoi(tempo)-1;
            printf("%d\n",chng_ind);
            j=0;
            st=11;
            key=' ';
            //tempo[0]='\0';
        }
    }
    if(st==11 && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9'|| key=='\r' || key=='\b' || key=='/' ) ){

        if( key == '\b'){
            j--;
            if(j<0) j=0;
            tempdate[j]='\0';
        }
        if (key != '\b' ) {
            tempdate[j]=key;
            j++;
        }
        if (key == '/' || key=='\r'){
            tempdate[j]='\0';
            tasklist[chng_ind].day=atoi(tempdate);
            tasklist[chng_ind].month=atoi(tmp+3);
            tasklist[chng_ind].year=atoi(tmp+6);
            if(key=='\r'){
                st=12;
                j=0;
                key=' ';
            }

        }

	}
    if(st==12 && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9'|| key=='\r' || key=='\b' || key==':')){
        if( key == '\b'){
            j--;
            if(j<0) j=0;
            temptime[j]='\0';

        }
        if (key != '\b' ) {
            temptime[j]=key;
            j++;
        }
        if ((key == ':' || key=='\r')){
            temptime[j]='\0';
            tasklist[chng_ind].hour=atoi(temptime);
            tasklist[chng_ind].minute=atoi(tmt+3);

            if(key=='\r'){
                for(int k=0;k<50;k++){
                    tempdate[k]='\0';
                    temptime[k]='\0';
                }
                st=10;
                j=0;
                tempo[0]='\0';
            }

        }

    }

}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
        saveandquit();
	}
	//place your codes for other keys here
}
void biostimefetch()
{
    time_t rawtime;
    tm * t;
    time(&rawtime);
    t= gmtime(&rawtime);
    pc.dd=t->tm_mday;
    pc.mm=t->tm_mon+1;
    pc.yy=t->tm_year+1900;
    pc.h=(t->tm_hour+6)%24;
    pc.mi=t->tm_min;
    pc.sec=t->tm_sec;
    //printf("%d:%d:%d\n",pc.h,pc.mi,pc.sec);
    //printf("%d/%d/%d\n",pc.dd,pc.mm,pc.yy);
}
void ringtonechk()
{
    if(st==0){
        musicstate=false;
        PlaySound("No - Sound Effect",NULL,SND_ASYNC);
    }
}

int main() {
    fetchtask();
    fetchsettings();
    iSetTimer(1000,alarm);
    iSetTimer (500,ringtonechk);
	//place your own initialization codes here.
	iInitialize(sw, sh, "ToDo & !ToDo");
	return 0;
}
