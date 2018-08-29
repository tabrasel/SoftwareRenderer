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
    std::ifstream objFile(resourcePath() + fileName);
    
    if (objFile.is_open())
    {
        int row = 0;
        while (getline(objFile, line))
        {
            std::vector<std::string> lineParts ;
            std::istringstream stm(line);
            std::string tok;
            
            while(stm >> tok)
            {
                lineParts.push_back(tok);
            }
            
            if (lineParts.size() > 0)
            {
                std::string elementType = lineParts[0];
                
                if (elementType == "v") {
                    if (lineParts.size() == 4)
                    {
                        float x = std::stof(lineParts[1]);
                        float y = std::stof(lineParts[2]);
                        float z = std::stof(lineParts[3]);
                        
                        sf::Vector3f objectPosition = sf::Vector3f(x, y, z);
                        Vertex* newVertex = new Vertex(objectPosition);
                        vertices.push_back(newVertex);
                    }
                } else if (elementType == "vt")
                {
                    if (lineParts.size() == 3)
                    {
                        float x = std::stof(lineParts[1]);
                        float y = std::stof(lineParts[2]);
                        sf::Vector2f* newTextureCoords = new sf::Vector2f(x, y);
                        textureCoords.push_back(newTextureCoords);
                    }
                } else if (elementType == "f")
                {
                    if (lineParts.size() == 4)
                    {
                        std::vector<int> polygonVertices;
                        
                        std::array<Vertex*, 3> polyVertices;
                        std::array<sf::Vector2f*, 3> polyTextureCoords;
                        
                        for (int i = 1; i < lineParts.size(); i++)
                        {
                            std::string vertexAttr = lineParts[i];
                            
                            int slashIndex1 = vertexAttr.find_first_of("/");
                            int slashIndex2 = vertexAttr.substr(0, slashIndex1 + 1).find_first_of("/");

                            int vertexPositionIndex = std::stoi(lineParts[i].substr(0, slashIndex1)) - 1;
                            int textureCoordsIndex = std::stoi(lineParts[i].substr(slashIndex1 + 1, slashIndex2)) - 1;
                            
                            polygonVertices.push_back(vertexPositionIndex);
                            
                            polyVertices[i - 1] = vertices[vertexPositionIndex];
                            polyTextureCoords[i - 1] = textureCoords[textureCoordsIndex];
                        }
                        
                        Vertex* v1 = vertices[polygonVertices[0]];
                        Vertex* v2 = vertices[polygonVertices[1]];
                        Vertex* v3 = vertices[polygonVertices[2]];
                        
                        Polygon* newPoly = new Polygon(polyVertices, polyTextureCoords);
                        
                        polygons.push_back(newPoly);
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < textureCoords.size(); i++)
    {
        std::cout << textureCoords[i]->x << ", " << textureCoords[i]->y << std::endl;
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
