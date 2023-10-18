#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct array {
  int *data;
  size_t capacity;
  size_t size;
};

/*
 * Create an empty array
 */
void array_create(struct array *self);

/*
 * Create an array with initial content
 */
void array_create_from(struct array *self, const int *other, size_t size);

/*
 * Destroy an array
 */
void array_destroy(struct array *self);

/*
 * Tell if the array is empty
 */
bool array_empty(const struct array *self);

/*
 * Get the size of the array
 */
size_t array_size(const struct array *self);

/*
 * Compare the array to another array (content and size)
 */
bool array_equals(const struct array *self, const int *content, size_t size);

/*
 * Add an element at the end of the array
 */
void array_push_back(struct array *self, int value);

/*
 * Remove the element at the end of the array
 */
void array_pop_back(struct array *self);

/*
 * Insert an element in the array (preserving the order)
 */
void array_insert(struct array *self, int value, size_t index);

/*
 * Remove an element in the array (preserving the order)
 */
void array_remove(struct array *self, size_t index);

/*
 * Get an element at the specified index in the array, or 0 if the index is not valid
 */
int array_get(const struct array *self, size_t index);

/*
 * Set an element at the specified index in the array to a new value, or do nothing if the index is not valid
 */
void array_set(struct array *self, size_t index, int value);

/*
 * Search for an element in the array.
 */
size_t array_search(const struct array *self, int value);

/*
 * Search for an element in the sorted array.
 */
size_t array_search_sorted(const struct array *self, int value);

/*
 * Tell if the array is sorted
 */
bool array_is_sorted(const struct array *self);

/*
 * Make a partition of the array between i and j (inclusive) and returns the index of the pivot
 */
ptrdiff_t array_partition(struct array *self, ptrdiff_t i, ptrdiff_t j);

/*
 * Sort the array with quick sort
 */
void array_quick_sort(struct array *self);

/*
 * Sort the array with heap sort
 */
void array_heap_sort(struct array *self);

/*
 * Tell if the array is a heap
 */
bool array_is_heap(const struct array *self);

/*
 * Add a value into the array considered as a heap
 */
void array_heap_add(struct array *self, int value);

/*
 * Get the value at the top of the heap
 */
int array_heap_top(const struct array *self);

/*
 * Remove the top value in the array considered as a heap
 */
void array_heap_remove_top(struct array *self);



struct list_node {
  int data;
  struct list_node *next;
  struct list_node *prev;
};

struct list {
  struct list_node *first;
  struct list_node *last;
};

/*
 * Create an empty list
 */
void list_create(struct list *self);

/*
 * Create a list with initial content
 */
void list_create_from(struct list *self, const int *other, size_t size);

/*
 * Destroy a list
 */
void list_destroy(struct list *self);

/*
 * Tell if the list is empty
 */
bool list_empty(const struct list *self);

/*
 * Get the size of the list
 */
size_t list_size(const struct list *self);

/*
 * Compare the list to an array (data and size)
 */
bool list_equals(const struct list *self, const int *data, size_t size);

/*
 * Add an element in the list at the beginning
 */
void list_push_front(struct list *self, int value);

/*
 * Remove the element at the beginning of the list
 */
void list_pop_front(struct list *self);

/*
 * Add an element in the list at the end
 */
void list_push_back(struct list *self, int value);

/*
 * Remove the element at the end of the list
 */
void list_pop_back(struct list *self);

/*
 * Insert an element in the list (preserving the order)
 * index is valid or equals to the size of the list (insert at the end)
 */
void list_insert(struct list *self, int value, size_t index);

/*
 * Remove an element in the list (preserving the order)
 * index is valid
 */
void list_remove(struct list *self, size_t index);

/*
 * Get the element at the specified index in the list or 0 if the index is not valid
 */
int list_get(const struct list *self, size_t index);

/*
 * Set an element at the specified index in the list to a new value, or do nothing if the index is not valid
 */
void list_set(struct list *self, size_t index, int value);

/*
 * Search for an element in the list and return its index or the size of the list if not present.
 */
size_t list_search(const struct list *self, int value);

/*
 * Tell if a list is sorted
 */
bool list_is_sorted(const struct list *self);

/*
 * Split a list in two. At the end, self should be empty.
 */
void list_split(struct list *self, struct list *out1, struct list *out2);

/*
 * Merge two sorted lists in an empty list. At the end, in1 and in2 should be empty.
 */
void list_merge(struct list *self, struct list *in1, struct list *in2);

/*
 * Sort a list with merge sort
 */
void list_merge_sort(struct list *self);



struct tree_node {
  int data;
  struct tree_node *left;
  struct tree_node *right;
};

struct tree {
  struct tree_node *root;
};

/*
 * Create an empty tree
 */
void tree_create(struct tree *self);

/*
 * Create a tree
 */
void tree_destroy(struct tree *self);

/*
 * Tell if the tree is empty
 */
bool tree_empty(const struct tree *self);

/*
 * Get the size of the tree
 */
size_t tree_size(const struct tree *self);

/*
 * Get the height of the tree
 */
size_t tree_height(const struct tree *self);

/*
 * Tell if a value is in the tree
 */
bool tree_contains(const struct tree *self, int value);

/*
 * Insert a value in the tree and return false if the value was already present
 */
bool tree_insert(struct tree *self, int value);

/*
 * Remove a value from the tree and return false if the value was not present
 */
bool tree_remove(struct tree *self, int value);

/*
 * A function type that takes an int and a pointer and returns void
 */
typedef void (*tree_func_t)(int value, void *user_data);

/*
 * Walk in the tree in pre order and call the function with user_data as a second argument
 */
void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data);

/*
 * Walk in the tree in in order and call the function with user_data as a second argument
 */
void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data);

/*
 * Walk in the tree in post order and call the function with user_data as a second argument
 */
void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data);


#ifdef __cplusplus
}
#endif

#endif // CONTAINERS_H
