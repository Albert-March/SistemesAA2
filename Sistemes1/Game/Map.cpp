#include "Map.h"
#include "../Utils/ConsoleControl.h"

#include <fstream>
#include <json/json.h>

Map::Map(World* world) : _world(world), _nodeMap(nullptr), player(nullptr) {
    if (!_world) {
        std::cout << "World no está inicializado." << std::endl;
    }
    StartEnemySpawning();
}

Map::~Map() {
    StopThreads();
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void Map::InitializeMap(NodeMap* sourceMap) {

    Vector2 size = sourceMap->GetSize();

    _nodeMap = new NodeMap(size, Vector2(0,0));

    for (int y = 0; y < size.Y; ++y) {
        for (int x = 0; x < size.X; ++x) {
            Vector2 position(x, y);
            Node* sourceNode = sourceMap->GetNode(x, y);

            if (sourceNode != nullptr) {
                INodeContent* content = sourceNode->GetContent();

                _nodeMap->SafePickNode(position, [content](Node* node) {
                    if (node != nullptr) {
                        if (content) {
                            node->SetContent(content);
                        }
                    }
                    });
            }
        }
    }
    DrawMap();  
}

void Map::SwitchToMap(NodeMap* sourceMap, NodeMap* targetMap) {
    if (_nodeMap == nullptr || targetMap == nullptr) {
        std::cout << "Uno de los mapas es nulo." << std::endl;
        return;
    }

    Vector2 size = _nodeMap->GetSize();

    for (int y = 0; y < size.Y; ++y) {
        for (int x = 0; x < size.X; ++x) {
            Vector2 position(x, y);
            Node* currentNode = _nodeMap->GetNode(x, y);

            if (currentNode != nullptr) {
                INodeContent* content = currentNode->GetContent();

                sourceMap->SafePickNode(position, [content](Node* node) {
                    if (node != nullptr) {
                        if (content) {
                            node->SetContent(content);
                        }
                    }
                    });
            }
        }
    }
    CC::Lock();
    CC::SetPosition(0, 0);
    CC::Unlock();

    _nodeMap = targetMap;
    DrawMap();
}

void Map::DrawMap()
{
    CC::Lock();
    CC::SetPosition(0, 0);
    CC::Unlock();

    Vector2 size = _nodeMap->GetSize();

    for (int y = 0; y < size.Y; ++y) {
        for (int x = 0; x < size.X; ++x) {
            Node* node = _nodeMap->GetNode(x, y);
            if (node != nullptr) {
                INodeContent* content = node->GetContent();
                if (content != nullptr) {
                    content->Draw(Vector2(0, 0));
                }
                else {
                    std::cout << " ";
                }
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    Vector2 mapSize = _nodeMap->GetSize();
    CC::Lock();
    CC::SetPosition(0, mapSize.Y);
    std::cout << "Map: " << mapIndex << std::endl;
    CC::Unlock();
    if (!player)
    {
        return;
    }
    CC::Lock();
    Vector2 oldPosition = player->position;
    CC::SetPosition(mapSize.Y + 5, 0);
    std::cout << "Monedas: " << player->coins;
    CC::SetPosition(mapSize.Y + 5, 1);
    std::cout << "Vidas: " << player->lives;
    CC::SetPosition(mapSize.Y + 5, 2);
    std::cout << "Pociones: " << player->potions;
    CC::SetPosition(mapSize.Y + 5, 3);
    if (!player->weapons.empty()) {
        std::cout << "Armas: ";
        for (const auto& weapon : player->weapons) {
            std::cout << weapon << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Armas: Ninguna" << std::endl;
    }
    CC::Unlock();
}

void Map::InitializePlayer(Player* p)
{  
    player = p;
    Vector2 playerPosition = player->position;

    stateMutex.lock();
    _nodeMap->SafePickNode(playerPosition, [](Node* node) {
        if (node != nullptr) {
            INodeContent* playerContent = new INodeContent(CellState::Player);
            node->SetContent(playerContent);
        }
        });
    stateMutex.unlock();

    stateMutex.lock();
    Node* node = _nodeMap->GetNode(playerPosition.X, playerPosition.Y);
    if (node != nullptr) {
        INodeContent* content = node->GetContent();
        if (content != nullptr) {
            CC::Lock();
            CC::SetPosition(playerPosition.X, playerPosition.Y);
            content->Draw(Vector2(0, 0));
            CC::Unlock();
        }
    }  
    stateMutex.unlock();

    Vector2 mapSize = _nodeMap->GetSize();
    CC::Lock();
    CC::SetPosition(0, mapSize.Y);
    CC::Unlock();
}

void Map::DrawPlayer(Vector2 position)
{
    player->position = position;
    _nodeMap->SafePickNode(position, [](Node* node) {
        if (node != nullptr) {
            INodeContent* playerContent = new INodeContent(CellState::Player);
            node->SetContent(playerContent);
        }
        });
    Node* node = _nodeMap->GetNode(position.X, position.Y);
    if (node != nullptr) {
        INodeContent* content = node->GetContent();
        if (content != nullptr) {
            CC::Lock();
            CC::SetPosition(position.X, position.Y);
            content->Draw(Vector2(0, 0));
            CC::Unlock();
        }
    }
}

void Map::StartInputListener() {
    inputSystem.AddListener(K_UP, [&]() { MovePlayer(0, -1); });
    inputSystem.AddListener(K_DOWN, [&]() { MovePlayer(0, 1); });
    inputSystem.AddListener(K_LEFT, [&]() { MovePlayer(-1, 0); });
    inputSystem.AddListener(K_RIGHT, [&]() { MovePlayer(1, 0); });

    inputSystem.AddListener(K_P, [&]() {
        if (player->potions > 0 && player->lives < 3)
        {
            player->lives++;
            player->potions--;
            DrawMap();
        }
        });

    inputThread = std::thread([&]() {
        inputSystem.StartListen();
        });
}

void Map::StartMovementControl(int intervalMs) {
    movementThread = std::thread([&, intervalMs]() {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
        });
}

void Map::MovePlayer(int x, int y) {
    if (!player) {
        std::cout << "Error";
        return;
    }
    stateMutex.lock();
    Vector2 oldPosition = player->position;
    Vector2 newPosition = Vector2(oldPosition.X + x, oldPosition.Y + y);
    stateMutex.unlock();


    Node* comprobarNode = _nodeMap->GetNode(newPosition.X, newPosition.Y);
    INodeContent* content = comprobarNode->GetContent();
    if (content->GetState() == CellState::Wall) {
        return;
    }
    
    if (content->GetState() == CellState::Chest || content->GetState() == CellState::Enemy) {

        if (content->GetState() == CellState::Enemy) {
            enemyToDelete = true;
        }
        int reward = rand() % 100 + 1;

        if (weapon == true)
        {
            if (reward <= 30) {
                reward = 1;
            }
            else if (reward <= 60) {
                reward = 2;
            }
            else {
                reward = 3;
            }
        }
        else
        {
            if (reward <= 50) {
                reward = 1;
            }
            else
                reward = 2;
        }
        _nodeMap->SafePickNode(newPosition, [reward, newPosition, this](Node* node) {
                INodeContent* rewardContent = nullptr;

            switch (reward) {
            case 1:
                rewardContent = new INodeContent(CellState::Coin);
                break;
            case 2:
                rewardContent = new INodeContent(CellState::Potion);
                break;
            case 3:
                rewardContent = new INodeContent(CellState::Weapon);
                this->weapon = false;
                break;
            default:
                std::cout << "Error: recompensa desconocida." << std::endl;
                return;
            }
            node->SetContent(rewardContent);
            CC::Lock();
            CC::SetPosition(newPosition.X, newPosition.Y);
            rewardContent->Draw(Vector2(0, 0));
            CC::Unlock();
        });
        return;
    }

    if (content->GetState() == CellState::Coin) {
        player->coins += 1;
        DrawMap();
    }
    if (content->GetState() == CellState::Potion) {
        player->potions += 1;
        DrawMap();
    }
    if (content->GetState() == CellState::Weapon) {
        player->AddWeapon("Lanza");
        DrawMap();
    }

    
    if (content->GetState() == CellState::Portal) {
        Vector2 size = _nodeMap->GetSize();
        int centerX = size.X / 2;
        int centerY = size.Y / 2;
        bool justEntered = false;

        NodeMap* currentMap = _world->GetNodeMap(mapIndex);
        NodeMap* nextMap;

        switch (mapIndex)
        {
        case 1:
            if (player->position.X == centerX && player->position.Y == size.Y -2 && !justEntered)
            { 
                mapIndex = 4;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, 1));
                justEntered = true;

            }
            if (player->position.X == size.X -2 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 2;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(1, centerY));
                justEntered = true;
            }
            break;
        case 2:
            if (player->position.X == centerX && player->position.Y == size.Y - 2 && !justEntered)
            {
                mapIndex = 5;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, 1));
                justEntered = true;

            }
            if (player->position.X == 1 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 1;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(size.X -2, centerY));
                justEntered = true;
            }
            if (player->position.X == size.X - 2 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 3;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(1, centerY));
                justEntered = true;
            }
            break;
        case 3:
            if (player->position.X == 1 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 2;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(size.X -2, centerY));
                justEntered = true;

            }
            if (player->position.X == centerX && player->position.Y == size.Y -2 && !justEntered)
            {
                mapIndex = 6;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, size.Y -2));
                justEntered = true;
            }
            break;
        case 4:
            if (player->position.X == centerX && player->position.Y == 1 && !justEntered)
            {
                mapIndex = 1;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, size.Y -2));
                justEntered = true;

            }
            if (player->position.X == size.X -2 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 5;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(1, centerY));
                justEntered = true;
            }
            if (player->position.X == centerX && player->position.Y == size.Y - 2 && !justEntered)
            {
                mapIndex = 7;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, 1));
                justEntered = true;
            }
            break;
        case 5:
            if (player->position.X == 1 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 4;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(size.X -2, centerY));
                justEntered = true;
            }
            if (player->position.X == size.X -2 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 6;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(1, centerY));
                justEntered = true;
            }
            if (player->position.X == centerX && player->position.Y == 1 && !justEntered)
            {
                mapIndex = 2;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, size.Y -2));
                justEntered = true;
                
            }           
            if (player->position.X == centerX && player->position.Y == size.Y -2 && !justEntered)
            {
                mapIndex = 8;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, 1));
                justEntered = true;
            }
            break;
        case 6:
            if (player->position.X == centerX && player->position.Y == 1 && !justEntered)
            {
                mapIndex = 3;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, size.Y - 2));
                justEntered = true;

            }
            if (player->position.X == 1 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 5;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(size.X -2, centerY));
                justEntered = true;
            }
            if (player->position.X == centerX && player->position.Y == size.Y - 2 && !justEntered)
            {
                mapIndex = 9;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, 1));
                justEntered = true;
            }
            break;
        case 7:
            if (player->position.X == centerX && player->position.Y == 1 && !justEntered)
            {
                mapIndex = 4;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, 1));
                justEntered = true;

            }
            if (player->position.X == size.X -2 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 8;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(1, centerY));
                justEntered = true;
            }
            break;
        case 8:
            if (player->position.X == 1 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 7;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(size.X -2, centerY));
                justEntered = true;

            }
            if (player->position.X == centerX && player->position.Y == 1 && !justEntered)
            {
                mapIndex = 5;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, size.Y -2));
                justEntered = true;

            }
            if (player->position.X == size.X -2 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 9;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(1, centerY));
                justEntered = true;

            }
            break;
        case 9:
            if (player->position.X == 1 && player->position.Y == centerY && !justEntered)
            {
                mapIndex = 8;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(size.X - 2, centerY));
                justEntered = true;

            }
            if (player->position.X == centerX && player->position.Y == 1 && !justEntered)
            {
                mapIndex = 6;
                nextMap = _world->GetNodeMap(mapIndex);
                SwitchToMap(currentMap, nextMap);
                DrawPlayer(Vector2(centerX, size.Y - 2));
                justEntered = true;

            }
            break;
        }
        justEntered = false;
        return;
    }

    _nodeMap->SafePickNode(oldPosition, [](Node* node) {
        if (node != nullptr) {
            INodeContent* emptyContent = new INodeContent(CellState::Empty);
            node->SetContent(emptyContent);
        }
        });
    Node* node = _nodeMap->GetNode(oldPosition.X, oldPosition.Y);
    if (node != nullptr) {
        INodeContent* content = node->GetContent();
        if (content != nullptr) {
            CC::Lock();
            CC::SetPosition(oldPosition.X, oldPosition.Y);
            content->Draw(Vector2(0, 0));
            CC::Unlock();
        }
    }
    stateMutex.lock();
    player->position = newPosition;
    stateMutex.unlock();

    _nodeMap->SafePickNode(newPosition, [](Node* node) {
        if (node != nullptr) {
            INodeContent* playerContent = new INodeContent(CellState::Player);
            node->SetContent(playerContent);
        }
        });
    Node* node2 = _nodeMap->GetNode(newPosition.X, newPosition.Y);
    if (node2 != nullptr) {
        INodeContent* content = node2->GetContent();
        if (content != nullptr) {
            CC::Lock();
            CC::SetPosition(newPosition.X, newPosition.Y);
            content->Draw(Vector2(0, 0));
            CC::Unlock();
        }
    }

    Vector2 mapSize = _nodeMap->GetSize();
    CC::SetPosition(0, mapSize.Y);

    if (enemyToDelete == true)
    {
        RemoveEnemy(newPosition);
    }
}

