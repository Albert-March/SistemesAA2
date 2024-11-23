//#include "Node.h"
//#include "../Utils/ConsoleControl.h"
//void Node::DrawContent(Vector2 offset)
//{
//	if (_content == nullptr) {
//		Vector2 pos = offset + _position;
//		CC::Lock();
//		CC::SetPosition(pos.X, pos.Y);
//		std::cout << " ";
//		CC::Unlock();
//		return;
//	}
//	_content->Draw(offset);
//}
//
//void Node::Lock()
//{
//	_classMutex.lock();
//}
//
//void Node::Unlock()
//{
//	_classMutex.unlock();
//}
//
//Node::Node(Vector2 position)
//{
//	_position = position;
//}
//
//
//INodeContent* Node::GetContent() {
//	return _content;
//}