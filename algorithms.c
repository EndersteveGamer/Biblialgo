#include "algorithms.h"

#include <assert.h>
#include <stdlib.h>

const int A = 2;

void array_create(struct array *self) {
    self->size = 0;
    self->capacity = 10;
    self->data = calloc(10, sizeof(int));
}

void array_create_from(struct array *self, const int *other, size_t size) {
    self->size = size;
    self->capacity = size;
    self->data = calloc(self->capacity, sizeof(int));
    for (size_t i = 0; i < size; i++) self->data[i] = other[i];
}

void array_destroy(struct array *self) {
    free(self->data);
}

bool array_empty(const struct array *self) {
    return self->size == 0;
}

size_t array_size(const struct array *self) {
    return self->size;
}

bool array_equals(const struct array *self, const int *content, size_t size) {
    if (array_size(self) != size) return false;
    for (size_t i = 0; i < size; i++) if (self->data[i] != content[i]) return false;
    return true;
}

void array_increase_capacity(struct array *self) {
    self->capacity *= A;
    int *data = calloc(self->capacity, sizeof(int));
    for (size_t i = 0; i < self->size; i++) data[i] = self->data[i];
    free(self->data);
    self->data = data;
}

void array_push_back(struct array *self, int value) {
    if (self->size >= self->capacity) array_increase_capacity(self);
    self->data[self->size] = value;
    self->size++;
}

void array_pop_back(struct array *self) {
    assert(self->size > 0);
    self->size--;
}

void array_insert(struct array *self, int value, size_t index) {
    if (self->size == self->capacity) array_increase_capacity(self);
    for (size_t i = self->size; i > index; i--) {
        self->data[i] = self->data[i - 1];
    }
    self->data[index] = value;
    self->size++;
}

void array_remove(struct array *self, size_t index) {
    for (size_t i = index; i < self->size - 1; i++) {
        self->data[i] = self->data[i + 1];
    }
    self->size--;
}

int array_get(const struct array *self, size_t index) {
    return index >= self->size ? 0 : self->data[index];
}

void array_set(struct array *self, size_t index, int value) {
    if (index >= self->size) return;
    self->data[index] = value;
}

size_t array_search(const struct array *self, int value) {
    size_t i = 0;
    for (; i < self->size; i++) {
        if (self->data[i] == value) return i;
    }
    return i;
}

size_t array_dicho_search(const struct array *self, int value, size_t start, size_t end) {
    if (start == end) return self->size;
    size_t middle = (start + end) / 2;
    if (value < self->data[middle]) {
        return array_dicho_search(self, value, start, middle);
    }
    if (self->data[middle] < value) {
        return array_dicho_search(self, value, middle + 1, end);
    }
    return middle;
}

size_t array_search_sorted(const struct array *self, int value) {
    return array_dicho_search(self, value, 0, self->size);
}

bool array_is_sorted(const struct array *self) {
    if (self->size < 2) return true;
    for (size_t i = 0; i < self->size - 1; i++) {
        if (self->data[i] > self->data[i + 1]) return false;
    }
    return true;
}

void array_swap(struct array *self, size_t firstIndex, size_t secondIndex) {
    if (firstIndex >= self->size || secondIndex >= self->size) return;
    int temp = self->data[firstIndex];
    self->data[firstIndex] = self->data[secondIndex];
    self->data[secondIndex] = temp;
}

ptrdiff_t array_partition(struct array *self, ptrdiff_t i, ptrdiff_t j) {
    ptrdiff_t pivotIndex = i;
    const int pivot = self->data[i];
    array_swap(self, pivotIndex, j);
    ptrdiff_t l = i;
    for (ptrdiff_t k = i; k < j; k++) {
        if (self->data[k] < pivot) {
            array_swap(self, k, l);
            l++;
        }
    }
    array_swap(self, l, j);
    return l;
}

void array_quick_sort_recursive(struct array *self, ptrdiff_t i, ptrdiff_t j) {
    if (i >= j) return;
    ptrdiff_t p = array_partition(self, i, j);
    array_quick_sort_recursive(self, i, p - 1);
    array_quick_sort_recursive(self, p + 1, j);
}

void array_quick_sort(struct array *self) {
    array_quick_sort_recursive(self, 0, (ptrdiff_t) self->size - 1);
}

bool array_heap_has_left_child(const struct array *self, size_t index) {
    return 2 * index + 1 < self->size;
}

bool array_heap_has_right_child(const struct array *self, size_t index) {
    return 2 * index + 2 < self->size;
}