void Map::StartEnemySpawning() {
    enemySpawnThread = std::thread([this]() {
        while (isRunning) {
            int waitTime = rand() % 20001 + 5000;
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
            SpawnEnemy();
        }
        });
}

void Map::SpawnEnemy() {
    if (!_nodeMap) return;

    Vector2 size = _nodeMap->GetSize();
    Vector2 spawnPosition;
    bool validSpawn = false;

    while (!validSpawn) {
        spawnPosition = Vector2(rand() % (size.X - 2) + 1, rand() % (size.Y - 2) + 1);
        Node* node = _nodeMap->GetNode(spawnPosition.X, spawnPosition.Y);

        Node* nodeUp = _nodeMap->GetNode(spawnPosition.X, spawnPosition.Y -1);
        Node* nodeDown = _nodeMap->GetNode(spawnPosition.X, spawnPosition.Y +1);
        Node* nodeLeft = _nodeMap->GetNode(spawnPosition.X -1, spawnPosition.Y);
        Node* nodeRight = _nodeMap->GetNode(spawnPosition.X +1, spawnPosition.Y);

        if (node && node->GetContent()->GetState() != CellState::Player &&
            nodeUp && node->GetContent()->GetState() != CellState::Player&&
            nodeDown && node->GetContent()->GetState() != CellState::Player&&
            nodeLeft && node->GetContent()->GetState() != CellState::Player&&
            nodeRight && node->GetContent()->GetState() != CellState::Player) {
            validSpawn = true;
        }
    }

    Enemy* enemy = new Enemy(spawnPosition, this);
    enemies.push_back(enemy);

    _nodeMap->SafePickNode(spawnPosition, [](Node* node) {
        node->SetContent(new INodeContent(CellState::Enemy));
        });

    DrawMap();
}

