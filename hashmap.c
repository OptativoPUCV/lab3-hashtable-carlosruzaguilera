#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (map == NULL || key == NULL) return;

    long idx = hash(key, map->capacity);
    while (map->buckets[idx] != NULL && map->buckets[idx]->key != NULL) {
        if (strcmp(map->buckets[idx]->key, key) == 0) return;
        idx = (idx + 1) % map->capacity; // Resolución de colisiones
    }

    Pair * newPair = (Pair *) malloc(sizeof(Pair));
    newPair->key = strdup(key);
    newPair->value = value;

    map->buckets[idx] = newPair;
    map->current = idx;
    map->size++;
}

void enlarge(HashMap * map) {
    if (map == NULL) return;

    long oldCapacity = map->capacity;
    Pair ** oldBuckets = map->buckets;

    map->capacity *= 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));
    map->size = 0;

    for (long i = 0; i < oldCapacity; i++) {
        if (oldBuckets[i] != NULL && oldBuckets[i]->key != NULL) {
            insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
            free(oldBuckets[i]->key);
            free(oldBuckets[i]);
        }
    }

    free(oldBuckets);
}



HashMap * createMap(long capacity) {
    HashMap * newMap = (HashMap *) malloc(sizeof(HashMap));
    if (newMap == NULL) return NULL;

    newMap->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    if (newMap->buckets == NULL) {
        free(newMap);
        return NULL;
    }

    newMap->capacity = capacity;
    newMap->current = -1;
    newMap->size = 0;

    return newMap;
}

void eraseMap(HashMap * map, char * key) {
    Pair * pair = searchMap(map, key);
    if (pair != NULL) {
        free(pair->key);
        pair->key = NULL;
        map->size--;
    }
}



Pair * searchMap(HashMap * map, char * key) {
    if (map == NULL || key == NULL) return NULL;

    long idx = hash(key, map->capacity);
    while (map->buckets[idx] != NULL) {
        if (map->buckets[idx]->key != NULL && strcmp(map->buckets[idx]->key, key) == 0) {
            map->current = idx;
            return map->buckets[idx];
        }
        idx = (idx + 1) % map->capacity;
    }

    return NULL;
}


Pair * firstMap(HashMap * map) {
    if (map == NULL) return NULL;

    for (long i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }

    return NULL;
}


Pair * nextMap(HashMap * map) {
  if (map == NULL) return NULL;

  for (long i = map->current + 1  < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i]
        
    }

    return NULL;
}
