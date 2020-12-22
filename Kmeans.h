#ifndef KMEANS_HH
#define KMEANS_HH
#include <vector>
#include <string>
using namespace std;
class Kmeans {
public:
	Kmeans();
	void dataset(string database_location);
	void kmeans(unsigned clusters, unsigned iterations);
	

private:
	unsigned m_clusters, m_iterations;
	string m_database_location;
	vector<vector<string>> m_dataset;
	vector<vector<string>> m_evaluate;
	vector<vector<string>> m_centroids;
};

#endif // !KMEANS_HH