size_t array_heap_left_index(const struct array *self, size_t index) {
    assert(array_heap_has_left_child(self, index));
    return 2 * index + 1;
}

size_t array_heap_right_index(const struct array *self, size_t index) {
    assert(array_heap_has_right_child(self, index));
    return 2 * index + 2;
}

size_t array_heap_parent_index(size_t index) {
    assert(index > 0);
    return (index - 1) / 2;
}

void array_heap_sort(struct array *self) {
    struct array temp;
    array_create(&temp);
    for (size_t i = 0; i < self->size; i++) array_heap_add(&temp, self->data[i]);
    for (size_t i = 0; i < self->size; i++) {
        self->data[self->size - i - 1] = array_heap_top(&temp);
        array_heap_remove_top(&temp);
    }
    array_destroy(&temp);
}

bool array_is_heap_recursive(const struct array *self, size_t index) {
    if (array_heap_has_left_child(self, index)) {
        if (array_get(self, index) < array_get(self, array_heap_left_index(self, index))) return false;
        if (!array_is_heap_recursive(self, array_heap_left_index(self, index))) return false;
    }
    if (array_heap_has_right_child(self, index)) {
        if (array_get(self, index) < array_get(self, array_heap_right_index(self, index))) return false;
        if (!array_is_heap_recursive(self, array_heap_right_index(self, index))) return false;
    }
    return true;
}

bool array_is_heap(const struct array *self) {
    if (self->size == 0) return true;
    return array_is_heap_recursive(self, 0);
}

void array_heap_add(struct array *self, int value) {
    size_t currentIndex = self->size;
    array_push_back(self, value);
    while (currentIndex > 0) {
        size_t parentIndex = array_heap_parent_index(currentIndex);
        if (self->data[currentIndex] < self->data[parentIndex]) break;
        array_swap(self, currentIndex, parentIndex);
        currentIndex = parentIndex;
    }
}

int array_heap_top(const struct array *self) {
    assert(self->size > 0);
    return array_get(self, 0);
}

void array_heap_remove_top(struct array *self) {
    size_t size = self->size;
    self->data[0] = self->data[size - 1];
    size_t i = 0;
    while (i < (size - 1) / 2) {
        size_t lt = 2 * i + 1;
        size_t rt = 2 * i + 2;
        if ((lt >= size || self->data[i] > self->data[lt]) && (rt >= size || self->data[i] > self->data[rt])) {
            break;
        }
        size_t j = (self->data[lt] > self->data[rt]) ? lt : rt;
        array_swap(self, i, j);
        i = j;
    }
    self->size--;
}


/*
 * list
 */



void list_create(struct list *self) {
    self->first = NULL;
    self->last = NULL;
}

void list_create_from(struct list *self, const int *other, size_t size) {
    struct list_node *prev = NULL;
    for (size_t i = 0; i < size; i++) {
        struct list_node *curr = malloc(sizeof(struct list_node));
        curr->data = other[i];
        curr->prev = prev;
        curr->next = NULL;
        if (prev == NULL) self->first = curr;
        else prev->next = curr;
        if (i == size - 1) self->last = curr;
        prev = curr;
    }
}

void list_destroy(struct list *self) {
    struct list_node *curr = self->first;
    while (curr != NULL) {
        struct list_node *next = curr->next;
        free(curr);
        curr = next;
    }
}

bool list_empty(const struct list *self) {
    return self->first == NULL;
}

size_t list_size(const struct list *self) {
    struct list_node *curr = self->first;
    size_t size = 0;
    while (curr != NULL) {
        curr = curr->next;
        size++;
    }
    return size;
}

bool list_equals(const struct list *self, const int *data, size_t size) {
    if (list_empty(self)) return size == 0;
    struct list_node *curr = self->first;
    for (size_t i = 0; i < size; i++) {
        if (curr == NULL) return false;
        if (curr->data != data[i]) return false;
        curr = curr->next;
    }
    return curr == NULL;
}

void list_push_front(struct list *self, int value) {
    struct list_node *new = malloc(sizeof(struct list_node));
    new->prev = NULL;
    new->next = self->first;
    new->data = value;
    if (!list_empty(self)) self->first->prev = new;
    else self->last = new;
    self->first = new;
}

void list_pop_front(struct list *self) {
    if (list_empty(self)) return;
    struct list_node *removed = self->first;
    if (removed->next != NULL) removed->next->prev = NULL;
    else self->last = NULL;
    self->first = removed->next;
    free(removed);
}

