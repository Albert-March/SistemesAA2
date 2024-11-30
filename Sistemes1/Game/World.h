#pragma once
#include "../3Nodes/NodeMap.h"
#include "../Utils/ConsoleControl.h"

#include <vector>

class World {
	private:
    std::vector<std::vector<NodeMap*>> _maps;
    NodeMap* currentMap;

public:
    World(Vector2 size, Vector2 offset) {
        _maps.resize(3, std::vector<NodeMap*>(3));

        int mapIndex = 1;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                // Crear un nuevo NodeMap para cada posición
                _maps[i][j] = new NodeMap(size, offset);

                // Inicializar el NodeMap con contenido
                InitializeMap(_maps[i][j], size, offset, mapIndex);
                mapIndex++;
            }
        }

        // Establecer el mapa inicial
        currentMap = _maps[0][0];
    }

    void InitializeMap(NodeMap* nodeMap, Vector2 size, Vector2 offset, int indexMap) {
        int centerX = size.X / 2;
        int centerY = size.Y / 2;

        for (int y = 0; y < size.Y; ++y) {
            for (int x = 0; x < size.X; ++x) {
                Vector2 position(x, y);
                INodeContent* content = nullptr;

                if (x == 0 || x == size.X - 1 || y == 0 || y == size.Y - 1) {

                    content = new INodeContent(CellState::Wall);

                }
                else {
                    content = new INodeContent(CellState::Empty);
                }

                // Establecemos el contenido según el índice del mapa
                switch (indexMap) {
                case 1: // Mapa 1
                    if ((x == size.X - 1 && y == centerX) || (x == centerY && y == size.Y - 1)) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 2: // Mapa 2
                    // Portales en las esquinas y en el borde
                    if (x == centerX && y == size.Y - 1 || x == 0 && y == centerY || x == size.X - 1 && y == centerY) {
                        content = new INodeContent(CellState::Portal);  // Portal en el borde izquierdo
                    }
                    break;
                case 3:
                    if (x == 0 && y == centerY || x == centerX && y == size.Y - 1) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 4:
                    if (x == size.X - 1 && y == centerY || x == centerX && y == 0 || x == centerX && y == size.Y - 1) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 5: //Centro
                    if (x == 0 && y == centerY || x == size.X - 1 && y == centerY || x == centerX && y == size.Y - 1 || x == centerX && y == 0) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 6:
                    if (x == 0 && y == centerY || x == centerX && y == 0 || x == centerX && y == size.Y - 1) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 7:
                    if (x == centerX && y == 0 || x == size.X - 1 && y == centerY) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 8:
                    if (x == centerX && y == 0 || x == 0 && y == centerY || x == size.X - 1 && y == centerY) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;
                case 9:
                    if (x == centerX && y == 0 || x == 0 && y == centerY) {
                        content = new INodeContent(CellState::Portal);
                    }
                    break;

                }
                nodeMap->SafePickNode(position, [content](Node* node) {
                    if (node != nullptr) {
                        node->SetContent(content);
                    }
                    });

            }
        }
        int numChests = rand() % 4 + 2;
        int placedChests = 0;

        while (placedChests < numChests) {
            int x = rand() % size.X;
            int y = rand() % size.Y;
            Node* node = nodeMap->GetNode(x, y);
            INodeContent* content = node->GetContent();
            if (content->GetState() == CellState::Empty)
            {
                content = new INodeContent(CellState::Chest);
                placedChests++;
                nodeMap->SafePickNode(Vector2(x,y), [content](Node* node) {
                    if (node != nullptr) {
                        node->SetContent(content);
                    }
                    });
            }
        }
    }

    NodeMap* GetNodeMap(int indexMap) {
    if (indexMap < 1 || indexMap > 9) {
        std::cout << "Índice fuera de rango. Debe estar entre 1 y 9." << std::endl;
        return nullptr;
    }

    int row = (indexMap - 1) / 3;
    int col = (indexMap - 1) % 3;

    return _maps[row][col];
    } 
};

