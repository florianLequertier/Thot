#pragma once

template<typename T>
class Singleton
{
public:
	static T& Instance() {
		static T _instance;
		return _instance;
	}

protected:
	Singleton() {}
public:
	Singleton(const Singleton<T>& other) = delete;
	void operator=(const Singleton<T>& other) = delete;
};