#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "crc32.h"
#include "hashmap.h"

static int hashmap_expand(hashmap_map_t *map);
static int hashmap_put_internal(hashmap_map_t *map, hashmap_elem_t *elem);
static int hashmap_remove_internal(hashmap_map_t *map, hashmap_elem_t *elem);

#define NUM_PRIMES  (28)
static const unsigned long MAP_SIZE_PRIME_LIST[NUM_PRIMES] =
{
    53,        97,     193,        389,        769,
    1543,      3079,       6151,       12289,      24593,
    49157,     98317,      196613,     393241,     786433,
    1572869,   3145739,    6291469,    12582917,   25165843,
    50331653,  100663319,  201326611,  402653189,  805306457,
    1610612741,    3221225473ul,   4294967291ul
};

/**
 * Get the next nearby prime size
 */
unsigned long get_next_prime_size(unsigned long n)
{
    int low = 0, high = NUM_PRIMES - 1;
    int mid = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        if (n == MAP_SIZE_PRIME_LIST[mid])
        {
            return n;
        }
        else if (n < MAP_SIZE_PRIME_LIST[mid])
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    if (low == NUM_PRIMES)
    {
        return MAP_SIZE_PRIME_LIST[NUM_PRIMES - 1];
    }

    return MAP_SIZE_PRIME_LIST[low];
}

/**
 * Hashing function for a string
 */
int find_hash_index(unsigned long table_size, const char* keystring, unsigned long *index)
{
    if (0 == table_size || NULL == keystring || NULL == index)
        return HM_E_FAIL;

    unsigned long key = crc32(keystring, strlen(keystring));

    /* Robert Jenkins' 32 bit Mix Function */
    key += (key << 12);
    key ^= (key >> 22);
    key += (key << 4);
    key ^= (key >> 9);
    key += (key << 10);
    key ^= (key >> 2);
    key += (key << 7);
    key ^= (key >> 12);

    /* Knuth's Multiplicative Method */
    key = (key >> 3) * 0x9E3779B1;
    *index = key % table_size;

    return HM_S_OK;
}

/**
 * Create an empty hashmap
 */
int hashmap_init(hashmap_map_t *map, unsigned long size, hashmap_func_t func)
{
    if (NULL == map)
        return HM_E_FAIL;

    unsigned long init_size = get_next_prime_size(size);
    map->elems = (hashmap_elem_t**) calloc(init_size, sizeof(hashmap_elem_t*));
    if (NULL == map->elems)
        return HM_E_NOMEM;

    memset(map->elems, 0, init_size * sizeof(hashmap_elem_t*));

    map->table_size = init_size;
    map->size = 0;
    map->func = func;
    map->internal_pointer = NULL;

    return HM_S_OK;
}

/**
 * Put an elem into map
 */
static int hashmap_put_internal(hashmap_map_t *map, hashmap_elem_t *elem)
{
    if (NULL == map || NULL == elem)
        return HM_E_FAIL;

    unsigned long index = 0;
    elem->next = NULL;
    hashmap_elem_t *tmp = NULL;

    if (HM_S_OK != find_hash_index(map->table_size, elem->key, &index))
        return HM_E_FAIL;

    if (NULL != map->elems[index])
    {
        tmp = map->elems[index];
        while (NULL != tmp)
        {
            if (0 == strcmp(tmp->key, elem->key))
            {
                return HM_E_KEYUSED;
            }
            tmp = tmp->next;
        }

        elem->next = map->elems[index];
    }
    map->elems[index] = elem;
    map->internal_pointer = elem;

    map->size++;

    return HM_S_OK;
}

/**
 * Expand current map size
 */
