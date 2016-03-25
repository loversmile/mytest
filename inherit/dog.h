/*************************************************************************
	> File Name: dog.h
	> Author: 
	> Mail: 
	> Created Time: Fri 25 Mar 2016 12:06:55 AM PDT
 ************************************************************************/

#ifndef _DOG_H
#define _DOG_H

#include "animal.h"

typedef struct dog_s_ dog_t;

struct dog_s_ {
    animal_t base; /* 继承自 animal 基类 */

    /* 以下还可以添加与 dog 相关的属性和方法(函数指针), 如: */
    /* char *owner; // dog 的主人 */
    /* void (*hunt)(const char *rabbit); // 猎兔犬 */
};

extern dog_t * dog_init();
extern void dog_die(dog_t * dog);

#endif
