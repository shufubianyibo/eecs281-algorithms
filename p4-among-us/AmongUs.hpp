// Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA16442622C7
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>

using namespace std;

enum class Area { LAB, DECON, OUTER };

struct Vertex {
    int x, y;
    Area area;
};
class AmongUs {
private:
    vector<Vertex> verts;
    size_t n;
    vector<size_t> bestTour;
    double bestLength;
    vector<vector<double>> dist;
    vector<double> mstDist;
    vector<char>   inMST;

    Area classify(int x, int y) {
        if (x < 0 && y < 0)   return Area::LAB;
        if (x <= 0 && y <= 0) return Area::DECON;
        return Area::OUTER;
    }

    double squaredDist(const Vertex& a, const Vertex& b) {
        double dx = (double)(a.x - b.x);
        double dy = (double)(a.y - b.y);
        return dx * dx + dy * dy;
    }

    double trueDist(const Vertex& a, const Vertex& b) {
        return sqrt(squaredDist(a, b));
    }

    double distMSTSq(const Vertex& a, const Vertex& b) {
        if ((a.area == Area::LAB   && b.area == Area::OUTER) ||
            (a.area == Area::OUTER && b.area == Area::LAB))
            return numeric_limits<double>::infinity();
        return squaredDist(a, b);
    }