static int hashmap_expand(hashmap_map_t *map)
{
    if (NULL == map)
        return HM_E_FAIL;

    unsigned long i = 0;
    unsigned long size = 0;
    hashmap_elem_t *tmp = NULL;
    unsigned long new_size = 0;         // New map size
    unsigned long table_size = 0;
    hashmap_elem_t **new_mem = NULL;    // New memory
    hashmap_elem_t **old_mem = NULL;    // Old memory
    unsigned long moved_number = 0;
    hashmap_elem_t *tmp_next = NULL;

    // Alloc new memory
    new_size = get_next_prime_size(map->table_size + 1);
    new_mem = (hashmap_elem_t**) malloc(new_size * sizeof(hashmap_elem_t*));
    if (NULL == new_mem)
        return HM_E_NOMEM;
    memset(new_mem, 0, new_size * sizeof(hashmap_elem_t*));

    // Reset the map info
    old_mem = map->elems;
    map->elems = new_mem;
    table_size = map->table_size;
    map->table_size = new_size;
    size = map->size;
    map->size = 0;

    // Move data from old memory to new memory
    for (i = 0; i < table_size; i++)
    {
        if (moved_number >= size)
            break;

        tmp = old_mem[i];
        while (NULL != tmp)
        {
            tmp_next = tmp->next;
            hashmap_put_internal(map, tmp);
            tmp = tmp_next;
            moved_number++;
        }
    }

    free(old_mem);

    return HM_S_OK;
}

/**
 * Create an elem
 */
hashmap_elem_t* hashmap_create_elem(const char* key, const void* data, unsigned int value_len)
{
    if (NULL == key || NULL == data)
        return NULL;

    hashmap_elem_t *elem = NULL;
    char* elem_key = NULL;
    void* elem_data = NULL;

    // Allocate elem memory
    elem = (hashmap_elem_t*) malloc(sizeof(hashmap_elem_t));
    if (NULL == elem)
        return NULL;

    // Allocate key memory
    elem_key = (char*) malloc(sizeof(char) * (strlen(key) + 1));
    if (NULL == elem_key)
    {
        free(elem);
        return NULL;
    }
    strcpy(elem_key, key);

    // Allocate data memory
    elem_data = (void*) malloc(value_len);
    if (NULL == elem_data)
    {
        free(elem);
        free(elem_key);
        return NULL;
    }
    memcpy(elem_data, data, value_len);

    // Set the elem
    elem->key = elem_key;
    elem->data = elem_data;
    elem->data_len = value_len;
    elem->next = NULL;

    return elem;
}

/**
 * Add a pair of key-value to the hashmap
 */
int hashmap_put(hashmap_map_t *map, const char* key, const void* data, unsigned int value_len)
{
    if (NULL == map || NULL == key || NULL == data)
        return HM_E_FAIL;

    int ret = HM_S_OK;
    hashmap_elem_t *elem = NULL;
    void *check_value = NULL;

    // When the map size is half of its table size, then expanding
    if (map->size >= map->table_size / 2)
        hashmap_expand(map);

    // Check whether key in
    ret = hashmap_get(map, key, &check_value, NULL);
    if (HM_S_OK == ret)
        return HM_E_KEYUSED;

    // Create the elem
    elem = hashmap_create_elem(key, data, value_len);
    if (NULL == elem)
        return HM_E_FAIL;

    // Put the elem
    ret = hashmap_put_internal(map, elem);

    return ret;
}

/**
 * Put or Update key,value in the map
 */
int hashmap_put_or_update(hashmap_map_t *map, const char* key, const void* data, unsigned int value_len)
{
    if (NULL == map || NULL == key || NULL == data)
        return HM_E_FAIL;

    int ret = 0;
    void *data_tmp = NULL;

    // Check whether the key is in map or not
    ret = hashmap_get(map, key, &data_tmp, NULL);
    if (HM_S_OK == ret)
    {
        ret = hashmap_remove(map, key);
        if (HM_S_OK != ret)
            return ret;
    }

    ret = hashmap_put(map, key, data, value_len);

    return ret;
}

/**
 * Reset the inter pointer for map traversal
 */
