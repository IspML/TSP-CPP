#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;

/* Author: Nolan Donaldson
 * CSCI 406 - Algorithms
 * Project 1: Timing Project
 * For reference, this was done in Clion.
*/

set<double> expath;

// Rudimentary 2D point class
class Point {
public:
    Point() {
        _X = 0;
        _Y = 0;
    }

    // Calculates absolute value of the distance between two points
    double distCalc(Point p) {
        double dist = sqrt(pow((_X - p.getX()), 2) + pow((_Y - p.getY()), 2));
        return dist;
    }

    bool operator != (Point &p) {
        return !(_X == p._X && _Y == p._Y);
    }

    int getX() {return _X;}
    int getY() {return _Y;}
    void setX(int X) {_X = X;}
    void setY(int Y) {_Y = Y;}

private:
    int _X;
    int _Y;
};

void print(vector<Point> points) {
    double cost = 0;

    // Calculates cost of path taken
    for (int i = 0; i < points.size() - 1; i++) {
        cost += points[i].distCalc(points[i + 1]);
    }

    cout << setprecision(3) << fixed << cost << endl;
    for (int i = 0; i < points.size(); i++) {
        cout << points[i].getX() << " " << points[i].getY() << endl;
    }
    cout << endl;
}

// Reads in properly formatted files
vector<Point> readInputFile() {
     ifstream input;
     int n;

     // Opens the input file, tests if it works, and reads in the information to a vector.
     input.open("input.txt");
     if (!input.is_open()) {
         cerr << "Cannot read file input.txt";
         exit(0);
     }

     input >> n;
     vector<Point> inPoints;
     Point temp;

     // Reads in contents of input file to array
     while(!input.eof()) {
         input >> n;
         temp.setX(n);
         input >> n;
         temp.setY(n);
         inPoints.push_back(temp);
     }
     return inPoints;
}

// Solves TSP using nearest neighbor algorithm and prints path
void nearestNeighborSol(vector<Point> points) {
    vector<Point> pathTaken;
    map<double, int> distPoints;
    set<int> pointsVisited;
    Point P_0 = *points.begin();
    Point P = P_0;
    pointsVisited.insert(0);
    pathTaken.push_back(P_0);
    // Compares the point P to every unvisited point left and finds the closest one to move to
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            // Checks to make sure the points being compared are unvisited
            if (points[i] != points[j] && pointsVisited.count(j) == 0) {
                distPoints.emplace(P.distCalc(points[j]), j);
            }
        }
        // Fills out set of previously visited points and updates P to be the current point
        if (!distPoints.empty()) {
            pointsVisited.insert(distPoints.begin()->second);
            P = points[distPoints.begin()->second];
            pathTaken.push_back(P);
        }
        distPoints.clear();
    }
    pathTaken.push_back(P_0);
    // TODO: Uncomment this code to print results of nearest neighbor algorithm
    //print(pathTaken);
}

void calculateShortest(vector<Point> points) {
    double cost = 0;

    // Calculates cost of path taken
    for (int i = 0; i < points.size() - 1; i++) {
        cost += points[i].distCalc(points[i + 1]);
    }
    expath.insert(cost);
}

// Solves TSP by trying all permutations to get optimal path
// Algorithm from: https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
void exhaustiveSol(vector<Point> points,  int startIndex, int endIndex) {
    // Base case
    if (startIndex == endIndex) {
        // Adds first point back to start of the vector to signify the return back to p_0
        points.push_back(points[0]);
        // TODO: Uncomment this code to print results of exhaustive algorithm
        //print(points);
        calculateShortest(points);
    }

    // Recursive case
    else {
        for (int i = startIndex; i <= endIndex; i++) {
            swap(points[startIndex], points[i]);
            exhaustiveSol(points, startIndex + 1, endIndex);
            swap(points[startIndex], points[i]);
        }
    }
}

void randGen(vector<Point>& points, int n) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-1000, 1000);

    // Fills points with n random points
    for (int i = 0; i < n; i++) {
        Point temp;
        int randomP = distribution(generator);
        temp.setX(randomP);
        randomP = distribution(generator);
        temp.setY(randomP);
        points.push_back(temp);
    }
}



int main() {
    vector<Point> points;
    int nNeighbor = 1000;
    int nPermutation = 8;
    // TODO: Uncomment this line of code to use the input file's contents for the algorithms
    points = readInputFile();

    // TODO: Uncomment this code AND lines 173-176 to use custom values of n with randomly generated points
    // randGen(points, nNeighbor);
    auto startPt = std::chrono::high_resolution_clock::now();
    cout << "Results using nearest neighbor algorithm: " << endl;
    nearestNeighborSol(points);
    auto stopPt = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopPt - startPt);
    cout << "Time taken by nearest neighbors algorithm: " << duration.count() << " microseconds" << endl;
    cout << endl;

    /*
    points.clear();
    randGen(points, nPermutation);
    */

    cout << "Results for exhaustive solution: " << endl;
    startPt = std::chrono::high_resolution_clock::now();
    //exhaustiveSol(points, 0, (points.size() - 1));
    stopPt = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stopPt - startPt);
    cout << "Time taken by exhaustive algorithm: " << duration.count() << " microseconds" << endl;
    auto e = expath.begin();
    cout << *e;
    return 0;
}
