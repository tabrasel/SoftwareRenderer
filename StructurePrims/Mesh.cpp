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
    std::string fileName = "teapot.obj";
    
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
                        float u = std::stof(lineParts[1]);
                        float v = std::stof(lineParts[2]);
                        sf::Vector2f* newTextureCoords = new sf::Vector2f(u, v);
                        textureCoords.push_back(newTextureCoords);
                    }
                } else if (elementType == "f")
                {
                    std::vector<Vertex*> polyVertices;
                    std::vector<sf::Vector2f*> polyTextureCoords;
                    
                    for (int i = 1; i < lineParts.size(); i++)
                    {
                        std::string vertexAttr = lineParts[i];
                     
                        int slashIndex1 = vertexAttr.find_first_of("/");
                        int slashIndex2 = vertexAttr.substr(slashIndex1 + 1, vertexAttr.length()).find_first_of("/");
                     
                        if (slashIndex1 > -1)
                        {
                            // When there is more than one vertex attribute index
                            int vertexPositionIndex = std::stoi(lineParts[i].substr(0, slashIndex1)) - 1;
                            polyVertices.push_back(vertices[vertexPositionIndex]);
                     
                            if (slashIndex2 > -1)
                            {
                                // When there are more than two vertex attribute indices (vertex pos, UV coords, and normal)
                                int textureCoordsIndex = std::stoi(vertexAttr.substr(slashIndex1 + 1, slashIndex2)) - 1;
                                polyTextureCoords.push_back(textureCoords[textureCoordsIndex]);
                            } else {
                                // When there are only two element indices (vertex pos and UV coords)
                                int textureCoordsIndex = std::stoi(vertexAttr.substr(slashIndex1 + 1, vertexAttr.length())) - 1;
                                polyTextureCoords.push_back(textureCoords[textureCoordsIndex]);
                            }
                        } else {
                            // If there is only one vertex attribute index (or none at all)
                            sf::Vector2f* newTextureCoords = new sf::Vector2f(0.0, 0.0);
                            polyTextureCoords.push_back(newTextureCoords);
                            int vertexPositionIndex = std::stoi(lineParts[i].substr(0, lineParts[i].size())) - 1;
                            polyVertices.push_back(vertices[vertexPositionIndex]);
                        }
                    }
                    
                    // Triangularize the polygons
                    for (int i = 0; i < polyVertices.size() - 2; i++)
                    {
                        std::array<Vertex*, 3> triVertices = { polyVertices[0], polyVertices[i + 1], polyVertices[i + 2] };
                        std::array<sf::Vector2f*, 3> triTextureCoords = { polyTextureCoords[0], polyTextureCoords[i + 1], polyTextureCoords[i + 2] };
                        
                        Polygon* newPoly = new Polygon(triVertices, triTextureCoords);
                        polygons.push_back(newPoly);
                    }
                }
            }
        }
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
