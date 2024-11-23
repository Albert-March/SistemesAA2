//#pragma once
//#include <vector>
//#include "Node.h"
//#include <functional>
//#include <mutex>
//#include <list>
//
//#include "Node.h"
//class NodeMap
//{
//public:
//	//Typedef: serveix per renombrar un tipus
//	//Si escric NodeColumn en realitat es el vector de l'esquerra
//	typedef std::vector<Node*> NodeColumn;
//	typedef std::vector<NodeColumn*> NodeGrid;
//
//	typedef std::function<void(Node* node)> SafePick;
//	typedef std::function<void(std::list<Node*> nodes)> SafeMultiPick;
//private:
//	Vector2 _offset;
//
//	Vector2 _size;
//	std::mutex _sizeMutex;
//
//	NodeGrid _grid;
//	std::mutex _gridMutex;
//
//	std::mutex _safeMultiNodeLockMutex;
//
//	Node* UnSafeGetNode(Vector2 position);
//
//public:
//	NodeMap(Vector2 size, Vector2 offset);
//
//	void UnSafeDraw();
//
//	void SafePickNode(Vector2 position, SafePick savePickAction);
//	void SafeMultiPickNode(std::list<Vector2> position, SafeMultiPick safeMultiPickAction );
//	Vector2 GetSize();
//};
//
