/**********calse***************************************************************
	> File Name: base.cpp
	> Author: jklou
	> Mail: 
	> Created Time: 2015年11月10日 星期二 09时18分15秒
 ************************************************************************/

#include <iostream>
using namespace std;

class Base{
public:
    Base(){
        cout<<"Base construct" << endl;
    }
    virtual void func(){
        cout << "Base func" << endl;
    }
    ~Base(){
        cout << "Base destruct" << endl;
    }
};

class Drive: public Base {
public:
    Drive(){
        cout << "Drive construct" << endl;
    }
    ~Drive(){
        cout << "Drive destruct" << endl;
    }
    void func(){
        cout << "Dirve func" << endl;
    }
};

int main()
{
    Base *p1, *p2;
    p1 = new Base();
    p2 = new Drive();
    Drive *p3;
    p3 = new Drive();

    p2->func();
    delete p1;
    delete p2;
    delete p3;
}
