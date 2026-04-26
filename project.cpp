#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstring>

using namespace std;

const int MAX_CITIES = 100;
const int INF = 1000000000;

class Graph {
private:
    vector<vector<int>> adjMatrix;
    vector<string> cityNames;
    vector<vector<pair<int, int>>> adjList;
    vector<vector<bool>> roadBlocked;

    int findCityIndex(const string& name) {
        for (int i = 0; i < cityNames.size(); i++) {
            if (cityNames[i] == name) {
                return i;
            }
        }
        return -1;
    }

    void findAllPathsDFS(int current, int end, vector<bool>& visited,
                        vector<int>& path, vector<vector<int>>& allPaths) {
        visited[current] = true;
        path.push_back(current);

        if (current == end) {
            allPaths.push_back(path);
        } else {
            for (int i = 0; i < adjList[current].size(); i++) {
                int neighbor = adjList[current][i].first;
                if (!visited[neighbor] && !roadBlocked[current][neighbor]) {
                    findAllPathsDFS(neighbor, end, visited, path, allPaths);
                }
            }
        }

        path.pop_back();
        visited[current] = false;
    }

    int calculatePathDistance(const vector<int>& path) {
        int distance = 0;
        for (int i = 0; i < path.size() - 1; i++) {
            for (int j = 0; j < adjList[path[i]].size(); j++) {
                if (adjList[path[i]][j].first == path[i + 1]) {
                    distance += adjList[path[i]][j].second;
                    break;
                }
            }
        }
        return distance;
    }

public:
    Graph() {
        adjMatrix.resize(MAX_CITIES, vector<int>(MAX_CITIES, 0));
        adjList.resize(MAX_CITIES);
        roadBlocked.resize(MAX_CITIES, vector<bool>(MAX_CITIES, false));

        // Automatically load sample data when Graph is created
        loadSampleData();
    }

    void addCity(const string& name) {
        if (findCityIndex(name) != -1) {
            cout << "City '" << name << "' already exists!\n";
            return;
        }

        if (cityNames.size() >= MAX_CITIES) {
            cout << "Maximum cities reached! Cannot add more cities.\n";
            return;
        }

        cityNames.push_back(name);
        cout << "City '" << name << "' added successfully! Total cities: " << cityNames.size() << "\n";
    }

    void removeCity(const string& name) {
        int index = findCityIndex(name);
        if (index == -1) {
            cout << "City '" << name << "' not found!\n";
            return;
        }

        for (int i = 0; i < cityNames.size(); i++) {
            adjMatrix[i][index] = 0;
            adjMatrix[index][i] = 0;
            roadBlocked[i][index] = false;
            roadBlocked[index][i] = false;
        }

        for (int i = 0; i < cityNames.size(); i++) {
            for (int j = 0; j < adjList[i].size(); j++) {
                if (adjList[i][j].first == index) {
                    adjList[i].erase(adjList[i].begin() + j);
                    j--;
                }
            }
        }

        cityNames.erase(cityNames.begin() + index);
        adjList.erase(adjList.begin() + index);
        adjList.push_back(vector<pair<int, int>>());

        roadBlocked.erase(roadBlocked.begin() + index);
        roadBlocked.push_back(vector<bool>(MAX_CITIES, false));
        for (int i = 0; i < roadBlocked.size(); i++) {
            roadBlocked[i].erase(roadBlocked[i].begin() + index);
            roadBlocked[i].push_back(false);
        }

        vector<vector<int>> newMatrix(MAX_CITIES, vector<int>(MAX_CITIES, 0));
        for (int i = 0; i < cityNames.size(); i++) {
            int oldI = i < index ? i : i + 1;
            for (int j = 0; j < cityNames.size(); j++) {
                int oldJ = j < index ? j : j + 1;
                newMatrix[i][j] = adjMatrix[oldI][oldJ];
            }
        }
        adjMatrix = newMatrix;

        for (int i = 0; i < cityNames.size(); i++) {
            for (int j = 0; j < adjList[i].size(); j++) {
                if (adjList[i][j].first > index) {
                    adjList[i][j].first--;
                }
            }
        }

        cout << "City '" << name << "' removed successfully! Remaining cities: " << cityNames.size() << "\n";
    }

