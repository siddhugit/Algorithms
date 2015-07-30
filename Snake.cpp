#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdlib>
using namespace std;


struct Node
{
      int length;
      vector<pair<int,int> > cords;
      bool operator==(const Node& other) const
      {
            for(int i = 0;i < length; ++i){
                  bool ret = (cords[i].first == other.cords[i].first && cords[i].second == other.cords[i].second);
                  if(!ret)
                        return false;
            }
            return true;
      }
	  bool operator<(const Node& other) const
      {
            for(int i = 0;i < length; ++i){
                 if(cords[i].first < other.cords[i].first)
                        return true;
				 else if(cords[i].first > other.cords[i].first)
                        return false;
				 if(cords[i].second < other.cords[i].second)
                        return true;
				 else if(cords[i].second > other.cords[i].second)
                        return false;
            }
            return false;
      }
      Node(int sz):length(sz),cords(sz){}
};

bool isVisited(const set<Node>& visited,const Node& node)
{
	if(visited.find(node) == visited.end())
		return false;
	return true;
}

bool isSnakeAt(int p,int q,const Node& node)
{
      for(int i = 0;i < node.length; ++i){
            bool ret = (node.cords[i].first == p && node.cords[i].second == q);
            if(ret)
                  return true;
      }
      return false;
}

vector<Node> getNextPositions(const set<Node>& visited,const vector<vector<char> >& grid,const Node& node,int m,int n)
{
      vector<Node> newPositions;
      int p,q;
      //row + 1
      p = node.cords[0].first + 1;
      q = node.cords[0].second;
      if(p < m && !isSnakeAt(p,q,node) && grid[p][q] == '.'){
            Node newPosition(node.length);
            newPosition.cords[0].first = p;newPosition.cords[0].second = q;
            for(int i = 1;i < newPosition.length; ++i){
                  newPosition.cords[i].first = node.cords[i-1].first;
				newPosition.cords[i].second = node.cords[i-1].second;
            }
            if(!isVisited(visited,newPosition))
                  newPositions.push_back(newPosition);
      }

      //row - 1
      p = node.cords[0].first - 1;
      q = node.cords[0].second;
      if(p >= 0 && !isSnakeAt(p,q,node) && grid[p][q] == '.'){
            Node newPosition(node.length);
            newPosition.cords[0].first = p;newPosition.cords[0].second = q;
            for(int i = 1;i < newPosition.length; ++i){
                  newPosition.cords[i].first = node.cords[i-1].first;
				newPosition.cords[i].second = node.cords[i-1].second;
            }
            if(!isVisited(visited,newPosition))
                  newPositions.push_back(newPosition);
      }

      //col + 1
      p = node.cords[0].first;
      q = node.cords[0].second + 1;
      if(q < n && !isSnakeAt(p,q,node) && grid[p][q] == '.'){
            Node newPosition(node.length);
            newPosition.cords[0].first = p;newPosition.cords[0].second = q;
            for(int i = 1;i < newPosition.length; ++i){
                newPosition.cords[i].first = node.cords[i-1].first;
				newPosition.cords[i].second = node.cords[i-1].second;
            }
            if(!isVisited(visited,newPosition))
                  newPositions.push_back(newPosition);
      }

      //col - 1
      p = node.cords[0].first;
      q = node.cords[0].second - 1;
      if(q >= 0 && !isSnakeAt(p,q,node) && grid[p][q] == '.'){
            Node newPosition(node.length);
            newPosition.cords[0].first = p;newPosition.cords[0].second = q;
            for(int i = 1;i < newPosition.length; ++i){
				newPosition.cords[i].first = node.cords[i-1].first;
				newPosition.cords[i].second = node.cords[i-1].second;
            }
            if(!isVisited(visited,newPosition))
                  newPositions.push_back(newPosition);
      }
      return newPositions;
}

void bfs(set<Node>& visited,const vector<vector<char> >& grid,const Node& node,map<Node,int>& distance,int m,int n,int appleI,int appleJ)
{
	deque<Node> Q;
	distance[node] = 0;
	Q.push_back(node);
	while(!Q.empty()){
        Node u = Q.front();
        Q.pop_front();
		if(u.cords[0].first == appleI && u.cords[0].second == appleJ){
			cout<<distance[u];
			return;
		}
		visited.insert(u);
		vector<Node> newPositions = getNextPositions(visited,grid,u,m,n);
		vector<Node>::const_iterator it = newPositions.begin();
		for(;it != newPositions.end(); ++it){
			distance[(*it)] = distance[u] + 1;
			Q.push_back(*it);
		}
	}
	cout<<-1;
}

int main()
{
    int m,n,k,appleI,appleJ;
	cin>>m>>n>>k;
	vector<vector<char> > grid(m,vector<char>(n,'.'));
	map<Node,int> distance;
	Node node(k);
	set<Node> visited;
	char line[1024];
	cin.getline(line,1024);
	for(int i = 0; i < m; ++i){
		cin.getline(line,1024);
		for(int j = 0; j < n; ++j){
			if(line[j] > '0' && line[j] <= '9'){
				node.cords[line[j] - '0' - 1].first = i;
				node.cords[line[j] - '0' - 1].second = j;
			}
			else if(line[j] == 'X'){
				grid[i][j] = 'X';
			}
			else if(line[j] == 'A'){
				appleI = i;
				appleJ = j;
			}
		}
      }
	bfs(visited,grid,node,distance,m,n,appleI,appleJ);
      return 0;
}