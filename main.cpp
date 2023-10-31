
// C++ program for the above approach 
  
#include <bits/stdc++.h> 
using namespace std; 
  
// DSU data structure 
// path compression + rank by union 
class DSU { 
    int* parent; 
    int* rank; 
  
public: 
    DSU(int n) 
    { 
        parent = new int[n]; 
        rank = new int[n]; 
  
        for (int i = 0; i < n; i++) { 
            parent[i] = -1; 
            rank[i] = 1; 
        } 
    } 
  
    // Find function 
    int find(int i) 
    { 
        if (parent[i] == -1) {
            return i;
        }
        return parent[i] = find(parent[i]); 
    } 
  
    // Union function 
    void unite(int x, int y) 
    { 
        int s1 = find(x); 
        int s2 = find(y); 
  
        if (s1 != s2) { 
            if (rank[s1] < rank[s2]) { 
                parent[s1] = s2; 
            } 
            else if (rank[s1] > rank[s2]) { 
                parent[s2] = s1; 
            } 
            else { 
                parent[s2] = s1; 
                rank[s1] += 1; 
            } 
        } 
    } 
}; 
  
class Graph { 
    vector<vector<int> > edgelist;
    vector<vector<int> > removelist;
    int V;
  
public: 
    Graph(int V) { 
        this->V = V;
    } 
    // Function to add edge in a graph 
    void addEdge(int x, int y, int w) 
    { 
        edgelist.push_back({ w, x, y }); 
    } 
    
    void checkEdge(int x, int y, int w) 
    { 
        removelist.push_back({ w, x, y }); 
    } 
  
    int callKruskals_mst(){
        DSU s(V);
        std::cout<<"we got here\n";
        int cost2=destroy_loops(s);
        int cost1=kruskals_mst(s);
        return cost1+cost2;
    }
    
    int kruskals_mst(DSU s) 
    { 
        // Sort all edges 
        sort(edgelist.begin(), edgelist.end()); 
  
        // Initialize the DSU
        int ans = 0; //final cost
        cout << "Following are the edges to be constructed in the MST" << endl; 
        for (auto edge : edgelist) { 
            int w = edge[0]; 
            int x = edge[1]; 
            int y = edge[2]; 
  
            // Take this edge in MST if it does 
            // not forms a cycle 
            if (s.find(x) != s.find(y)) { 
                s.unite(x, y); 
                ans += w; 
                cout << x << " -- " << y << " == " << w 
                     << endl; 
            } 
        } 
        cout << "Minimum Cost Spanning Tree: " << ans;
        return ans;
    } 

    int destroy_loops(DSU s) 
    { 
        // Sort all edges 
        sort(removelist.begin(), removelist.end());
        std::reverse(removelist.begin(), removelist.end());
        // Initialize the DSU 
        int ans = 0; //final cost
        cout << "Following are the edges to be "
                "deconstructed from MST"
             << endl; 
        for (auto edge : removelist) { 
            int w = edge[0]; 
            int x = edge[1]; 
            int y = edge[2]; 
  
            // Take this edge in MST if it does 
            // not forms a cycle
            if (s.find(x) != s.find(y)) { 
                s.unite(x, y); 
            } else {
                ans += w; 
                cout << x << " -- " << y << " == " << w 
                     << endl;
            }
        }
        cout << "cost of destroying roads: " << ans<<"\n"; 
        return ans;
    } 
};


// Function to convert a character to its corresponding cost
int convertCharToIndex(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        // Convert uppercase letters 'A' to 'Z' to the range 0 to 25
        return ch - 'A';
    } else if (ch >= 'a' && ch <= 'z') {
        // Convert lowercase letters 'a' to 'z' to the range 26 to 51
        return ch - 'a' + 26;
    } else {
        // will be integers
        return ch - '0';
    }
}
  
// Driver code 
int main() 
{ 
    
    // check roads already built if they cause loop:
    //      sort all roads already made by least cost to destroy
    //      check if any form loops from the cheapest road, if they do, destroy the road.
    //      repeat with next cheapest road
    //      now add all leftover roads to the prebuilt array
    // now time to initialize the roads to be built
    //      count amount of preconnections, start with array already premade with already made roads
    //      sort all roads by cost to build
    //      build cheapest road if it doesnt make loop
    //      repeat until n-1 roads, with the next cheapest build
    
    std::string str1, str2, str3;

    // Input format: "country, build, destroy"
    std::cin >> str1 >> str2 >> str3;

    str1.erase(std::remove(str1.begin(), str1.end(), ','), str1.end());
    str2.erase(std::remove(str2.begin(), str2.end(), ','), str2.end());
    str3.erase(std::remove(str3.begin(), str3.end(), ','), str3.end());

    // Parse N from the length of the input strings
    int N; // Number of cities
    N = sqrt(str1.length());
    
    Graph b(N);

    // Initialize matrices for country, build, and destroy
    int country[N][N];
    int build[N][N];
    int destroy[N][N];

    // Parse the "country" string and fill the country matrix
    int indexcount=0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value=convertCharToIndex(str1[indexcount]);
            country[i][j] = value;
            indexcount++;
        }
    }
    
     // Parse the "build" string and fill the country matrix
    indexcount=0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value=convertCharToIndex(str2[indexcount]);
            build[i][j] = value;
            indexcount++;
        }
    }
    
     // Parse the "destroy" string and fill the country matrix
    indexcount=0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value=convertCharToIndex(str3[indexcount]);
            destroy[i][j] = value;
            indexcount++;
        }
    }
    
    //011000,101000,110000,000011,000101,000110 ABDFFF,BACFFF,DCAFFF,FFFABD,FFFBAC,FFFDCA ABDFFF,BACFFF,DCAFFF,FFFABD,FFFBAC,FFFDCA
    /*000,000,000 ABC,DEF,GHI ABC,DEF,GHI
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout<<country[i][j]<<", ";
            std::cout<<build[i][j]<<", ";
            std::cout<<destroy[i][j]<<"\n";
        }
    }*/

    for (int i = 0; i < N-1; i++) {
        for (int j = i+1; j < N; j++) {
            if (country[i][j]==1){
            b.checkEdge(i,j,destroy[i][j]);
            }
        }
    }
    
    for (int i = 0; i < N-1; i++) {
        for (int j = i+1; j < N; j++) {
            if (country[i][j]==0){
            b.addEdge(i,j,build[i][j]);
            }
        }
    }
    //int finalCost=b.callKruskals_mst();
    int finalCost=b.callKruskals_mst();
    std::cout<<"\nthis is the final cost: "<<finalCost;
    /*
    // graph with 4 edges
    Graph g(4); 
    // begin, end, cost
    g.addEdge(0, 1, 10); 
    g.addEdge(1, 3, 15); 
    g.addEdge(2, 3, 4); 
    g.addEdge(2, 0, 6); 
    g.addEdge(0, 3, 5); 
  */
    // Function call 
  
    return 0; 
}
