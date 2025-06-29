#include<windows.h>
#include<bits/stdc++.h>
#include<conio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>
//#define float double
using namespace std;
struct skilll{
	int num;
	string name;
	string type;
	int sp;
	string cmd[100];
	string con;
};
struct effectt{
	string name;
	int dur;
	int stack;
	int maxstack;
	string cmd[100];
	
};
struct str{
	string id;
	string name;
	float nhp;
	int hp;
	int att;
	int def;
	int spd;
	float php;
	float patt;
	float pdef;
	float pspd;
	float crate;
	float cdmg;
	float breakeff;
	float effhit;
	float effres;
	float energy;
	float maxenergy;
	float err;
	float dmgboost;
	int team;
	int skinum;
	bool dead=0;
	skilll skill[10];
	effectt eff[100];
	effectt buff[100];
	effectt debuff[100];
};
str unit[12];
float act[12],dist[12];
int cur=0,sp=3;
int nxt=0,mode,target,tact=0,last=0,maxsp=5,nowult=0,lasttar=1;
const int n=10;
string filename;
void read(int num),show(int num),damage(),step(),start(),speed(),att(int t,int val),display(bool flag),detail(int num,int x,bool flag),show_queue(int x);
void ult(int n);
bool operations(char s);
bool command(string s);
void removeNewline(string& s) {
    if (!s.empty() && s.back() == '\n') {
        s.pop_back(); // 删除最后一个字符（换行符）
    }
}
void gotoxy(int x,int y) 
{
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
	return 0;
} 
bool sameteam(int a,int b)
{
	return unit[a].team==unit[b].team;
}
bool disable(int t,int n)
{
	string temp=unit[n].skill[mode].type;
	if(unit[t].dead) return 1; 
	return (sameteam(t,n)&&(temp=="单攻"||temp=="群攻"||temp=="扩散"||temp=="弹射"||temp=="妨害"))||(!sameteam(t,n)&&(temp=="回复"||temp=="辅助"||temp=="防御"));
}
float thp(int num)
{
	return unit[num].hp*(1+unit[num].php/100);
}
float tspd(int num)
{
	return unit[num].spd*(1+unit[num].pspd/100.0);
}
float tatt(int num)
{
	return unit[num].att*(1+unit[num].patt/100.0);
}
float tdef(int num)
{
	return unit[num].def*(1+unit[num].pdef/100.0);
}
string divide(string s,int x)
{
	int i;
	string part="";
	for(i=0;;i++)
	{
		if(s[i]==' ') i++,x--;
		if(x==1) break;
	}
	for(;;i++)
	{
		if(s[i]==' '||i==s.size()) break;
		part+=s[i];
	}
	return part;
}
int str_to_int(string s)
{
	int flag=0;
	if(s[0]=='-') flag=1;
	int val=0;
	for(int i=flag;i<s.size();i++)
	{
		if(s[i]<'0'||s[i]>'9') continue;
		val*=10;
		val+=s[i]-48;
	}
	if(flag) val=0-val;
	return val;
}
void in(auto& a)
{
	string s;
	cin>>s;
	//cout<<s;
	cin>>a;
}
void read(int num)
{
	in(unit[num].name);
	in(unit[num].hp);
	in(unit[num].att);
	in(unit[num].def);
	in(unit[num].spd);
	in(unit[num].php);
	unit[num].php=unit[num].php*100.0/unit[num].hp;
	in(unit[num].patt);
	unit[num].patt=unit[num].patt*100.0/unit[num].att;
	in(unit[num].pdef);
	unit[num].pdef=unit[num].pdef*100.0/unit[num].def;
	in(unit[num].pspd);
	unit[num].pspd=unit[num].pspd*100.0/unit[num].spd;
	in(unit[num].crate);
	in(unit[num].cdmg);
	in(unit[num].breakeff);
	in(unit[num].effhit);
	in(unit[num].effres);
	in(unit[num].maxenergy);
	in(unit[num].err);
	in(unit[num].dmgboost);
	in(unit[num].team);
	string s,temp;
	unit[num].nhp=unit[num].hp+unit[num].hp*unit[num].php/100;
	if(num>4) return;
	for(int i=0;;i++)
	{
		in(unit[num].skill[i].name);
		if(unit[num].skill[i].name=="NULL")
		{
			unit[num].skill[i].con="NOOOOO";
			continue;
		}
		else if(i>=3) in(unit[num].skill[i].con);
		in(unit[num].skill[i].type);
		in(unit[num].skill[i].sp);
		//cout<<1<<endl; 
		unit[num].skinum=i;
		for(int j=0;;j++)
		{
			getline(cin,s);
			removeNewline(s);
			if(s=="EOF") break;
			if(s=="end") break;
			unit[num].skill[i].cmd[j]=s;
			unit[num].skill[i].num=j;
		}
		if(s=="EOF") break;
	}
}
void ski()
{
	cur=nxt;
    cout<<"*"<<"使用了【"<<unit[cur].skill[mode].name<<"】"<< endl;
    for(int j=1;j<=unit[cur].skill[mode].num;j++)
    {
    	command(unit[cur].skill[mode].cmd[j]);
	}
}
void show(int num)
{
	cout<<"编号："<<num<<endl;
	cout<<"名字："<<unit[num].name<<endl;
	cout<<"生命："<<unit[num].hp<<"+"<<unit[num].hp*unit[num].php/100<<endl;
	cout<<"攻击："<<unit[num].att<<"+"<<unit[num].att*unit[num].patt/100<<endl;
	cout<<"防御："<<unit[num].def<<"+"<<unit[num].def*unit[num].pdef/100<<endl;
	cout<<"速度："<<unit[num].spd<<"+"<<unit[num].spd*unit[num].pspd/100<<endl;
	cout<<"暴击："<<unit[num].crate<<endl;
	cout<<"爆伤："<<unit[num].cdmg<<endl;
	cout<<"阵营："<<unit[num].team<<endl;
	for(int i=0;i<=unit[num].skinum;i++)
	{
		cout<<"技能："<<unit[num].skill[i].name<<endl;
		for(int j=1;j<=unit[num].skill[i].num;j++)
		{
			cout<<unit[num].skill[i].cmd[j]<<endl;
		}
	}
	cout<<endl;
}
// 修改show_queue函数中的最小值查找逻辑
void show_queue(int x)
{
    float temp_act[18];
    for (int i = 0; i < n; i++) {
        temp_act[i] = act[i];
    }
    vector<pair<int, int>> queue;
    int steps = 0;
    while (steps < 15) {
        float min_act = 99999;
        int current_nxt = -1;
        
        for (int i = 0; i < n; i++) {
            if (unit[i].name.empty() || unit[i].dead) continue;
            if (temp_act[i] < min_act ) {
                min_act = temp_act[i];
                current_nxt = i;
            }
        }

        if (current_nxt == -1) break;

        queue.push_back({current_nxt, min_act});
        steps++;

        // 原有更新逻辑保持不变
        temp_act[current_nxt] += 10000.0 / tspd(current_nxt);
	}
    // 显示部分保持不变...
    int y=0;
	color(15);
	gotoxy(x,y++);
	cout<<"行动序列"<<endl;
    for (int i = 0; i < queue.size(); i++)
	{
		gotoxy(x,y++);
		if(queue[i].first<=4) color(10);
		else color(4);
        cout<<unit[queue[i].first].name<<" ";
        color(15);
		cout<<queue[i].second <<" ";
		if(queue[i].first==target) cout<<"←";
		cout<<"           "<<endl; 
    }
}
void speed(string s)
{
	string part="";
	int i,no,val,mode;
	for(i=0;;i++)
	{
		if(s[i]==' ') break;
		if(i==s.size())
		{
			cout<<"指令错误"<<endl;
			return;
		}
		part+=s[i];
	}
	i++;
	no=str_to_int(part);
	part="";
	for(;;i++)
	{
		if(s[i]==' ') break;
		if(i==s.size())
		{
			cout<<"指令错误"<<endl;
			return;
		}
		part+=s[i];
	}
	i++;
	if(part=="set") mode=1;
	else if(part=="add") mode=2;
	else
	{
		cout<<"指令错误"<<endl;
		return;
	}
	part="";
	for(;i<s.size();i++)
	{
		part+=s[i];
	}
	val=str_to_int(part);
	cout<<"已将"<<unit[no].name<<"的速度";
	if(mode==1) 
	{
		cout<<"设置";
		unit[no].spd=val;
	}
	else 
	{
		cout<<"增加";
		unit[no].spd+=val;
	}
	cout<<val<<endl;
	
}
vector<int> parse_target(const string& t, int main_target) {
    vector<int> targets;
    
    // 解析基础目标
    if (t == "s") { // 自身
        targets.push_back(nxt);
    } else if (t == "t") { // 主目标
        targets.push_back(main_target);
    } else if (t == "c") { // 我方全体
        for (int i=0; i<=4; i++) {
            if (!unit[i].name.empty() && !unit[i].dead) targets.push_back(i);
        }
    } else if (t == "e") { // 敌方全体
        for (int i=5; i<=9; i++) {
            if (!unit[i].name.empty() && !unit[i].dead) targets.push_back(i);
        }
    }
    // 新增：随机选择敌方目标（rX格式，如 r3）
    else if (t.size() >= 1 && t[0] == 'r') {
        // 提取随机次数
        string num_str = t.substr(1);
        int count = str_to_int(num_str);
        
        // 收集有效敌方目标
        vector<int> valid_enemies;
        for (int i=5; i<=9; i++) {
            if (!unit[i].name.empty() && !unit[i].dead) {
                valid_enemies.push_back(i);
            }
        }
        
        // 随机选择 count 次（允许重复）
        if (!valid_enemies.empty()) {
            for (int i=0; i<count; i++) {
                int idx = rand() % valid_enemies.size();
                targets.push_back(valid_enemies[idx]);
            }
        }
    }
    // 解析相对目标（如 t-1/t+1 或 s-1/s+1）
     else if (t.find("t") == 0 || t.find("s") == 0) {
        char base_type = t[0]; // 't' 或 's'
        int base = (base_type == 't') ? main_target : nxt;
        
        // 提取偏移量（例如 "t-1" 中的 -1）
        string offset_str = t.substr(1);
        int delta = str_to_int(offset_str);
        
        // 确定方向和需要跳过的合法目标数量
        int direction = (delta > 0) ? 1 : -1;
        int steps_needed = abs(delta);
        
        int current_pos = base; // 从基准位置开始
        while (steps_needed > 0) {
            current_pos += direction; // 移动一步
            
            // 边界检查
            if (current_pos < 5 || current_pos >= n) {
                break; // 越界，停止搜索
            }
            
            // 检查当前位置是否合法（存在且未死亡）
            if (!unit[current_pos].name.empty() && !unit[current_pos].dead) {
                steps_needed--;
                
                // 当找到足够合法目标时，记录最终位置
                if (steps_needed == 0) {
                    targets.push_back(current_pos);
                }
            }
        }
    }
    // 直接数字目标
    else if (isdigit(t[0])) {
        int target = str_to_int(t);
        if (target >=0 && target <n) targets.push_back(target);
    }

    // 过滤无效目标（空单位/死亡单位）
    vector<int> valid_targets;
    for (int t : targets) {
        if (t >=0 && t <n && !unit[t].name.empty() && !unit[t].dead) {
            valid_targets.push_back(t);
        }
    }
    return valid_targets;
}
void ult(int n)
{
	int temp1=nxt,temp2=mode;
	if(nxt>4) target=lasttar;
	last=nxt;
	nxt=n;
	mode=2; 
	nowult=n;
	display(0);
	system("cls");
	display(0);
	color(15);
	cout<<unit[n].name<<"          "<<endl;
	cout<<">终结技";
	cout<<"("<<unit[n].skill[mode].type<<")     "<<endl;
	cout<<"【"<<unit[n].skill[mode].name<<"】                  "<<endl;
	cout<<">目标："<<target<<"  "<<unit[target].name<<"                   "<<endl;
	char c; 
	do
	{
		display(0);
		color(15);
		cout<<unit[nxt].name;
		cout<<"的终结技";
		color(10);
		cout<<"(激活)        "<<endl;
		color(15);
		c=getch();	
	}while(operations(c));
	unit[n].energy=0; 
	for(int i=0;i<3;i++) cout<<"                     "<<endl;
	for(int i=0;i<n;i++)
	{
		if(unit[i].nhp<=0)
		{
			unit[i].nhp=0;
			unit[i].dead=1;
		} 
	}
	if(nxt<=4&&nxt!=0) lasttar=target;
	sp+=unit[nxt].skill[mode].sp;
	if(sp>maxsp) sp=maxsp;
	display(1);
	color(15);
	cout<<unit[nxt].name<<"的终结技";
	color(9);
	cout<<"(结算)     "<<endl;
	color(15);
	char s=getch();
	while(s!=VK_RETURN)
	{
		if((s=='w'||s=='a')&&target>1)
		{
			target--;
		}
		else if((s=='s'||s=='d')&&target<9)
		{
			target++;
		}
		lasttar=target;
		display(1);
		s=getch();
	}
	system("cls");
	nxt=temp1;
	mode=temp2;
	nowult=-1;
	if(nxt<=4&&nxt!=0)
	{
		target=lasttar;
		display(0);
	}
	else
	{
		target=0;
		display(0);
	}
	if(nxt<=4&&nxt!=0)
	{
		if(mode==1) cout<<">战技";
		else cout<<">普攻";
		cout<<"("<<unit[nxt].skill[mode].type<<")        "<<endl;
		cout<<"【"<<unit[nxt].skill[mode].name<<"】                  "<<endl;
		cout<<">目标："<<target<<"  "<<unit[target].name<<"                   "<<endl;
	}
}
void att(int t,int val)
{
	float dmg,base,defpart,respart,breakpart,boostpart,critpart,weakpart;
	bool crit;
	base=tatt(nxt)*val/100.0;
	defpart=1-(tdef(t)/(tdef(t)+80*10+200));
	respart=1.0;
	breakpart=0.9;
	boostpart=1.0;
	weakpart=1.0;
	critpart=1.0;
	if((rand()) / (RAND_MAX + 1.0) < unit[nxt].crate / 100.0) 
	{
		crit=1;
		critpart=unit[nxt].cdmg/100.0+1;
	}
	else crit=0;
	dmg=base*defpart*respart*breakpart*boostpart*weakpart*critpart;
	cout<<">对"<<unit[t].name<<"造成"<<(int)dmg<<"点伤害（";
	if(!crit) cout<<"未";
	cout<<"暴击）"<<endl; 
	unit[t].nhp-=dmg;
	if(unit[t].nhp<=0)
	{
		unit[t].nhp=0;
	} 
}
void charge(int t,int val)
{
	unit[t].energy+=val*unit[t].err/100.0;
	if(unit[t].energy>unit[t].maxenergy) unit[t].energy=unit[t].maxenergy;
} 
// 命令处理函数
bool command(string s) {
	string target_str;
	int value;
    istringstream iss(s);
    string operation;
    iss >> operation;
    iss >> target_str;
    iss >> value;
    // 默认主目标为当前目标变量（假设已维护）
    int main_target = target; 
    vector<int> targets = parse_target(target_str, main_target);
    vector<string> tags; // 存储所有标签
    
    // 剩余部分作为标签（支持多标签）
    string tag;
    while (iss >> tag) {
        tags.push_back(tag);
    }
    for (const string& tag : tags)
	{
        if (tag == "t!=s")
		{
            for (int t : targets)
			{
                if (t == nxt)
				{
                    return true;
                }
            }
        } 
        else if (tag.find("team=") != string::npos)
		{
        }
    }
    if (operation == "empty") {
        cout <<">"<< "（命令未实装）" << endl;
    } 
    else if (operation == "att") {
        for (int t : targets) {
            att(t, value); 
        }
    } 
    else if (operation == "charge") {
        for (int t : targets) {
            charge(t, value);
        }
    }
    else if (operation == "pull") {
        for (int t : targets) {
            cout <<">"<< unit[t].name << "行动提前" << value<<"%      " << endl;
            dist[t]-=100.0*value;
        }
	}
    else if (operation == "eff") {
        string effect;
        iss >> effect;
        // 调用效果函数（示例）
        cout << "[EFF] 目标数:" << targets.size() 
             << " 效果:" << effect 
             << " 数值:" << value << endl;
        // eff(targets, effect, value);
    }
    
    return true;
}
bool operations(char s)
{
	const int loc=17; 
	if(s>='1'&&s<='4')
	{
		if(unit[s-48].energy!=unit[s-48].maxenergy)
		{
			gotoxy(0,loc);
			color(4);
			cout<<"能量不足";
			while(!getch());
			gotoxy(0,loc);
			cout<<"              ";
			return 1;
		}
		if(nowult!=-1)
		{
			gotoxy(0,loc);
			color(4);
			cout<<"请先释放当前终结技";
			while(!getch());
			gotoxy(0,loc);
			cout<<"                   ";
			return 1;
		}
		ult(s-48);
		return 1;
	}
	if(nxt<=4&&nxt!=0)
	{
		if(s=='q')
		{
			mode=0;
		}
		else if(s=='e')
		{
			mode=1;
			if(sp+unit[nxt].skill[mode].sp<0)
			{
				mode=0;
				gotoxy(0,loc);
				color(4);
				cout<<"战技点不足";
				while(!getch());
				gotoxy(0,loc);
				cout<<"                 ";
				return 1;
			}
		}
		else if((s=='w'||s=='a')&&target>1)
		{
			target--;
		}
		else if((s=='s'||s=='d')&&target<9)
		{
			target++;
		}
		else if(s==VK_RETURN)
		{
			if(disable(target,nxt))
			{
				gotoxy(0,loc);
				color(4);
				cout<<"无法对该目标释放";
				while(!getch());
				gotoxy(0,loc);
				cout<<"                 ";
				return 1;
			}
			ski();
			return 0;
		}
		if(mode==0) cout<<">普攻";
		else if(mode==1) cout<<">战技";
		else if(mode==2) cout<<">终结技";
		else if(mode==3) cout<<">强化普攻";
		else if(mode==4) cout<<">强化战技";
		else cout<<"error!"; 
		cout<<"("<<unit[nxt].skill[mode].type<<")      "<<endl;
		cout<<"【"<<unit[nxt].skill[mode].name<<"】                  "<<endl;
		cout<<">目标："<<target<<"  "<<unit[target].name<<"                   "<<endl;
		return 1;
	} 
	else if(s==VK_RETURN)
	{
		if(nxt>4||nxt==0) 
		{
			return 0;
		}
	}
	return 1;
}
void step()
{
	dist[nxt]=10000.00;
	for(int i=0;i<n;i++)
	{
		if(dist[i]<0) dist[i]=0;
		if(unit[i].name==""||unit[i].dead) continue;
		act[i]=dist[i]/tspd(i);
	}
//	act[nxt]=dist[nxt]/(unit[nxt].spd*(1+unit[nxt].pspd/100.0));
	last=nxt;
	nxt=0;
	float minn=999999;
	for(int i=0;i<n;i++)
	{
		if(unit[i].name==""||unit[i].dead) continue;
		if(minn>act[i])
		{
			minn=act[i];
			nxt=i;
		}
	}
	float temp=act[nxt];
//	cout<<unit[nxt].name<<"的回合"<<endl;
	for(int i=0;i<n;i++)
	{
		act[i]-=temp;
		dist[i]-=tspd(i)*1.0*temp;
	}
	tact+=temp;
}
void start()
{
	for(int i=0;i<n;i++)
	{
		if(unit[i].name==""||unit[i].dead) continue;
		dist[i]=10000.0;
		unit[i].energy=unit[i].maxenergy*0.5;
		act[i]=dist[i]*1.0/tspd(i);
	}
	cout<<"战斗开始"<<endl; 
	step();
	system("cls");
	char c;
	lasttar=1;
	nowult=-1;
	for(;;)
	{
		for(;;)
		{
			if(nxt<=4&&nxt!=0)
			{
				mode=0;
				target=lasttar;
				display(0);
				color(15);
				cout<<unit[nxt].name<<"的回合        "<<endl;
				cout<<">普攻";
				cout<<"("<<unit[nxt].skill[mode].type<<")"<<endl;
				cout<<"【"<<unit[nxt].skill[mode].name<<"】                  "<<endl;
				cout<<">目标："<<target<<"  "<<unit[target].name<<"                   "<<endl;
			}
			else
			{
				mode=3;
				target=0;
			}
			do
			{
				display(0);
				color(15);
				cout<<unit[nxt].name<<"的回合";
				color(10);
				cout<<"(行动)        "<<endl;
				color(15);
				c=getch();	
			}while(operations(c));
			for(int i=0;i<3;i++) cout<<"                     "<<endl;
			for(int i=0;i<n;i++)
			{
				if(unit[i].nhp<=0)
				{
					unit[i].nhp=0;
					unit[i].dead=1;
				} 
			}
			if(nxt<=4&&nxt!=0) lasttar=target;
			sp+=unit[nxt].skill[mode].sp;
			if(sp>maxsp) sp=maxsp;
			step();
			display(1);
			color(15);
			cout<<unit[last].name<<"的回合";
			color(9);
			cout<<"(结算)     "<<endl;
			color(15);
			//gotoxy(0,17);
			char s=getch();
			while(s!=VK_RETURN)
			{
				if(last<=4&&last!=0)
				{
					if((s=='w'||s=='a')&&target>1)
					{
						target--;
					}
					else if((s=='s'||s=='d')&&target<9)
					{
						target++;
					}
					lasttar=target;
					display(1);
				}
				s=getch();
			}
			system("cls");
		}
		
	}
}
void detail(int num,int x,bool flag)
{
	string space="         ";
	int y=0;
	gotoxy(x,y++);
	if(((unit[nxt].team!=1&&!flag)||(unit[last].team!=1&&flag))&&nowult==-1)
	{
		color(4);
		cout<<"敌方回合";
		return; 
	} 
	color(15);
	cout<<num<<" "<<unit[num].name<<space<<endl;
	gotoxy(x,y++);
	if(unit[num].team==1) color(9);
	else color(4);
	cout<<(int)unit[num].nhp<<"/"<<(int)thp(num)<<space;
	gotoxy(x,y++);
	if(unit[num].team==1)
	{
		color(10);
		cout<<"能量："<<unit[num].energy<<"/"<<unit[num].maxenergy<<space;
	}
	else
	{
		color(6);
		cout<<"韧性："<<unit[num].energy<<"/"<<unit[num].maxenergy<<space;
	}
	color(15);
	gotoxy(x,y++);
	cout<<"生命："<<unit[num].hp<<"+"<<(int)unit[num].hp*unit[num].php/100<<space;
	gotoxy(x,y++);
	cout<<"攻击："<<unit[num].att<<"+"<<(int)unit[num].att*unit[num].patt/100<<space;
	gotoxy(x,y++);
	cout<<"防御："<<unit[num].def<<"+"<<(int)unit[num].def*unit[num].pdef/100<<space;
	gotoxy(x,y++);
	cout<<"速度："<<unit[num].spd<<"+"<<(int)unit[num].spd*unit[num].pspd/100<<space;
	gotoxy(x,y++);
	cout<<"暴击："<<unit[num].crate<<space;
	gotoxy(x,y++);
	cout<<"爆伤："<<unit[num].cdmg<<space;
	gotoxy(x,y++);
	cout<<"击破特攻："<<unit[num].breakeff<<space;
	gotoxy(x,y++);
	cout<<"效果命中："<<unit[num].effhit<<space;
	gotoxy(x,y++);
	cout<<"效果抵抗："<<unit[num].effres<<space;
	gotoxy(x,y++);
	cout<<"回能速率："<<unit[num].err<<space;
	gotoxy(x,y++);
	cout<<"属性增伤："<<unit[num].dmgboost<<space;
}
void display(bool flag)
{
	detail(target,30,flag);
	show_queue(50);
	gotoxy(0,0);
	for(int i=1;i<n;i++)
	{
		if((last==i&&flag)||(nxt==i&&!flag))
		{
			if(i<=4) color(10);
			else color(4);
		} 
		else color(15);
		if(unit[i].dead) color(8);
		if(unit[i].name.empty()) cout<<i<<" NULL";
		else cout<<i<<" "<<unit[i].name;
		color(4);
		string temp=unit[nxt].skill[mode].type;
		if(target==i&&(unit[nxt].team==1||unit[last].team==1))
		{
			cout<<"←";
			if(flag) cout<<"  ";
			else if(disable(target,nxt))
			{
				color(4);
				cout<<"×"; 
			}
			else if(temp=="单攻")
			{
				color(15);
				cout<<"< ";
			}
			else if(temp=="群攻")
			{
				color(15);
				cout<<"] ";
			}
			else if(temp=="扩散")
			{
				color(15);
				cout<<"> ";
			}
			else if(temp=="回复")
			{
				color(10);
				cout<<"+ ";
			}
			else if(temp=="防御")
			{
				color(9);
				cout<<"V ";
			}
			else if(temp=="妨害")
			{
				color(4);
				cout<<"↓";
			}
			else if(temp=="辅助")
			{
				color(10);
				cout<<"↑";
			}
			else if(temp=="弹射")
			{
				color(15);
				cout<<"W ";
			}
			if(unit[i].name.empty()) cout<<"  ";
			else 
			{
				for(int j=unit[i].name.size();j<6;j++) cout<<" ";
			}
		}
		else 
		{
			if(unit[i].name.empty()) cout<<"      ";
			else 
			{
				for(int j=unit[i].name.size();j<10;j++) cout<<" ";
			}
		}
		if(i<=4)
		{
			if(unit[i].dead) color(8);
			else color(9);
			if(!unit[i].name.empty()) cout<<int(unit[i].nhp*100.0/thp(i))<<"% ";
			if(!unit[i].name.empty()&&!unit[i].dead) 
			{
				if(unit[i].energy==unit[i].maxenergy)
				{
					if(nowult==i) 
					{
						color(15*16+10);
						cout<<"ULT";
					}
					else
					{
						 color(10*16);
						cout<<"RDY";
					}
				}
				else
				{
					color(10);
					int temp=int(unit[i].energy*100.0/unit[i].maxenergy);
					if(temp<10) cout<<" ";
					cout<<temp<<"%";
				}
			}
			else cout<<"     ";
			cout<<endl;
		}
		else
		{
			if(unit[i].dead) color(8);
			else color(4);
			if(!unit[i].name.empty()) cout<<int(unit[i].nhp*1.0/thp(i)*100.0)<<"%   "<<endl;
		}
		if(i==4) cout<<endl;
	}
	cout<<endl;
	color(15);
	cout<<"行动值："<<tact<<endl;
	cout<<"战技点："<<sp<<endl; 
}
int main()
{
	system("title Honkai Starrail Simulation —— Made by reeper");
	srand((unsigned)time(NULL));
    // 读取 battle.txt 中的角色ID
    ifstream battleFile("battle.txt");
    if (!battleFile.is_open()) {
        cerr << "无法打开 battle.txt" << endl;
        return 1;
    }

    for (int i = 1; i <= 9; ++i) {
        battleFile >> unit[i].id;
    }
    battleFile.close();

    // 读取角色/敌人数据文件
    for (int i = 1; i <= 9; ++i) {
        if (unit[i].id == "0") continue;

        string filePath;
        if (i <= 4) {
            filePath = "Data/characters/" + unit[i].id + ".txt";
        } else {
            filePath = "Data/enemies/" + unit[i].id + ".txt";
        }

        ifstream dataFile(filePath);
        if (!dataFile.is_open()) {
            cerr << "无法打开文件: " << filePath << endl;
            continue;
        }

        // 临时重定向 cin 到数据文件
        streambuf* originalCinBuf = cin.rdbuf();
        cin.rdbuf(dataFile.rdbuf());

        read(i);  // 复用原有的 read 函数

        // 恢复 cin 并关闭文件
        cin.rdbuf(originalCinBuf);
        dataFile.close();
        cin.clear(); // 清除可能的错误状态
    }

    // 显示所有单位数据
    for (int i = 0; i <= 9; ++i) {
        if (!unit[i].name.empty()) {
            show(i);
        }
    }
	memset(act, 0, sizeof(act));
	unit[0].spd=90;
    start();  // 进入战斗循环
    return 0;
} 
