#include "Dungeon.h"
#include <QDebug>
#include <QTime>
std::vector<std::vector<int>> directions{{0,-1},{0,1},{-1,0},{1,0}};
Dungeon::Dungeon(QWidget *parent): QWidget(parent)
{
    Total_Floor=7;
    //开辟堆上三维数组
    map=new int **[Total_Floor];
    for(int i=0;i<Total_Floor;i++)
    {
        map[i]=new int *[12];
        for(int j=0;j<12;j++)
        {
            map[i][j]=new int[16];
            for(int k=0;k<16;k++)
            {
                map[i][j][k]=0;
            }
        }
    }

    QTime time=QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    initialFogArr();
    initialMapString();
    initialMonsterPic();
    initialrole();
    initialenemy();

    GetitemNum=0;//获得物品信息栏初始化
    itemNum=0;
    MonsterNum=0;
    fight_period_it = 0;//战斗状态信息栏初始化
    fight_end_it = 0;
    moveDirection=-1;
    moveNum=0;
    isFighting=false;
    display_it=0;
    PickaxeUse=false;
    isDead=false;

    fac=new Factory();
    music=new Sound();
}
Dungeon::~Dungeon(){}
void Dungeon::initialrole()
{
    role.name="Mario";
    role.level=1;
    role.exp=0;
    role.hp=100;
    role.mp=100;
    role.atk=5;
    role.def=3;
    role.scoreNum=0;
    role.Pickaxe=0;
    role.Downstairs=0;
    role.redkey=0;
}

void Dungeon::initialenemy()
{
    enemy[0].name="小星星";//15
    enemy[0].hp=60;
    enemy[0].atk=4;//4
    enemy[0].def=1;//1
    enemy[0].exp=4;
    enemy[0].gold=5;

    enemy[1].name="毒蘑菇";//16
    enemy[1].hp=100;
    enemy[1].atk=9;
    enemy[1].def=5;
    enemy[1].exp=8;
    enemy[1].gold=10;

    enemy[2].name="绿鸭子";//18
    enemy[2].hp=200;
    enemy[2].atk=16;
    enemy[2].def=12;
    enemy[2].exp=16;
    enemy[2].gold=20;

    enemy[3].name="红鸭子";//19
    enemy[3].hp=300;
    enemy[3].atk=24;
    enemy[3].def=14;
    enemy[3].exp=32;
    enemy[3].gold=30;

    enemy[4].name="食人花-绿";//20
    enemy[4].hp=600;
    enemy[4].atk=32;
    enemy[4].def=20;
    enemy[4].exp=42;
    enemy[4].gold=40;

    enemy[5].name="食人花-红";//21
    enemy[5].hp=900;
    enemy[5].atk=46;
    enemy[5].def=22;
    enemy[5].exp=52;
    enemy[5].gold=46;

    enemy[6].name="飞行鸭";//22
    enemy[6].hp=1200;
    enemy[6].atk=82;
    enemy[6].def=42;
    enemy[6].exp=64;
    enemy[6].gold=60;

}
void Dungeon::initialFogArr()
{
    memset(FogArr,0,sizeof(FogArr));
    for(int i=0;i<2;++i)
    {
        for(int j=6;j<9;++j)
            FogArr[3][i][j]=15;
    }
    memset(haveVisited,0,sizeof(haveVisited));
}

//动态贴图方式
void Dungeon::initialMapString()
{
    for(int i=0;i<41;++i)
    {
        for(int j=0;j<4;++j)
        {
            if(isStatic(i))
            {
                DungeonStytle[i][j]=QString("border-image: url(:/Dungeon/image/Dungeon/")+
                        QString::number(i)+QString(".png);");
                break;
            }
            else
                DungeonStytle[i][j]=QString("border-image: url(:/Dungeon/image/Dungeon/")+
                        QString::number(i)+"/"+QString::number(i)+QString::number(j)+QString(".png);");
        }
    }
}


void Dungeon::initialMonsterPic()
{
    MonsterPic[0]="border-image: url(:/info/image/information/start.png);";
    MonsterPic[1]="border-image: url(:/info/image/information/badmushroom.png);";
    MonsterPic[2]="border-image: url(:/info/image/information/greenDuck.png);";
    MonsterPic[3]="border-image: url(:/info/image/information/flyDuck.png);";
    MonsterPic[4]="border-image: url(:/info/image/information/绿色食人花.png);";
    MonsterPic[5]="border-image: url(:/info/image/information/红色食人花.png);";
    MonsterPic[6]="border-image: url(:/info/image/information/飞行鸭.png);";
}

int Dungeon::pk(struct Monster & m)
{
    //计算伤害值，-1表示打不动对方防御，-2表示双方都打不动对方防御
    if (role.atk <= m.def)
    {
        if (m.atk > role.def)
            return -1;//打不过
        else
            return -2;//怪物防御太高，先增加你的攻击力吧
    }
    else//人物的攻击力大于怪物的防御，但是依然打不过的情况
    {
        if(m.atk>role.def)
        {
            int i=0;
            int role_hp=role.hp;
            int enemy_hp=m.hp;
            while(role_hp>0)
            {
                role_hp=role.hp - (m.atk - role.def)*i;
                enemy_hp=m.hp - (role.atk - m.def) * i;
                i++;
            }
            if(enemy_hp>=0)
            {
                return 1;
            }
        }
    }
    return 0;
}
int Dungeon::calc_damage(int MonsterNum)
{
    if (role.atk <= enemy[MonsterNum].def)
    {
        if (enemy[MonsterNum].atk > role.def)
            return -1;
        else
            return -2;
    }
    else
    {
        if (enemy[MonsterNum].atk > role.def)
        {
            int atk_times = (enemy[MonsterNum].hp - 1) / (role.atk - enemy[MonsterNum].def);//怪物攻击的次数
            return atk_times * (enemy[MonsterNum].atk - role.def);//造成的伤害
        }
        else
        {
            return 0;//怪物破不了我们的防，造成的伤害为0
        }
    }

}
int Dungeon::sum(int arr[],int n)
{
    if(arr==nullptr)
        return 0;
    int res=0;
    for(int i=0;i<n;++i)
        res+=arr[i];
    return res;
}
void Dungeon::dfs(int arr[12][16],bool visited[12][16],QStack<std::pair<int,int>> &s,int i,int j)
{
    if(i<0||i>=12||j<0||j>=16||arr[i][j]==0||visited[i][j])
        return;
    visited[i][j]=true;
    s.push(std::pair<int,int>(i,j));
    for(auto dir:directions)
        dfs(arr,visited,s,i+dir[0],j+dir[1]);
}
void Dungeon::bfs(int arr[12][16],QStack<std::pair<int,int>> &s,std::pair<int,int> start,std::pair<int,int> end)
{
    bool visited[12][16]{false};
    QQueue<std::pair<int, int>> q;
    q.enqueue(start);
    visited[start.first][start.second] = true;
    while (!q.empty())
    {
        int size = q.size();
        while (size-- > 0)
        {
            std::pair<int, int> temp = q.dequeue();;
            int ci = temp.first, cj = temp.second;
            s.push(temp);
            int index = 0;
            visited[ci][cj] = true;
            for (auto dir : directions)
            {
                int ni = ci + dir[0], nj = cj + dir[1];
                if (ni < 0 || ni >= 12 || nj < 0 || nj >= 16 || visited[ni][nj] || arr[ni][nj] == 0)
                {
                    index++;
                    if (index >= 4)
                        s.pop();
                    continue;
                }
                if (ni == end.first&&nj == end.second)
                    return;
                q.enqueue(std::pair<int, int>(ni, nj));
            }
        }
    }
}
void Dungeon::doDfs(int arr[12][16],QStack<std::pair<int,int>>& s,bool visited[12][16])
{
    int dfsi=rand()%12;
    int dfsj=rand()%16;
    while(arr[dfsi%12][dfsj%16]!=1)
    {
        dfsj++;
        if(dfsj%16==0)
            dfsi++;
    }
    dfs(arr,visited,s,dfsi%12,dfsj%16);
}