    void addRoad(const string& city1, const string& city2, int distance) {
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found! Please check city names.\n";
            return;
        }

        if (idx1 == idx2) {
            cout << "Cannot connect a city to itself!\n";
            return;
        }

        if (distance <= 0) {
            cout << "Distance must be positive!\n";
            return;
        }

        adjMatrix[idx1][idx2] = distance;
        adjMatrix[idx2][idx1] = distance;
        roadBlocked[idx1][idx2] = false;
        roadBlocked[idx2][idx1] = false;

        bool exists = false;
        for (int i = 0; i < adjList[idx1].size(); i++) {
            if (adjList[idx1][i].first == idx2) {
                exists = true;
                adjList[idx1][i].second = distance;
                break;
            }
        }

        if (!exists) {
            adjList[idx1].push_back(make_pair(idx2, distance));
            adjList[idx2].push_back(make_pair(idx1, distance));
        } else {
            for (int i = 0; i < adjList[idx2].size(); i++) {
                if (adjList[idx2][i].first == idx1) {
                    adjList[idx2][i].second = distance;
                    break;
                }
            }
        }

        cout << "Road added between " << city1 << " and " << city2 << " with distance " << distance << " km\n";
    }

    void removeRoad(const string& city1, const string& city2) {
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found!\n";
            return;
        }

        adjMatrix[idx1][idx2] = 0;
        adjMatrix[idx2][idx1] = 0;
        roadBlocked[idx1][idx2] = false;
        roadBlocked[idx2][idx1] = false;

        for (int i = 0; i < adjList[idx1].size(); i++) {
            if (adjList[idx1][i].first == idx2) {
                adjList[idx1].erase(adjList[idx1].begin() + i);
                break;
            }
        }

        for (int i = 0; i < adjList[idx2].size(); i++) {
            if (adjList[idx2][i].first == idx1) {
                adjList[idx2].erase(adjList[idx2].begin() + i);
                break;
            }
        }

        cout << "Road between " << city1 << " and " << city2 << " removed successfully!\n";
    }

    void simulateTrafficUpdate(const string& city1, const string& city2, int newTime) {
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found!\n";
            return;
        }

        if (adjMatrix[idx1][idx2] == 0) {
            cout << "No road exists between " << city1 << " and " << city2 << "!\n";
            return;
        }

        int oldTime = adjMatrix[idx1][idx2];
        adjMatrix[idx1][idx2] = newTime;
        adjMatrix[idx2][idx1] = newTime;

        for (int i = 0; i < adjList[idx1].size(); i++) {
            if (adjList[idx1][i].first == idx2) {
                adjList[idx1][i].second = newTime;
                break;
            }
        }

        for (int i = 0; i < adjList[idx2].size(); i++) {
            if (adjList[idx2][i].first == idx1) {
                adjList[idx2][i].second = newTime;
                break;
            }
        }

        cout << "Traffic update: " << city1 << " - " << city2 << " time changed from "
             << oldTime << " to " << newTime << " minutes\n";
    }

    void updateRoadCondition(const string& city1, const string& city2, bool blocked) {
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);

        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found!\n";
            return;
        }

        if (adjMatrix[idx1][idx2] == 0) {
            cout << "No road exists between " << city1 << " and " << city2 << "!\n";
            return;
        }

        roadBlocked[idx1][idx2] = blocked;
        roadBlocked[idx2][idx1] = blocked;

        if (blocked) {
            cout << "Road between " << city1 << " and " << city2 << " is now BLOCKED!\n";
        } else {
            cout << "Road between " << city1 << " and " << city2 << " is now OPEN!\n";
        }
    }

    void displayAdjacencyMatrix() {
        if (cityNames.empty()) {
            cout << "No cities in the system!\n";
            return;
        }

        cout << "\n============================================================\n";
        cout << "ADJACENCY MATRIX REPRESENTATION\n";
        cout << "============================================================\n\n";

        cout << "        ";
        for (int i = 0; i < cityNames.size(); i++) {
            string displayName = cityNames[i];
            if (displayName.length() > 8) {
                displayName = displayName.substr(0, 8);
            }
            cout << displayName << " ";
        }
        cout << "\n";

        for (int i = 0; i < cityNames.size(); i++) {
            string displayName = cityNames[i];
            if (displayName.length() > 8) {
                displayName = displayName.substr(0, 8);
            }
            cout << displayName << " ";
            for (int j = 0; j < cityNames.size(); j++) {
                if (roadBlocked[i][j]) {
                    cout << "X ";
                } else {
                    cout << adjMatrix[i][j] << " ";
                }
            }
            cout << "\n";
        }
        cout << "(X = Road Blocked)\n";
    }

    void displayAdjacencyList() {
        if (cityNames.empty()) {
            cout << "No cities in the system!\n";
            return;
        }

        cout << "\n============================================================\n";
        cout << "ADJACENCY LIST REPRESENTATION\n";
        cout << "============================================================\n\n";

        for (int i = 0; i < cityNames.size(); i++) {
            cout << "[" << cityNames[i] << "] -> ";
            if (adjList[i].empty()) {
                cout << "No connections (Isolated city)";
            } else {
                for (int j = 0; j < adjList[i].size(); j++) {
                    if (roadBlocked[i][adjList[i][j].first]) {
                        cout << cityNames[adjList[i][j].first] << "(" << adjList[i][j].second << "km)[BLOCKED]";
                    } else {
                        cout << cityNames[adjList[i][j].first] << "(" << adjList[i][j].second << "km)";
                    }
                    if (j < adjList[i].size() - 1) {
                        cout << " -> ";
                    }
                }
            }
            cout << "\n";
        }
    }

    void BFS(const string& startCity) {
        int start = findCityIndex(startCity);
        if (start == -1) {
            cout << "City '" << startCity << "' not found!\n";
            return;
        }

        vector<bool> visited(cityNames.size(), false);
        queue<int> q;

        cout << "\nBFS Traversal starting from '" << startCity << "': ";

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << cityNames[current];

            bool hasUnvisited = false;
            for (int i = 0; i < adjList[current].size(); i++) {
                if (!visited[adjList[current][i].first] && !roadBlocked[current][adjList[current][i].first]) {
                    hasUnvisited = true;
                    break;
                }
            }

            if (!q.empty() || hasUnvisited) {
                cout << " -> ";
            }

            for (int i = 0; i < adjList[current].size(); i++) {
                int neighbor = adjList[current][i].first;
                if (!visited[neighbor] && !roadBlocked[current][neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << "\n";
    }

    void DFS(const string& startCity) {
        int start = findCityIndex(startCity);
        if (start == -1) {
            cout << "City '" << startCity << "' not found!\n";
            return;
        }

        vector<bool> visited(cityNames.size(), false);
        stack<int> s;

        cout << "\nDFS Traversal starting from '" << startCity << "': ";

        s.push(start);

        while (!s.empty()) {
            int current = s.top();
            s.pop();

            if (!visited[current]) {
                visited[current] = true;
                cout << cityNames[current];

                bool hasUnvisited = false;
                for (int i = 0; i < adjList[current].size(); i++) {
                    if (!visited[adjList[current][i].first] && !roadBlocked[current][adjList[current][i].first]) {
                        hasUnvisited = true;
                        break;
                    }
                }

                if (!s.empty() || hasUnvisited) {
                    cout << " -> ";
                }

                for (int i = adjList[current].size() - 1; i >= 0; i--) {
                    int neighbor = adjList[current][i].first;
                    if (!visited[neighbor] && !roadBlocked[current][neighbor]) {
                        s.push(neighbor);
                    }
                }
            }
        }
        cout << "\n";
    }

    void findShortestPath(const string& startCity, const string& endCity) {
        int start = findCityIndex(startCity);
        int end = findCityIndex(endCity);

        if (start == -1 || end == -1) {
            cout << "One or both cities not found!\n";
            return;
        }

        if (start == end) {
            cout << "Start and end cities are the same! Distance: 0 km\n";
            return;
        }

        vector<int> distance(cityNames.size(), INF);
        vector<int> parent(cityNames.size(), -1);
        vector<bool> visited(cityNames.size(), false);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        distance[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int current = pq.top().second;
            pq.pop();

            if (visited[current]) continue;
            visited[current] = true;

            if (current == end) break;

            for (int i = 0; i < adjList[current].size(); i++) {
                int nextCity = adjList[current][i].first;
                int roadDistance = adjList[current][i].second;

                if (!roadBlocked[current][nextCity] && !visited[nextCity] &&
                    distance[current] + roadDistance < distance[nextCity]) {
                    distance[nextCity] = distance[current] + roadDistance;
                    parent[nextCity] = current;
                    pq.push(make_pair(distance[nextCity], nextCity));
                }
            }
        }

        if (distance[end] == INF) {
            cout << "No path exists between " << startCity << " and " << endCity << "!\n";
            return;
        }

        cout << "\n==================================================\n";
        cout << "SHORTEST PATH ANALYSIS\n";
        cout << "==================================================\n";
        cout << "From: " << startCity << "\n";
        cout << "To: " << endCity << "\n";
        cout << "Total Distance: " << distance[end] << " km\n";
        cout << "Path: ";

        stack<int> path;
        int current = end;
        while (current != -1) {
            path.push(current);
            current = parent[current];
        }

        while (!path.empty()) {
            cout << cityNames[path.top()];
            path.pop();
            if (!path.empty()) cout << " -> ";
        }
        cout << "\n==================================================\n";
    }

    void findAllPaths(const string& startCity, const string& endCity) {
        int start = findCityIndex(startCity);
        int end = findCityIndex(endCity);

        if (start == -1 || end == -1) {
            cout << "One or both cities not found!\n";
            return;
        }

        vector<bool> visited(cityNames.size(), false);
        vector<int> path;
        vector<vector<int>> allPaths;

        findAllPathsDFS(start, end, visited, path, allPaths);

        cout << "\n==================================================\n";
        cout << "ALL PATHS BETWEEN " << startCity << " AND " << endCity << "\n";
        cout << "==================================================\n";

        if (allPaths.empty()) {
            cout << "No paths found between " << startCity << " and " << endCity << "!\n";
        } else {
            cout << "Found " << allPaths.size() << " path(s):\n\n";
            for (int i = 0; i < allPaths.size(); i++) {
                cout << "Path " << (i + 1) << " (Distance: " << calculatePathDistance(allPaths[i]) << " km): ";
                for (int j = 0; j < allPaths[i].size(); j++) {
                    cout << cityNames[allPaths[i][j]];
                    if (j < allPaths[i].size() - 1) cout << " -> ";
                }
                cout << "\n";
            }
        }
    }

    void findPathWithMinimumStops(const string& startCity, const string& endCity) {
        int start = findCityIndex(startCity);
        int end = findCityIndex(endCity);

        if (start == -1 || end == -1) {
            cout << "One or both cities not found!\n";
            return;
        }

        if (start == end) {
            cout << "Start and end cities are the same! Stops: 0\n";
            return;
        }

        vector<int> distance(cityNames.size(), INF);
        vector<int> parent(cityNames.size(), -1);
        vector<bool> visited(cityNames.size(), false);
        queue<int> q;

        distance[start] = 0;
        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int i = 0; i < adjList[current].size(); i++) {
                int neighbor = adjList[current][i].first;
                if (!visited[neighbor] && !roadBlocked[current][neighbor]) {
                    visited[neighbor] = true;
                    distance[neighbor] = distance[current] + 1;
                    parent[neighbor] = current;
                    q.push(neighbor);

                    if (neighbor == end) break;
                }
            }
        }

        if (distance[end] == INF) {
            cout << "No path exists between " << startCity << " and " << endCity << "!\n";
            return;
        }

        cout << "\n==================================================\n";
        cout << "PATH WITH MINIMUM STOPS\n";
        cout << "==================================================\n";
        cout << "From: " << startCity << "\n";
        cout << "To: " << endCity << "\n";
        cout << "Minimum Stops: " << (distance[end] - 1) << "\n";
        cout << "Path: ";

        stack<int> path;
        int current = end;
        while (current != -1) {
            path.push(current);
            current = parent[current];
        }

        while (!path.empty()) {
            cout << cityNames[path.top()];
            path.pop();
            if (!path.empty()) cout << " -> ";
        }
        cout << "\n==================================================\n";
    }

    void findCitiesWithinRadius(const string& centerCity, int radius) {
        int center = findCityIndex(centerCity);
        if (center == -1) {
            cout << "City '" << centerCity << "' not found!\n";
            return;
        }

        vector<int> distance(cityNames.size(), INF);
        queue<int> q;

        distance[center] = 0;
        q.push(center);

        vector<string> citiesInRadius;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int i = 0; i < adjList[current].size(); i++) {
                int neighbor = adjList[current][i].first;
                int roadDistance = adjList[current][i].second;

                if (!roadBlocked[current][neighbor] &&
                    distance[current] + roadDistance < distance[neighbor] &&
                    distance[current] + roadDistance <= radius) {

                    distance[neighbor] = distance[current] + roadDistance;
                    q.push(neighbor);

                    if (distance[neighbor] <= radius && neighbor != center) {
                        citiesInRadius.push_back(cityNames[neighbor]);
                    }
                }
            }
        }

        cout << "\n==================================================\n";
        cout << "CITIES WITHIN " << radius << " KM OF " << centerCity << "\n";
        cout << "==================================================\n";

        if (citiesInRadius.empty()) {
            cout << "No cities found within " << radius << " km of " << centerCity << "!\n";
        } else {
            cout << "Found " << citiesInRadius.size() << " city(ies) within radius:\n";
            for (int i = 0; i < citiesInRadius.size(); i++) {
                int cityIndex = findCityIndex(citiesInRadius[i]);
                cout << "• " << citiesInRadius[i] << " (" << distance[cityIndex] << " km away)\n";
            }
        }
    }

    void findIsolatedCities() {
        vector<string> isolatedCities;

        for (int i = 0; i < cityNames.size(); i++) {
            bool hasConnection = false;
            for (int j = 0; j < adjList[i].size(); j++) {
                if (!roadBlocked[i][adjList[i][j].first]) {
                    hasConnection = true;
                    break;
                }
            }
            if (!hasConnection && adjList[i].size() > 0) {
                isolatedCities.push_back(cityNames[i]);
            } else if (adjList[i].empty()) {
                isolatedCities.push_back(cityNames[i]);
            }
        }

        cout << "\nISOLATED CITIES ANALYSIS\n";
        cout << "------------------------------\n";

        if (isolatedCities.empty()) {
            cout << "No isolated cities found! All cities are connected.\n";
        } else {
            cout << "Found " << isolatedCities.size() << " isolated city(ies):\n";
            for (int i = 0; i < isolatedCities.size(); i++) {
                cout << "  * " << isolatedCities[i] << "\n";
            }
        }
    }

    void searchCitiesByPrefix(const string& prefix) {
        if (prefix.empty()) {
            cout << "Please enter a valid prefix!\n";
            return;
        }

        vector<string> matchingCities;

        for (int i = 0; i < cityNames.size(); i++) {
            if (cityNames[i].length() >= prefix.length()) {
                bool match = true;
                for (int j = 0; j < prefix.length(); j++) {
                    if (cityNames[i][j] != prefix[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    matchingCities.push_back(cityNames[i]);
                }
            }
        }

        cout << "\nSEARCH RESULTS for prefix '" << prefix << "'\n";
        cout << "-----------------------------------\n";

        if (matchingCities.empty()) {
            cout << "No cities found with the prefix '" << prefix << "'\n";
        } else {
            cout << "Found " << matchingCities.size() << " matching city(ies):\n";
            for (int i = 0; i < matchingCities.size(); i++) {
                cout << "  * " << matchingCities[i] << "\n";
            }
        }
    }

    void findConnectedComponents() {
        vector<bool> visited(cityNames.size(), false);
        vector<vector<string>> components;

        for (int i = 0; i < cityNames.size(); i++) {
            if (!visited[i]) {
                vector<string> component;
                queue<int> q;

                q.push(i);
                visited[i] = true;

                while (!q.empty()) {
                    int current = q.front();
                    q.pop();

                    component.push_back(cityNames[current]);

                    for (int j = 0; j < adjList[current].size(); j++) {
                        int neighbor = adjList[current][j].first;
                        if (!visited[neighbor] && !roadBlocked[current][neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
                components.push_back(component);
            }
        }

        cout << "\nCONNECTED COMPONENTS ANALYSIS\n";
        cout << "----------------------------------------\n";
        cout << "Total connected components: " << components.size() << "\n\n";

        for (int i = 0; i < components.size(); i++) {
            cout << "Component " << (i + 1) << " (" << components[i].size() << " cities): ";
            for (int j = 0; j < components[i].size(); j++) {
                cout << components[i][j];
                if (j < components[i].size() - 1) cout << ", ";
            }
            cout << "\n";
        }
    }

    void displayCityStatistics() {
        cout << "\n==================================================\n";
        cout << "CITY NETWORK STATISTICS\n";
        cout << "==================================================\n";

        cout << "Total Cities: " << cityNames.size() << "\n";

        int totalRoads = 0;
        int blockedRoads = 0;
        int maxDegree = 0;
        string maxDegreeCity;
        int isolatedCount = 0;

        for (int i = 0; i < cityNames.size(); i++) {
            int degree = 0;
            for (int j = 0; j < adjList[i].size(); j++) {
                if (!roadBlocked[i][adjList[i][j].first]) {
                    degree++;
                } else {
                    blockedRoads++;
                }
            }
            totalRoads += degree;

            if (degree > maxDegree) {
                maxDegree = degree;
                maxDegreeCity = cityNames[i];
            }

            if (degree == 0) {
                isolatedCount++;
            }
        }

        totalRoads /= 2;
        blockedRoads /= 2;

        cout << "Total Roads: " << totalRoads << "\n";
        cout << "Blocked Roads: " << blockedRoads << "\n";
        cout << "Most Connected City: " << maxDegreeCity << " (" << maxDegree << " connections)\n";
        cout << "Isolated Cities: " << isolatedCount << "\n";

        if (cityNames.empty()) {
            cout << "Average Connections per City: 0\n";
        } else {
            double avg = (double)(totalRoads * 2) / cityNames.size();
            cout << "Average Connections per City: " << avg << "\n";
        }
    }

    void displayAllCities() {
        if (cityNames.empty()) {
            cout << "No cities in the system!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "ALL CITIES IN SYSTEM\n";
        cout << "========================================\n";

        for (int i = 0; i < cityNames.size(); i++) {
            cout << i + 1 << ". " << cityNames[i];
            int activeConnections = 0;
            for (int j = 0; j < adjList[i].size(); j++) {
                if (!roadBlocked[i][adjList[i][j].first]) {
                    activeConnections++;
                }
            }

            if (activeConnections == 0) {
                cout << " [ISOLATED]";
            } else {
                cout << " (" << activeConnections << " active connections)";
            }
            cout << "\n";
        }
        cout << "Total: " << cityNames.size() << " cities\n";
    }

    void clearAllData() {
        char confirm;
        cout << "Are you sure you want to clear all data? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            cityNames.clear();
            adjMatrix = vector<vector<int>>(MAX_CITIES, vector<int>(MAX_CITIES, 0));
            adjList = vector<vector<pair<int, int>>>(MAX_CITIES);
            roadBlocked = vector<vector<bool>>(MAX_CITIES, vector<bool>(MAX_CITIES, false));
            cout << "All data cleared successfully!\n";
        } else {
            cout << "Operation cancelled.\n";
        }
    }

    void loadSampleData() {
        // Clear any existing data first
        cityNames.clear();
        adjMatrix = vector<vector<int>>(MAX_CITIES, vector<int>(MAX_CITIES, 0));
        adjList = vector<vector<pair<int, int>>>(MAX_CITIES);
        roadBlocked = vector<vector<bool>>(MAX_CITIES, vector<bool>(MAX_CITIES, false));

        // Add cities directly to the vector
        cityNames.push_back("Delhi");
        cityNames.push_back("Mumbai");
        cityNames.push_back("Chennai");
        cityNames.push_back("Kolkata");
        cityNames.push_back("Bangalore");
        cityNames.push_back("Hyderabad");
        cityNames.push_back("Pune");
        cityNames.push_back("Jaipur");

        // Manually create the adjacency matrix and list for sample data
        // Delhi connections
        adjMatrix[0][1] = 1400; adjMatrix[1][0] = 1400; // Delhi-Mumbai
        adjMatrix[0][3] = 1500; adjMatrix[3][0] = 1500; // Delhi-Kolkata
        adjMatrix[0][7] = 300;  adjMatrix[7][0] = 300;  // Delhi-Jaipur

        // Mumbai connections
        adjMatrix[1][5] = 700;  adjMatrix[5][1] = 700;  // Mumbai-Hyderabad
        adjMatrix[1][4] = 1000; adjMatrix[4][1] = 1000; // Mumbai-Bangalore
        adjMatrix[1][6] = 150;  adjMatrix[6][1] = 150;  // Mumbai-Pune

        // Chennai connections
        adjMatrix[2][4] = 350;  adjMatrix[4][2] = 350;  // Chennai-Bangalore
        adjMatrix[2][5] = 600;  adjMatrix[5][2] = 600;  // Chennai-Hyderabad
        adjMatrix[2][3] = 1600; adjMatrix[3][2] = 1600; // Chennai-Kolkata

        // Bangalore connections
        adjMatrix[4][5] = 500;  adjMatrix[5][4] = 500;  // Bangalore-Hyderabad

        // Hyderabad connections
        adjMatrix[5][6] = 550;  adjMatrix[6][5] = 550;  // Hyderabad-Pune

        // Build adjacency list from matrix
        for (int i = 0; i < cityNames.size(); i++) {
            for (int j = 0; j < cityNames.size(); j++) {
                if (adjMatrix[i][j] > 0) {
                    adjList[i].push_back(make_pair(j, adjMatrix[i][j]));
                }
            }
        }
    }

    void reloadSampleData() {
        loadSampleData();
        cout << "Sample data reloaded successfully!\n";
        cout << "Loaded " << cityNames.size() << " cities with road connections.\n";
    }

    void displayBlockedRoads() {
        cout << "\n========================================\n";
        cout << "BLOCKED ROADS\n";
        cout << "========================================\n";

        bool foundBlocked = false;
        vector<vector<bool>> recorded(cityNames.size(), vector<bool>(cityNames.size(), false));

        for (int i = 0; i < cityNames.size(); i++) {
            for (int j = 0; j < cityNames.size(); j++) {
                if (roadBlocked[i][j] && !recorded[i][j] && adjMatrix[i][j] > 0) {
                    cout << "• " << cityNames[i] << " <--> " << cityNames[j] << "\n";
                    recorded[i][j] = true;
                    recorded[j][i] = true;
                    foundBlocked = true;
                }
            }
        }

        if (!foundBlocked) {
            cout << "No blocked roads in the system!\n";
        }
    }
};

void displayMainMenu() {
    cout << "\n======================================================================\n";
    cout << "              PATHEFINDER: GRAPH-BASED NAVIGATION SYSTEM\n";
    cout << "======================================================================\n";
    cout << "1.  Display All Cities\n";
    cout << "2.  Add City\n";
    cout << "3.  Remove City\n";
    cout << "4.  Add Road\n";
    cout << "5.  Remove Road\n";
    cout << "6.  Display Adjacency Matrix\n";
    cout << "7.  Display Adjacency List\n";
    cout << "8.  BFS Traversal\n";
    cout << "9.  DFS Traversal\n";
    cout << "10. Find Shortest Path (Dijkstra)\n";
    cout << "11. Find All Paths\n";
    cout << "12. Find Path with Minimum Stops\n";
    cout << "13. Find Cities Within Radius\n";
    cout << "14. Simulate Traffic Update\n";
    cout << "15. Update Road Condition (Block/Unblock)\n";
    cout << "16. Display Blocked Roads\n";
    cout << "17. Find Isolated Cities\n";
    cout << "18. Search Cities by Prefix\n";
    cout << "19. Find Connected Components\n";
    cout << "20. Display City Statistics\n";
    cout << "21. Clear All Data\n";
    cout << "22. Reload Sample Data\n";
    cout << "23. Exit\n";
    cout << "----------------------------------------------------------------------\n";
    cout << "Enter your choice (1-23): ";
}

int main() {
    Graph navigationSystem;
    int choice;
    string city1, city2, prefix;
    int distance, radius, newTime;
    bool blocked;

    cout << "\nWelcome to PathFinder Navigation System!\n";
    cout << "Your intelligent graph-based route planning solution\n";
    cout << "Sample data has been preloaded with 8 major Indian cities!\n";

    while (true) {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                navigationSystem.displayAllCities();
                break;

            case 2:
                cout << "Enter city name to add: ";
                cin >> city1;
                navigationSystem.addCity(city1);
                break;

            case 3:
                cout << "Enter city name to remove: ";
                cin >> city1;
                navigationSystem.removeCity(city1);
                break;

            case 4:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                cout << "Enter distance (km): ";
                cin >> distance;
                navigationSystem.addRoad(city1, city2, distance);
                break;

            case 5:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                navigationSystem.removeRoad(city1, city2);
                break;

            case 6:
                navigationSystem.displayAdjacencyMatrix();
                break;

            case 7:
                navigationSystem.displayAdjacencyList();
                break;

            case 8:
                cout << "Enter starting city for BFS: ";
                cin >> city1;
                navigationSystem.BFS(city1);
                break;

            case 9:
                cout << "Enter starting city for DFS: ";
                cin >> city1;
                navigationSystem.DFS(city1);
                break;

            case 10:
                cout << "Enter starting city: ";
                cin >> city1;
                cout << "Enter destination city: ";
                cin >> city2;
                navigationSystem.findShortestPath(city1, city2);
                break;

            case 11:
                cout << "Enter starting city: ";
                cin >> city1;
                cout << "Enter destination city: ";
                cin >> city2;
                navigationSystem.findAllPaths(city1, city2);
                break;

            case 12:
                cout << "Enter starting city: ";
                cin >> city1;
                cout << "Enter destination city: ";
                cin >> city2;
                navigationSystem.findPathWithMinimumStops(city1, city2);
                break;

            case 13:
                cout << "Enter center city: ";
                cin >> city1;
                cout << "Enter radius (km): ";
                cin >> radius;
                navigationSystem.findCitiesWithinRadius(city1, radius);
                break;

            case 14:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                cout << "Enter new travel time (minutes): ";
                cin >> newTime;
                navigationSystem.simulateTrafficUpdate(city1, city2, newTime);
                break;

            case 15:
                cout << "Enter first city: ";
                cin >> city1;
                cout << "Enter second city: ";
                cin >> city2;
                cout << "Block road? (1 for yes, 0 for no): ";
                cin >> blocked;
                navigationSystem.updateRoadCondition(city1, city2, blocked);
                break;

            case 16:
                navigationSystem.displayBlockedRoads();
                break;

            case 17:
                navigationSystem.findIsolatedCities();
                break;

            case 18:
                cout << "Enter prefix to search: ";
                cin >> prefix;
                navigationSystem.searchCitiesByPrefix(prefix);
                break;

            case 19:
                navigationSystem.findConnectedComponents();
                break;

            case 20:
                navigationSystem.displayCityStatistics();
                break;

            case 21:
                navigationSystem.clearAllData();
                break;

            case 22:
                navigationSystem.reloadSampleData();
                break;

            case 23:
                cout << "\nThank you for using PathFinder Navigation System!\n";
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice! Please enter a number between 1-23.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
