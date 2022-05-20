#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"

#define NAME_SIZE   20
#define TABLE_LEN   3

typedef struct integer_key_st
{
    int id;
    char name[NAME_SIZE];
    UT_hash_handle hh;
}integer_key_st;

static void add_user(integer_key_st **users, int user_id, const char *user_name)
{
    integer_key_st *user = NULL;
    HASH_FIND_INT(*users, &user_id, user);
    if (user == NULL)
    {
        user = malloc(sizeof(integer_key_st));
        if (user == NULL)
            exit(-1);
        user->id = user_id;
        HASH_ADD_INT(*users, id, user);
    }
    strcpy(user->name, user_name);
    return;
}

static void find_user(integer_key_st *users, int user_id, integer_key_st **user)
{
    HASH_FIND_INT(users, &user_id, *user);
    return;
}

static void delete_user(integer_key_st **users, integer_key_st *del_user)
{
    HASH_DEL(*users, del_user);
    free(del_user);
    return;
}

static void show_user(integer_key_st *users)
{
    integer_key_st *s;
    for (s = users; s != NULL; s = s->hh.next)
    {
        printf("id is %d, name is %s.\n", s->id, s->name);
    }
    return;
}

static void delete_all(integer_key_st **users)
{
    integer_key_st *current, *tmp;
    HASH_ITER(hh, *users, current, tmp)
    {
        HASH_DEL(*users, current);
        free(current);
    }
    return;
}

static int by_name(const integer_key_st *a, const integer_key_st *b)
{
    return strcmp(a->name, b->name);
}

int main(int argc, char const *argv[])
{
    integer_key_st *users = NULL;
    const char *name_arr[] = {"Alice", "Bob", "Trudy"};

    for (int i = 0; i < TABLE_LEN; i++)
    {
        add_user(&users, i, name_arr[i]);
    }

    integer_key_st *find_rel = NULL;
    find_user(users, 1, &find_rel);
    printf("find user: id is %d, name is %s.\n", find_rel->id, find_rel->name);

    show_user(users);

    unsigned int count =  HASH_COUNT(users);
    printf("The hash table has %u items.\n", count);

    integer_key_st *add = malloc(sizeof *add);
    if (add == NULL)
        exit(-1);
    add->id = 1;
    strcpy(add->name, "Spencer");
    HASH_REPLACE_INT(users, id, add, find_rel);
    delete_user(&users, find_rel);

    show_user(users);

    count =  HASH_COUNT(users);
    printf("The hash table has %u items.\n", count);

    printf("sorting......\n");

    HASH_SORT(users, by_name);

    show_user(users);

    delete_all(&users);

    exit(0);
}
