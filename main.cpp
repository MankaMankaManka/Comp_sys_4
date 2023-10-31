#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class DSU {
    public:
    int* parentArray;
    int* rankArray;
    DSU(int n) {
        parentArray = new int[n];
        rankArray = new int[n];
        for (int i = 0; i < n; i++) {
            parentArray[i] = -1;
            rankArray[i] = 1;
        }
    }
    int find(int vertex) {
        if (parentArray[vertex] == -1) {
            return vertex;
        }
        return parentArray[vertex] = find(parentArray[vertex]);
    }

    void unite(int vertexX, int vertexY) {
        int rootX = find(vertexX);
        int rootY = find(vertexY);

        if (rootX != rootY) {
            if (rankArray[rootX] < rankArray[rootY]) {
                parentArray[rootX] = rootY;
            } 
            else if (rankArray[rootX] > rankArray[rootY]) {
                parentArray[rootY] = rootX;
            } 
            else {
                parentArray[rootY] = rootX;
                rankArray[rootX] += 1;
            }
        }
    }
};

class Graph {
    std::vector<std::vector<int>> edgeList;
    std::vector<std::vector<int>> removalList;
    int numVertices;

public:
    Graph(int numVertices) {
        this->numVertices = numVertices;
    }

    void addEdge(int vertexX, int vertexY, int weight) {
        edgeList.push_back({ weight, vertexX, vertexY });
    }
    void checkEdge(int vertexX, int vertexY, int weight) {
        removalList.push_back({ weight, vertexX, vertexY });
    }

    int computeTotalCost() {
        DSU roadNetwork(numVertices);

        int removalCost = destroyLoops(roadNetwork);
        int edgeCost = computeKruskalsMST(roadNetwork);

        return edgeCost + removalCost;
    }

    int computeKruskalsMST(DSU roadNetwork) {
        sort(edgeList.begin(), edgeList.end());
        int totalEdgeCost = 0;

        for (auto edge : edgeList) {
            int weight = edge[0];
            int vertexX = edge[1];
            int vertexY = edge[2];

            if (roadNetwork.find(vertexX) != roadNetwork.find(vertexY)) {
                roadNetwork.unite(vertexX, vertexY);
                totalEdgeCost += weight;
            }
        }
        return totalEdgeCost;
    }

    int destroyLoops(DSU roadNetwork) {
        sort(removalList.begin(), removalList.end());
        std::reverse(removalList.begin(), removalList.end());
        int totalRemovalCost = 0;

        for (auto edge : removalList) {
            int weight = edge[0];
            int vertexX = edge[1];
            int vertexY = edge[2];

            if (roadNetwork.find(vertexX) != roadNetwork.find(vertexY)) {
                roadNetwork.unite(vertexX, vertexY);
            } else {
                totalRemovalCost += weight;
            }
        }
        return totalRemovalCost;
    }
};

int convertCharToIndex(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A';
    } else if (ch >= 'a' && ch <= 'z') {
        return ch - 'a' + 26;
    } else {
        return ch - '0';
    }
}

int main() {
    std::string countryData, buildData, destroyData;

    // Input format: country, build, destroy
    std::cin >> countryData >> buildData >> destroyData;

    countryData.erase(std::remove(countryData.begin(), countryData.end(), ','), countryData.end());
    buildData.erase(std::remove(buildData.begin(), buildData.end(), ','), buildData.end());
    destroyData.erase(std::remove(destroyData.begin(), destroyData.end(), ','), destroyData.end());

    int numVertices;
    numVertices = sqrt(countryData.length());
    Graph roadGraph(numVertices);

    int countryMatrix[numVertices][numVertices];
    int buildMatrix[numVertices][numVertices];
    int destroyMatrix[numVertices][numVertices];

    int index = 0;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            int value = convertCharToIndex(countryData[index]);
            countryMatrix[i][j] = value;
            index++;
        }
    }

    index = 0;

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            int value = convertCharToIndex(buildData[index]);
            buildMatrix[i][j] = value;
            index++;
        }
    }

    index = 0;

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            int value = convertCharToIndex(destroyData[index]);
            destroyMatrix[i][j] = value;
            index++;
        }
    }

    for (int i = 0; i < numVertices - 1; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (countryMatrix[i][j] == 1) {
                roadGraph.checkEdge(i, j, destroyMatrix[i][j]);
            }
        }
    }

    for (int i = 0; i < numVertices - 1; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (countryMatrix[i][j] == 0) {
                roadGraph.addEdge(i, j, buildMatrix[i][j]);
            }
        }
    }

    int finalCost = roadGraph.computeTotalCost();
    std::cout << finalCost;

    return 0;
}
