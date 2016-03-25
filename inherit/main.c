/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Fri 25 Mar 2016 12:10:16 AM PDT
 ************************************************************************/


#include <stdio.h>

#include "animal.h"
#include "dog.h"
//#include "cat.h"

int main(int argc, const char *argv[])
{
    dog_t *dog = dog_init();

    /* dog 类测试 */
    animal_eat(dog, "bones");
    animal_walk(dog, 5);
    animal_talk(dog, "wuang wuang wuang...");

    /* cat 类测试 */
  //  animal_eat(cat, "fish");
  //  animal_walk(cat, 3);
  //  animal_talk(cat, "miao miao miao...");


}
