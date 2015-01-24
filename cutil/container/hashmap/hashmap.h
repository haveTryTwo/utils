#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * define RETURN STATUS
 */
#define HM_E_NOMEM    (-6)         // Malloc Memory failed, NO MEM
#define HM_E_KEYUSED  (-5)         // Key already existed
#define HM_E_OUTMEM   (-4)         // Out of Memory
#define HM_E_NOTFOUND (-3)         // No such element
#define HM_E_OVERFLOW (-2)         // Hashmap is full
#define HM_E_FAIL     (-1)         // Hashmap api fail
#define HM_S_OK       (0)          // Success 

/**
 * hashmap_func_t is a pointer to a function that can take one void_ptr arguments
 * and return an integer. Returns status code.
 */
typedef int (*hashmap_func_t)(void*);

/*
 * A element to keep keys and values
 */
typedef struct _hashmap_elem_t
{
    char*         key;            // pointer to actual key storage
    void*         data;           // pointer to actual data storage
    size_t        data_len;       // data length
    struct _hashmap_elem_t *next;
} hashmap_elem_t;

/*
 * A hashmap has maximum size and current size, as well as the elems to hold
 */
typedef struct _hashmap_map_t
{
    unsigned long table_size;
    unsigned long size;
    hashmap_func_t func;
    hashmap_elem_t *internal_pointer;
    hashmap_elem_t **elems;
} hashmap_map_t;

/**
 * Get the next nearby prime size
 */
extern unsigned long get_next_prime_size(unsigned long n);

/**
 * Initialize the hashmap
 */
extern int hashmap_init(hashmap_map_t *map, unsigned long size, hashmap_func_t func);

/**
 * get the index of key
 */
extern int find_hash_index(unsigned long table_size, const char* keystring, unsigned long *index);

/**
 * Create an elem
 */
extern hashmap_elem_t* hashmap_create_elem(const char* key, const void* data, unsigned int value_len);

/**
* Add an element to the hashmap.
*/
extern int hashmap_put(hashmap_map_t *map, const char* key, const void* data, unsigned int value_len);

/**
 * Put or Update key,value in the map
 */
int hashmap_put_or_update(hashmap_map_t *map, const char* key, const void* data, unsigned int value_len);

/**
 * Reset the inter pointer for map traversal
 */
extern int hashmap_internal_pointer_reset(hashmap_map_t *map);

/**
 * Check whether map has more elements for map traversal
 */
extern int hashmap_has_more_elems(hashmap_map_t *map);

/**
 * Move inter pointer forward for map traversal
 */
extern int hashmap_move_forward(hashmap_map_t *map);

/**
 * Get current key according to map traversal
 */
extern int hashmap_get_current_key(hashmap_map_t *map, char **key);

/**
 * Get current data according to map traversal
 */
extern int hashmap_get_current_data(hashmap_map_t *map, void **data, size_t *data_len);

/**
 * Get the data from map by key
 */
extern int hashmap_get(hashmap_map_t *map, const char* key, void **data, size_t *data_len);

/**
 * Remove an element from map by key
 */
extern int hashmap_remove(hashmap_map_t *map, const char* key);

/**
 * Destroy the map
 */
extern int hashmap_destroy(hashmap_map_t *map);

/**
 * Get the size of current map
 */
unsigned long hashmap_size(hashmap_map_t *map);

#if defined(__cplusplus)
}
#endif

#endif /* __HASHMAP_H__ */
