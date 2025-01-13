#include <iostream>
#include <set>
#include <vector>
#include <tuple>
#include <algorithm>

// staircase structure
class Staircase {
public:
    // set of triples (y, z, x), ordered by y, and then by z
    std::set<std::tuple<int, int, int>> steps;

    // check if point (y, z, x) is above the current staircase
    bool isAbove(int y, int z, std::tuple<int, int, int>& dominatingStep) {
        // first element with y >= current y
        auto it = steps.lower_bound({y, -1, -1});  // -1 as dummy values for z and x

        // if there is no such element or the z-value of the found step is <= z, return true (point is above)
        if (it == steps.end()) return true;
        
        // If the point is not above, store dominating point
        dominatingStep = *it;
        return std::get<1>(*it) < z; // Compare the z-value of the found step
    }

    void update(int x, int y, int z) {
        // remove any steps that are dominated
        auto it = steps.lower_bound({y, -1, -1});
        while (it != steps.end() && std::get<0>(*it) == y && std::get<1>(*it) <= z) {
            it = steps.erase(it);  // Remove dominated steps with same y and smaller z
        }
        // add the new step (x, y, z)
        steps.insert({y, z, x});
    }

};

// function to find maximal points
std::vector<std::tuple<int, int, int>> maximalPoints(std::vector<std::tuple<int, int, int>>& points) {
    // sort points in descending order by x-coordinate
    std::sort(points.begin(), points.end(), [](const auto& a, const auto& b) {
        return std::get<0>(a) > std::get<0>(b);
    });

    Staircase staircase;
    std::vector<std::tuple<int, int, int>> maximalSet;
    std::vector<std::string> dominationMessages;  // collecting domination messages

    for (const auto& [x, y, z] : points) {
        std::tuple<int, int, int> dominatingStep;
        if (staircase.isAbove(y, z, dominatingStep)) {
            // point is maximal
            maximalSet.push_back({x, y, z});
            staircase.update(x, y, z);
        } else {
            // not maximal, store the domination message
            dominationMessages.push_back(
                "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ") not maximal. "
                + "Dominated by (" + std::to_string(std::get<2>(dominatingStep)) + ", " 
                + std::to_string(std::get<0>(dominatingStep)) + ", " + std::to_string(std::get<1>(dominatingStep)) + ")."
            );
        }
    }

    // print domination messages 
    std::cout << "\nDomination Information:\n";
    for (const auto& msg : dominationMessages) {
        std::cout << msg << "\n";
    }

    return maximalSet;
}

// random point tester
std::vector<std::tuple<int, int, int>> generateRandomPoints(int n, int limit = 100) {
    std::vector<std::tuple<int, int, int>> points;
    for (int i = 0; i < n; ++i) {
        int x = rand() % limit;
        int y = rand() % limit;
        int z = rand() % limit;
        points.push_back({x, y, z});
    }
    return points;
}

int main() {
    srand(time(0));  // uses current time as seed

    auto points = generateRandomPoints(10);
    std::cout << "Random Points:\n";
    for (const auto& [x, y, z] : points) {
        std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }

    // maximal points
    auto maximal = maximalPoints(points);
    
    std::cout << "\nMaximal Points:\n";
    for (const auto& [x, y, z] : maximal) {
        std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }

    return 0;
}
