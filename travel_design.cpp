#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<windows.h>
#include<graphics.h> 
using namespace std;
typedef struct lists
{	string seq;//班次，号数均为3位，若为0开头，则交通方式为汽车；1开头则交通方式为火车；2开头则交通方式为飞机 
	int start_pos;//始发站 
	float start_time;//出发时间 
	int arrive_pos;//终点站 
    float arrive_time;//到达时间 
/********城市对应号码*********/
/**********0、绵阳低************/
/**********1、成都中************/
/**********2、北京高************/
/**********3、南京高**********/
/**********4、上海高************/
/**********5、郑州中************/
/**********6、德州低************/
/**********7、阜阳低************/
/**********8、大庆中************/
/**********9、鹰潭低********/
}lists;
typedef struct travellers
{
	int start_choice;/*选择的起点*/ 
	int arrive_choice;/*选择的终点*/ 
	int plan_choice;/*选择的旅行策略，0为最少风险策略，1为限时最少策略*/ 
	int timerequir;/*时间限制策略中允许的最大时间*/ 
 } travellers; 
ifstream in("a.txt");/*从a.txt中输入车次表*/ 
ofstream ou("client.txt");/*向client.txt中输出日志文件*/ 
int citythrough[50];//记录经过的城市 
int citycount=0; //记录经过的城市的数量 
lists tab[167];
int mainstack[15];
int coumainsta=1;//主栈城市的数量 
int cousupsta=0;//辅栈城市的数量 
float minrisk=9999;//最小风险值 
string number[50];//记录交通工具的序号 
int timespend[50];//每一小段路程所耗的时间 
int stopway[50]; //每个城市停留的时间 
int timesum=0;//所用的总时间 
int timesum1=0;
float systemhour;//记录系统时间 
float stt;
float temphour;
int flags=0;//标识是在旅途中还是停留在城市里 
int jishu=1;//用于日志输出时城市的计数 
int timescounts=0;//用于日志输出时时间的计数 
int sleeptime=10000;
mouse_msg msg = { 0 };
typedef struct gro
{
	int group[15];
	int len=10;
}gro;
typedef struct grous
{
	gro g[30];
}grous;
int jud(int x)/*用于查找函数路线的判断*/ 
{
	for(int i=0;i<coumainsta;i++)
	{
		if(x==mainstack[i])
		return 1;
	}
	return 0;
}
float cityrisk(int seq2)/*记录城市的风险值*/ 
{
	switch(tab[seq2].start_pos)
	{
		case 0:
		case 6:
		case 7:
		case 9:
			return 0.2;
		case 1:
		case 5:
		case 8:
		    return 0.5;
		case 2:
		case 3:
		case 4:
			return 0.9;
	}
}
float risksums(float sum,int seq2,int seq2before,int flag)/*完成附加功能，记录总的风险值*/ 
{
	float time1;
	float time2;
if(flag==1)
{
if(tab[seq2before].arrive_time>tab[seq2].start_time)
time2=(tab[seq2].start_time+24)-tab[seq2before].arrive_time;
else
time2=tab[seq2].start_time-tab[seq2before].arrive_time;
}
else time2=0;
if(tab[seq2].arrive_time<tab[seq2].start_time)
time1=(tab[seq2].arrive_time+24)-tab[seq2].start_time;
else
time1=tab[seq2].arrive_time-tab[seq2].start_time;
switch(tab[seq2].seq[0])
{
	case '0':
		sum=sum+time1*2.0*cityrisk(seq2)+time2*cityrisk(seq2);
		break;
	case '1':
		sum=sum+time1*5.0*cityrisk(seq2)+time2*cityrisk(seq2);
		break;
	case '2':
		sum=sum+time1*9.0*cityrisk(seq2)+time2*cityrisk(seq2);
		break;
}
	return sum;
}
void cityprint(int citythrough)/*根据城市的序号在命令行中输出城市的名称*/ 
{
	switch(citythrough)
	{
			case 0:
				cout<<"绵阳"<<endl;
				break;
			case 1:
				cout<<"成都"<<endl;
				break;
			case 2:
				cout<<"北京"<<endl;
				break;
			case 3:
				cout<<"南京"<<endl;
				break;
			case 4:
				cout<<"上海"<<endl;
				break;
			case 5:
				cout<<"郑州"<<endl;
				break;
			case 6:
				cout<<"德州"<<endl;
				break;
			case 7:
				cout<<"阜阳"<<endl;
				break;
			case 8:
				cout<<"大庆"<<endl;
				break;
			case 9:
				cout<<"鹰潭"<<endl;
				break;
	}

}
void logcityprint(int citythrough)/*根据城市的序号在日志中中输出城市的名称*/ 
{
	switch(citythrough)
	{
			case 0:
				ou<<"绵阳"<<endl;
				break;
			case 1:
				ou<<"成都"<<endl;
				break;
			case 2:
				ou<<"北京"<<endl;
				break;
			case 3:
				ou<<"南京"<<endl;
				break;
			case 4:
				ou<<"上海"<<endl;
				break;
			case 5:
				ou<<"郑州"<<endl;
				break;
			case 6:
				ou<<"德州"<<endl;
				break;
			case 7:
				ou<<"阜阳"<<endl;
				break;
			case 8:
				ou<<"大庆"<<endl;
				break;
			case 9:
				ou<<"鹰潭"<<endl;
				break;
	}
}
void graphcityprint(int citythrough,char *x)/*在图形界面中写出城市名称*/ 
{
	switch(citythrough)
	{
			case 0:
				strcpy(x,"绵阳");
				break;
			case 1:
				strcpy(x,"成都");
				break;
			case 2:
				strcpy(x,"北京");
				break;
			case 3:
				strcpy(x,"南京");
				break;
			case 4:
				strcpy(x,"上海");
				break;
			case 5:
				strcpy(x,"郑州");
				break;
			case 6:
				strcpy(x,"德州");
				break;
			case 7:
				strcpy(x,"阜阳");
				break;
			case 8:
				strcpy(x,"大庆");
				break;
			case 9:
				strcpy(x,"鹰潭");
				break;
	}
 } 