void Map::MoveEnemy(Enemy* enemy) {
    if (!_nodeMap) return;

    Vector2 oldPos = enemy->GetPosition();
    Vector2 newPos = oldPos;
    bool moved = false;

    for (int i = 0; i < 4; i++) {
        int direction = (rand() % 4) + 1;

        switch (direction) {
        case 1: newPos = Vector2(oldPos.X, oldPos.Y - 1); break;
        case 2: newPos = Vector2(oldPos.X, oldPos.Y + 1); break;
        case 3: newPos = Vector2(oldPos.X - 1, oldPos.Y); break;
        case 4: newPos = Vector2(oldPos.X + 1, oldPos.Y); break;
        }

        Node* node = _nodeMap->GetNode(newPos.X, newPos.Y);
        if (node && node->GetContent()->GetState() == CellState::Empty) {

            _nodeMap->SafePickNode(oldPos, [](Node* node) {
                node->SetContent(new INodeContent(CellState::Empty));
                });

            enemy->SetPosition(newPos);

            _nodeMap->SafePickNode(newPos, [](Node* node) {
                node->SetContent(new INodeContent(CellState::Enemy));
                });

            DrawMap();
            moved = true;
            break;
        }

        if (node && node->GetContent()->GetState() == CellState::Player)
        {
            player->lives--;
            DrawMap();
            moved = true;
            break;
        }
    }
}