void list_push_back(struct list *self, int value) {
    struct list_node *new = malloc(sizeof(struct list_node));
    new->next = NULL;
    new->prev = self->last;
    new->data = value;
    if (!list_empty(self)) self->last->next = new;
    else self->first = new;
    self->last = new;
}

void list_pop_back(struct list *self) {
    if (list_empty(self)) return;
    struct list_node *removed = self->last;
    if (removed->prev != NULL) removed->prev->next = NULL;
    else self->first = NULL;
    self->last = removed->prev;
    free(removed);
}


void list_insert(struct list *self, int value, size_t index) {
    if (index == 0) list_push_front(self, value);
    struct list_node *prev = self->first;
    for (size_t i = 0; i < index - 1; i++) {
        if (prev == NULL) return;
        prev = prev->next;
    }
    struct list_node *new = malloc(sizeof(struct list_node));
    new->data = value;
    new->next = prev == NULL ? NULL : prev->next;
    if (new->next == NULL) self->last = new;
    else new->next->prev = new;
    new->prev = prev;
    if (prev == NULL) self->first = new;
    else prev->next = new;
}


void list_remove(struct list *self, size_t index) {
    struct list_node *removed = self->first;
    for (size_t i = 0; i < index; i++) {
        if (removed == NULL) return;
        removed = removed->next;
    }
    if (removed->prev != NULL) removed->prev->next = removed->next;
    else self->first = removed->next;
    if (removed->next != NULL) removed->next->prev = removed->prev;
    else self->last = removed->prev;
    free(removed);
}

int list_get(const struct list *self, size_t index) {
    struct list_node *curr = self->first;
    for (size_t i = 0; i < index; i++) {
        if (curr == NULL) return 0;
        curr = curr->next;
    }
    return curr == NULL ? 0 : curr->data;
}

void list_set(struct list *self, size_t index, int value) {
    struct list_node *curr = self->first;
    for (size_t i = 0; i < index; i++) {
        if (curr == NULL) return;
        curr = curr->next;
    }
    if (curr == NULL) return;
    curr->data = value;
}

size_t list_search(const struct list *self, int value) {
    size_t index = 0;
    struct list_node *curr = self->first;
    while (curr != NULL) {
        if (curr->data == value) break;
        index++;
        curr = curr->next;
    }
    return index;
}

bool list_is_sorted(const struct list *self) {
    if (list_empty(self) || self->first->next == NULL) return true;
    struct list_node *curr = self->first;
    while (curr->next != NULL) {
        if (curr->data > curr->next->data) return false;
        curr = curr->next;
    }
    return true;
}

void list_split(struct list *self, struct list *out1, struct list *out2) {
    size_t size = list_size(self) / 2;
    if (size % 2 == 1) size--;
    if (size <= 1) {
        out1->first = self->first;
        out1->last = self->first;
    }
    struct list_node *mid = self->first;
    for (size_t i = 0; i < size; i++) mid = mid->next;
    out1->first = self->first;
    out1->last = mid;
    out2->first = mid->next;
    out2->last = self->last;
    mid->next = NULL;
    out1->first->prev = NULL;
    self->first = NULL;
    self->last = NULL;
}

void list_merge(struct list *self, struct list *in1, struct list *in2) {
    while (!list_empty(in1) || !list_empty(in2)) {
        if (list_empty(in2) || (!list_empty(in1) && in1->first->data < in2->first->data)) {
            int removed = in1->first->data;
            list_push_back(self, removed);
            list_pop_front(in1);
        }
        else {
            int removed = in2->first->data;
            list_push_back(self, removed);
            list_pop_front(in2);
        }
    }
}

void list_merge_sort(struct list *self) {
    if (self->first == NULL) return;
    if (self->first->next == NULL) return;
    if (self->first->next->next == NULL) {
        if (list_get(self, 0) > list_get(self, 1)) {
            int first = list_get(self, 0);
            list_set(self, 0, list_get(self, 1));
            list_set(self, 1, first);
        }
    }
    struct list l1;
    list_create(&l1);
    struct list l2;
    list_create(&l2);
    list_split(self, &l1, &l2);
    list_merge_sort(&l1);
    list_merge_sort(&l2);
    list_merge(self, &l1, &l2);
}


/*
 * tree
 */

void tree_create(struct tree *self) {
    self->root = NULL;
}

