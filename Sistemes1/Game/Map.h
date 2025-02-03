#pragma once
#include "../2inputSystem/InputSystem.h"
#include "../Game/World.h"
#include "../3Nodes/NodeMap.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

#include <vector>
#include <mutex>

class Map {
private:
    World* _world;
    NodeMap* _nodeMap;
    Player* player;
    std::vector<Enemy*> enemies;
    int mapIndex = 5;

    bool weapon = true;
    bool enemyToDelete = false;
    bool isRunning = true;

    std::mutex stateMutex;
    std::thread inputThread;
    std::thread movementThread;
    std::thread enemySpawnThread;
    std::mutex enemiesMutex;
    InputSystem inputSystem;

    std::thread autoSaveThread;
    bool autoSaveRunning = false;

    void SpawnEnemy();

public:
    Map(World* world);
    ~Map();

    void InitializeMap(NodeMap* sourceMap);
    void DrawMap();

    void InitializePlayer(Player* p);
    void DrawPlayer(Vector2 position);
    void MovePlayer(int dx, int dy);
    void StartInputListener();
    void StartMovementControl(int intervalMs);
    void StopThreads();

    void StartEnemySpawning();

    void SwitchToMap(NodeMap* sourceMap, NodeMap* targetMap);

    void MoveEnemy(Enemy* enemy);
    void RemoveEnemy(Vector2 position);

    void SaveGame();

    void AutoSave();
    void StopAutoSave();
};