void Map::RemoveEnemy(Vector2 position) {
    enemiesMutex.lock();
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i]->GetPosition() == position) {
            enemies[i]->StopMoving();
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
            break;
        }
    }
    enemiesMutex.unlock();

    DrawMap();
}

void Map::StopThreads() {
    isRunning = false;

    if (inputThread.joinable()) inputThread.join();
    if (movementThread.joinable()) movementThread.join();
    if (enemySpawnThread.joinable()) enemySpawnThread.join();

    inputSystem.StopListen();
}

void Map::SaveGame() {
    Json::Value saveData;

    if (player) {
        saveData["player"]["position"]["x"] = player->position.X;
        saveData["player"]["position"]["y"] = player->position.Y;
        saveData["player"]["health"] = player->health;
        saveData["player"]["lives"] = player->lives;
        saveData["player"]["damage"] = player->damage;
        saveData["player"]["coins"] = player->coins;
        saveData["player"]["potions"] = player->potions;

        Json::Value jsonWeapons(Json::arrayValue);
        for (const auto& weapon : player->weapons) {
            jsonWeapons.append(weapon);
        }
        saveData["player"]["weapons"] = jsonWeapons;
    }

    Json::Value enemiesArray(Json::arrayValue);
    for (const auto& enemy : enemies) {
        Json::Value enemyData;
        enemyData["position"]["x"] = enemy->GetPosition().X;
        enemyData["position"]["y"] = enemy->GetPosition().Y;
        enemyData["health"] = enemy->IsDead() ? 0 : 100;
        enemiesArray.append(enemyData);
    }
    saveData["enemies"] = enemiesArray;

    saveData["mapIndex"] = mapIndex;

    std::ofstream file("savegame.json");
    if (file.is_open()) {
        file << saveData;
        file.close();
    }
    else {
    }
}

void Map::AutoSave() {
    if (autoSaveRunning) return;

    autoSaveRunning = true;
    autoSaveThread = std::thread([this]() {
        while (autoSaveRunning) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            SaveGame();
        }
        });
}

void Map::StopAutoSave() {
    if (!autoSaveRunning) return;

    autoSaveRunning = false;

    if (autoSaveThread.joinable()) {
        autoSaveThread.join();
    }
}