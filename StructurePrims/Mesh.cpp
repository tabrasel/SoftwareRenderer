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
                        float u = std::stof(lineParts[1]);
                        float v = std::stof(lineParts[2]);
                        std::cout << "vt: " << u << ", " << v << std::endl;
                        sf::Vector2f* newTextureCoords = new sf::Vector2f(u, v);
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
                            std::cout << vertexAttr << std::endl;
                            
                            int slashIndex1 = vertexAttr.find_first_of("/");
                            int slashIndex2 = vertexAttr.substr(slashIndex1 + 1, vertexAttr.length()).find_first_of("/");
                            
                            std::cout << "'" << lineParts[i].substr(slashIndex1 + 1, slashIndex2) << "'" << std::endl;

                            int vertexPositionIndex = std::stoi(lineParts[i].substr(0, slashIndex1)) - 1;
                            int textureCoordsIndex = std::stoi(lineParts[i].substr(slashIndex1 + 1, slashIndex2)) - 1;
                            
                            polygonVertices.push_back(vertexPositionIndex);
                            
                            polyVertices[i - 1] = vertices[vertexPositionIndex];
                            polyTextureCoords[i - 1] = textureCoords[textureCoordsIndex];
                            
                            //`std::cout << textureCoords[textureCoordsIndex]->x << ", " << textureCoords[textureCoordsIndex]->y << std::endl;
                            std::cout << "TextureCoordsFileIndex: " << std::stoi(lineParts[i].substr(slashIndex1 + 1, slashIndex2)) << ", TextureCoordsArrayIndex: " << textureCoordsIndex << std::endl;
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
        //std::cout << "#" << i + 1 << ": " << textureCoords[i]->x << ", " << textureCoords[i]->y << std::endl;
    }
    
    for (int i = 0; i < polygons.size(); i++)
    {
        std::cout << "Poly:\n(" << polygons[i]->getTextureCoords()[0]->x << ", " << polygons[i]->getTextureCoords()[0]->y << ")\n(" << polygons[i]->getTextureCoords()[1]->x << ", " << polygons[i]->getTextureCoords()[1]->y << ")\n(" << polygons[i]->getTextureCoords()[2]->x << ", " << polygons[i]->getTextureCoords()[2]->y << ")" << std::endl;
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