DWORD WINAPI interpret_key(LPVOID IpParamter)//声明的线程函数,用于动态查询的模块 
{
	char c=getchar();//获取键盘输入 
	if(c=='c')
	{
		cout<<"当前时刻为"<<(int)systemhour%24<<endl;
	if(systemhour-temphour<timespend[jishu]&&flags==0)
	{
		cout<<"当前在序号为"<<number[jishu]<<"的交通工具上"<<endl;
	}
    else if((systemhour-temphour<stopway[jishu]&&flags==1)||timescounts==timesum)
    {
    	cout<<"当前在";
    	cityprint(citythrough[jishu]);
	}
	else
	cout<<"现在交通工具和城市的交接处"<<endl;
	}
	if(c=='f')
	{sleeptime/=2;//加速 
	cout<<"游客速度加快1倍,现在延迟时间为"<<sleeptime<<"毫秒"<<endl;
    }
	if(c=='s')  
	{sleeptime*=2;//减速
	cout<<"游客速度减慢1倍,现在延迟时间为"<<sleeptime<<"毫秒"<<endl;
    }

}
void printcondition()/*日志文件输出模块*/ 
{
	systemhour=stt;
	temphour=systemhour;
	cout<<"游客已出发，若您需要查询此时刻游客的状态，请输入小写c字母即可查询,输入f可对游客加速1倍，输入s可对游客减速1倍，初始速度为现实10秒对应系统一小时"<<endl; 
	while(timescounts<=timesum)
	{
	  HANDLE hThread1 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/ 
	   HANDLE hThread2 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	    HANDLE hThread3 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	     HANDLE hThread4 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	      HANDLE hThread5 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	       HANDLE hThread6 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	        HANDLE hThread7 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	         HANDLE hThread8 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	          HANDLE hThread9 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	           HANDLE hThread10 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/
	            HANDLE hThread11 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/ 
	            HANDLE hThread12 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/ 
	            HANDLE hThread13 = CreateThread(NULL, 0, interpret_key, NULL, 0, NULL);/*创建的线程句柄*/ 
	ou<<"当前时刻为"<<(int)systemhour%24<<endl;
	if(systemhour-temphour<timespend[jishu]&&flags==0)/*flags为0，此时旅客在交通工具上*/ 
	{
		ou<<"当前在序号为"<<number[jishu]<<"的交通工具上"<<endl;
	}
	else if(systemhour-temphour>=timespend[jishu]&&flags==0)
	{
	  flags=1;
	  temphour=systemhour;
	  jishu++;
    }
    if((systemhour-temphour<stopway[jishu]&&flags==1)||timescounts==timesum)/*flags为0，此时乘客在城市中停留*/ 
    {
    	ou<<"当前在";
    	logcityprint(citythrough[jishu]);
	}
	else if(systemhour-temphour>=stopway[jishu]&&flags==1)
	{
		flags=0;
		ou<<"当前在序号为"<<number[jishu]<<"的交通工具上"<<endl;
		temphour=systemhour;
	}
	timescounts++;
	systemhour++;
	Sleep(sleeptime);/*睡眠函数*/ 
    }
    cout<<"游客已到达目的地，查询结束"<<endl; 
}
void leastrisk(int start,int end)/*最小风险策略函数*/ 
{
 mainstack[coumainsta]=start;
 int temp[15];
 grous grou;
 for(int i=0,j=0;i<10;i++,j++)
 {
 	if(i==start)
 	{
 		j--;
 		continue;
	 }
	 temp[j]=i;
 }
 temp[9]=-1;
 for(int i=0;i<10;i++)
 {
 	grou.g[cousupsta].group[i]=temp[i];
 }
 while(coumainsta!=0)
 { 
 	if(grou.g[cousupsta].group[0]!=-1)
 	{
 		mainstack[++coumainsta]=grou.g[cousupsta].group[0];
 		grou.g[cousupsta].len--;
 		for(int j=0;j<grou.g[cousupsta].len;j++)
 		{
 			grou.g[cousupsta].group[j]=grou.g[cousupsta].group[j+1];
		 }
 		
	 }
	 else
	 {  while(coumainsta>=1&&grou.g[cousupsta].group[0]==-1)
	 	{
		coumainsta--;
	 	cousupsta--;
	    }
	    if(coumainsta>0)
	    {
	    	mainstack[++coumainsta]=grou.g[cousupsta].group[0];
	    	grou.g[cousupsta].len--;
 		for(int j=0;j<grou.g[cousupsta].len;j++)
 		{
 			grou.g[cousupsta].group[j]=grou.g[cousupsta].group[j+1];
		 }
		}
	 }
	 for(int i=0,j=0;i<10;i++,j++)
   {
 	if(i==mainstack[coumainsta]||jud(i)==1)
 	 {
 		j--;
 		if(i==9)
 		{
		 temp[j]=-1;
		 grou.g[++cousupsta].len=j+1;
	    }
 		continue;
	 }
	 temp[j]=i;
	 if(i==9)
	 {
	  temp[j+1]=-1;
	  grou.g[++cousupsta].len=j+2;
     }
   }
 for(int i=0;i<grou.g[cousupsta].len;i++)
 {
 	grou.g[cousupsta].group[i]=temp[i];
 }
 if(mainstack[coumainsta]==end)/*此时已经找到了一个路线，现在来判断是否为最小风险的路线，如果是，则记录其路线、风险值和所消耗的时间*/ 
 {float risksum=0;
 int curren_num;
 int numbertemp=1;
 int timesumtemp=0;
 int timespendcount=1;
 int timestoptemp[50]={0};
 int timestopcount=2;
 string temp2[50];
 int timetemp[50];
 float sttp;
 	    if(coumainsta!=2) 
 	    {
		 for(int ks=1;ks<coumainsta;ks++)
 	    {
 		for(int r=0;r<166;r++)
 		{
 			if(tab[r].start_pos==mainstack[ks]&&tab[r].arrive_pos==mainstack[ks+1])
 			{ 
			     if(ks!=1)
				 {
				 risksum=risksums(risksum,r,curren_num,1);
			     }
			     else if(ks==1)risksum=risksums(risksum,r,r,0);
 				 temp2[numbertemp++]=tab[r].seq;
 				 if(tab[r].arrive_time>tab[r].start_time)
 				 timetemp[timespendcount++]=tab[r].arrive_time-tab[r].start_time;
 				 else
 				 timetemp[timespendcount++]=tab[r].arrive_time-tab[r].start_time+24;
 				 if(ks!=1)
				  {
				  	if(tab[r].start_time<tab[curren_num].arrive_time)
				  	timestoptemp[timestopcount++]=tab[r].start_time-tab[curren_num].arrive_time+24;
				  	else
				  	timestoptemp[timestopcount++]=tab[r].start_time-tab[curren_num].arrive_time;
				   } 
				  if(ks==1)
				  sttp=tab[r].start_time;
				 	curren_num=r;
 				 break;
			 }
		 }
		}
 		for(int sss=1;sss<coumainsta;sss++)
		{
			if(sss!=1)
			timesumtemp+=timestoptemp[sss];  
			timesumtemp+=timetemp[sss];
		}
		if(risksum<minrisk&&risksum>0)
	    {timesum=timesumtemp;
         minrisk=risksum;
         stt=sttp;
		for(int ss=1;ss<=coumainsta;ss++)
		{
			citythrough[ss]=mainstack[ss];
			if(ss!=coumainsta)
			{
			number[ss]=temp2[ss];
			timespend[ss]=timetemp[ss];
			 if(ss!=1)
		    {
		    	stopway[ss]=timestoptemp[ss];
			}
		    }

		}
				citycount=coumainsta; 
	    }
	    }
	 	coumainsta--;
	 	cousupsta--;
	 	while(coumainsta>=1&&grou.g[cousupsta].group[0]==-1)
	 	{
		coumainsta--;
	 	cousupsta--;
	    }
 }
}
}
void timelimit(int start,int end,int time)/*整体内容与leastrisk函数相似，但是多了一点点判断时间的内容*/ 
{
 mainstack[coumainsta]=start;
 int temp[15];
 grous grou;
 for(int i=0,j=0;i<10;i++,j++)
 {
 	if(i==start)
 	{
 		j--;
 		continue;
	 }
	 temp[j]=i;
 }
 temp[9]=-1;
 for(int i=0;i<10;i++)
 {
 	grou.g[cousupsta].group[i]=temp[i];
 }
 while(coumainsta!=0)
 { 
 	if(grou.g[cousupsta].group[0]!=-1)
 	{
 		mainstack[++coumainsta]=grou.g[cousupsta].group[0];
 		grou.g[cousupsta].len--;
 		for(int j=0;j<grou.g[cousupsta].len;j++)
 		{
 			grou.g[cousupsta].group[j]=grou.g[cousupsta].group[j+1];
		 }
 		
	 }
	 else
	 {  while(coumainsta>=1&&grou.g[cousupsta].group[0]==-1)
	 	{
		coumainsta--;
	 	cousupsta--;
	    }
	    if(coumainsta>0)
	    {
	    	mainstack[++coumainsta]=grou.g[cousupsta].group[0];
	    	grou.g[cousupsta].len--;
 		for(int j=0;j<grou.g[cousupsta].len;j++)
 		{
 			grou.g[cousupsta].group[j]=grou.g[cousupsta].group[j+1];
		 }
		}
	 }
	 for(int i=0,j=0;i<10;i++,j++)
   {
 	if(i==mainstack[coumainsta]||jud(i)==1)
 	 {
 		j--;
 		if(i==9)
 		{
		 temp[j]=-1;
		 grou.g[++cousupsta].len=j+1;
	    }
 		continue;
	 }
	 temp[j]=i;
	 if(i==9)
	 {
	  temp[j+1]=-1;
	  grou.g[++cousupsta].len=j+2;
     }
   }
 for(int i=0;i<grou.g[cousupsta].len;i++)
 {
 	grou.g[cousupsta].group[i]=temp[i];
 }
 if(mainstack[coumainsta]==end)
 {float risksum=0;
 int curren_num;
 int numbertemp=1;
 int timesumtemp=0;
 int timespendcount=1;
 int timestoptemp[50];
 int timestopcount=2;
 string temp2[50];
 int timetemp[50];
 float sttp;
 		if(coumainsta!=2) 
 	    {
		 for(int ks=1;ks<coumainsta;ks++)
 	    {
 		for(int r=0;r<166;r++)
 		{
 			if(tab[r].start_pos==mainstack[ks]&&tab[r].arrive_pos==mainstack[ks+1])
 			{ 
			     if(ks!=1)
				 {
				 risksum=risksums(risksum,r,curren_num,1);
			     }
			     else if(ks==1)risksum=risksums(risksum,r,r,0);
 				 temp2[numbertemp++]=tab[r].seq;
 				 if(tab[r].arrive_time>tab[r].start_time)
 				 timetemp[timespendcount++]=tab[r].arrive_time-tab[r].start_time;
 				 else
 				 timetemp[timespendcount++]=tab[r].arrive_time-tab[r].start_time+24;
 				  if(ks!=1)
				  {
				  	if(tab[r].start_time<tab[curren_num].arrive_time)
				  	timestoptemp[timestopcount++]=tab[r].start_time-tab[curren_num].arrive_time+24;
				  	else
				  	timestoptemp[timestopcount++]=tab[r].start_time-tab[curren_num].arrive_time;
				   } 
				   if(ks==1)
				   sttp=tab[r].start_time;
				    curren_num=r;
 				 break;
			 }
		}
		}
 		for(int sss=1;sss<coumainsta;sss++)
		{
			if(sss!=1)
			timesumtemp+=timetemp[sss]+timestoptemp[sss];
			else
			timesumtemp+=timetemp[sss];
		}
		if(timesumtemp<=time)/*判断时间是否满足乘客的要求，如果满足则再判断是否是最小风险，否则直接略去*/ 
	    {
		if(risksum<minrisk&&risksum>0)
	    {timesum=timesumtemp;
	    stt=sttp;
         minrisk=risksum;
		for(int ss=1;ss<=coumainsta;ss++)
		{
			citythrough[ss]=mainstack[ss];
			if(ss!=coumainsta)
			{
			number[ss]=temp2[ss];
			timespend[ss]=timetemp[ss];
		    }
		    if(ss!=1)
		    {
		    	stopway[ss]=timestoptemp[ss];
			}
		}
				citycount=coumainsta; 
	    }
	    }
	    }
	 	coumainsta--;
	 	cousupsta--;
	 	while(coumainsta>=1&&grou.g[cousupsta].group[0]==-1)
	 	{
		coumainsta--;
	 	cousupsta--;
	    }
	}
 }
}
void printway()/*线路输出模块*/ 
{cout<<"您的线路为："<<endl; 
	for(int i=2;i<=citycount;i++)
	{
		if(i==2)cout<<"先";
		else if(i==citycount)cout<<"最后";
		else cout<<"然后"; 
		cout<<"到达了";
		cityprint(citythrough[i]);
		cout<<"乘搭交通的序号为"<<number[i-1]<<endl; 
		cout<<"这段旅途耗时为"<<timespend[i-1]<<"小时"<<endl; 
		if(i!=citycount)
		cout<<"在车站等待了"<<stopway[i]<<"小时"<<endl;
	}
	cout<<"总共用时为"<<timesum<<"小时"<<endl;
}
void graphcity(int citythrough,int &positionx,int &positiony)/*根据城市的序号得到城市在图形界面中的位置*/ 
{
	switch(citythrough)
	{
			case 0:
				positionx=219;
				positiony=240;
				break;
			case 1:
				positionx=209;
				positiony=245;
				break;
			case 2:
			    positionx=299;
				positiony=157;
				break;
			case 3:
				positionx=330;
				positiony=206;
				break;
			case 4:
				positionx=340;
				positiony=228;
				break;
			case 5:
				positionx=292;
				positiony=210;
				break;
			case 6:
				positionx=318;
				positiony=186;
				break;
			case 7:
				positionx=311;
				positiony=220;
				break;
			case 8:
				positionx=359;
				positiony=82;
				break;
			case 9:
				positionx=309;
				positiony=268;
				break;
	}
}
void setbar(PIMAGE img1)/*绘制图形界面中的各个功能模块*/ 
{
	getimage(img1, "1.jpg", 500, 500);
 	putimage(0, 0, img1);
 	setbkcolor(WHITE);
 	setfillcolor(RED);
 	bar(50,400,150,450);
 	setfont(25, 0, "幼圆");
	outtextxy(50,420,"风险最少");
	setfillcolor(RED);
 	bar(300,400,450,450);
 	setfont(25, 0, "幼圆");
	outtextxy(300,420,"限时风险最少"); 
	setfillcolor(RED);
	bar(450,50,530,100);
	setfont(25, 0, "幼圆");
	outtextxy(450,60,"关闭");
	setfont(25, 0, "幼圆");
	outtextxy(450,136,"时刻：");
	setfont(25, 0, "幼圆");
	outtextxy(458,195,"当前在：");
	setfont(25, 0, "幼圆");
	outtextxy(151,360,"请点击您选择的策略");
} 
void draw(travellers tra)/*进行总体的图形绘制*/ 
{
	 	 	initgraph(620, 500);/*初始化图形*/ 
 	int positionx1;//图形界面中城市的横坐标 
    int positiony1;//图形界面中城市的纵坐标 
    int positionx2;//图形界面中城市的横坐标 
    int positiony2;//图形界面中城市的纵坐标 
    int cjishu=1; 
 	PIMAGE img1 = newimage();
 	setbar(img1);
	int flagg=0;
	int flagss=0;
	char timestr[5];
	char citystr[20]; 
	char *numberstring=new char[10];
    for (; is_run(); delay_fps(60))
    {
    		while (mousemsg())
		{
			msg = getmouse(); //时刻记录鼠标的动作 
		}
		if (msg.is_left() && msg.is_down())/*检测鼠标的点击*/ 
		{
			if (msg.x >= 50 && msg.y >= 400 && msg.x <= 150 && msg.y <= 450&&flagg==0) //风险最少 
			{
			leastrisk(tra.start_choice,tra.arrive_choice);
			cout<<"最小风险数值为"<<minrisk<<endl;
			printway(); 
			flagg=1;}
			if(msg.x >= 300 && msg.y >= 400 && msg.x <= 450 && msg.y <= 450&&flagg==0)//限时风险最少
			{cout<<"您选择的为限时最少风险策略,请输入您允许的最大时间:"<<endl;
			  cin>>tra.timerequir;
			  timelimit(tra.start_choice,tra.arrive_choice,tra.timerequir);
 	          if(minrisk==9999)
	          cout<<"您的允许时间太短，无相应线路"<<endl; 
	          else
	          {
	          	cout<<"最小风险数值为"<<minrisk<<endl;
			    printway(); 
			  }
			flagg=1;
		    }
		    if(flagg==1)
		    {
		    systemhour=stt;
			int timespendcount=0;
			graphcity(citythrough[cjishu],positionx1,positiony1);
			graphcity(citythrough[cjishu+1],positionx2,positiony2);
			int xposition=abs(positionx1-positionx2)/timespend[cjishu];
			int yposition=abs(positiony1-positiony2)/timespend[cjishu];
			while(timescounts<timesum)
			{
			itoa((int)systemhour%24,timestr,10);
			outtextxy(500,136,timestr);
			if(flagss==1)
			{graphcityprint(citythrough[cjishu],citystr);
				outtextxy(460,230,citystr); 
				outtextxy(460,300,"下一站：");
				graphcityprint(citythrough[cjishu+1],citystr);
				outtextxy(460,330,citystr);
			}
			if(flagss==0)
			{
				outtextxy(460,230,"序号为");
				numberstring=(char *)number[cjishu].data();
				outtextxy(544,230,numberstring);
				outtextxy(460,260,"的交通工具上");
				outtextxy(460,300,"下一站：");
				graphcityprint(citythrough[cjishu+1],citystr);
				outtextxy(460,330,citystr);
				
			}
			setcolor(RED);//图形外面是红色的边框 
            circle(positionx1,positiony1,5);
            if(flagss==1)
            {
            	timespendcount++;
            	if(timespendcount>=stopway[cjishu])
            	{
            		timespendcount=0;
            		flagss=0;
				}
			}
            if(flagss==0)
			{
			if(positionx1<positionx2)
			{
			positionx1+=xposition;
		    }
			else
			positionx1-=xposition;
			if(positiony1<positiony2)
			positiony1+=yposition;
			else
			positiony1-=yposition;
			timespendcount++;
		    }
			if(timespendcount>timespend[cjishu]&&flagss==0)
			{
				timespendcount=0;
				cjishu++;
				graphcity(citythrough[cjishu],positionx1,positiony1);
				graphcity(citythrough[cjishu+1],positionx2,positiony2);
				 xposition=abs(positionx1-positionx2)/timespend[cjishu];
			     yposition=abs(positiony1-positiony2)/timespend[cjishu];
			     flagss=1;
			     timescounts--;
			}
			Sleep(1000);
			cleardevice();
	        setbar(img1);
			if(timescounts==timesum-1)
			{
			  circle(positionx1,positiony1,5);
			  graphcityprint(citythrough[cjishu+1],citystr);
			  outtextxy(460,230,citystr);
			  outtextxy(460,260,"到达目的地");
			itoa((int)systemhour%24,timestr,10);
			outtextxy(500,136,timestr);
		    }
		    			timescounts++;
			systemhour++;
		    }
		}
		}
			if (msg.x >= 450 && msg.y >= 50 && msg.x <=530 && msg.y <= 100)/*点击关闭按钮关闭图形*/ 
 	{if(msg.is_left()){
				closegraph();
			}
	 }
	}
 }  
