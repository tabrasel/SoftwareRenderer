#include "Mesh.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "ResourcePath.hpp"
#include "Vertex.hpp"
#include "Polygon.hpp"

Mesh::Mesh()
{
    std::string fileName = "cube.obj";
    
    std::string line;
    std::ifstream objFile(resourcePath() + "cube.obj");
 
    /*
     v 1.000000 -1.000000 -1.000000
     vt 0.750000 0.750000
     vn 0.000000 0.000000 -1.000000
     f 5/1/1 1/2/1 4/3/1
    */
    
    if (objFile.is_open())
    {
        int row = 0;
        while (getline(objFile, line))
        {
            std::vector<std::string> result ;
            std::istringstream stm(line);
            std::string tok;
            
            while(stm >> tok)
            {
                result.push_back(tok);
            }
            
            if (result.size() > 0)
            {
                std::string what = result[0];
                
                if (what == "v") {
                    if (result.size() == 4)
                    {
                        float x = std::stof(result[1]);
                        float y = std::stof(result[2]);
                        float z = std::stof(result[3]);
                        
                        std::cout << "Making a vertex (" << x << ", " << y << ", " << z << ")" << std::endl;
                        sf::Vector3f objectPosition = sf::Vector3f(x, y, z);
                        Vertex vertex = Vertex(objectPosition);
                        vertices.push_back(vertex);
                    }
                } else if (what == "f")
                {
                    if (result.size() == 4)
                    {
                        std::vector<int> polygonVertices;
                        for (int i = 1; i < result.size(); i++)
                        {
                            int vertIndex = std::stoi(result[i].substr(0, result[i].find_first_of("/")));
                            polygonVertices.push_back(vertIndex);
                        }
                        
                        std::cout << "Making a face out of vertices #" << polygonVertices[0] << ", #" << polygonVertices[1] << ", and #" << polygonVertices[2] << std::endl;
                        
                        Vertex v1 = vertices[polygonVertices[0] + 1];
                        Vertex v2 = vertices[polygonVertices[1] + 1];
                        Vertex v3 = vertices[polygonVertices[2] + 1];
                        
                        Polygon polygon = Polygon(v1, v2, v3);
                        polygons.push_back(polygon);
                    }
                }
            }
        }
    }
}
