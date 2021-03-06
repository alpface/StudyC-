//
//  02单例模式-饿汉式.cpp
//  04
//
//  Created by xiaoyuan on 2018/11/21.
//  Copyright © 2018 xiaoyuan. All rights reserved.
//

/*
 一般情况下，我们建立的一些类是属于工具性质的，基本不用存储太多的跟自身有关的数据，在这种情况下，每次都去new一个对象，即增加了开销，也使得代码更加臃肿。其实，我们只需要一个实例对象就可以。如果采用全局或者静态变量的方式，会影响封装性，难以保证别的代码不会对全局变量造成影响。
 考虑到这些需要，我们将默认的构造函数声明为私有的，这样就不会被外部所new了，甚至可以将析构函数也声明为私有的，这样就只有自己能够删除自己了。在Java和C#这样纯的面向对象的语言中，单例模式非常好实现，直接就可以在静态区初始化instance，然后通过getInstance返回，这种就被称为饿汉式单例类。也有些写法是在getInstance中new instance然后返回，这种就被称为懒汉式单例类，但这涉及到第一次getInstance的一个判断问题。

 */

#include <iostream>
#include <mutex>

using namespace std;

class Singleton {
public:
    
    /*
    static Singleton* getInstance()
     {
     if (instance == NULL)
     instance = new Singleton();
     
     return instance;
     }
     */
    // 这样就可以了，保证只取得了一个实例。但是在多线程的环境下却不行了，因为很可能两个线程同时运行到if (instance == NULL)这一句，导致可能会产生两个实例。于是就要在代码中加锁。
    /*
    
       static Singleton* getInstance()
    {
        lock();
        if (instance == NULL)
        {
            instance = new Singleton();
        }
        unlock();
        
        return instance;
    }
    */
    // 但这样写的话，会稍稍映像性能，因为每次判断是否为空都需要被锁定，如果有很多线程的话，就爱会造成大量线程的阻塞。于是大神们又想出了双重锁定。
    // 这样只够极低的几率下，通过越过了if (instance == NULL)的线程才会有进入锁定临界区的可能性，这种几率还是比较低的，不会阻塞太多的线程，但为了防止一个线程进入临界区创建实例，另外的线程也进去临界区创建实例，又加上了一道防御if (instance == NULL)，这样就确保不会重复创建了。
    static Singleton *getInstance()
    {
        static Singleton *instance;
        if (NULL != instance) {
            return instance;
        }
        std::mutex mt;
        mt.lock(); // 加锁
        if (NULL == instance) {
            instance = new Singleton();
        }
        mt.unlock(); // 释放锁
        return instance;
    }
private:
    Singleton()
    {
        
    }
    // 把拷贝构造函数和=操作符也设置为私有，防止被复制
    Singleton(const Singleton &s)
    {
        
    }
    
};

void test()
{
    Singleton *s1 = Singleton::getInstance();
    Singleton *s2 = Singleton::getInstance();
    Singleton *s3 = Singleton::getInstance();
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
}

int main(int argc, const char *argv[])
{
    test();
    return EXIT_SUCCESS;
}
