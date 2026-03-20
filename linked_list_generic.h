#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

//TODO:Make a game of snake, make struct for vector

struct LinkedListNode {
    struct LinkedListNode* next_ptr;
    struct LinkedListNode* prev_ptr;
};

struct LinkedList {
    size_t value_size;
    struct LinkedListNode* first_ptr;
    struct LinkedListNode* last_ptr;
};

typedef struct {
    struct LinkedListNode* next;
} LinkedListIterator;

struct LinkedList* ll_create(size_t value_size);

void ll_free(struct LinkedList* list);

void ll_insert_first(struct LinkedList* list, void* value);

void ll_insert_last(struct LinkedList* list, void* value);

void* ll_get(const struct LinkedList* list, size_t index);

//void* ll_get_all(const struct LinkedList* list, size_t index);

void ll_remove_first(struct LinkedList* list);

void ll_remove_last(struct LinkedList* list);

size_t ll_length(const struct LinkedList* list);

LinkedListIterator ll_iterate(struct LinkedList* list);

void* ll_iterator_next(LinkedListIterator* iterator);

int ll_iterator_has_next(LinkedListIterator* iterator);
