#include "MapGenerator.h"
std::vector<std::vector<int>> directions{{0,-1},{0,1},{-1,0},{1,0}};
MapGenerator::MapGenerator()
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
}
int MapGenerator::sum(int arr[],int n)
{
    if(arr==nullptr)
        return 0;
    int res=0;
    for(int i=0;i<n;++i)
        res+=arr[i];
    return res;
}
void MapGenerator::dfs(int arr[12][16],bool visited[12][16],stack<std::pair<int,int>> &s,int i,int j)
{
    if(i<0||i>=12||j<0||j>=16||arr[i][j]==0||visited[i][j])
        return;
    visited[i][j]=true;
    s.push(std::pair<int,int>(i,j));
    for(auto dir:directions)
        dfs(arr,visited,s,i+dir[0],j+dir[1]);
}
void MapGenerator::bfs(int arr[12][16],stack<std::pair<int,int>> &s,std::pair<int,int> start,std::pair<int,int> end)
{
    bool visited[12][16]{false};
    queue<std::pair<int, int>> q;
    q.push(start);
    visited[start.first][start.second] = true;
    while (!q.empty())
    {
        int size = q.size();
        while (size-- > 0)
        {
            std::pair<int, int> temp = q.front();;
            int ci = temp.first, cj = temp.second;
			q.pop();
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
                q.push(std::pair<int, int>(ni, nj));
            }
        }
    }
}
void MapGenerator::doDfs(int arr[12][16],stack<std::pair<int,int>>& s,bool visited[12][16])
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

void MapGenerator::setExit(int arr[12][16],std::pair<int,int> &start,std::pair<int,int> &end)
{    
    stack<std::pair<int,int>> s;
    bool visited[12][16]{false};

    doDfs(arr,s,visited);
    //int startPos=rand()%floor;
    int startPos=0;
	stack<std::pair<int,int>> dups=s;
	while(dups.size()>static_cast<size_t>(startPos+1))
		dups.pop();
    int starti=dups.top().first,startj=dups.top().second;
    int endi=s.top().first,endj=s.top().second;

    while(std::abs(starti-endi)<=3||std::abs(startj-endj)<=3)
    {
        while(!s.empty())
			s.pop();
        for(int i=0;i<12;++i)
        {
            for(int j=0;j<16;++j)
                if(visited[i][j])
                    visited[i][j]=false;
        }
        doDfs(arr,s,visited);
		stack<std::pair<int,int>> dups=s;
		while(dups.size()>static_cast<size_t>(startPos+1))
			dups.pop();
    	starti=dups.top().first;
		startj=dups.top().second;
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
void MapGenerator::GenerateMonster(int arr[12][16],int floor)
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
void MapGenerator::GenerateItem(int arr[12][16],int floor)
{
    int totalBuffItemNum=5-floor+1;
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

    int totalToolItemNum=4-floor+1;
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
void MapGenerator::GenerateEquip()
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

void MapGenerator::GenerateElement(int arr[12][16],int floor)
{
    GenerateMonster(arr,floor);
    GenerateItem(arr,floor);
}
bool MapGenerator::isValid(int arr[12][16],int i,int j,int floor)
{
    return arr[i][j]==1||arr[i][j]==6||(arr[i][j]>=8&&arr[i][j]<=14+(floor<=2?1:floor))
            ||(arr[i][j]>=23&&arr[i][j]<=27)||arr[i][j]==31||arr[i][j]==32;
}

void MapGenerator::RandomGeneraterMap(int arr[12][16],int floor)
{
    std::pair<int,int> start;
    std::pair<int,int> end;
    setExit(arr,start,end);
    GenerateElement(arr,floor);
    stack<std::pair<int,int>> s;
    bfs(arr,s,start,end);
    while(!s.empty())
    {
        std::pair<int,int> temp=s.top();
		s.pop();
        if(!isValid(arr,temp.first,temp.second,floor))
            arr[temp.first][temp.second]=rand()%(floor<3?2:5)+14;
    }
}

void MapGenerator::initialMap()
{
    int floor=1;
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
    GenerateEquip();
}
