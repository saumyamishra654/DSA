#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <sstream>
#include <iomanip>

/*
Time Complexity:

1. DP Table:
   - Outer Loop (gap): runs from gap = 2 to n-1, approximately n iterations.
   - Middle Loop (i): for each gap, index i runs from 0 to n-gap-1, approximately n-gap iterations.
   - Inner Loop (k): For each pair (i, j), index k runs from i+1 to j-1, approximately gap-1 iterations.
     Total = Sum over all gaps (O(n) * O(gap)) ≈ O(n^3).

2. Traceback:
   - traceback function is called recursively for each diagonal added to the triangulation.
   - Since there are n-3 diagonals in a polygon with n vertices, this step is O(n).

3. Overall Complexity:
   - The dominant term comes from the DP table filling, which is O(n^3).
   - Traceback adds an O(n) term, which gets subsumed by DP table filling, resulting in O(n^3).

*/

// struct for point
struct Point {
    double x, y;
};

// computing euclidean distance between two points
double distance(const Point &a, const Point &b) {
    return hypot(a.x - b.x, a.y - b.y);
}

// recursive function to reconstruct the added diagonals
void traceback(int i, int j, const std::vector<std::vector<int>> &s,
               std::vector<std::pair<int, int>> &diagonals, int n) {

    if (j <= i + 1) return; // base case: if subpolygon has fewer than 3 vertices, do nothing
    int k = s[i][j]; // retrieve index k, which gave ideal diagonal between i and j
    traceback(i, k, s, diagonals, n); // recurse on i to k
    traceback(k, j, s, diagonals, n); //recurse on k, j
    if (k != i + 1)
        diagonals.emplace_back(i, k); // if k is not immediately after i, add the diagonal between i and k
    if (k != j - 1)
        diagonals.emplace_back(k, j);  // if k is not immediately before j, add the diagonal between k and j
}

int main() {
    std::ifstream infile("Input.txt");
    std::string line;
    std::vector<Point> vertices;

    // Read and parse the input file
    if (infile) {
        getline(infile, line);
        std::stringstream ss(line);
        std::string coord;
        while (ss >> coord) {
            size_t commaPos = coord.find(',');
            if (commaPos != std::string::npos) {
                Point p;
                p.x = std::stod(coord.substr(0, commaPos));
                p.y = std::stod(coord.substr(commaPos + 1));
                vertices.push_back(p);
            }
        }
        infile.close();
    } else {
        std::cerr << "error opening input file" << std::endl;
        return 1;
    }
    // figuring out polygon size
    int n = vertices.size();
    if (n < 3) {
        std::cerr << "less than 3 vertices" << std::endl;
        return 1;
    }

    // intialise DP and s tables
    std::vector<std::vector<double>> dp(n, std::vector<double>(n, 0));
    std::vector<std::vector<int>> s(n, std::vector<int>(n, -1));

    // DP table filling
    for (int gap = 2; gap < n; ++gap) {  // iterate over gaps from 2 to n-1 (subpolygon sizes)
        for (int i = 0; i + gap < n; ++i) {
            int j = i + gap;
            dp[i][j] = std::numeric_limits<double>::infinity(); // initialize dp[i][j] with infinity
            for (int k = i + 1; k < j; ++k) { // consider all possible k between i and j
                double cost = dp[i][k] + dp[k][j];
                // add cost of diagonal (i, j) if not adjacent
                if (i != k - 1)
                    cost += distance(vertices[i], vertices[k]);  // add length of diagonal (i, k) to cost
                // add cost of diagonal between k and j if they are not adjacent
                if (k != j - 1)
                    cost += distance(vertices[k], vertices[j]);  // add length of diagonal (k, j) to cost
                if (cost < dp[i][j]) {  // if the calculated cost is less than current dp[i][j]
                    dp[i][j] = cost; // update dp table
                    s[i][j] = k; // store k for traceback
                }
            }
        }
    }

    // reconstruct solution
    std::vector<std::pair<int, int>> diagonals;
    traceback(0, n - 1, s, diagonals, n);

    // calculate the total minimal length, located in bottom right corner of dp table
    double totalLength = dp[0][n - 1];

    // print results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total Minimal Length: " << totalLength << std::endl;
    std::cout << "Added Diagonals:" << std::endl;
    for (const auto &d : diagonals) {
        std::cout << "Between vertices " << d.first << " and " << d.second << std::endl;
    }

    return 0;
}