int hashmap_internal_pointer_reset(hashmap_map_t *map)
{
    if (NULL == map)
        return HM_E_FAIL;

    unsigned long i = 0;

    if (0 == map->size)
    {
        map->internal_pointer = NULL;
        return HM_S_OK;
    }

    for (i = 0; i < map->table_size; i++)
    {
        if (NULL != map->elems[i])
        {
            map->internal_pointer = map->elems[i];
            return HM_S_OK;
        }
    }

    map->internal_pointer = NULL;
    return HM_S_OK;
}

/**
 * Check whether map has more elements for map traversal
 */
int hashmap_has_more_elems(hashmap_map_t *map)
{
    if (NULL == map || NULL == map->internal_pointer)
        return HM_E_FAIL;

    return HM_S_OK;
}

/**
 * Move inter pointer forward for map traversal
 */
int hashmap_move_forward(hashmap_map_t *map)
{
    if (NULL == map || NULL == map->internal_pointer)
        return HM_E_FAIL;

    unsigned long i = 0;
    hashmap_elem_t *tmp = NULL;

    // Next elem of current elem Not NULL
    tmp = map->internal_pointer;
    if (NULL != tmp->next)
    {
        map->internal_pointer = tmp->next;
        return HM_S_OK;
    }

    if (HM_S_OK != find_hash_index(map->table_size, tmp->key, &i))
        return HM_E_FAIL;

    for (i = i + 1; i < map->table_size; i++)
    {
        if (NULL != map->elems[i])
        {
            map->internal_pointer = map->elems[i];
            return HM_S_OK;
        }
    }

    map->internal_pointer = NULL;
    return HM_S_OK;
}

/**
 * Get current key according to map traversal
 */
int hashmap_get_current_key(hashmap_map_t *map, char **key)
{
    if (NULL == map || NULL == key || NULL == map->internal_pointer)
        return HM_E_FAIL;

    *key = map->internal_pointer->key;
    return HM_S_OK;
}

/**
 * Get current data according to map traversal
 */
int hashmap_get_current_data(hashmap_map_t *map, void **data, size_t *data_len)
{
    if (NULL == map || NULL == data || NULL == map->internal_pointer)
        return HM_E_FAIL;

    *data = map->internal_pointer->data;
    if (NULL != data_len)
        *data_len = map->internal_pointer->data_len;

    return HM_S_OK;
}

int hashmap_get(hashmap_map_t *map, const char* key, void **data, size_t *data_len)
{
    if (NULL == map || NULL == key || NULL == data)
        return HM_E_FAIL;

    unsigned long i = 0;
    hashmap_elem_t *tmp = NULL;

    if (HM_S_OK != find_hash_index(map->table_size, key, &i))
        return HM_E_FAIL;

    if (NULL == map->elems[i])
        return HM_E_NOTFOUND;

    tmp = map->elems[i];
    while (NULL != tmp)
    {
        if (0 == strcmp(tmp->key, key))
        {
            *data = tmp->data;
            if (NULL != data_len)
                *data_len = tmp->data_len;

            return HM_S_OK;
        }
        tmp = tmp->next;
    }

    return HM_E_NOTFOUND;
}

/**
 * Remove the element from map
 */
static int hashmap_remove_internal(hashmap_map_t *map, hashmap_elem_t *elem)
{
    if (NULL == map || NULL == elem)
        return HM_E_FAIL;

    // Deal with the data with user's function
    if (NULL != map->func)
        map->func(elem->data);

    // Set the inter pointer
    if (map->internal_pointer == elem)
        map->internal_pointer = NULL;

    // Free the malloc memory
    free(elem->key);
    free(elem->data);
    free(elem);

    map->size--;

    return HM_S_OK;
}

/**
 * Remove the element from the map by key
 */
int hashmap_remove(hashmap_map_t *map, const char* key)
{
    if (NULL == map || NULL == key)
        return HM_E_FAIL;

    unsigned long i = 0;
    hashmap_elem_t *tmp = NULL;
    hashmap_elem_t *tmp_pre = NULL;

    if (HM_S_OK != find_hash_index(map->table_size, key, &i))
        return HM_E_FAIL;

    if (NULL == map->elems[i])
        return HM_E_NOTFOUND;

    tmp = map->elems[i];
    while (NULL != tmp)
    {
        if (0 == strcmp(tmp->key, key))
        {
            // Set the list
            if (tmp == map->elems[i])
            {
                map->elems[i] = tmp->next;
            }
            else
            {
                tmp_pre->next = tmp->next;
            }

            return hashmap_remove_internal(map, tmp);
        }
        tmp_pre = tmp;
        tmp = tmp->next;
    }

    return HM_E_NOTFOUND;
}

