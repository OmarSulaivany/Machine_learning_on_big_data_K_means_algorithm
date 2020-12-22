#include "Kmeans.h"
#include<iostream>
#include<vector>
#include <time.h>
#include<cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <omp.h>
using namespace std;

Kmeans::Kmeans():m_clusters(2), m_iterations(1),m_database_location("simple.csv"){}

void Kmeans::dataset(string database_location)
{
	this->m_database_location = database_location;
	fstream data;
	string line;
	string cell;
	vector<string> v1;
	double c;
	data.open(this->m_database_location, ios::in);
	getline(data, line);

#pragma omp parallel for 
	while (getline(data, line))
	{
		v1.clear();
		istringstream ss(line);
		
		while (getline(ss, cell, ','))
			v1.push_back(cell);
		this->m_dataset.push_back(v1);
		
		
	}
	
	data.close();

	
#pragma omp parallel for 
	for (unsigned i = 0; i < this->m_dataset.size(); i++)
	{
		for (unsigned j = 0; j < this->m_dataset[i].size(); j++)
		{
			if (sscanf_s(this->m_dataset[i][j].c_str(), "%lf", &c) != 1)
			{
				//cout << this->m_dataset[i][j] << " is text   ";		
				this->m_dataset[i][j] = { "0" };
			
			}
			
		}
		
	}
	
}

