#pragma once
#include "../2inputSystem/InputSystem.h"
#include "../Game/World.h"
#include "../3Nodes/NodeMap.h"
#include "../Player/Player.h"


class Map
{
private:
    World* _world;
    NodeMap* _nodeMap;
    Player* player;
    int mapIndex = 5;
    bool weapon = true;

    bool isRunning = true;
    std::mutex stateMutex;
    std::thread inputThread;
    std::thread movementThread;
    InputSystem inputSystem;
public:

    Map(World* world);

    void InitializeMap(NodeMap* sourceMap);
    void DrawMap();

    void InitializePlayer(Player* p);
    void DrawPlayer(Vector2 poition);
    void MovePlayer(int dx, int dy);
    void StartInputListener();
    void StartMovementControl(int intervalMs);
    void StopThreads();

    void SwitchToMap(NodeMap* sourceMap, NodeMap* targetMap);
};