/**
 * Destroy the map
 */
int hashmap_destroy(hashmap_map_t *map)
{
    if (NULL == map)
        return HM_E_FAIL;

    unsigned long i = 0;
    hashmap_elem_t *tmp = NULL;
    hashmap_elem_t *tmp_next = NULL;

    for (i = 0; i < map->table_size && map->size > 0; i++)
    {
        tmp = map->elems[i];
        while (NULL != tmp)
        {
            tmp_next = tmp->next;
            hashmap_remove_internal(map, tmp);
            tmp = tmp_next;
        }
    }
    free(map->elems);
    map->elems = NULL;
    map->table_size = 0;
    map->func = NULL;

    return HM_S_OK;
}

/**
 * Return the size of current map
 */
unsigned long hashmap_size(hashmap_map_t *map)
{
    if (NULL == map)
        return 0;

    return map->size;
}

#ifdef __HASHMAP_MAIN_TEST__

int main(int argc, char *argv[])
{
    int i = 0;
    hashmap_map_t map;
    int ret = HM_S_OK;
    char *ret_key = NULL;
    char *ret_value = NULL;
    size_t ret_value_len = 0;
    char key_buf[1024] = {0};
    char value_buf[1024] = {0};
    const char *key = "first key";
    const char *value = "first value";

    ret = hashmap_init(&map, 0, NULL);
    assert(HM_S_OK == ret);

    // insert value
    ret = hashmap_put(&map, key, value, strlen(value) + 1);
    assert(HM_S_OK == ret);

    ret = hashmap_get(&map, key, (void*)&ret_value, &ret_value_len);
    assert(HM_S_OK == ret);
    assert(0 == memcmp(value, ret_value, ret_value_len));
    printf("key:%s; value:%s\n", key, value);

    // insert again
    value = "first value new";
    ret = hashmap_put(&map, key, value, strlen(value) + 1);
    assert(HM_E_KEYUSED == ret);
    
    // insert or update
    ret = hashmap_put_or_update(&map, key, value, strlen(value) + 1);
    assert(HM_S_OK == ret);

    ret = hashmap_get(&map, key, (void*)&ret_value, &ret_value_len);
    assert(HM_S_OK == ret);
    assert(0 == memcmp(value, ret_value, ret_value_len));
    printf("key:%s; value:%s\n", key, value);

    // remove 
    ret = hashmap_remove(&map, key);
    assert(HM_S_OK == ret);
    ret = hashmap_remove(&map, key);
    assert(HM_E_NOTFOUND == ret);

    // iterator the map
    for (i = 0; i < 10; i++)
    {
        snprintf(key_buf, sizeof(key_buf), "%d key", i);
        ret = snprintf(value_buf, sizeof(key_buf), "%d value", i);
        ret = hashmap_put(&map, key_buf, value_buf, ret + 1);
        assert(HM_S_OK == ret);
    }
    assert(hashmap_size(&map) == 10);
    printf("\nmap size:%ld\n", hashmap_size(&map));

    for (hashmap_internal_pointer_reset(&map);
            HM_S_OK == hashmap_has_more_elems(&map);
            hashmap_move_forward(&map))
    {
        ret = hashmap_get_current_key(&map, &ret_key);
        assert(HM_S_OK == ret);
        ret = hashmap_get_current_data(&map, (void**)&ret_value, &ret_value_len);
        printf("%s => %s\n", ret_key, ret_value);
    }

    // destroy map
    ret = hashmap_destroy(&map);
    assert(ret == HM_S_OK);

    return 0;
}

#endif
