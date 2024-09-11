#pragma once
#include <iostream>
#include <mutex>

// ����ʽ
class SingleInstance {
public:
	SingleInstance(const SingleInstance&) = delete;

	SingleInstance& operator=(const SingleInstance&) = delete;

	static SingleInstance* getInstance() { return instance; }

private:
	SingleInstance() = default;
	static SingleInstance* instance;
};
SingleInstance* SingleInstance::instance = new SingleInstance();


#if 0
// ����ʽ
class LazySingleInstance {
public:
	LazySingleInstance(const LazySingleInstance&) = delete;

	LazySingleInstance& operator=(const LazySingleInstance&) = delete;

	static LazySingleInstance* getInstance() 
	{
		if (instance == nullptr) {
			// �ڳ�ʼ������ʱ������ж���߳��ߵ���һ��
			std::lock_guard<std::mutex> lock(mtx);
			if (instance == nullptr) {
				instance = new LazySingleInstance();
			}
		}
		return instance;
	}

private:
	LazySingleInstance() = default;
	static std::mutex mtx;
	static LazySingleInstance* instance;
};
LazySingleInstance* LazySingleInstance::instance = nullptr;
std::mutex LazySingleInstance::mtx;

#endif

#if 0
// ��������call_onceʵ��
class LazySingleInstance {
public:
	LazySingleInstance(const LazySingleInstance&) = delete;

	LazySingleInstance& operator=(const LazySingleInstance&) = delete;

	static LazySingleInstance* getInstance() 
	{
		std::call_once(initFlag, []() {
			instance = new LazySingleInstance();
			});
		return instance;
	}
private:
	LazySingleInstance() = default;
	static LazySingleInstance* instance;
	static std::once_flag initFlag;
};
LazySingleInstance* LazySingleInstance::instance = nullptr;
std::once_flag LazySingleInstance::initFlag;

#endif

// ͨ��C++11ʵ�ֵ�
class LazySingleInstance {
public:
	LazySingleInstance(const LazySingleInstance&) = delete;

	LazySingleInstance& operator=(const LazySingleInstance&) = delete;

	static LazySingleInstance* getInstance()
	{
		static LazySingleInstance* instance = new LazySingleInstance();
		return instance;
	}
private:
	LazySingleInstance() = default;
};