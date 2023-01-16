# include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iostream>
using namespace std;


// Define a class or struct to store information about a facility or customer
struct Location {
  int rank;
  double setup_cost;
  double capacity;
  double demand;
  double x;
  double y;
};

// Define a function to calculate the Euclidean distance between two locations
double euclidean_distance(const Location& l1, const Location& l2) {
  return sqrt(pow(l1.x - l2.x, 2) + pow(l1.y - l2.y, 2));
}
class DemandComparator {
public:
  // Return true if the demand of l1 is greater than the demand of l2
  bool operator()(const Location& l1, const Location& l2) const {
    return l1.demand > l2.demand;
  }
};
// returns the index of the nearest facility a customer can be placed
 int Find_Nearest_Facilities(const vector<Location> &facilities, const Location& customer){
    double smallest_d = 1e10;
    int index = -1;
   for(int i =0; i<facilities.size(); i++){
     double temp = euclidean_distance(facilities[i], customer);
     if( temp <= smallest_d && customer.demand <= facilities[i].capacity){
       index = i;
       smallest_d = temp;
     }
       
   }
    return index;
    
  }

// Define a function to solve the facility location problem by ordering customers
// by demand and placing them in the closest facility
vector<int> solve_facility_location(vector<Location>& facilities, vector<Location>& customers) {
  // Sort the customers in descending order of their demand
  vector<Location> sorted_customers = customers;
  sort(sorted_customers.begin(), sorted_customers.end(), DemandComparator());

  // Initialize the assignment of customers to facilities
  vector<int> assignment(sorted_customers.size());
  for (int i = 0; i < sorted_customers.size(); ++i) {
    assignment[i] = -1;
  }

  // Iterate over the customers in order of their demand
  for (int i = 0; i < sorted_customers.size(); i++) {
  // this returns the nearest  facility the customer can be placed in
    int index = Find_Nearest_Facilities(facilities, sorted_customers[i]);
    // Iterate over the facilities in order of their distance to the customer
    bool placed = false;
     int counter = 0;
    while(!placed){
      assignment[sorted_customers[i].rank] = facilities[index].rank;
      facilities[index].capacity -=  sorted_customers[i].demand;
      placed = true;
    }
  
  }

  return assignment;
}
int main() {

  string filename;
  ifstream InputFile;
  ofstream OutputFile;
  cout << "File name: ";
  cin >> filename;
  InputFile.open(filename.data());

  while (!InputFile.is_open()) {
    cout << "Wrong filename! Please try again: ";
    InputFile.clear();
    cout << "File name: ";
    cin >> filename;
    InputFile.open(filename.data());
  }
  // Read the number of facilities and customers
  int N, M;
  InputFile >> N >> M;

  // Read the facility information
  vector<Location> facilities(N);
  for (int i = 0; i < N; ++i) {
    facilities[i].rank = i;
    InputFile >> facilities[i].setup_cost >> facilities[i].capacity >> facilities[i].x >> facilities[i].y;
  }

  // Read the customer information
  vector<Location> customers(M);
  for (int i = 0; i < M; ++i) {
    customers[i].rank = i;
    InputFile >> customers[i].demand >> customers[i].x >> customers[i].y;
  }

  // Solve the facility location problem
  vector<int> assignment = solve_facility_location(facilities, customers);
  // Compute the total cost of the assignment
  double total_cost = 0;
  for (int i = 0; i < facilities.size(); ++i) {
    for (int j = 0; j < customers.size(); ++j) {
      if (assignment[j] == facilities[i].rank) {
        total_cost += facilities[i].setup_cost;
        break;
      }
    }
  }
  for (int i = 0; i < customers.size(); ++i) {
    if (assignment[i] != -1) {
      total_cost += euclidean_distance(customers[i], facilities[assignment[i]]);
    }
  }
  // Print the assignment of customers to facilities
  OutputFile.open("output-"+filename);
  OutputFile<< total_cost<<endl;
  for (int i = 0; i < assignment.size(); ++i) {
    OutputFile<< assignment[i] << " ";
  }
  

  return 0;
}

