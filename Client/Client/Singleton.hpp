#pragma once

template <typename T>
class Singleton
{
public:
	static void Initialize(void);
	static void Deinitialize(void);

	static T&	GetInstance(void);
	static T*	GetIntancePtr(void);

private:
	static T*	m_pInstance;
};

template <typename T>
T* Singleton<T>::m_pInstance = nullptr;

template <typename T>
inline void Singleton<T>::Initialize()
{
	m_pInstance = new T();
}

template< typename T>
inline void Singleton<T>::Deinitialize()
{
	delete m_pInstance;
}

template <typename T>
inline T& Singleton<T>::GetInstance()
{
	return *m_pInstance;
}

template <typename T>
inline T* Singleton<T>::GetIntancePtr()
{
	return m_pInstance;
}
