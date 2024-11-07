#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <string>
#include <functional>
#include <map>

#include "kmeans.h"
#include "Point.h"
#include "utility"
#include "json.hpp"

using json = nlohmann::json;

using namespace std;

struct AnalysisTest
{
    int constant;
    map<int, vector<pair<double, double>>> times_Kmeans;
};

void printExecutionTime(double time) {
    printf("\nExecution Time: %.2f ms\n", time);
}

double executionTime(const std::function<void()>& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

void runKmeansTests(Kmeans& kb, int seed, int k, int iter, bool print) {
    std::vector<std::pair<std::string, 
        std::function<unordered_map<Point, vector<Point>>()>>> methods = {
        {"Brute Force", [&]() { return kb.kmeans(k, iter, print); }},
        {"KD-Tree", [&]() { return kb.kmeans_kd(k, iter, print); }}
    };

    printf("Seed %d:\n", seed);
    for (const auto& method : methods) {
        srand(seed);
        printf("\nkmeans %s:", method.first.c_str());
        unordered_map<Point, vector<Point>> clusters;
        printExecutionTime( executionTime([&]() { clusters = method.second(); }));
        if (seed == 4 || seed == 9) {
            std::string filename = ( method.first == "Brute Force" ? "f" : "k")
                                    + std::to_string(seed) + ".json";
            kb.ExportToJson(clusters, filename);
        }
    }
    printf("\n");
}

void analysisToJson(const std::vector<AnalysisTest>& analysis,const std::string& filename) {
    json j = json::array();
    for (const auto& test : analysis) {
        json jTest;
        jTest["constant"] = test.constant;

        json jTimesKmeans;
        for (const auto& time_Kmeans : test.times_Kmeans) {
            json jTimes = json::array();
            for (const auto& kmeans : time_Kmeans.second) {
                jTimes.push_back({ {"Brute Force", kmeans.first}, {"KD-Tree", kmeans.second} });
            }
            jTimesKmeans[std::to_string(time_Kmeans.first)] = jTimes;
        }
        jTest["times_Kmeans"] = jTimesKmeans;
        j.push_back(jTest);
    }

    std::ofstream file(filename);
    file << j.dump(4); // 4 de identacion
    file.close();
}

std::vector<AnalysisTest> runAnalysisTests(const std::vector<int>& constants, const std::vector<int>& variables, int iter, bool strct) 
{
    std::vector<AnalysisTest> analysis;
    for (auto& constV : constants) {
        AnalysisTest times{ constV };

        for (auto& var : variables) {
            srand(constV);
            Kmeans kc("data2k.csv", strct ? var : constV);
            double timeBF = executionTime([&]() { kc.kmeans(strct ? constV : var, iter); });
            double timeKD = executionTime([&]() { kc.kmeans_kd(strct ? constV : var, iter); });
            times.times_Kmeans[var].emplace_back(timeBF, timeKD);
        }
        analysis.push_back(times);
    }
    return analysis;
}

int main() {
	vector<int> nFijo = { 1000, 1450, 1900, 2400 };
	vector<int> kFijo = { 5, 15, 25, 50, 75 };
	vector<int> nVariable = { 1000, 1150, 1300, 1450, 1600, 1750, 1900, 2050, 2200, 2400 };
	vector<int> kVariable = { 5, 15, 25, 50, 75, 100, 125, 150, 200 };
	
    int nClusters = 18;
    int iter = 20;
    int points = 2400;
    bool print = false;
	//b) 10 semillas diferentes
    printf("Ejercicio B:\n");
    for (int i = 0; i < 10; i++) {
        Kmeans kb("data2k.csv", points);
        runKmeansTests(kb, i + 1, nClusters, iter, print);
    }
    //c) k fijo
    analysisToJson(runAnalysisTests(kFijo, nVariable, iter, true), "kFijo.json");
    //d) n fijo
    analysisToJson(runAnalysisTests(nFijo, kVariable, iter, false), "nFijo.json");

}