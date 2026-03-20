#include "linked_list_generic.h"
#include <stdio.h>
#include <string.h>


struct LinkedList* ll_create(size_t value_size) {

    struct LinkedList* list = malloc(sizeof(struct LinkedList));

    list->first_ptr = NULL;
    list->last_ptr = NULL;
    list->value_size = value_size;

    return list;
}

void ll_free(struct LinkedList* list) {

    if(!list) {
        return;
    }

    struct LinkedListNode* current_node = list->first_ptr;
    while(current_node != NULL) {

        struct LinkedListNode* node_to_free = current_node;
        current_node = current_node->next_ptr;
        free(node_to_free);
    }
    free(list);
}

void ll_insert_first(struct LinkedList* list, void* value) {

    struct LinkedListNode* node = malloc(sizeof(struct LinkedListNode) + list->value_size);

    if(list->first_ptr == NULL){
        list->last_ptr = node;
        node->next_ptr = NULL;
    } else {
        node->next_ptr = list->first_ptr;
        list->first_ptr->prev_ptr = node;
    }

    memcpy(node + 1, value, list->value_size);

    node->prev_ptr = NULL;
    list->first_ptr = node;
}

void ll_insert_last(struct LinkedList* list, void* value){

    struct LinkedListNode* node = malloc(sizeof(struct LinkedListNode) + list->value_size);

    if(list->last_ptr == NULL){
        list->first_ptr = node;
        node->prev_ptr = NULL;
    } else {
        node->prev_ptr = list->last_ptr;
        list->last_ptr->next_ptr = node;
    }

    memcpy(node + 1, value, list->value_size);

    node->next_ptr = NULL;
    list->last_ptr = node;
}

void* ll_get(const struct LinkedList* list, size_t index) {

    struct LinkedListNode* node = list->first_ptr;

    for(int i = 0; i < index; i++) {

        if(!node) {
            return NULL;
        }
        node = node->next_ptr;
    }
    if(!node) {
        return NULL;
    }
    return node + 1;
}

/*void* ll_get_all(const struct LinkedList* list, size_t list_length) {

    void* array = malloc((sizeof(struct LinkedListNode) + list->value_size) * list_length);

    struct LinkedListNode* current_node = list->first_ptr;
    while(current_node != NULL) {

        current_node = current_node->next_ptr;
    }
}*/

void ll_remove_first(struct LinkedList* list){

    struct LinkedListNode* node_to_free = list->first_ptr;

    if(list->first_ptr == list->last_ptr) {
        list->last_ptr = NULL;
    } else {
        list->first_ptr->next_ptr->prev_ptr = NULL;
    }
    list->first_ptr = list->first_ptr->next_ptr;

    free(node_to_free);
}

void ll_remove_last(struct LinkedList* list){

    struct LinkedListNode* node_to_free = list->last_ptr;

    if(list->first_ptr == list->last_ptr) {
        list->first_ptr = NULL;
    } else {
        list->last_ptr->prev_ptr->next_ptr = NULL;
    }
    list->last_ptr = list->last_ptr->prev_ptr;

    free(node_to_free);
}

size_t ll_length(const struct LinkedList* list) {

    struct LinkedListNode* current_node = list->first_ptr;
    size_t count = 0;
    while(current_node != NULL) {

        count++;
        current_node = current_node->next_ptr;
    }
    return count;
}

LinkedListIterator ll_iterate(struct LinkedList* list) {
    LinkedListIterator iterator = {.next = list->first_ptr};
    return iterator;
}

void* ll_iterator_next(LinkedListIterator* iterator) {
    if(iterator->next == NULL) {
        return NULL;
    }
    void* next_value = iterator->next + 1;
    iterator->next = iterator->next->next_ptr;
    return next_value;
}

int ll_iterator_has_next(LinkedListIterator* iterator) {
    if(iterator->next) {
        return 1;
    } else {
        return 0;
    }
}
