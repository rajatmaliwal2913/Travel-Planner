#include <bits/stdc++.h>
using namespace std;

// Store flight details
class FlightInfo {
public:
    string flightNumber;
    int departureCity;
    int arrivalCity;
    int flightCost;
    int departureTime, arrivalTime;

    FlightInfo(string fl, int city1, int city2, int pri, int dep, int arr) {
        flightNumber = fl;
        departureCity = city1;
        arrivalCity = city2;
        flightCost = pri;
        departureTime = dep;
        arrivalTime = arr;
    }

    FlightInfo() {}
};

// Store current state in the priority queue
class TripNode {
public:
    int totalCost;
    int currentCity;
    int currentTime;
    vector<FlightInfo> path;

    TripNode(int cost, int city, int time) {
        totalCost = cost;
        currentCity = city;
        currentTime = time;
    }

    TripNode() {}
};

// Class for the priority queue to compare nodes based on cost
class CostComparator {
public:
    bool operator()(const TripNode& node1, const TripNode& node2) {
        return node1.totalCost > node2.totalCost;
    }
};

// Convert time from HHMM format to minutes since midnight
int timeToMinutes(int time) {
    int minutes = time % 100;
    int hours = time / 100;
    return hours * 60 + minutes;
}

// Find the cheapest trip using a priority queue
int calculateCheapestTrip(vector<vector<FlightInfo>> &flightGraph, int startCity, int endCity, int earliestDep, int latestArr) {
    priority_queue<TripNode, vector<TripNode>, CostComparator> pq;
    map<string, FlightInfo> visitedFlights;
    TripNode startNode(0, startCity, earliestDep - 30);
    pq.push(startNode);

    while (!pq.empty()) {
        TripNode currentNode = pq.top();
        pq.pop();

        int currentCost = currentNode.totalCost;
        int city = currentNode.currentCity;
        int time = currentNode.currentTime;

        if (city == endCity && time <= latestArr) {
            return currentCost;
        }

        int earliestDepTime = time + 30;
        for (FlightInfo &flight : flightGraph[city]) {
            if (flight.departureTime >= earliestDepTime && flight.arrivalTime <= latestArr && visitedFlights.find(flight.flightNumber) == visitedFlights.end()) {
                TripNode nextNode(currentCost + flight.flightCost, flight.arrivalCity, flight.arrivalTime);
                nextNode.path = currentNode.path;
                nextNode.path.push_back(flight);
                pq.push(nextNode);
                visitedFlights[flight.flightNumber] = flight;
            }
        }
    }

    return -1; // If no valid path is found
}

int main() {
    int cityCount;
    cout << "Enter the number of cities: ";
    cin >> cityCount;

    vector<vector<FlightInfo>> flightGraph(cityCount + 1);

    int flightCount;
    cout << "Enter the number of flights: ";
    cin >> flightCount;

    cout << "Enter flight details (Departure City, Arrival City, Departure Time (HHMM), Arrival Time (HHMM), Flight Number, Price):\n";
    while (flightCount--) {
        int depCity, arrCity, depTime, arrTime, price;
        string flightNo;
        cin >> depCity >> arrCity >> depTime >> arrTime >> flightNo >> price;
        depTime = timeToMinutes(depTime);
        arrTime = timeToMinutes(arrTime);
        flightGraph[depCity].emplace_back(flightNo, depCity, arrCity, price, depTime, arrTime);
    }

    int queryCount;
    cout << "Enter the number of queries: ";
    cin >> queryCount;

    cout << "Enter each query (Source City, Destination City, Earliest Departure Time (HHMM), Latest Arrival Time (HHMM)):\n";
    while (queryCount--) {
        int source, destination, depTime, arrTime;
        cin >> source >> destination >> depTime >> arrTime;
        depTime = timeToMinutes(depTime);
        arrTime = timeToMinutes(arrTime);
        int result = calculateCheapestTrip(flightGraph, source, destination, depTime, arrTime);
        if (result == -1) {
            cout << "No valid path found\n";
        } else {
            cout << "Cheapest trip cost: " << result << "\n";
        }
    }

    return 0;
}

// SAMPLE INPUT AND OUTPUT
/*Enter the number of cities: 4
Enter the number of flights: 5
Enter flight details (Departure City, Arrival City, Departure Time (HHMM), Arrival Time (HHMM), Flight Number, Price):
1 2 0800 0900 AA123 100
1 3 0700 0800 BB456 200
2 3 1000 1100 CC789 150
3 4 1200 1300 DD012 100
2 4 0900 1000 EE345 300
Enter the number of queries: 2
Enter each query (Source City, Destination City, Earliest Departure Time (HHMM), Latest Arrival Time (HHMM)):
1 4 0600 1400
Cheapest trip cost: 300
2 4 0800 1300
Cheapest trip cost: 250
*/

