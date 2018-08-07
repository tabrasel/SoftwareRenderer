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
                        
                        sf::Vector3f objectPosition = sf::Vector3f(x, y, z);
                        Vertex* newVertex = new Vertex(objectPosition);
                        vertices.push_back(newVertex);
                    }
                } else if (what == "f")
                {
                    if (result.size() == 4)
                    {
                        std::vector<int> polygonVertices;
                        for (int i = 1; i < result.size(); i++)
                        {
                            int vertIndex = std::stoi(result[i].substr(0, result[i].find_first_of("/"))) - 1;
                            polygonVertices.push_back(vertIndex);
                        }
                        
                        Vertex* v1 = vertices[polygonVertices[0]];
                        Vertex* v2 = vertices[polygonVertices[1]];
                        Vertex* v3 = vertices[polygonVertices[2]];
                         
                        Polygon* newPolygon = new Polygon(v1, v2, v3);
                        polygons.push_back(newPolygon);
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < vertices.size(); i++)
    {
        //std::cout << "Vertex #" << i + 1 << " " << &vertices[i] << " at (" << vertices[i].getWorldPosition().x << ", " << vertices[i].getWorldPosition().y << ", " << vertices[i].getWorldPosition().z << ")" << std::endl;
    }
}

std::vector<Vertex*>& Mesh::getVertices()
{
    return vertices;
}

std::vector<Polygon*>& Mesh::getPolygons()
{
    return polygons;
}
