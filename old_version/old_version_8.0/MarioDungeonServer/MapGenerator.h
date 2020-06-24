#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
using namespace std;
class MapGenerator
{
public:
	MapGenerator();
	void initialMap();
	int ***map;//三维数组
	int Total_Floor;
private:
	void RandomGeneraterMap(int arr[12][16], int floor);
    void GenerateElement(int arr[12][16],int floor);
    void GenerateMonster(int arr[12][16],int floor);
    void GenerateItem(int arr[12][16],int floor);
    void GenerateEquip();
    bool isValid(int arr[12][16],int i,int j,int floor);
    int sum(int arr[],int n);
    void doDfs(int arr[12][16],stack<std::pair<int,int>>& s,bool visited[12][16]);
    void setExit(int arr[12][16], std::pair<int, int> &start, std::pair<int, int> &end);
    void dfs(int arr[12][16], bool visited[12][16], stack<std::pair<int, int> > &s, int i, int j);
    void bfs(int arr[12][16], stack<std::pair<int,int>> &s,std::pair<int, int> start, std::pair<int, int> end);
};

#endif  //MAPGENERATOR_H