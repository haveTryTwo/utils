HashMap
=======

hashmap is a map library for C designed to store KEY/VALUE, and it contains:

* VALUE can be binary
* Space wil be increased automatically
* Custom function that if is defined will be called after a key removed 
* Iteration functions like PHP map can be used


```c
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
```