int main()/*主函数*/ 
{
   int is=0;
   if(in!=0)
{
    while(in.eof()==false)/*将txt文件中的数据记录到结构体中*/ 
    {
        in>>tab[is].seq>>tab[is].start_pos>>tab[is].start_time>>tab[is].arrive_pos>>tab[is].arrive_time;
        is++;
    }
}
travellers tra;
cout<<"请选择你出发的起点，你达到的终点以及你的旅行策略和查询方式（0/最少风险，1/限时最少风险，2/图像查询）"<<endl;
ou<<"你出发的起点，你达到的终点以及你的旅行策略为"<<endl;
cin>>tra.start_choice>>tra.arrive_choice>>tra.plan_choice;
ou<<tra.start_choice<<" "<<tra.arrive_choice<<" "<<tra.plan_choice<<endl;
if(tra.plan_choice==0)/*选择最少风险策略*/ 
{cout<<"您选择的为最少风险策略"<<endl; 
	leastrisk(tra.start_choice,tra.arrive_choice);
	cout<<"最小风险数值为"<<minrisk<<endl;
	printway();/*输出线路*/ 
	printcondition(); /*日志输出和动态查询*/ 
 } 
 else if(tra.plan_choice==1)/*选择限时最小风险策略*/ 
 {cout<<"您选择的为限时最少风险策略,请输入您允许的最大时间:"<<endl;
 ou<<"您允许的最大时间为";
  cin>>tra.timerequir; 
  ou<<tra.timerequir<<endl;
 	timelimit(tra.start_choice,tra.arrive_choice,tra.timerequir);
 	if(minrisk==9999)
	 cout<<"您的允许时间太短，无相应线路"<<endl;
	 else 
 	 {	
		 	cout<<"最小风险数值为"<<minrisk<<endl;
	printway();/*输出线路*/
	printcondition();/*日志输出和动态查询*/ 
	} 
 }
 else if(tra.plan_choice==2) /*通过图形显示游客的旅行线路*/ 
 {
	cout<<"您选择了图像查询旅行策略"<<endl; 
	draw(tra);/*根据旅客选择绘制图形*/ 

 }
 else
  	cout<<"选择的旅行策略只能为0或1或2，请重新输入"<<endl;/*策略选择输入错误，需要重新输入策略*/
 in.close();/*关闭文件*/ 
 ou.close();
	return 0;
 }

