/*************************************************************************
	> File Name: animal.c
	> Author: 
	> Mail: 
	> Created Time: Thu 24 Mar 2016 11:54:06 PM PDT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "animal.h"

animal_t *animal_init(char *name)
{
    assert(name != NULL); 
    size_t name_len = strlen(name);

    animal_t *animal = (animal_t*)malloc(sizeof(animal_t)+sizeof(animal_ops_t) + name_len + 1);

    memset(animal, 0, (sizeof(animal_t)+sizeof(animal_ops_t)+name_len+1));

    animal->name = (char *)animal + sizeof(animal_t);
    memcpy(animal, name, name_len);
    animal->animal_ops = (animal_ops_t *)((char *)animal+ sizeof(animal_t) + name_len + 1);

    return animal;
}

/* 基类的有关操作，如吃，走，说等等 */
void animal_eat(animal_t *animal, char *food)
{
    animal->animal_ops->eat(food);
    return;
}

void animal_walk(animal_t *animal, int steps)
{
    animal->animal_ops->walk(steps);
    return;
}

void animal_talk(animal_t *animal, char *msg)
{
    animal->animal_ops->talk(msg);
    return;
}

/* 基类的析构函数，需要显示调用 */
void animal_die(animal_t *animal)
{
    assert(animal != NULL);

    free(animal);
    return;
}
