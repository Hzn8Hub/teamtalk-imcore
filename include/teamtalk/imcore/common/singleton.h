/**
 * @author: luochenhao
 * @email: lch2022fox@163.com
 * @time: Fri 01 May 2026 21:08:47 CST
 * @brief: 单例模式模板类Singleton（非线程安全）
 * @note: 该模板类只能用于非线程环境，不支持多线程环境下的单例模式
 */

#ifndef TEAMTALK_IMCORE_COMMON_SINGLETON_H_
#define TEAMTALK_IMCORE_COMMON_SINGLETON_H_

namespace teamtalk::imcore::common {

template <typename T>
class Singleton {
 public:
  static T& Instance() {
    if (Singleton::s_instance == NULL)
      Singleton::s_instance = CreateInstance();
    return *(Singleton::s_instance);
  }
  static T* GetInstance() {
    if (Singleton::s_instance == NULL)
      Singleton::s_instance = CreateInstance();
    return Singleton::s_instance;
  }
  static T* getInstance() {
    if (Singleton::s_instance == NULL)
      Singleton::s_instance = CreateInstance();
    return Singleton::s_instance;
  }
  static void Destroy() {
    if (Singleton::s_instance != NULL) {
      DestroyInstance(Singleton::s_instance);
      Singleton::s_instance = 0;
    }
  }

 protected:
  Singleton() { Singleton::s_instance = static_cast<T*>(this); }
  ~Singleton() { Singleton::s_instance = NULL; }

 private:
  static T* CreateInstance() { return new T(); }
  static void DestroyInstance(T* p) { delete p; }

  static T* s_instance;

 private:
  // 默认禁止拷贝和赋值操作
  explicit Singleton(Singleton const&) {}
  Singleton& operator=(Singleton const&) { return *this; }
};

template <typename T>
T* Singleton<T>::s_instance = NULL;

}  // namespace teamtalk::imcore::common

#endif  // TEAMTALK_IMCORE_COMMON_SINGLETON_H_
