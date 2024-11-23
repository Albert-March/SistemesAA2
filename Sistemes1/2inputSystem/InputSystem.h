#pragma once
#include "../Utils/InputsConsts.h"
#include <functional>
#include <thread>
#include <mutex>
#include <map>
#include <list>

class InputSystem
{
public:

	class KeyBinding {
		friend class InputSystem; //el input system podrá crear y destruir keyBindings
	public:
		typedef std::function<void()> OnKeyPress; //Funció landa
	private:
		int _key;       
		OnKeyPress _onKeyPress;

		KeyBinding(int key, OnKeyPress onKeyPress);
		~KeyBinding();
	};

	typedef std::list<KeyBinding*> KeyBindingList;
	typedef std::map<int, KeyBindingList> KeyBindingMap;

public:
	InputSystem();
	~InputSystem();

	KeyBinding* AddListener(int key, KeyBinding::OnKeyPress onKeyPress);
	void RemoveAndDeleteListener(KeyBinding* keyBinding);

	void AddListenerAsync(int key, KeyBinding::OnKeyPress onKeyPress);
	void RemoveAndDeleteListenerAsync(KeyBinding* keyBinding);

	void StartListen();
	void StopListen();

private:

	std::mutex _classMutex;
	KeyBindingMap _keyBindingsMap;
	
	enum State {
		Starting = 0,
		Listening = 0,
		Stopping = 2,
		Stopped = 3
	};

	State _state = Stopped;

	void ListenLoop();
};

