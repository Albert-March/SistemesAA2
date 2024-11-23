//#pragma once
//#include "Vector2.h"
//#include "INodeContent.h"
//#include <mutex>
//class Node
//{
//private:
//	Vector2 _position; 
//	INodeContent* _content = nullptr;
//	std::mutex _classMutex;
//public:
//	Node(Vector2 position);
//
//
//	INodeContent* GetContent();
//
//	
//
//	template<typename T, typename = typename std::enable_if<std::is_base_of<INodeContent, T>::value>::type>
//	T* GetContent() {
//		T* contentCasted = dynamic_cast<T*>(_content);
//		return content;
//	}
//
//	void SetContent(INodeContent* newContent);
//	void DrawContent(Vector2 offset);
//	//Crear funcions auxiliars:
//	// 1. bool isEmpty()
//	// 2. void CheckContent() 
//	void Lock();
//	void Unlock();
//};
//