void Kmeans::kmeans(unsigned clusters, unsigned iterations)
{


	this->m_iterations = iterations;
	this->m_clusters = clusters;
	double com = 0.0;
	double minElementIndex;
	vector<double> v5;
	vector<vector<double>> clustering;
	vector<double> c_cluster;
	vector<string> cen;
	vector<double> v6;
	vector<double> v7;
	vector<string> v3;
	vector<unsigned> counter;

	double back;
	unsigned count = 0;
	double avg;
	unsigned random_number;

	
	//cout << "============================================\nCentroids\n";

	srand((unsigned)time(NULL));
	random_number = rand() % this->m_dataset.size();

#pragma omp parallel for 
	for (unsigned i = 0; i < this->m_clusters; i++)
	{
		v3.clear();
		for (unsigned j = 0; j < this->m_dataset[i].size(); j++)
		{
			v3.push_back(this->m_dataset[random_number][j]);
		}
		this->m_centroids.push_back(v3);

		random_number = rand() % this->m_dataset.size();
	}

	/*for (unsigned i = 0; i < this->m_centroids.size(); i++)
	{
		for (unsigned j = 0; j < this->m_centroids[i].size(); j++)
		{
			cout << this->m_centroids[i][j] << " ";
		}
		cout << endl;
	}

	cout << "======================================\n";*/


	unsigned cent_size = this->m_centroids.size();
	unsigned cent_i_size = this->m_centroids[0].size();

#pragma omp parallel for 
	for (unsigned epochs = 0; epochs <= this->m_iterations; ++epochs)
	{
		c_cluster.clear();
		for (unsigned i = 0; i < this->m_dataset.size(); ++i)
		{
			v5.clear();
			for (unsigned j = 0; j < this->m_clusters; ++j)
			{
				for (unsigned k = 0; k < this->m_dataset[i].size(); ++k)
				{
					com += abs(stod(this->m_dataset[i][k]) - stod(this->m_centroids[j][k]));
				}
				v5.push_back(com);
				com = 0;
			}
			minElementIndex = min_element(v5.begin(), v5.end()) - v5.begin();
			v5.push_back(minElementIndex);
			clustering.push_back(v5);
			minElementIndex = 0;
		}

		//cout << "Iteration: " << epochs << endl << endl;
		//cout << "Clustering\n\n";

		for (unsigned i = 0; i < clustering.size(); i++)
		{
			for (unsigned j = 0; j < clustering[i].size(); j++)
			{
				//cout << clustering[i][j] << " ";
			}
			//cout << "       Cluster: " << clustering[i].back();
			c_cluster.push_back(clustering[i].back());
			//	cout << endl;
		}

		//cout << "=======================================\n";

		//this->m_centroids.clear();


		for (unsigned i = 0; i < cent_size; i++)
		{
			cen.clear();
			for (unsigned j = 0; j < cent_i_size; j++)
			{
				cen.push_back("0");
				this->m_centroids[i][j] = cen[j];

			}


		}

		for (unsigned i = 0; i < this->m_dataset.size(); ++i)
		{
			v6.clear();
			v7.clear();
			for (unsigned j = 0; j < this->m_dataset[i].size(); ++j)
			{
				back = clustering[i].back();
				//cout << "Back:" << back<<"  ";

				v6.push_back(stod(this->m_dataset[i][j]));
				v7.push_back(stod(this->m_centroids[back][j]));

				v7[j] = v6[j] + v7[j];

				this->m_centroids[back][j] = to_string(v7[j]);

			}
		}
		//==============================================================================


		counter.resize(clustering[0].size() - 1);


		for (unsigned i = 0; i < counter.size(); ++i)
		{
			counter[i] = 0;
		}


		for (unsigned i = 0; i < clustering.size(); ++i)
		{
			for (unsigned j = 0; j < clustering[i].size() - 1; ++j)
			{

				if (clustering[i].back() == j)
				{
					count += 1;
					counter[j] = counter[j] + count;
				}
			}
			count = 0;
		}
		//cout << "Counter: \n";
		//for (unsigned i = 0; i < counter.size(); ++i)
			//cout << counter[i] << "  ";
		//cout << endl;

		/*cout << "=======================================\n";


		for (unsigned i = 0; i < this->m_centroids.size(); i++)
		{
			for (unsigned j = 0; j < this->m_centroids[i].size(); j++)
			{
				cout << this->m_centroids[i][j] << " ";
			}
			cout << endl;
		}*/

		//cout << "=======================================\n";
		//cout << "Updated centroids\n";

		for (unsigned i = 0; i < this->m_centroids.size(); i++)
		{
			for (unsigned j = 0; j < this->m_centroids[i].size(); j++)
			{
				avg = (stod(this->m_centroids[i][j])) / counter[i];
				this->m_centroids[i][j] = to_string(avg);
				//cout << this->m_centroids[i][j] << " ";
			}
			//cout << endl;
		}
		//cout << "=======================================\n";
		clustering.clear();
		system("CLS");
		cout << "Number of clusters: " << this->m_clusters << endl;
		cout << "Iterations: " << epochs<< " of "<<iterations;
	}
	cout << endl << endl;
	cout << "\n\n===================================================\n\n";
	fstream data;
	string line;
	string cell;
	string headers;
	vector<string> v1;
	double c;
	data.open(this->m_database_location, ios::in);


	getline(data, line);
	headers = line;
#pragma omp parallel for 
	while (getline(data, line))
	{
		v1.clear();
		istringstream ss(line);

		while (getline(ss, cell, ','))
			v1.push_back(cell);
		this->m_evaluate.push_back(v1);


	}

	data.close();



	//cout << "Rows : " << this->m_evaluate.size() << endl;
	//cout << "Columns : " << this->m_evaluate[0].size() << endl<<endl;;

	// Displaying the Dataset 
	unsigned size = this->m_dataset.size();
#pragma omp parallel for 
	for (unsigned i = 0; i < size; i++)
	{
		this->m_evaluate[i].push_back(to_string(c_cluster[i]));
	}

	string swaping;
#pragma omp parallel for 
	for (unsigned i = 0; i < this->m_evaluate.size(); ++i)
	{
		for (unsigned j = 0; j < this->m_evaluate.size(); ++j)
		{
			if (this->m_evaluate[i].back() < this->m_evaluate[j].back())
			{
				for (unsigned k = 0; k < this->m_evaluate[i].size(); ++k)
				{
					swaping = this->m_evaluate[i][k];
					this->m_evaluate[i][k] = this->m_evaluate[j][k];
					this->m_evaluate[j][k] = swaping;
				}
			}
		}
	}
	//cout << "\n\n===================================================\n\n";
	cout <<" Headers:\n" <<headers<<endl<<endl;
	cout << "\n===================================================\n";
	cout << "Cluster: " << stod(this->m_evaluate[0].back()) << endl << endl;
	unsigned d = 0;

#pragma omp parallel for 
	for (unsigned i = 0; i < this->m_evaluate.size(); ++i)
	{
		
		for (unsigned j = 0; j < this->m_evaluate[i].size()-1; ++j)
		{
			cout << this->m_evaluate[i][j] << "  ";
		}
		
		if (stod(this->m_evaluate[i].back()) == d)
		{
			cout << endl;

		}
		else
		{
			cout << "\n\n===================================================\n\n";
			//cout << headers;
			cout << "Cluster: " << stod(this->m_evaluate[i].back()) << endl<<endl;

		}
		d = stod(this->m_evaluate[i].back());
		
	}
	cout << "\n\n===================================================\n\n";
}
