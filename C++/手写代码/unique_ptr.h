#pragma once
#include <iostream>

template <typename T>
class UniquePtr {
public:
	explicit UniquePtr(T* ptr = nullptr):ptr_(ptr) {}

	UniquePtr& operator=(const UniquePtr&) = delete;
	UniquePtr(const UniquePtr&) = delete;

	// 允许移动构造和移动拷贝
	UniquePtr(UniquePtr&& other) noexcept;
	UniquePtr& operator=(UniquePtr&& other) noexcept;

	void reset(T* ptr = nullptr);

	T* get() const { return ptr_; }

	void release();

	T& operator*() const;

	T* operator->() const;

	explicit operator bool() const { return ptr_ != nullptr; }

	~UniquePtr();

private:
	T* ptr_;
};

template<typename T>
inline UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept: ptr_(other.ptr_)
{
	other.ptr_ = nullptr;
}

template<typename T>
inline UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept
{
	if (this != &other) {
		delete ptr_;
		ptr_ = other.ptr_;
		other.ptr_ = nullptr;
	}
	return *this;
}

template<typename T>
inline void UniquePtr<T>::reset(T* ptr)
{
	delete ptr_;
	ptr_ = ptr;
}

template<typename T>
inline void UniquePtr<T>::release()
{
	T* temp = ptr_;
	ptr_ = nullptr;
	return temp;
}

template<typename T>
inline T& UniquePtr<T>::operator*() const
{
	return *ptr_;
}

template<typename T>
inline T* UniquePtr<T>::operator->() const
{
	return ptr_:
}

template<typename T>
inline UniquePtr<T>::~UniquePtr()
{
	delete ptr_;
	ptr_ = nullptr;
}