    void precomputeDist() {
        dist.assign(n, vector<double>(n, 0.0));
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j)
                dist[i][j] = trueDist(verts[i], verts[j]);
    }

    bool promising(const vector<size_t>& tour, size_t permLength, double currentLen) {
        double minToUnvisited = numeric_limits<double>::infinity();
        double minToStart = numeric_limits<double>::infinity();
        for (size_t i = permLength; i < n; ++i) {
            double d1 = dist[tour[permLength-1]][tour[i]];
            double d2 = dist[tour[0]][tour[i]];
            if (d1 < minToUnvisited) minToUnvisited = d1;
            if (d2 < minToStart) minToStart = d2;
        }
        
        double base_cost = currentLen + minToUnvisited + minToStart;
        if (base_cost >= bestLength) return false;

        size_t remaining = n - permLength;
        for (size_t i = 0; i < remaining; ++i) {
            mstDist[i] = numeric_limits<double>::infinity();
            inMST[i] = false;
        }
        mstDist[0] = 0.0;
        double mstWeight = 0.0;

        for (size_t iter = 0; iter < remaining; ++iter) {
            size_t u = remaining;
            double min_d = numeric_limits<double>::infinity();
            for (size_t i = 0; i < remaining; ++i) {
                if (!inMST[i] && mstDist[i] < min_d) {
                    min_d = mstDist[i];
                    u = i;
                }
            }
            
            inMST[u] = true;
            mstWeight += min_d;
            if (base_cost + mstWeight >= bestLength) {
                return false; 
            }

            if (iter == remaining - 1) break;

            for (size_t v = 0; v < remaining; ++v) {
                if (!inMST[v]) {
                    double d = dist[tour[permLength + u]][tour[permLength + v]];
                    if (d < mstDist[v]) mstDist[v] = d;
                }
            }
        }
        return true;
    }

    void genPerms(vector<size_t>& tour, size_t permLength, double currentLen) {
        if (permLength == n) {
            double total = currentLen + dist[tour[n-1]][tour[0]];
            if (total < bestLength) {
                bestLength = total;
                bestTour   = tour;
            }
            return;
        }
        if (!promising(tour, permLength, currentLen)) return;

        for (size_t i = permLength; i < n; ++i) {
            double newLen = currentLen + dist[tour[permLength-1]][tour[i]];
            
            if (newLen >= bestLength) continue; 
            
            swap(tour[permLength], tour[i]);
            genPerms(tour, permLength + 1, newLen);
            swap(tour[permLength], tour[i]);
        }
    }

    void twoOpt(vector<size_t>& tour) {
        bool improved = true;
        while (improved) {
            improved = false;
            for (size_t i = 0; i < n - 1; ++i) {
                for (size_t j = i + 2; j < n; ++j) {
                    if (i == 0 && j == n - 1) continue;
                    double before = trueDist(verts[tour[i]], verts[tour[i + 1]]) +
                                    trueDist(verts[tour[j]], verts[tour[(j + 1) % n]]);
                    double after  = trueDist(verts[tour[i]], verts[tour[j]]) +
                                    trueDist(verts[tour[i + 1]], verts[tour[(j + 1) % n]]);
                    if (after < before - 1e-10) {
                        reverse(tour.begin() + static_cast<long>(i + 1),
                                tour.begin() + static_cast<long>(j + 1));
                        improved = true;
                    }
                }
            }
        }
    }

    // Part A
    void solveMST() {
        vector<double> minDist(n, numeric_limits<double>::infinity());
        vector<bool>   inMST(n, false);
        vector<size_t> parent(n, n);   
        minDist[0] = 0.0;
        double totalWeight = 0.0;

        for (size_t iter = 0; iter < n; ++iter) {
            size_t u = n;             
            for (size_t i = 0; i < n; ++i)
                if (!inMST[i] && (u == n || minDist[i] < minDist[u]))
                    u = i;

            if (minDist[u] == numeric_limits<double>::infinity()) {
                cerr << "Cannot construct MST\n";
                exit(1);
            }

            inMST[u] = true;
            totalWeight += sqrt(minDist[u]);

            for (size_t v = 0; v < n; ++v) {
                if (!inMST[v]) {
                    double d = distMSTSq(verts[u], verts[v]);
                    if (d < minDist[v]) {
                        minDist[v] = d;
                        parent[v] = u;
                    }
                }
            }
        }

        cout << fixed << setprecision(2) << totalWeight << "\n";
        for (size_t i = 1; i < n; ++i)
            cout << min(i, parent[i]) << " " << max(i, parent[i]) << "\n";
    }

    double tourLength(const vector<size_t>& tour) {  
        double total = 0.0;
        for (size_t i = 0; i < n; ++i)
            total += trueDist(verts[tour[i]], verts[tour[(i + 1) % n]]);
        return total;
    }

    // Part B 
    void solveFastTSP() {
        vector<size_t> tour;           
        tour.reserve(n);
        tour.push_back(0);
        tour.push_back(1);
        tour.push_back(2);
        
        for (size_t step = 3; step < n; ++step) {
            double bestChange = numeric_limits<double>::infinity();
            size_t bestPos = 0;
            for (size_t curr = 0; curr < tour.size(); ++curr) {
                size_t next = (curr + 1) % tour.size();
                
                double change = trueDist(verts[tour[curr]], verts[step]) +
                                trueDist(verts[step], verts[tour[next]]) -
                                trueDist(verts[tour[curr]], verts[tour[next]]);
                
                if (change < bestChange) {
                    bestChange = change;
                    bestPos = curr + 1; 
                }
            }
            
            tour.insert(tour.begin() + bestPos, step); 
        }
        
        
        cout << fixed << setprecision(2) << tourLength(tour) << "\n";
        for (size_t i = 0; i < n; ++i)
            cout << tour[i] << " \n"[i == n - 1];
    }
    
    // Part C
    
    void solveOptTSP() {
        mstDist.resize(n);
        inMST.resize(n); 
        precomputeDist();
        
        vector<size_t> tour;
        tour.reserve(n);
        tour.push_back(0);
        tour.push_back(1);
        tour.push_back(2);

        for (size_t step = 3; step < n; ++step) {
            double bestChange = numeric_limits<double>::infinity();
            size_t bestPos = 0;
            for (size_t curr = 0; curr < tour.size(); ++curr) {
                size_t next = (curr + 1) % tour.size();
                
                double change = dist[tour[curr]][step] +
                                dist[step][tour[next]] -
                                dist[tour[curr]][tour[next]];
                
                if (change < bestChange) {
                    bestChange = change;
                    bestPos = curr + 1; 
                }
            }
            tour.insert(tour.begin() + bestPos, step); 
        }

        twoOpt(tour); 
        
        bestLength = tourLength(tour);
        bestTour   = tour;

        genPerms(tour, 1, 0.0);

        cout << fixed << setprecision(2) << bestLength << "\n";
        for (size_t i = 0; i < n; ++i)
            cout << bestTour[i] << " \n"[i == n - 1];
    }


public:
    AmongUs() {
        cin >> n;                      
        verts.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            Vertex v;
            cin >> v.x >> v.y;
            v.area = classify(v.x, v.y);
            verts.push_back(v);
        }
    }

    void solve(const string& mode) {
        if      (mode == "MST")     solveMST();
        else if (mode == "FASTTSP") solveFastTSP();
        else if (mode == "OPTTSP")  solveOptTSP();
    }
};