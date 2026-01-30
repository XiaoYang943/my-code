#include <iostream>

class MySingleton {

 public:
    MySingleton(const MySingleton&)= delete;
    MySingleton& operator = (const MySingleton&)= delete;

    static MySingleton* getInstance() {
      if ( !instance ){
        instance= new MySingleton();
      }
      return instance;
    }
    
 private:
    static MySingleton* instance;
    MySingleton()= default;
    ~MySingleton()= default;
};

MySingleton* MySingleton::instance= nullptr;

/*
 * 单例就是全局变量，应该尽可能避免单例
 * 问题：
 * 谁来负责单例的销毁？
 * 是否应该允许从单例派生？
 * 如何以线程安全的方式初始化单例？
 * 静态初始化顺序问题？
 */
int main() {

  std::cout << MySingleton::getInstance() << "\n";
  std::cout << MySingleton::getInstance() << "\n";

}