void tree_node_destroy(struct tree_node *self) {
    if (self == NULL) return;
    tree_node_destroy(self->left);
    tree_node_destroy(self->right);
    free(self);
}

void tree_destroy(struct tree *self) {
    tree_node_destroy(self->root);
    self->root = NULL;
}

bool tree_node_contains(const struct tree_node *self, int value) {
    if (self == NULL) return false;
    if (value < self->data) return tree_node_contains(self->left, value);
    if (value > self->data) return tree_node_contains(self->right, value);
    return true;
}

bool tree_contains(const struct tree *self, int value) {
    return tree_node_contains(self->root, value);
}

struct tree_node* tree_node_create(int value) {
    struct tree_node *new = malloc(sizeof(struct tree_node));
    new->data = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}

bool tree_node_insert(struct tree_node *self, int value) {
    if (value < self->data) {
        if (self->left != NULL) return tree_node_insert(self->left, value);
        self->left = tree_node_create(value);
        return true;
    }
    if (value > self->data) {
        if (self->right != NULL) return tree_node_insert(self->right, value);
        self->right = tree_node_create(value);
        return true;
    }
    return false;
}

bool tree_insert(struct tree *self, int value) {
    if (self->root != NULL) return tree_node_insert(self->root, value);
    self->root = tree_node_create(value);
    return true;
}

struct tree_node *tree_delete_minimum(struct tree_node *self, struct tree_node **min) {
    if (self->left == NULL) {
        struct tree_node *right = self->right;
        self->right = NULL;
        *min = self;
        return right;
    }
    self->left = tree_delete_minimum(self->left, min);
    return self;
}

struct tree_node *tree_delete(struct tree_node *self) {
    struct tree_node *left = self->left;
    struct tree_node *right = self->right;
    free(self);
    self = NULL;
    if (left == NULL && right == NULL) return NULL;
    if (left == NULL) return right;
    if (right == NULL) return left;
    right = tree_delete_minimum(right, &self);
    self->left = left;
    self->right = right;
    return self;
}

struct tree_node *tree_node_remove(struct tree_node *self, int value, bool *found) {
    if (self == NULL) {
        *found = false;
        return NULL;
    }
    if (value < self->data) {
        self->left = tree_node_remove(self->left, value, found);
        return self;
    }
    if (value > self->data) {
        self->right = tree_node_remove(self->right, value, found);
        return self;
    }
    return tree_delete(self);
}

bool tree_remove(struct tree *self, int value) {
    bool found = true;
    self->root = tree_node_remove(self->root, value, &found);
    return found;
}

bool tree_empty(const struct tree *self) {
    return self->root == NULL;
}

size_t tree_node_size(const struct tree_node *self) {
    if (self == NULL) return 0;
    return 1 + tree_node_size(self->left) + tree_node_size(self->right);
}

size_t tree_size(const struct tree *self) {
    return tree_node_size(self->root);
}

size_t tree_node_height(const struct tree_node *self) {
    if (self == NULL) return 0;
    size_t left = tree_node_height(self->left);
    size_t right = tree_node_height(self->right);
    return 1 + (left > right ? left : right);
}

size_t tree_height(const struct tree *self) {
    return tree_node_height(self->root);
}

void tree_node_walk_pre_order(const struct tree_node *self, tree_func_t func, void *user_data) {
    if (self == NULL) return;
    func(self->data, user_data);
    tree_node_walk_pre_order(self->left, func, user_data);
    tree_node_walk_pre_order(self->right, func, user_data);
}

void tree_walk_pre_order(const struct tree *self, tree_func_t func, void *user_data) {
    tree_node_walk_pre_order(self->root, func, user_data);
}

void tree_node_walk_in_order(const struct tree_node *self, tree_func_t func, void *user_data) {
    if (self == NULL) return;
    tree_node_walk_in_order(self->left, func, user_data);
    func(self->data, user_data);
    tree_node_walk_in_order(self->right, func, user_data);
}

void tree_walk_in_order(const struct tree *self, tree_func_t func, void *user_data) {
    tree_node_walk_in_order(self->root, func, user_data);
}

void tree_node_walk_post_order(const struct tree_node *self, tree_func_t func, void *user_data) {
    if (self == NULL) return;
    tree_node_walk_post_order(self->left, func, user_data);
    tree_node_walk_post_order(self->right, func, user_data);
    func(self->data, user_data);
}

void tree_walk_post_order(const struct tree *self, tree_func_t func, void *user_data) {
    tree_node_walk_post_order(self->root, func, user_data);
}