void Dungeon::setExit(int arr[12][16],std::pair<int,int> &start,std::pair<int,int> &end)
{    
    QStack<std::pair<int,int>> s;
    bool visited[12][16]{false};

    doDfs(arr,s,visited);
    //int startPos=rand()%floor;
    int startPos=0;
    int starti=s[startPos].first,startj=s[startPos].second;
    int endi=s.top().first,endj=s.top().second;

    while(std::abs(starti-endi)<=3||std::abs(startj-endj)<=3)
    {
        s.clear();
        for(int i=0;i<12;++i)
        {
            for(int j=0;j<16;++j)
                if(visited[i][j])
                    visited[i][j]=false;
        }
        doDfs(arr,s,visited);
        starti=s[startPos].first;
        startj=s[startPos].second;
        endi=s.top().first;
        endj=s.top().second;
    }

    start.first=starti;
    start.second=startj;
    end.first=endi;
    end.second=endj;
    arr[starti][startj]=6;
    arr[endi][endj]=7;
}
void Dungeon::GenerateMonster(int arr[12][16],int floor)
{
    int totalMonsterNum=40+floor*2;
    int MonsterArr[9]{0};
    int MonsterInitial[9]{8,8,5,3,1,0,0,0,0};
    for(int i=0;i<9;++i)
    {
        if(i==0)
        {
            MonsterArr[i]=MonsterInitial[i]-(floor-1)*2;
            if(MonsterArr[i]<=2)
                MonsterArr[i]=2;
        }
        else if(i==1)
        {
            MonsterArr[i]=MonsterInitial[i]-(floor-1)*2;
            if(MonsterArr[i]<=2)
                MonsterArr[i]=0;
        }
        else if(i==2)
            MonsterArr[i]=5;
        else if(i==3)
            MonsterArr[i]=3;
        else
        {
            MonsterArr[i]=MonsterInitial[i]+(floor-1)*(0.5*(i-3));
        }
    }
    while(sum(MonsterArr,9)<totalMonsterNum)
    {
        for(int i=floor-1;i<9;++i)
            MonsterArr[i]+=1;
    }
    while(MonsterArr[3]>3)
    {
        MonsterArr[2]++;
        MonsterArr[3]--;
    }
    int GenerateMonsterNum=0;
    int MonsterArrIndex=0;
    while(GenerateMonsterNum<totalMonsterNum)
    {
        for(int i=0;i<12;++i)
        {
            for(int j=0;j<16;++j)
            {
                if(arr[i][j]==1)
                {
                    if(MonsterArr[MonsterArrIndex++%9]!=0)
                    {
                        arr[i][j]= (MonsterArrIndex-1)%9+14;
                        MonsterArr[(MonsterArrIndex-1)%9]--;
                    }
                    i+=rand()%6;
                    j+=rand()%8;
                    GenerateMonsterNum++;
                    if(GenerateMonsterNum>=totalMonsterNum)
                        return;
                }
            }
        }
    }
}
void Dungeon::GenerateItem(int arr[12][16],int floor)
{
    int totalBuffItemNum=4+floor;
    int BuffItemArr[5]{0};
    int BuffItemInitial[5]{2,1,1,0,0};
    for(int i=0;i<5;++i)
    {
        if(i==2)
            BuffItemArr[i]=BuffItemInitial[i]+(floor-1)*0.5;
        else
        {
            BuffItemArr[i]=BuffItemInitial[i]+(floor-1)*1;
        }
    }
    while(sum(BuffItemArr,5)<totalBuffItemNum)
    {
        for(int i=0;i<3+floor-1;++i)
            BuffItemArr[i]+=1;
    }
    int GenerateBuffItemNum=0;
    int BuffItemArrIndex=0;
    bool GenerateBuffItemDone=false;
    while(GenerateBuffItemNum<totalBuffItemNum)
    {
        for(int i=0;i<12;++i)
        {
            for(int j=0;j<16;++j)
            {
                if(arr[i][j]==1)
                {
                    if(BuffItemArr[BuffItemArrIndex++%5]!=0)
                    {
                        arr[i][j]= (BuffItemArrIndex-1)%5+23;
                        BuffItemArr[(BuffItemArrIndex-1)%5]--;
                    }
                    i+=rand()%6;
                    j+=rand()%8;
                    GenerateBuffItemNum++;
                    if(GenerateBuffItemNum>=totalBuffItemNum)
                    {
                        GenerateBuffItemDone=true;
                        break;
                    }
                }
            }
            if(GenerateBuffItemDone)
                break;
        }
        if(GenerateBuffItemDone)
            break;
    }

    int totalToolItemNum=2+floor;
    int ToolItemArr[2]{0};
    int ToolItemInitial[2]{2,1};
    for(int i=0;i<2;++i)
    {
        if(i==0)
            ToolItemArr[i]=ToolItemInitial[i]+(floor-1)*1;
        else
            ToolItemArr[i]=ToolItemInitial[i]+(floor-1)*0.5;
    }
    while(sum(ToolItemArr,1)<totalToolItemNum)
    {
        for(int i=0;i<1;++i)
            ToolItemArr[i]+=1;
    }
    int GenerateToolItemNum=0;
    int ToolItemArrIndex=0;
    while(GenerateToolItemNum<totalToolItemNum)
    {
        for(int i=0;i<12;++i)
        {
            for(int j=0;j<16;++j)
            {
                if(arr[i][j]==1)
                {
                    if(ToolItemArr[ToolItemArrIndex++%2]!=0)
                    {
                        arr[i][j]= (ToolItemArrIndex-1)%2+31;
                        ToolItemArr[(ToolItemArrIndex-1)%2]--;
                    }
                    i+=rand()%6;
                    j+=rand()%8;
                    GenerateToolItemNum++;
                    if(GenerateToolItemNum>=totalToolItemNum)
                        return;
                }
            }
        }
    }

}
void Dungeon::GenerateEquip()
{
    int ironSwordFloor=2;
    int flowerSwordFloor=4;
    int goldenSwordFloor=7;
    if(map[ironSwordFloor-1][10][14]==6||map[ironSwordFloor-1][10][14]==7)
    {
        map[ironSwordFloor-1][10][13]=28;
    }
    else
        map[ironSwordFloor-1][10][14]=28;
    if(map[flowerSwordFloor-1][10][14]==6||map[flowerSwordFloor-1][10][14]==7)
    {
        map[flowerSwordFloor-1][10][13]=29;
    }
    else
        map[flowerSwordFloor-1][10][14]=29;
    if(map[goldenSwordFloor-1][10][14]==6||map[goldenSwordFloor-1][10][14]==7)
    {
        map[goldenSwordFloor-1][10][13]=30;
    }
    else
        map[goldenSwordFloor-1][10][14]=30;
}

