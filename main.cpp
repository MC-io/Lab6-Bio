#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>

void get_clusters_at_rank(std::vector<std::string> labels, std::vector<std::vector<float>> distance_matrix, int rank, std::string mode)
{
    std::ofstream file("result.txt");
    file << "graph dendrogram {\nnode [shape=plaintext];\n";
    for (auto label : labels) 
    {
        file << label << ";\n";
    }
    for (int k = 0; k < rank; k++)
    {
        float min_dist = std::numeric_limits<float>::max();
        int min_row{}, min_col{};
        for (int i = 0; i < labels.size(); i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (distance_matrix[i][j] < min_dist)
                {
                    min_dist = distance_matrix[i][j];
                    min_row = i;
                    min_col = j;
                }
            }
        }
        std::cout << min_dist << '\n';
        std::cout << min_row << min_col << '\n';

        std::vector<std::string> new_labels = labels;
        new_labels.erase(new_labels.begin() + min_row);
        new_labels[min_col] += labels[min_row];

        std::vector<std::vector<float>> new_distance_matrix(new_labels.size(), std::vector<float>(new_labels.size(),0));

        for (int i = 0; i < new_labels.size(); i++)
        {
            for (int j = 0; j < i; j++)
            {
                int ii {}, jj{};
                if (i >= min_row) ii = i + 1;
                else ii = i;
                if (j >= min_row) jj = j + 1;
                else jj = j;

                if (i == min_col)
                {
                    if (mode == "MIN") new_distance_matrix[i][j] = std::min(distance_matrix[ii][jj], distance_matrix[min_row][jj]);
                    else if (mode == "MAX") new_distance_matrix[i][j] = std::max(distance_matrix[ii][jj], distance_matrix[min_row][jj]);
                    else new_distance_matrix[i][j] = (distance_matrix[ii][jj] + distance_matrix[min_row][jj]) / 2.f;
                }
                else if (j == min_col)
                {
                    if (mode == "MIN") new_distance_matrix[i][j] = std::min(distance_matrix[ii][jj], distance_matrix[ii][min_row]);
                    else if (mode == "MAX") new_distance_matrix[i][j] = std::max(distance_matrix[ii][jj], distance_matrix[ii][min_row]);
                    else new_distance_matrix[i][j] = (distance_matrix[ii][jj] + distance_matrix[ii][min_row]) / 2.f;
                }
                else
                {
                    new_distance_matrix[i][j] = distance_matrix[ii][jj];
                }
                new_distance_matrix[j][i] = new_distance_matrix[i][j];
            }
        }

        file << new_labels[min_col] << "[label=\"" << new_labels[min_col] << "\"];\n";
        file << new_labels[min_col] << " -- " << labels[min_row] << ";\n";
        file << new_labels[min_col] << " -- " <<  labels[min_col] << ";\n";

        labels = new_labels;
        distance_matrix = new_distance_matrix;

    }
    file << '}';

    for (const auto & a: labels)
        std::cout << a << ' ';
    std::cout << '\n';

     for (int i = 0; i < labels.size(); i++)
    {
        for (int j = 0; j < labels.size(); j++)
        {
            std::cout << distance_matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

int main()
{
    std::vector<std::vector<float>> dist_matrix = 
    {
        {0, 2.15, 0.7, 1.07, 0.85, 1.16, 1.56},
        {2.15, 0, 1.53, 1.14, 1.38, 1.01, 2.83} ,
        {0.7, 1.53, 0, 0.43, 0.21, 0.55, 1.86},
        {1.07, 1.14, 0.43, 0, 0.29, 0.22, 2.02},
        {0.85, 1.38, 0.21, 0.29, 0, 0.41, 2.02},
        {1.16, 1.01, 0.55, 0.22, 0.41, 0, 2.05},    
        {1.56, 2.83, 1.86, 2.04, 2.02, 2.05, 0},    
    };

    std::vector<std::string> labels = {"A","B","C","D","E","F","G"};


    get_clusters_at_rank(labels, dist_matrix, 6, "AVG");



    return 0;
}