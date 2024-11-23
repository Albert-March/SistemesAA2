#include "InputSystem.h"
#include "../Utils/ConsoleControl.h"
InputSystem::KeyBinding::KeyBinding(int key, OnKeyPress onKeyPress)
{
	_key = key;
	_onKeyPress = onKeyPress;
}

InputSystem::KeyBinding::~KeyBinding()
{
}

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem() {
	for (std::pair<int, KeyBindingList> pair : _keyBindingsMap) {
		KeyBindingList bindingsList = pair.second;
		for (KeyBinding* binding : bindingsList) {
			delete binding;
		}
	}
	_keyBindingsMap.clear();
}

InputSystem::KeyBinding* InputSystem::AddListener(int key, KeyBinding::OnKeyPress onKeyPress)
{
	_classMutex.lock();
	if (_keyBindingsMap.find(key) == _keyBindingsMap.end()){
		_keyBindingsMap[key] = KeyBindingList();
	}

	KeyBinding* keyBinding = new KeyBinding(key, onKeyPress);
	_keyBindingsMap[key].push_back(keyBinding);

	_classMutex.unlock();

	return keyBinding;
}

void InputSystem::RemoveAndDeleteListener(KeyBinding* keyBinding)
{
	int key = keyBinding->_key;
	_classMutex.lock();

	if (_keyBindingsMap.find(key) != _keyBindingsMap.end()) {
		KeyBindingList list = _keyBindingsMap[key];
		list.remove(keyBinding);
		delete keyBinding;
		if (list.size() <= 0) {
			_keyBindingsMap.erase(key);
		}
	}

	_classMutex.unlock();
}

void InputSystem::AddListenerAsync(int key, KeyBinding::OnKeyPress onKeyPress)
{
	std::thread* addListenerThread = new std::thread(&InputSystem::AddListener, this, key, onKeyPress);
	addListenerThread->detach();
}

void InputSystem::RemoveAndDeleteListenerAsync(KeyBinding* keyBinding)
{
	std::thread* removeListenerThread = new std::thread(&InputSystem::RemoveAndDeleteListener, this, keyBinding);
	removeListenerThread->detach();
}

void InputSystem::StartListen()
{
	_classMutex.lock();

	if (_state != Stopped) {
		_classMutex.unlock();
		return;
	}

	_state = Starting;

	std::thread* listenLoopThread = new std::thread(&InputSystem::ListenLoop, this);
	listenLoopThread->detach(); 

	_classMutex.unlock();
}

void InputSystem::StopListen()
{
	_classMutex.lock();
	
	if (_state != Starting) {
		_classMutex.unlock();
			return;
	}

	_state = Stopping;
	
	_classMutex.unlock();
}

void InputSystem::ListenLoop()
{
	_classMutex.lock();
	_state = Listening;
	State currentState = _state;
	CC::Lock();
	CC::ClearKeyBuffer();
	CC::Unlock();
	_classMutex.unlock();

	while (currentState == Listening) {
		int key = CC::ReadNextKey();
		
		if (key != 0) {
			_classMutex.lock();

			KeyBindingMap::iterator it = _keyBindingsMap.find(key); //Manera més òptima
			if (it != _keyBindingsMap.end()) {
				KeyBindingList list = it->second;

				for (KeyBinding* binding : list) {
					binding->_onKeyPress();  //Falta unlock, així que petarà
					/*std::thread* onKeyPressThread = new std::thread(binding->_onKeyPress);
					onKeyPressThread->detach();*/
				}
			}

			/*if (_keyBindingsMap.find(key) != _keyBindingsMap.end()) {  //Manera menys òptima
				KeyBindingList list = _keyBindingsMap[key];

				//TODO
			}*/
			

			_classMutex.unlock();
		}

		_classMutex.lock();
		currentState = _state;
		_classMutex.unlock();
	}

	_classMutex.lock();
	if (_state == Stopping) {
		_state = Stopped;
	}
	_classMutex.unlock();
}