void Dungeon::GenerateElement(int arr[12][16],int floor)
{
    GenerateMonster(arr,floor);
    GenerateItem(arr,floor);
}
bool Dungeon::isValid(int arr[12][16],int i,int j,int floor)
{
    return arr[i][j]==1||arr[i][j]==6||(arr[i][j]>=8&&arr[i][j]<=14+(floor<2?2:floor))
            ||(arr[i][j]>=23&&arr[i][j]<=27)||arr[i][j]==31||arr[i][j]==32;
}

void Dungeon::RandomGeneraterMap(int arr[12][16],int floor)
{
    std::pair<int,int> start;
    std::pair<int,int> end;
    setExit(arr,start,end);
    GenerateElement(arr,floor);
    QStack<std::pair<int,int>> s;
    bfs(arr,s,start,end);
    while(!s.empty())
    {
        std::pair<int,int> temp=s.pop();
        if(!isValid(arr,temp.first,temp.second,floor))
            arr[temp.first][temp.second]=rand()%(floor<3?2:5)+14;
    }
}

void Dungeon::initialMap()
{
    floor=1;
    MAX_HP=100;
    MAX_MP=100;
    deadTimes=0;
    isPre=false;
    isNext=false;
    initialrole();
    while(1)
    {

        if(floor==1)
        {
            int tempmap[12][16]={
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,
                0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,
                0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,
                0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,
                0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,
                0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,
                0,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0,
                0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,0,
                0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,0

            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        if(floor==2)
        {
            int tempmap[12][16]={
                1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,
                1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,
                1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,
                1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
                1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,0,
                1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,
                1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,
                1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,
                1,0,1,1,1,1,0,1,1,1,0,1,1,1,1,0,
                0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0
            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        if(floor==3)
        {
            int tempmap[12][16]={
                1,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,
                1,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,
                0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                0,0,1,0,0,1,1,1,1,0,1,0,0,0,0,0,
                1,1,1,1,0,0,1,1,0,0,1,0,1,1,1,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,
                0,0,0,0,0,0,1,1,0,0,1,0,1,1,1,1,
                1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,
            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        if(floor==4)
        {
            int tempmap[12][16]={
                1,1,1,1,0,1,8,9,10,1,0,1,1,1,1,1,
                1,1,1,1,0,1,11,12,13,1,0,1,1,1,1,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,
                0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,
                1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,
                1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,
                1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,
            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        if(floor==5)
        {
            int tempmap[12][16]={
                1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,
                1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,
                1,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,
                0,0,0,0,1,0,1,0,1,1,1,0,1,1,0,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,
                0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,
                1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,
                1,1,1,1,1,0,1,0,1,1,1,0,0,0,0,0,
                0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,1,
                1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,
            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        if(floor==6)
        {
            int tempmap[12][16]={
                1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
                1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,1,
                1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,
                1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,
                1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,
                1,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,
                1,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,
                1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,
                1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,
                1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,
            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        if(floor==7)
        {
            int tempmap[12][16]={
                1,0,1,1,0,1,0,1,1,1,0,1,0,1,0,1,
                1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,
                1,0,1,1,0,1,1,1,0,1,0,1,0,1,0,1,
                1,0,1,1,1,1,0,1,0,1,1,1,1,1,1,1,
                1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
                1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
                1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,
                1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,
                1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
                1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,
            };
            RandomGeneraterMap(tempmap,floor);
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[floor-1][i][j]=tempmap[i][j];
                }
            }
        }
        floor++;
        if(floor>Total_Floor)
        {
            break;
        }
    }
    floor=1;
    setCharacterPos(floor-1,6);
    GenerateEquip();
}
void Dungeon::setCharacterPos(int floor,int target)
{
    bool findflag=false;
    int i,j;
    for(i=0;i<12;i++)
    {
        for(j=0;j<16;j++)
        {
            if(map[floor][i][j]==target)
            {
                findflag=true;
                break;
            }
        }
        if(findflag)
            break;
    }
    if(!findflag)
        return;
    if(i<11&&((map[floor][i+1][j]==1||isDynamic(floor,i+1,j))
            ||(map[floor][i+1][j]>=23&&map[floor][i+1][j]<=27)))
    {
        if(map[floor][i+1][j]!=1)
            map[floor][i+1][j]=1;
        x=i+1;
        y=j;
    }
    else if(i>0&&((map[floor][i-1][j]==1||isDynamic(floor,i-1,j))
            ||(map[floor][i-1][j]>=23&&map[floor][i-1][j]<=27)))
    {
        if(map[floor][i-1][j]!=1)
            map[floor][i-1][j]=1;
        x=i-1;
        y=j;
    }
    else if(j<15&&((map[floor][i][j+1]==1||isDynamic(floor,i,j+1))
            ||(map[floor][i][j+1]>=23&&map[floor][i][j+1]<=27)))
    {
        if(map[floor][i][j+1]!=1)
            map[floor][i][j+1]=1;
        x=i;
        y=j+1;
    }
    else if(j>0&&((map[floor][i][j-1]==1||isDynamic(floor,i,j-1))
            ||(map[floor][i][j-1]>=23&&map[floor][i][j-1]<=27)))
    {
        if(map[floor][i][j-1]!=1)
            map[floor][i][j-1]=1;
        x=i;
        y=j-1;
    }
    else
    {
        x=y=0;
    }
    if(target==6)
        updateFogArea(floor,x,y);
}

void Dungeon::initialCharacterPos()
{
    if(isPre)
    {
        setCharacterPos(floor-2,7);
        isPre=false;
    }
    if(isNext)
    {
        setCharacterPos(floor,6);
        isNext=false;
    }
}
void Dungeon::initialDugeon()
{
    initialFogWidget();
    initialMap();
    Store=new StoreWidget(ptr_MianWindow);//类中如果不定义子窗口，则不能作为MianWindow的子窗口
    Store->setGeometry(292,192,256,352);
    Store->hide();

    menu=new Menu(ptr_MianWindow);
    menu->setGeometry(160,160,520,320);
    menu->hide();

    initialinfoWidget();
    initialFightWin();//希望显示在MainWidget之上的窗口，要在初始化MainWidget后再初始化
    initialSetNameWin();
    initialGetitemWin();

    GetitemTimer=new QTimer(this);
    FightTimer=new QTimer(this);
    dynamicEffectTimer=new QTimer(this);

    connect(GetitemTimer,SIGNAL(timeout()),this,SLOT(GetitemWinshow()));
    connect(FightTimer,SIGNAL(timeout()),this,SLOT(FightWinshow()));
    connect(Store->haveBuyTimer,SIGNAL(timeout()),this,SLOT(OpenStore()));
    connect(dynamicEffectTimer,SIGNAL(timeout()),this,SLOT(ShowdynamicEffect()));
}
void Dungeon::initialFogWidget()
{
    int BASIC_WIDTH=832/16;
    int BASIC_HEIGHT=672/12;

    for(int i=0;i<12;++i)
    {
        for(int j=0;j<16;++j)
        {
            FogWidget[i][j]=new QWidget(ptr_MianWindow);
            FogWidget[i][j]->setGeometry(j*BASIC_WIDTH,i*BASIC_HEIGHT,BASIC_WIDTH,BASIC_HEIGHT);
            FogWidget[i][j]->setAutoFillBackground(true);
            FogWidget[i][j]->hide();
        }
    }

}
void Dungeon::initialinfoWidget()
{
    infoWidget=new QWidget(ptr_MianWindow);
    QFont fontLabel("Microsoft YaHei" ,12, 75);
    QFont fontNum("Microsoft YaHei" ,10, 65);
    QFont fontName("Microsoft YaHei" ,10, 55);
    QString labelbg_style="border-image: url(:/interface/image/interface/labelbg.png);";
    QString CharacterPic_style="border-image: url(:/info/image/information/Character.png);";
    QString Pickaxe_style="border-image: url(:/info/image/information/Pickaxe.png);";
    QString Downstairs_style="border-image: url(:/info/image/information/Downstairs.png);";
    QString redkey_style="border-image: url(:/info/image/information/redkey.png);";
    QString button_style="QPushButton{border-image:url(:/interface/image/interface/labelbg.png);color:white;border-radius:10px;}"
                         "QPushButton:hover{border-image:url(:/interface/image/interface/PushButtonhoverbg.png); color: black;}"
                         "QPushButton:pressed{border-image:url(:/interface/image/interface/PushButtonPressbg.png);}";


    QLabel *CharacterPic=fac->CreateQLabel(infoWidget,20,20,40,40,"",CharacterPic_style);
    QLabel *label_Name=fac->CreateQLabel(infoWidget,20,70,100,30,"人们叫他",labelbg_style,fontLabel,Qt::AlignLeft);
    CharacterName=fac->CreateQLabel(infoWidget,130,70,170,30,"",labelbg_style,fontName,Qt::AlignLeft);
    QLabel *label_level=fac->CreateQLabel(infoWidget,140,20,40,40,"级",labelbg_style,fontLabel);
    QLabel *label_hp=fac->CreateQLabel(infoWidget,20,110,65,30,"生命",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_mp=fac->CreateQLabel(infoWidget,20,150,65,30,"魔法",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_atk=fac->CreateQLabel(infoWidget,20,230,65,30,"攻击",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_def=fac->CreateQLabel(infoWidget,20,270,65,30,"防御",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_score=fac->CreateQLabel(infoWidget,20,310,65,30,"金币",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_exp=fac->CreateQLabel(infoWidget,20,190,65,30,"经验",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_Pickaxe=fac->CreateQLabel(infoWidget,20,370,40,40,"",Pickaxe_style);
    QLabel *label_Downstairs=fac->CreateQLabel(infoWidget,20,420,40,40,"",Downstairs_style);
    QLabel *label_redkey=fac->CreateQLabel(infoWidget,20,470,40,40,"",redkey_style);
    QLabel *label_floor1=fac->CreateQLabel(infoWidget,20,520,40,40,"第",labelbg_style,fontLabel);
    QLabel *label_floor2=fac->CreateQLabel(infoWidget,140,520,40,40,"层",labelbg_style,fontLabel);


    level=fac->CreateQLabel(infoWidget,80,20,40,40,QString::number(role.level),labelbg_style,fontNum);
    hp=fac->CreateQLabel(infoWidget,105,110,100,30,QString::number(role.hp),labelbg_style,fontNum);
    mp=fac->CreateQLabel(infoWidget,105,150,100,30,QString::number(role.mp),labelbg_style,fontNum);
    atk=fac->CreateQLabel(infoWidget,105,230,100,30,QString::number(role.atk),labelbg_style,fontNum);
    def=fac->CreateQLabel(infoWidget,105,270,100,30,QString::number(role.def),labelbg_style,fontNum);
    score=fac->CreateQLabel(infoWidget,105,310,100,30,QString::number(role.scoreNum),labelbg_style,fontNum);
    exp=fac->CreateQLabel(infoWidget,105,190,100,30,QString::number(role.exp),labelbg_style,fontNum);
    PickaxeNum=fac->CreateQLabel(infoWidget,105,370,100,30,QString::number(role.Pickaxe),labelbg_style,fontNum);
    DownstairsNum=fac->CreateQLabel(infoWidget,105,420,100,30,QString::number(role.Downstairs),labelbg_style,fontNum);
    redkeyNum=fac->CreateQLabel(infoWidget,105,470,100,30,QString::number(role.redkey),labelbg_style,fontNum);
    Floor=fac->CreateQLabel(infoWidget,80,520,40,40,QString::number(floor),labelbg_style,fontNum);

    connect(menu->ReturnMainMenu,SIGNAL(clicked()),this,SLOT(ReturnMainMenuPlay()));
    connect(menu->Restart,SIGNAL(clicked()),this,SLOT(RestartPlay()));
    connect(menu->Continue,SIGNAL(clicked()),this,SLOT(ContinuePlay()));

    infoWidget->setMinimumSize(300,672);
    infoWidget->setStyleSheet("color:white;border-image: url(:/interface/image/interface/infobg.png);");
    infoWidget->hide();
}
void Dungeon::ReturnMainMenuPlay()//返回主菜单
{
    infoWidget->hide();//隐藏状态界面
    hideDungeon();//隐藏地牢界面
    hideFog();
    menu->hide();
    surface->isok=false;
    music->InterfaceBGM->play();
    music->InterfaceBGMTimer->start(10);
    NameEdit->clear();
    surface->show();//开始界面显示后，主窗口大小还是不变
    ptr_MianWindow->resize(832,672);//需要重新设置主窗口大小
    surface->resizeEvent(nullptr);

    surface->showMianMenu();
    initialFogArr();
    initialMap();
    updateStatusData();
}
void Dungeon::RestartPlay()//重新开始
{
    menu->hide();
    initialFogArr();
    initialMap();
    ShowDungeon();
    ShowFog();
    updateStatusData();
}
void Dungeon::ContinuePlay()//继续游戏
{
    menu->hide();
}

void Dungeon::ShowFog()
{
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            QPalette *palette=new QPalette;
            palette->setBrush(QPalette::Background,
                    QBrush(QImage(":/Dungeon/image/Dungeon/fog.png").
                           copy((FogArr[floor-1][i][j]/4)*54,(FogArr[floor-1][i][j]%4)*58,54,58)
                                ));
            FogWidget[i][j]->setPalette(*palette);
            FogWidget[i][j]->show();
        }
    }
}
void Dungeon::hideFog()
{
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            FogWidget[i][j]->hide();
        }
    }
}

void Dungeon::ShowDungeon()
{
    int BASIC_WIDTH=832/16;
    int BASIC_HEIGHT=672/12;
    if(surface->surfaceShow)
    {
        surface->hide();
        gLayout_Map->removeWidget(surface);
        surface->surfaceShow=false;
    }


    People->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
    gLayout_Map->addWidget(People,x,y);
    People->setStyleSheet(DungeonStytle[3][0]);
    People->show();//用就显示
    if(floor==0)
        floor=1;
    else if(floor>=8)
        floor=7;
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            MazeWidget[i][j]->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
            MazeWidget[i][j]->show();//要用的窗口必须要显示出来
            MazeWidget[i][j]->setStyleSheet(DungeonStytle[map[floor-1][i][j]][0]);
        }
    }
    dynamicEffectTimer->start(500);

    music->DungeonBGM->play();
    music->DungeonBGMTimer->start(10);
}
void Dungeon::hideDungeon()
{
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            MazeWidget[i][j]->hide();
        }
    }
    People->hide();
    surface->Dungeonisok=false;
    dynamicEffectTimer->stop();
    music->DungeonBGM->stop();
    music->DungeonBGMTimer->stop();
}
void Dungeon::initialSetNameWin()
{
    QString button_style="QPushButton{background-color:black;color:white;border-radius:10px;}"
                         "QPushButton:hover{background-color:black;color:springgreen;}"
                         "QPushButton:pressed{background-color:black;color:springgreen;}";
    QFont font("Microsoft YaHei" ,12, 35);

    NameEdit=fac->CreateQLineEdit(ptr_MianWindow,320,260,200,40,"color:black",font);
    SetDone=fac->CreateQPushButton(ptr_MianWindow,360,320,120,30,"确定",button_style,font);
    connect(SetDone,SIGNAL(clicked()),this,SLOT(EnterDungeon()));
    NameEdit->hide();
    SetDone->hide();
}
void Dungeon::EnterDungeon()
{
    if(NameEdit->text()=="")
    {
        return;
    }
    else
    {
        NameEdit->hide();
        SetDone->hide();
        Character_name->setText(NameEdit->text());
        CharacterName->setText(NameEdit->text());
        infoWidget->show();
        ShowDungeon();
        ShowFog();
        music->InterfaceBGM->stop();
        music->InterfaceBGMTimer->stop();
    }
}
void Dungeon::initialGetitemWin()
{
    //显示获得物品信息
    QFont font("Microsoft YaHei" ,12, 75);
    GetitemsShow=fac->CreateQLabel(ptr_MianWindow,120,340,600,50,"","color:orangered;background-color:transparent;",font);
    GetitemsShow->hide();
}
void Dungeon::initialFightWin()
{
    QFont font("Microsoft YaHei" ,12, 75);
    QFont fontName("Microsoft YaHei" ,8, 55);
    QFont font_VS("Microsoft YaHei" ,24, 75);
    QString FightWidget_style="color:orangered;background-color:black;";
    QString Character_pic_style="border-image: url(:/info/image/information/Character.png);";
    FightWidget=new QWidget(ptr_MianWindow);
    FightWidget->setStyleSheet(FightWidget_style);
    FightWidget->setGeometry(160,160,520,320);

    Monster_pic=fac->CreateQLabel(FightWidget,25,60,40,40,"",FightWidget_style,font);
    Monster_name=fac->CreateQLabel(FightWidget,10,160,100,40,"",FightWidget_style,font);
    Monster_hpText=fac->CreateQLabel(FightWidget,105,20,100,40,"生命值",FightWidget_style,font);
    Monster_hp=fac->CreateQLabel(FightWidget,105,60,100,40,"",FightWidget_style,font);
    Monster_atkText=fac->CreateQLabel(FightWidget,105,120,100,40,"攻击力",FightWidget_style,font);
    Monster_atk=fac->CreateQLabel(FightWidget,105,160,100,40,"",FightWidget_style,font);
    Monster_defText=fac->CreateQLabel(FightWidget,105,220,100,40,"防御力",FightWidget_style,font);
    Monster_def=fac->CreateQLabel(FightWidget,105,260,100,40,"",FightWidget_style,font);


    Character_pic=fac->CreateQLabel(FightWidget,445,60,40,40,"",Character_pic_style,fontName);
    Character_name=fac->CreateQLabel(FightWidget,400,160,120,40,role.name,FightWidget_style,font);
    Character_hpText=fac->CreateQLabel(FightWidget,335,20,100,40,"生命值",FightWidget_style,font);
    Character_hp=fac->CreateQLabel(FightWidget,335,60,100,40,"",FightWidget_style,font);
    Character_atkText=fac->CreateQLabel(FightWidget,335,120,100,40,"攻击力",FightWidget_style,font);
    Character_atk=fac->CreateQLabel(FightWidget,335,160,100,40,"",FightWidget_style,font);
    Character_defText=fac->CreateQLabel(FightWidget,335,220,100,40,"防御力",FightWidget_style,font);
    Character_def=fac->CreateQLabel(FightWidget,335,260,100,40,"",FightWidget_style,font);

    QLabel *VS=fac->CreateQLabel(FightWidget,225,100,100,80,"VS",FightWidget_style,font_VS);

    FightWidget->hide();
}
void Dungeon::updateStatusData()
{
    if(role.exp>=100*role.level)
    {
        MAX_HP+=role.level*50;
        role.hp=MAX_HP;
        MAX_MP+=role.level*5;
        role.mp=MAX_MP;
        role.atk+=3;
        role.def+=2;
        role.level+=1;
        role.exp=0;
        itemNum=-3;
        GetitemTimer->start(100);
        music->SoundPlay(1);
    }
    score->setText(QString::number(role.scoreNum));
    level->setText(QString::number(role.level));
    exp->setText(QString::number(role.exp));
    hp->setText(QString::number(role.hp));
    mp->setText(QString::number(role.mp));
    atk->setText(QString::number(role.atk));
    def->setText(QString::number(role.def));
    PickaxeNum->setText(QString::number(role.Pickaxe));
    DownstairsNum->setText(QString::number(role.Downstairs));
    redkeyNum->setText(QString::number(role.redkey));
    Floor->setText(QString::number(floor));
}
void Dungeon::GetitemWinshow()
{
    if(GetitemNum==0)
    {
        switch(itemNum)
        {
        case -1:
        {
            GetitemsShow->setText(QString::fromWCharArray(L"你因死亡失去 ") + QString::number(20*deadTimes) + QString::fromWCharArray(L" 生命上限 ")
                            + QString::number(5*deadTimes)+ QString::fromWCharArray(L" 魔法上限 ")
                            + QString::number(40*deadTimes)+ QString::fromWCharArray(L" 金币 "));
            GetitemsShow->show();
        }
            break;
        case -2:
        {
            GetitemsShow->setText(QString::fromWCharArray(L"获得经验值 ") + QString::number(enemy[MonsterNum].exp) + QString::fromWCharArray(L" 金币 ") + QString::number(enemy[MonsterNum].gold));
            GetitemsShow->show();
        }
            break;
        case -3:
        {
            GetitemsShow->setText("Level Up!!!");
            GetitemsShow->show();
        }
            break;
        case 0:
        {
            GetitemsShow->setText("你们谁也干不掉谁！");
            GetitemsShow->show();
        }
            break;
        case 1:
        {
            GetitemsShow->setText("你的生命值太低，需要提高才能击败目标！");
            GetitemsShow->show();
        }
            break;
        case 23:
        {
            GetitemsShow->setText("获得小回血蘑菇!生命值加 100");
            GetitemsShow->show();
        }
            break;
        case 24:
        {
            GetitemsShow->setText("获得回蓝蘑菇!魔法值加 20");
            GetitemsShow->show();
        }
            break;
        case 25:
        {
            GetitemsShow->setText("获得大回血蘑菇！增加50点生命值上限！");
            GetitemsShow->show();
        }
            break;
//        case 11:
//        {
//            GetitemsShow->setText("突然从箱子里跳出一只绿鸭子并冲向你，你不得不与之战斗！！！");
//            GetitemsShow->show();
//        }
//            break;
//        case 12:
//        {
//            GetitemsShow->setText("突然从箱子里跳出一只红鸭子并飞向你，你不得不与之战斗！！！");
//            GetitemsShow->show();
//        }
        case 16:
        {
            GetitemsShow->setText("不能使用破墙镐，魔法值低于20！");
            GetitemsShow->show();
        }
            break;
        case 17:
        {

            GetitemsShow->setText("不能使用下楼器，魔法值低于40！");
            GetitemsShow->show();

        }
            break;
        case 18:
        {
            GetitemsShow->setText("打不开门, 缺少紫钥匙！");
            GetitemsShow->show();
        }
            break;
        case 26:
        {
            GetitemsShow->setText("获得绿龟壳，防御力加 3");
            GetitemsShow->show();
        }
            break;
        case 27:
        {
            GetitemsShow->setText("获得红龟壳，攻击力加 2");
            GetitemsShow->show();
        }
            break;
        case 28:
        {
            GetitemsShow->setText("获得铁剑，攻击力加 8");
            GetitemsShow->show();
        }
            break;//一定要break;不然会接着执行后面的语句
        case 29:
        {
            GetitemsShow->setText("获得花剑，攻击力加 16");
            GetitemsShow->show();
        }
            break;
        case 30:
        {
            GetitemsShow->setText("获得金剑，攻击力加 24");
            GetitemsShow->show();
        }
            break;
        case 31:
        {
            GetitemsShow->setText("获得一把破墙镐");
            GetitemsShow->show();
        }
            break;
        case 32:
        {
            GetitemsShow->setText("发现一个下楼器");
            GetitemsShow->show();
        }
            break;
        case 12:
        {
            GetitemsShow->setText("你没有足够的金币");
            GetitemsShow->show();
        }
            break;
        }
        GetitemNum++;
    }
    else if(GetitemNum<=11)//控制显示时间
    {
        GetitemNum++;
    }
    else
    {
        GetitemNum=0;
        GetitemTimer->stop();
        GetitemsShow->hide();
    }
}
void Dungeon::deadCall()
{
    deadTimes++;
    MAX_HP-=(20*deadTimes);
    MAX_HP=MAX_HP<0?0:MAX_HP;
    if(MAX_HP==0)
        GameOverCall();
    MAX_MP-=(5*deadTimes);
    MAX_MP=MAX_MP<0?0:MAX_MP;
    role.hp=MAX_HP/deadTimes;
    role.mp=MAX_MP;
    role.exp=0;
    role.scoreNum-=(40*deadTimes);
    if(role.scoreNum<0)
        role.scoreNum=0;
    itemNum=-1;
    GetitemTimer->start(100);
    isDead=true;
    music->SoundPlay(5);
    People->setStyleSheet("border-image: url(:/Dungeon/image/Dungeon/dead.png);");
}
void Dungeon::GameOverCall()
{
    menu->show();
    menu->pauseEvent->setText("GAME OVER！");
    menu->Continue->hide();
}

void Dungeon::FightWinshow()//战斗界面
{
    if (fight_period_it == 0)
    {
        isFighting=true;
        //准备战斗界面
        Monster_hp->setText(QString::number(enemy[MonsterNum].hp));
        Monster_atk->setText(QString::number(enemy[MonsterNum].atk));
        Monster_def->setText(QString::number(enemy[MonsterNum].def));
        Character_hp->setText(QString::number(role.hp));
        Character_atk->setText(QString::number(role.atk));
        Character_def->setText(QString::number(role.def));
        FightWidget->show();
        Monster_pic->setStyleSheet(MonsterPic[MonsterNum]);
        Monster_name->setText(enemy[MonsterNum].name);
        fight_period_it = 1;
        music->FightSound->play();
        music->FightSoundTimer->start(100);
    }//fight_period_it 代表对战次数，奇数次计算人物对怪物造成的伤害，偶数次计算怪物对人造成的伤害
    else if (fight_period_it % 2 == 1 && fight_end_it == 0)
    {
        int damage=role.atk - enemy[MonsterNum].def;
        if (enemy[MonsterNum].hp - (damage>0?damage:0) * (fight_period_it / 2 + 1) <= 0)
        {
            Monster_hp->setText(QString::number(0));//fight_period_it / 2 + 1 代表人物对怪物攻击的累计次数
            fight_end_it = 1;
        }
        else
        {
            Monster_hp->setText(QString::number(enemy[MonsterNum].hp - (damage>0?damage:0) * (fight_period_it / 2 + 1)));
            fight_period_it++;
        }

    }
    else if (fight_period_it % 2 == 0 && fight_end_it == 0)
    {
        int damage=enemy[MonsterNum].atk - role.def;
        if(role.hp - (damage>0?damage:0) * (fight_period_it / 2)<=0)
        {
            Character_hp->setText(QString::number(0));
            fight_end_it = 1;
        }
        else
        {
            if(enemy[MonsterNum].atk > role.def)
                Character_hp->setText(QString::number(role.hp - (damage>0?damage:0) * (fight_period_it / 2)));
            fight_period_it++;
        }
    }
    else
    {
        if (fight_end_it <= 2)//可以控制界面存在时长
        {
            fight_end_it++;
        }
        else if(fight_end_it == 3)
        {
            //结算战斗结果
            int damage = calc_damage(MonsterNum);
            if(damage==-1||damage<0||role.hp-damage<=0)
                deadCall();
            else
            {
                role.hp -= damage;
                role.scoreNum += enemy[MonsterNum].gold;
                role.exp += enemy[MonsterNum].exp;
                itemNum=-2;
                GetitemTimer->start(100);
            }
            //隐藏战斗界面
            FightWidget->hide();
            music->FightSound->stop();
            music->FightSoundTimer->stop();
            updateStatusData();//更新战斗结果
            fight_end_it++;
        }
        else if (fight_end_it <= 5)//可以控制界面存在时长
        {
            fight_end_it++;
        }
        else //fight_end_it =6 的时候,战斗状态信息栏才隐藏
        {
            FightTimer->stop();
            if(!isDead)
                Move();
            isFighting=false;
            fight_end_it = 0;
            fight_period_it = 0;
            MonsterNum = 0;
            isDead=false;
        }
    }
}
bool Dungeon::isStatic(int num)//静态贴图
{
    return (num>=0&&num<=3)||(num>=6&&num<=13)||(num>=23&&num<=32);
}

bool Dungeon::isDynamic(int floor,int i,int j)
{
    return map[floor][i][j]>=14&&map[floor][i][j]<=22;
}
void Dungeon::Move()
{
    switch(moveDirection)
    {
    case 0://up
    {
        x-=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[2][0]);
    }
        break;
    case 1://down
    {
        x+=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[3][0]);
    }
        break;
    case 2://left
    {
        y-=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[4][moveNum%4]);
    }
        break;
    case 3://right
    {
        y+=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[5][moveNum%4]);
    }
        break;
    }
    moveNum++;
    updateFogArea(floor-1,x,y);
}
void Dungeon::ShowdynamicEffect()
{
    if (display_it <= 2)
        display_it += 1;
    else
        display_it = 0;
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            if(isDynamic(floor-1,i,j))
            {
                MazeWidget[i][j]->setStyleSheet(DungeonStytle[map[floor-1][i][j]][display_it]);
            }
            else
            {
                MazeWidget[i][j]->setStyleSheet(DungeonStytle[map[floor-1][i][j]][0]);
            }
        }
    }

}
void Dungeon::OpenStore()
{
    if(role.scoreNum-Store->Currstore_price<0)//金币不够，不能购买
    {
        itemNum=12;//提示玩家
        GetitemTimer->start(100);
        Store->haveBuyTimer->stop();
    }
    else
    {
        role.scoreNum-=Store->Currstore_price;
        if(Store->chooseOption==0)
        {
            MAX_HP+=100 * (Store->BuyTimes-1);
            role.hp+=100 * (Store->BuyTimes-1);
            hp->setText(QString::number(role.hp));
            score->setText(QString::number(role.scoreNum));
            if(role.scoreNum-Store->Nextstore_price<0)//判断下一次是否可以购买
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();//购买完成
        }
        else if(Store->chooseOption==1)
        {
            role.mp+=20 * (Store->BuyTimes-1);
            mp->setText(QString::number(role.mp));
            score->setText(QString::number(role.scoreNum));

            if(role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else if(Store->chooseOption==2)
        {
            role.atk+=2;
            atk->setText(QString::number(role.atk));
            score->setText(QString::number(role.scoreNum));

            if(role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else if(Store->chooseOption==3)
        {
            role.def+=4;
            def->setText(QString::number(role.def));
            score->setText(QString::number(role.scoreNum));

            if(role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else
        {
            return;
        }

    }
    return;
}
void Dungeon::changeHP(int num)
{
    if(role.hp+num>=MAX_HP)
    {
        role.hp=MAX_HP;
    }
    else
    {
        role.hp+=num;
    }
}
void Dungeon::changeMP(int num)
{
    if(role.mp+num>=MAX_MP)
    {
        role.mp=MAX_MP;
    }
    else
    {
        role.mp+=num;
    }
}

void Dungeon::QuestionBox(int num)
{
    int temp1=0;
    int temp2=0;
    if(num==0)
    {
        temp1=x-1;
        temp2=y;
    }
    else if(num==1)
    {
        temp1=x+1;
        temp2=y;
    }
    else if(num==2)
    {
        temp1=x;
        temp2=y-1;
    }
    else if(num==3)
    {
        temp1=x;
        temp2=y+1;

    }
    QMessageBox message(QMessageBox::Information,"发现箱子！","箱子上写着问号，不知道里面装着什么，是否打开？",QMessageBox::Yes|QMessageBox::No,ptr_MianWindow);
    message.setIconPixmap(QPixmap(":/info/image/information/box.png"));
    message.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
    if (message.exec()==QMessageBox::Yes)
    {
        int Num=rand()%5+23;
        switch(Num)
        {
        case 23:
        {
            itemNum=23;
            GetitemTimer->start(100);
            changeHP(100);
            updateStatusData();
            map[floor-1][temp1][temp2]=1;
            MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[1][0]);
            return;
        }
            break;
        case 24:
        {
            itemNum=24;
            GetitemTimer->start(100);
            changeMP(20);
            updateStatusData();
            map[floor-1][temp1][temp2]=1;
            MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[1][0]);
            return;
        }
            break;
        case 25:
        {
            itemNum=25;
            GetitemTimer->start(100);
            MAX_HP+=100;
            changeHP(100);
            updateStatusData();
            map[floor-1][temp1][temp2]=1;
            MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[1][0]);
            return;
        }
            break;
        case 26:
        {
            QMessageBox mes(QMessageBox::NoIcon, "你打开了箱子", "突然从箱子里跳出一只绿鸭子并冲向你，你不得不与之战斗！！！");
            mes.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
            mes.setIconPixmap(QPixmap(":/info/image/information/greenDuck.png"));
            mes.exec();

            if(pk(enemy[2])==-2)
            {
                itemNum=0;
                GetitemTimer->start(100);
                map[floor-1][temp1][temp2]=18;
                MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[18][0]);
            }
            else
            {
                MonsterNum=2;
                FightTimer->start(50);
                moveDirection=num;
            }
            return;
        }
            break;
        case 27:
        {
            QMessageBox mes(QMessageBox::NoIcon, "你打开了箱子", "突然从箱子里跳出一只红鸭子并飞向你，你不得不与之战斗！！！");
            mes.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
            mes.setIconPixmap(QPixmap(":/info/image/information/flyDuck.png"));
            mes.exec();

            if(pk(enemy[3])==-2)
            {
                itemNum=0;
                GetitemTimer->start(100);
                map[floor-1][temp1][temp2]=19;
                MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[19][0]);
            }
            else
            {
                MonsterNum=3;
                FightTimer->start(50);
                moveDirection=num;
            }
            return;
        }
            break;
        }
    }
    else
    {
        return;
    }

}
void Dungeon::items(int _itemNum, int _moveDirection)
{
    QLabel *target=new QLabel;
    int *tempNum=nullptr;
    switch(_itemNum)
    {
    case 23:
    {
        changeHP(100);
        hp->setText(QString::number(role.hp));
        music->SoundPlay(4);
    }
        break;
    case 24:
    {
        changeMP(20);
        mp->setText(QString::number(role.mp));
        music->SoundPlay(4);
    }
        break;
    case 25:
    {
        MAX_HP+=50;
        changeHP(50);
        hp->setText(QString::number(role.hp));
        music->SoundPlay(4);
    }
        break;
    case 19:
    {
        target=redkeyNum;
        tempNum=&(role.redkey);
        music->SoundPlay(3);
    }
        break;
    case 31:
    {
        target=PickaxeNum;
        tempNum=&(role.Pickaxe);
        music->SoundPlay(3);
    }
        break;
    case 32:
    {
        target=DownstairsNum;
        tempNum=&(role.Downstairs);
        music->SoundPlay(3);
    }
        break;
    case 26:
    {
        role.def+=3;
        def->setText(QString::number(role.def));
        music->SoundPlay(3);
    }
        break;
    case 27:
    {
        role.atk+=2;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(3);
    }
        break;
    case 28:
    {
        role.atk+=8;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(1);
    }
        break;
    case 29:
    {
        role.atk+=16;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(1);
    }
        break;
    case 30:
    {
        role.atk+=24;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(1);
    }
        break;
    }
    itemNum=_itemNum;
    GetitemTimer->start(100);
    moveDirection=_moveDirection;
    Move();
    if(itemNum==31||itemNum==32||itemNum==21)
    {
        (*tempNum)+=1;
        target->setText(QString::number(*tempNum));
    }
}
void Dungeon::Monsters(int _MonsterNum,int _moveDirection)
{
    if(pk(enemy[MonsterNum])==-2)
    {
        itemNum=0;
        GetitemTimer->start(100);
        return;
    }
    else
    {
        MonsterNum=_MonsterNum;
        FightTimer->start(50);
        moveDirection=_moveDirection;
    }
}
void Dungeon::checkPrefloor()
{
    if(floor==1)
    {
        QMessageBox message(QMessageBox::NoIcon, "对不起！", "你不能走出地牢！");
        message.setIconPixmap(QPixmap(":/info/image/information/sorry.png"));
        message.exec();
        return;
    }
    isPre=true;
    initialCharacterPos();
    People->hide();
    floor-=1;
    Floor->setText(QString::number(floor));
    ShowDungeon();
    ShowFog();
    music->SoundPlay(2);//放在尾部才会音效持久
}

void Dungeon::checkNextfloor()
{
    if(floor==Total_Floor)
    {
        QMessageBox message(QMessageBox::NoIcon, "你想干啥？", "这是地牢最后一层了！");
        message.setIconPixmap(QPixmap(":/info/image/information/question.png"));
        message.exec();
        return;
    }
    isNext=true;
    initialCharacterPos();
    People->hide();
    floor+=1;
    Floor->setText(QString::number(floor));
    ShowDungeon();
    ShowFog();
    music->SoundPlay(2);
}
void Dungeon::breakWall(int _moveDirection)
{
    if(!PickaxeUse)
        return;
    if(role.Pickaxe==0)
        return;
    else if(role.mp<20)
    {
        itemNum=16;
        GetitemTimer->start(100);
        return;
    }
    QMessageBox message(QMessageBox::Information,"你有破墙镐","是否使用？",QMessageBox::Yes|QMessageBox::No,ptr_MianWindow);
    message.setIconPixmap(QPixmap(":/info/image/information/Pickaxe.png"));
    message.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
    if (message.exec()==QMessageBox::Yes)
    {
        role.Pickaxe--;
        role.mp-=20;
        updateStatusData();
        moveDirection=_moveDirection;
        Move();
    }
    else
        return;
}
void Dungeon::DownstairsRun()
{
    if(role.Downstairs==0)
        return;
    else if(role.mp<40)
    {
        itemNum=17;
        GetitemTimer->start(100);
        return;
    }
    role.Downstairs--;
    role.mp-=20;
    updateStatusData();

    bool findflag=false;
    int i,j;
    for(i=0;i<12;i++)
    {
        for(j=0;j<16;j++)
        {
            if(map[floor-1][i][j]==7)
            {
                findflag=true;
                break;
            }
        }
        if(findflag)
            break;
    }
    updateFogArea(floor-1,i,j);
}

void Dungeon::addFogArea(int floor,int i,int j,int num)
{
    if(i>=0&&j>=0&&i<12&&j<16)
    {
        FogArr[floor][i][j]+=num;
        if(FogArr[floor][i][j]==8||FogArr[floor][i][j]>=13)
            FogArr[floor][i][j]=15;
        QPalette palette;
        palette.setBrush(QPalette::Background,
                QBrush(QImage(":/Dungeon/image/Dungeon/fog.png").
                       copy((FogArr[floor][i][j]/4)*54,(FogArr[floor][i][j]%4)*58,54,58)
                            ));
        FogWidget[i][j]->setPalette(palette);
    }
}

void Dungeon::updateFogArea(int floor,int i,int j)
{
    if(haveVisited[floor][i][j]==0)
    {
        addFogArea(floor,i,j,15);//当前
        addFogArea(floor,i-1,j,12);//上
        addFogArea(floor,i+1,j,3);//下
        addFogArea(floor,i,j-1,5);//左
        addFogArea(floor,i,j+1,10);//右
//        addFogArea(floor,i-1,j-1,4);//左上
//        addFogArea(floor,i-1,j+1,8);//右上
//        addFogArea(floor,i+1,j-1,1);//左下
//        addFogArea(floor,i+1,j+1,2);//右下
        haveVisited[floor][i][j]=1;
    }
}

void Dungeon::Operation(int num)
{
    bool flag;
    int _x,_y;
    if(num==0)
    {
        flag=x>0;
        _x=x-1;
        _y=y;
    }
    else if(num==1)
    {
        flag=x<11;
        _x=x+1;
        _y=y;
    }
    else if(num==2)
    {
        flag=y>0;
        _x=x;
        _y=y-1;
    }
    else if(num==3)
    {
        flag=y<15;
        _x=x;
        _y=y+1;
    }
    else
        return;

    if(flag&&map[floor-1][_x][_y]==0)
    {
        breakWall(num);
    }
    else if(flag&&map[floor-1][_x][_y]==1)
    {
        moveDirection=num;
        Move();
    }
    else if(flag&&map[floor-1][_x][_y]==6)
    {
        checkPrefloor();
    }
    else if(flag&&map[floor-1][_x][_y]==7)
    {
        checkNextfloor();
    }
    else if(flag&&map[floor-1][_x][_y]==12)
    {
        Store->show();
        if(role.scoreNum-Store->Currstore_price>=0)
        {
            Store->setChooseEnable=true;
        }
        Store->StoreWinisShow=true;
    }
    else if(flag&&map[floor-1][_x][_y]==14)
    {
        moveDirection=num;
        Move();
        role.scoreNum+=10;
        score->setText(QString::number(role.scoreNum));
        music->SoundPlay(0);
    }
    else if(flag&&map[floor-1][_x][_y]==15)
    {
        Monsters(0,num);
    }
    else if(flag&&map[floor-1][_x][_y]==16)
    {
        Monsters(1,num);
    }
    else if(flag&&map[floor-1][_x][_y]==17)
    {
        QuestionBox(num);
    }
    else if(flag&&map[floor-1][_x][_y]==18)
    {
        Monsters(2,num);
    }
    else if(flag&&map[floor-1][_x][_y]==19)
    {
        Monsters(3,num);
    }
    else if(flag&&map[floor-1][_x][_y]==20)
    {
        Monsters(4,num);
    }
    else if(flag&&map[floor-1][_x][_y]==21)
    {
        Monsters(5,num);
    }
    else if(flag&&map[floor-1][_x][_y]==22)
    {
        Monsters(6,num);
    }
    else if(flag&&map[floor-1][_x][_y]==23)
    {
        items(23,num);
    }
    else if(flag&&map[floor-1][_x][_y]==24)
    {
        items(24,num);
    }
    else if(flag&&map[floor-1][_x][_y]==25)
    {
        items(25,num);
    }
    else if(flag&&map[floor-1][_x][_y]==26)
    {
        items(26,num);
    }
    else if(flag&&map[floor-1][_x][_y]==27)
    {
        items(27,num);
    }
    else if(flag&&map[floor-1][_x][_y]==28)
    {
        items(28,num);
    }
    else if(flag&&map[floor-1][_x][_y]==29)
    {
        items(29,num);
    }
    else if(flag&&map[floor-1][_x][_y]==30)
    {
        items(30,num);
    }
    else if(flag&&map[floor-1][_x][_y]==31)
    {
        items(31,num);
    }
    else if(flag&&map[floor-1][_x][_y]==32)
    {
        items(32,num);
    }
    else
    {
        return;
    }
}

void Dungeon::keyPressEvent(QKeyEvent *event)
{
    if(isFighting)//keyevent 是实时检测的，只要在战斗结束后再设置isFighting为false，就可以了
    {
        return;
    }
    else if(Store->StoreWinisShow)
    {
        return;
    }
    else if(menu->MenuWinShow)
    {
        return;
    }

    if(event->key()==Qt::Key_W||event->key()==Qt::Key_Up)
        Operation(0);
    else if(event->key()==Qt::Key_S||event->key()==Qt::Key_Down)
        Operation(1);
    else if(event->key()==Qt::Key_A||event->key()==Qt::Key_Left)
        Operation(2);
    else if(event->key()==Qt::Key_D||event->key()==Qt::Key_Right)
        Operation(3);
    else if(event->key()==Qt::Key_P)
        PickaxeUse=!PickaxeUse;
    else if(event->key()==Qt::Key_N)
        DownstairsRun();
    else if(event->key()==Qt::Key_Escape)
        menu->show();
    else
        return;
}

