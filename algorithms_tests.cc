#include "gtest/gtest.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <array>

#include "algorithms.h"

#define BIG_SIZE 1000

/*
 * array_create
 */

TEST(ArrayCreateTest, Empty) {
  struct array a;
  array_create(&a);

  EXPECT_TRUE(array_empty(&a));
  EXPECT_TRUE(array_size(&a) == 0);

  array_destroy(&a);
}

/*
 * array_create_from
 */

TEST(ArrayCreateFromTest, OneElement) {
  static const int origin[] = { 1 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_empty(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));
  EXPECT_TRUE(array_equals(&a, origin, std::size(origin)));

  array_destroy(&a);
}

TEST(ArrayCreateFromTest, ManyElements) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_empty(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));
  EXPECT_TRUE(array_equals(&a, origin, std::size(origin)));

  array_destroy(&a);
}

/*
 * array_equals
 */

TEST(ArrayEqualsTest, DifferentElement) {
  static const int origin[]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  static const int reference[] = { 1, 2, 3, 4, 5, 6, 7, 8, 10 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_equals(&a, reference, std::size(reference)));

  array_destroy(&a);
}

TEST(ArrayEqualsTest, SizeShorter) {
  static const int origin[]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  static const int reference[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_equals(&a, reference, std::size(reference)));

  array_destroy(&a);
}

TEST(ArrayEqualsTest, SizeLarger) {
  static const int origin[]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  static const int reference[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_equals(&a, reference, std::size(reference)));

  array_destroy(&a);
}

/*
 * array_push_back
 */

TEST(ArrayPushBackTest, OneElement) {
  struct array a;
  array_create(&a);
  array_push_back(&a, 1);

  EXPECT_FALSE(array_empty(&a));
  EXPECT_TRUE(array_size(&a) == 1);
  EXPECT_EQ(array_get(&a, 0), 1);

  array_destroy(&a);
}

TEST(ArrayPushBackTest, ManyElements) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct array a;
  array_create(&a);

  for (size_t i = 0; i < std::size(origin); ++i) {
    array_push_back(&a, origin[i]);
  }

  EXPECT_FALSE(array_empty(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));


  for (size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(array_get(&a, i), origin[i]);
  }

  EXPECT_TRUE(array_equals(&a, origin, std::size(origin)));

  array_destroy(&a);
}

TEST(ArrayPushBackTest, Stressed) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_push_back(&a, i + 1);
    EXPECT_EQ(array_size(&a), static_cast<std::size_t>(i + 1));
  }

  EXPECT_FALSE(array_empty(&a));

  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_EQ(array_get(&a, i), i + 1);
  }

  array_destroy(&a);
}

/*
 * array_pop_back
 */

TEST(ArrayPopBackTest, OneElement) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_EQ(array_size(&a), std::size(origin));

  array_pop_back(&a);

  EXPECT_EQ(array_size(&a), std::size(origin) - 1);

  for (std::size_t i = 0; i < std::size(origin) - 1; ++i) {
    EXPECT_EQ(array_get(&a, i), origin[i]);
  }

  array_destroy(&a);
}

TEST(ArrayPopBackTest, ManyElements) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_EQ(array_size(&a), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    array_pop_back(&a);
  }

  EXPECT_TRUE(array_empty(&a));
  EXPECT_EQ(array_size(&a), 0u);

  array_destroy(&a);
}

/*
 * array_insert
 */

TEST(ArrayInsertTest, Beginning) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 42, 9, 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_insert(&a, 42, 0); // insert at the beginning

  EXPECT_EQ(array_size(&a), std::size(expected));
  EXPECT_TRUE(array_equals(&a, expected, std::size(expected)));

  array_destroy(&a);
}

TEST(ArrayInsertTest, Middle) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 42, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_insert(&a, 42, 3); // insert in the middle

  EXPECT_EQ(array_size(&a), std::size(expected));
  EXPECT_TRUE(array_equals(&a, expected, std::size(expected)));

  array_destroy(&a);
}

TEST(ArrayInsertTest, End) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 2, 4, 0, 8, 42 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_insert(&a, 42, std::size(origin)); // insert at the end

  EXPECT_EQ(array_size(&a), std::size(expected));
  EXPECT_TRUE(array_equals(&a, expected, std::size(expected)));

  array_destroy(&a);
}

TEST(ArrayInsertTest, Stressed) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_insert(&a, i + 1, 4);
    EXPECT_EQ(array_size(&a), std::size(origin) + i + 1);
  }

  array_destroy(&a);
}


/*
 * array_remove
 */

TEST(ArrayRemoveTest, Beginning) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_remove(&a, 0); // remove at the beginning

  EXPECT_EQ(array_size(&a), std::size(expected));
  EXPECT_TRUE(array_equals(&a, expected, std::size(expected)));

  array_destroy(&a);
}

TEST(ArrayRemoveTest, Middle) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_remove(&a, 3); // remove in the middle

  EXPECT_EQ(array_size(&a), std::size(expected));
  EXPECT_TRUE(array_equals(&a, expected, std::size(expected)));

  array_destroy(&a);
}

TEST(ArrayRemoveTest, End) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 2, 4, 0 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_remove(&a, std::size(origin) - 1); // remove at the end

  EXPECT_EQ(array_size(&a), std::size(expected));
  EXPECT_TRUE(array_equals(&a, expected, std::size(expected)));

  array_destroy(&a);
}

/*
 * array_get
 */

TEST(ArrayGetTest, ValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(array_get(&a, i), origin[i]);
  }

  array_destroy(&a);
}

TEST(ArrayGetTest, NotValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_EQ(array_get(&a, std::size(origin)), 0);
  EXPECT_EQ(array_get(&a, -1), 0);

  array_destroy(&a);
}

/*
 * array_set
 */

TEST(ArraySetTest, ValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    array_set(&a, i, origin[i] - 1);
    EXPECT_EQ(array_get(&a, i), origin[i] - 1);
  }

  EXPECT_EQ(array_size(&a), std::size(origin));

  array_destroy(&a);
}

TEST(ArraySetTest, NotValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  array_set(&a, std::size(origin), 42);
  array_set(&a, -1, 42);

  EXPECT_EQ(array_size(&a), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(array_get(&a, i), origin[i]);
  }

  array_destroy(&a);
}

/*
 * array_search
 */

TEST(ArraySearchTest, Present) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  for (size_t i = 0; i < std::size(origin); ++i) {
    size_t index = array_search(&a, origin[i]);
    EXPECT_EQ(index, i);
  }

  array_destroy(&a);
}

TEST(ArraySearchTest, NotPresent) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  size_t index = array_search(&a, -1);
  EXPECT_EQ(index, std::size(origin));

  array_destroy(&a);
}

/*
 * array_search_sorted
 */

TEST(ArraySearchSortedTest, Present) {
  static const int origin[] = { 1, 2, 3, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  for (size_t i = 0; i < std::size(origin); ++i) {
    size_t index = array_search_sorted(&a, origin[i]);
    EXPECT_EQ(index, i);
  }

  array_destroy(&a);
}


TEST(ArraySearchSortedTest, NotPresentBefore) {
  static const int origin[] = { 1, 2, 3, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  size_t index = array_search_sorted(&a, -1); // before everything
  EXPECT_EQ(index, std::size(origin));

  array_destroy(&a);
}

TEST(ArraySearchSortedTest, NotPresentAfter) {
  static const int origin[] = { 1, 2, 3, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  size_t index = array_search_sorted(&a, 15); // after everything
  EXPECT_EQ(index, std::size(origin));

  array_destroy(&a);
}

TEST(ArraySearchSortedTest, NotPresentMiddle) {
  static const int origin[] = { 1, 2, 3, 5, 6, 7, 8, 9 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  size_t index = array_search_sorted(&a, 4); // in the middle of other elements
  EXPECT_EQ(index, std::size(origin));

  array_destroy(&a);
}

/*
 * array_is_sorted
 */

TEST(ArrayIsSortedTest, Empty) {
  struct array a;
  array_create(&a);

  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayIsSortedTest, Sorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_TRUE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayIsSortedTest, NotSorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 10, 9, 8, 7, 6 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_sorted(&a));

  array_destroy(&a);
}

TEST(ArrayIsSortedTest, NotSortedAtTheEnd) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_sorted(&a));

  array_destroy(&a);
}

/*
 * array_partition
 */

TEST(ArrayPartitionTest, WholeSorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  ptrdiff_t p = array_partition(&a, 0, std::size(origin) - 1);
  int pivot = array_get(&a, p);

  for (ptrdiff_t i = 0; i < p; ++i) {
    EXPECT_LT(array_get(&a, i), pivot);
  }

  for (ptrdiff_t i = p + 1; i < static_cast<ptrdiff_t>(std::size(origin)); ++i) {
    EXPECT_GT(array_get(&a, i), pivot);
  }

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayPartitionTest, WholeNotSorted) {
  static const int origin[] = { 5, 4, 1, 6, 10, 3, 0, 9, 8, 2, 7 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  ptrdiff_t p = array_partition(&a, 0, std::size(origin) - 1);
  int pivot = array_get(&a, p);

  for (ptrdiff_t i = 0; i < p; ++i) {
    EXPECT_LT(array_get(&a, i), pivot);
  }

  for (ptrdiff_t i = p + 1; i < static_cast<ptrdiff_t>(std::size(origin)); ++i) {
    EXPECT_GT(array_get(&a, i), pivot);
  }

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayPartitionTest, PartNotSorted) {
  static const int origin[] = { 5, 4, 1, 6, 10, 3, 0, 9, 8, 2, 7 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  ptrdiff_t p = array_partition(&a, 3, 8);
  int pivot = array_get(&a, p);

  for (ptrdiff_t i = 3; i < p; ++i) {
    EXPECT_LT(array_get(&a, i), pivot);
  }

  for (ptrdiff_t i = p + 1; i < 8; ++i) {
    EXPECT_GT(array_get(&a, i), pivot);
  }

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayPartitionTest, Single) {
  static const int origin[] = { 5, 4, 1, 6, 10, 3, 0, 9, 8, 2, 7 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  ptrdiff_t p = array_partition(&a, 5, 5);

  EXPECT_EQ(p, 5);

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

/*
 * array_quick_sort
 */

TEST(ArrayQuickSortTest, NotSorted) {
  static const int origin[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_sorted(&a));

  array_quick_sort(&a);

  EXPECT_TRUE(array_is_sorted(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayQuickSortTest, SortedBackward) {
  static const int origin[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_sorted(&a));

  array_quick_sort(&a);

  EXPECT_TRUE(array_is_sorted(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayQuickSortTest, Sorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_TRUE(array_is_sorted(&a));

  array_quick_sort(&a);

  EXPECT_TRUE(array_is_sorted(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

/*
 * array_heap_sort
 */

TEST(ArrayHeapSortTest, NotSorted) {
  static const int origin[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_sorted(&a));

  array_heap_sort(&a);

  EXPECT_TRUE(array_is_sorted(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayHeapSortTest, SortedBackward) {
  static const int origin[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_sorted(&a));

  array_heap_sort(&a);

  EXPECT_TRUE(array_is_sorted(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

TEST(ArrayHeapSortTest, Sorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_TRUE(array_is_sorted(&a));

  array_heap_sort(&a);

  EXPECT_TRUE(array_is_sorted(&a));
  EXPECT_EQ(array_size(&a), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(array_search(&a, val), std::size(origin));
  }

  array_destroy(&a);
}

/*
 * array_is_heap
 */

TEST(ArrayIsHeapTest, Empty) {
  struct array a;
  array_create(&a);

  EXPECT_TRUE(array_is_heap(&a));

  array_destroy(&a);
}

TEST(ArrayIsHeapTest, SimpleHeap1) {
  static const int origin[] = { 13, 6, 8, 5, 3 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_TRUE(array_is_heap(&a));

  array_destroy(&a);
}

TEST(ArrayIsHeapTest, SimpleHeap2) {
  static const int origin[] = { 81, 45, 24, 21, 6, 17, 19, 14 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_TRUE(array_is_heap(&a));

  array_destroy(&a);
}

TEST(ArrayIsHeapTest, NotHeap1) {
  static const int origin[] = { 6, 5, 4, 3, 1, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_heap(&a));

  array_destroy(&a);
}

TEST(ArrayIsHeapTest, NotHeap2) {
  static const int origin[] = { 6, 5, 4, 3, 1, 0, 8 };

  struct array a;
  array_create_from(&a, origin, std::size(origin));

  EXPECT_FALSE(array_is_heap(&a));

  array_destroy(&a);
}

/*
 * array_heap_add
 */

TEST(ArrayHeapAddTest, Stressed) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_heap_add(&a, i);
    EXPECT_EQ(i, array_heap_top(&a));
    EXPECT_TRUE(array_is_heap(&a));
  }

  array_destroy(&a);
}

/*
 * array_heap_remove_top
 */

TEST(ArrayHeapRemoveTopTest, Stressed) {
  struct array a;

  array_create(&a);

  for (int i = 0; i < BIG_SIZE; ++i) {
    array_heap_add(&a, i);
  }

  EXPECT_TRUE(array_is_heap(&a));

  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_EQ(BIG_SIZE - i - 1, array_heap_top(&a));
    array_heap_remove_top(&a);
  }

  EXPECT_TRUE(array_empty(&a));

  array_destroy(&a);
}


/*
 * list_create
 */

TEST(ListCreateTest, Empty) {
  struct list l;

  list_create(&l);

  EXPECT_TRUE(list_empty(&l));
  EXPECT_EQ(list_size(&l), 0u);
  EXPECT_EQ(l.first, nullptr);
  EXPECT_EQ(l.last, nullptr);

  list_destroy(&l);
}

/*
 * list_create_from
 */

TEST(ListCreateFromTest, OneElement) {
  static const int origin[] = { 1 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));
  EXPECT_TRUE(list_equals(&l, origin, std::size(origin)));
  EXPECT_EQ(l.first, l.last);


  list_destroy(&l);
}

TEST(ListCreateFromTest, ManyElements) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));
  EXPECT_TRUE(list_equals(&l, origin, std::size(origin)));


  list_destroy(&l);
}

/*
 * list_equals
 */

TEST(ListEqualsTest, DifferentElement) {
  static const int origin[]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  static const int reference[] = { 1, 2, 3, 4, 5, 6, 7, 8, 10 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_equals(&l, reference, std::size(reference)));

  list_destroy(&l);
}

TEST(ListEqualsTest, SizeShorter) {
  static const int origin[]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  static const int reference[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_equals(&l, reference, std::size(reference)));

  list_destroy(&l);
}

TEST(ListEqualsTest, SizeLarger) {
  static const int origin[]    = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  static const int reference[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_equals(&l, reference, std::size(reference)));

  list_destroy(&l);
}

/*
 * list_push_front
 */

TEST(ListPushFrontTest, OneElement) {
  struct list l;
  list_create(&l);

  list_push_front(&l, 1);

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), 1u);
  EXPECT_EQ(list_get(&l, 0), 1);
  EXPECT_EQ(l.first, l.last);


  list_destroy(&l);
}


TEST(ListPushFrontTest, ManyElements) {
  static const int origin[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };

  struct list l;
  list_create(&l);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    list_push_front(&l, origin[std::size(origin) - i - 1]);
  }

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(list_get(&l, i), origin[i]);
  }

  EXPECT_TRUE(list_equals(&l, origin, std::size(origin)));

  list_destroy(&l);
}


TEST(ListPushFrontTest, Stressed) {
  struct list l;
  list_create(&l);

  for (int i = 0; i < BIG_SIZE; ++i) {
    list_push_front(&l, i + 1);
    EXPECT_EQ(list_get(&l, 0), i + 1);
    EXPECT_EQ(list_size(&l), static_cast<std::size_t>(i + 1));
  }

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), static_cast<std::size_t>(BIG_SIZE));

  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_EQ(list_get(&l, BIG_SIZE - i - 1), i + 1);
  }

  list_destroy(&l);
}

/*
 * list_pop_front
 */

TEST(ListPopFrontTest, OneElement) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_EQ(list_size(&l), std::size(origin));

  list_pop_front(&l);

  EXPECT_EQ(list_size(&l), std::size(origin) - 1);

  for (std::size_t i = 0; i < std::size(origin) - 1; ++i) {
    EXPECT_EQ(list_get(&l, i), origin[i + 1]);
  }

  list_destroy(&l);
}

TEST(ListPopFrontTest, ManyElements) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_EQ(list_size(&l), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    list_pop_front(&l);
  }

  EXPECT_TRUE(list_empty(&l));
  EXPECT_EQ(list_size(&l), 0u);

  list_destroy(&l);
}

/*
 * list_push_back
 */

TEST(ListPushBackTest, OneElement) {
  struct list l;
  list_create(&l);

  list_push_back(&l, 1);

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), 1u);
  EXPECT_EQ(list_get(&l, 0), 1);
  EXPECT_EQ(l.first, l.last);


  list_destroy(&l);
}


TEST(ListPushBackTest, ManyElements) {
  static const int origin[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };

  struct list l;
  list_create(&l);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    list_push_back(&l, origin[i]);
  }

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(list_get(&l, i), origin[i]);
  }

  EXPECT_TRUE(list_equals(&l, origin, std::size(origin)));

  list_destroy(&l);
}


TEST(ListPushBackTest, Stressed) {
  struct list l;
  list_create(&l);

  for (int i = 0; i < BIG_SIZE; ++i) {
    list_push_back(&l, i + 1);
    EXPECT_EQ(list_get(&l, i), i + 1);
    EXPECT_EQ(list_size(&l), static_cast<std::size_t>(i + 1));
  }

  EXPECT_FALSE(list_empty(&l));
  EXPECT_EQ(list_size(&l), static_cast<std::size_t>(BIG_SIZE));

  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_EQ(list_get(&l, i), i + 1);
  }

  list_destroy(&l);
}

/*
 * list_pop_back
 */

TEST(ListPopBackTest, OneElement) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_EQ(list_size(&l), std::size(origin));

  list_pop_back(&l);

  EXPECT_EQ(list_size(&l), std::size(origin) - 1);

  for (std::size_t i = 0; i < std::size(origin) - 1; ++i) {
    EXPECT_EQ(list_get(&l, i), origin[i]);
  }

  list_destroy(&l);
}

TEST(ListPopBackTest, ManyElements) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_EQ(list_size(&l), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    list_pop_back(&l);
  }

  EXPECT_TRUE(list_empty(&l));
  EXPECT_EQ(list_size(&l), 0u);

  list_destroy(&l);
}

/*
 * list_insert
 */

TEST(ListInsertTest, Beginning) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 42, 9, 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_insert(&l, 42, 0); // insert at the beginning

  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l);
}

TEST(ListInsertTest, Middle) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 42, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_insert(&l, 42, 3); // insert in the middle

  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l);
}

TEST(ListInsertTest, End) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 2, 4, 0, 8, 42 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_insert(&l, 42, std::size(origin)); // insert at the end

  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l);
}

TEST(ListInsertTest, Stressed) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  for (int i = 0; i < BIG_SIZE; ++i) {
    list_insert(&l, i + 1, 4);
    EXPECT_EQ(list_size(&l), std::size(origin) + i + 1);
  }

  list_destroy(&l);
}

/*
 * list_remove
 */

TEST(ListRemoveTest, Beginning) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_remove(&l, 0); // remove at the beginning

  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l);
}

TEST(ListRemoveTest, Middle) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_remove(&l, 3); // remove in the middle

  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l);
}

TEST(ListRemoveTest, End) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };
  static const int expected[] = { 9, 3, 7, 2, 4, 0 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_remove(&l, std::size(origin) - 1); // remove at the end

  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l);
}

/*
 * list_get
 */

TEST(ListGetTest, ValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(list_get(&l, i), origin[i]);
  }

  list_destroy(&l);
}

TEST(ListGetTest, NotValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_EQ(list_get(&l, std::size(origin)), 0);
  EXPECT_EQ(list_get(&l, -1), 0);

  list_destroy(&l);
}

/*
 * list_set
 */

TEST(ListSetTest, ValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    list_set(&l, i, origin[i] - 1);
    EXPECT_EQ(list_get(&l, i), origin[i] - 1);
  }

  EXPECT_EQ(list_size(&l), std::size(origin));

  list_destroy(&l);
}

TEST(ListSetTest, NotValidIndex) {
  static const int origin[] = { 9, 3, 7, 2, 4, 0, 8 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  list_set(&l, std::size(origin), 42);
  list_set(&l, -1, 42);

  EXPECT_EQ(list_size(&l), std::size(origin));

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_EQ(list_get(&l, i), origin[i]);
  }

  list_destroy(&l);
}

/*
 * list_search
 */

TEST(ListSearchTest, Present) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  for (size_t i = 0; i < std::size(origin); ++i) {
    size_t index = list_search(&l, origin[i]);
    EXPECT_EQ(index, i);
  }

  list_destroy(&l);
}

TEST(ListSearchTest, NotPresent) {
  static const int origin[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  size_t index = list_search(&l, -1);
  EXPECT_EQ(index, std::size(origin));

  list_destroy(&l);
}

/*
 * list_is_sorted
 */

TEST(ListIsSortedTest, Empty) {
  struct list l;
  list_create(&l);

  EXPECT_TRUE(list_is_sorted(&l));

  list_destroy(&l);
}

TEST(ListIsSortedTest, Sorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_TRUE(list_is_sorted(&l));

  list_destroy(&l);
}

TEST(ListIsSortedTest, NotSorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 10, 9, 8, 7, 6 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_is_sorted(&l));

  list_destroy(&l);
}

TEST(ListIsSortedTest, NotSortedAtTheEnd) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_is_sorted(&l));

  list_destroy(&l);
}

/*
 * list_split
 */

TEST(ListSplitTest, EvenSize) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  assert(std::size(origin) % 2 == 0);

  std::size_t half = std::size(origin) / 2;

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  struct list l1;
  list_create(&l1);

  struct list l2;
  list_create(&l2);

  list_split(&l, &l1, &l2);

  EXPECT_TRUE(list_empty(&l));
  EXPECT_EQ(list_size(&l1), half);
  EXPECT_EQ(list_size(&l2), half);

  for (auto val : origin) {
    EXPECT_TRUE(list_search(&l1, val) != half || list_search(&l2, val) != half);
  }

  list_destroy(&l2);
  list_destroy(&l1);
  list_destroy(&l);
}


TEST(ListSplitTest, OddSize) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  assert(std::size(origin) % 2 == 1);

  std::size_t half = std::size(origin) / 2;

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  struct list l1;
  list_create(&l1);

  struct list l2;
  list_create(&l2);

  list_split(&l, &l1, &l2);

  std::size_t sz1 = list_size(&l1);
  std::size_t sz2 = list_size(&l2);

  EXPECT_TRUE(list_empty(&l));
  EXPECT_EQ(sz1 + sz2, std::size(origin));

  EXPECT_TRUE((sz1 == half && sz2 == half + 1) || (sz1 == half + 1 && sz2 == half));

  for (auto val : origin) {
    EXPECT_TRUE(list_search(&l1, val) != sz1 || list_search(&l2, val) != sz2);
  }

  list_destroy(&l2);
  list_destroy(&l1);
  list_destroy(&l);
}

/*
 * list_merge
 */

TEST(ListMergeTest, BothNonEmpty) {
  static const int origin1[] = { 0, 1, 3, 6, 10 };
  static const int origin2[] = { 2, 4, 5, 7, 8, 9 };
  static const int expected[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct list l;
  list_create(&l);

  struct list l1;
  list_create_from(&l1, origin1, std::size(origin1));

  struct list l2;
  list_create_from(&l2, origin2, std::size(origin2));

  list_merge(&l, &l1, &l2);

  EXPECT_TRUE(list_empty(&l1));
  EXPECT_TRUE(list_empty(&l2));
  EXPECT_EQ(list_size(&l), std::size(expected));
  EXPECT_TRUE(list_equals(&l, expected, std::size(expected)));

  list_destroy(&l2);
  list_destroy(&l1);
  list_destroy(&l);
}

TEST(ListMergeTest, FirstEmpty) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct list l;
  list_create(&l);

  struct list l1;
  list_create(&l1);

  struct list l2;
  list_create_from(&l2, origin, std::size(origin));

  list_merge(&l, &l1, &l2);

  EXPECT_TRUE(list_empty(&l1));
  EXPECT_TRUE(list_empty(&l2));
  EXPECT_EQ(list_size(&l), std::size(origin));
  EXPECT_TRUE(list_equals(&l, origin, std::size(origin)));

  list_destroy(&l2);
  list_destroy(&l1);
  list_destroy(&l);
}

TEST(ListMergeTest, SecondEmpty) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct list l;
  list_create(&l);

  struct list l1;
  list_create_from(&l1, origin, std::size(origin));

  struct list l2;
  list_create(&l2);

  list_merge(&l, &l1, &l2);

  EXPECT_TRUE(list_empty(&l1));
  EXPECT_TRUE(list_empty(&l2));
  EXPECT_EQ(list_size(&l), std::size(origin));
  EXPECT_TRUE(list_equals(&l, origin, std::size(origin)));

  list_destroy(&l2);
  list_destroy(&l1);
  list_destroy(&l);
}

/*
 * list_merge_sort
 */

TEST(ListMergeSortTest, NotSorted) {
  static const int origin[] = { 8, 4, 1, 6, 10, 3, 0, 9, 5, 2, 7 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_is_sorted(&l));

  list_merge_sort(&l);

  EXPECT_TRUE(list_is_sorted(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(list_search(&l, val), std::size(origin));
  }

  list_destroy(&l);
}

TEST(ListMergeSortTest, SortedBackward) {
  static const int origin[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_FALSE(list_is_sorted(&l));

  list_merge_sort(&l);

  EXPECT_TRUE(list_is_sorted(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(list_search(&l, val), std::size(origin));
  }

  list_destroy(&l);
}

TEST(ListMergeSortTest, Sorted) {
  static const int origin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct list l;
  list_create_from(&l, origin, std::size(origin));

  EXPECT_TRUE(list_is_sorted(&l));

  list_merge_sort(&l);

  EXPECT_TRUE(list_is_sorted(&l));
  EXPECT_EQ(list_size(&l), std::size(origin));

  for (int val : origin) {
    EXPECT_NE(list_search(&l, val), std::size(origin));
  }

  list_destroy(&l);
}

/*
 * tree_create
 */

TEST(TreeCreateTest, Empty) {
  struct tree t;

  tree_create(&t);

  EXPECT_TRUE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 0u);
  EXPECT_EQ(tree_height(&t), 0u);

  tree_destroy(&t);
}


/*
 * tree_insert
 */

TEST(TreeInsertTest, OneElement) {
  struct tree t;

  tree_create(&t);
  bool inserted = tree_insert(&t, 0);

  EXPECT_TRUE(inserted);
  EXPECT_FALSE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 1u);
  EXPECT_EQ(tree_height(&t), 1u);
  EXPECT_TRUE(tree_contains(&t, 0));

  tree_destroy(&t);
}

TEST(TreeInsertTest, TwoElements) {
  struct tree t;

  tree_create(&t);
  bool inserted1 = tree_insert(&t, 1);
  bool inserted2 = tree_insert(&t, 2);

  EXPECT_TRUE(inserted1);
  EXPECT_TRUE(inserted2);
  EXPECT_FALSE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 2u);
  EXPECT_EQ(tree_height(&t), 2u);
  EXPECT_TRUE(tree_contains(&t, 1));
  EXPECT_TRUE(tree_contains(&t, 2));

  tree_destroy(&t);
}

TEST(TreeInsertTest, AlreadyPresent) {
  struct tree t;
  tree_create(&t);

  bool inserted1 = tree_insert(&t, 5);

  EXPECT_TRUE(inserted1);

  bool inserted2 = tree_insert(&t, 7);

  EXPECT_TRUE(inserted2);

  bool inserted3 = tree_insert(&t, 7); // already present

  EXPECT_FALSE(inserted3);
  EXPECT_FALSE(tree_empty(&t));
  EXPECT_EQ(tree_size(&t), 2u);
  EXPECT_EQ(tree_height(&t), 2u);
  EXPECT_TRUE(tree_contains(&t, 5));
  EXPECT_TRUE(tree_contains(&t, 7));

  tree_destroy(&t);
}


TEST(TreeInsertTest, ManyElements) {
  static const int values[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(values); ++i) {
    bool inserted = tree_insert(&t, values[i]);
    EXPECT_TRUE(inserted);
  }

  for (std::size_t i = 0; i < std::size(values); ++i) {
    EXPECT_TRUE(tree_contains(&t, values[i]));
  }

  for (int i = 1; i <= 19; i += 2) {
    EXPECT_FALSE(tree_contains(&t, i));
  }

  tree_destroy(&t);
}

static size_t log_2(size_t x) {
  size_t res = 0;

  while (x != 0) {
    res++;
    x /= 2;
  }

  return res;
}

TEST(TreeInsertTest, Stressed) {
  struct tree t;
  tree_create(&t);

  std::size_t expected = 0;
  std::srand(0);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = std::rand();

    if (tree_insert(&t, value)) {
      ++expected;
    }

    EXPECT_EQ(tree_size(&t), expected);
  }

  EXPECT_FALSE(tree_empty(&t));

  size_t size = tree_size(&t);
  size_t height = tree_height(&t);

  EXPECT_EQ(size, expected);
  EXPECT_LE(height, size);
  EXPECT_LE(log_2(size + 1), height);

  tree_destroy(&t);
}

/*
 * tree_remove
 */

TEST(TreeRemoveTest, ManyElements) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    EXPECT_TRUE(tree_contains(&t, origin[i]));

    bool removed = tree_remove(&t, origin[i]);

    EXPECT_TRUE(removed);
    EXPECT_FALSE(tree_contains(&t, origin[i]));
    EXPECT_EQ(tree_size(&t), std::size(origin) - i - 1);
  }

  tree_destroy(&t);
}

TEST(TreeRemoveTest, NotPresentBefore) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  bool removed = tree_remove(&t, 1);

  EXPECT_FALSE(removed);
  EXPECT_EQ(tree_size(&t), std::size(origin));

  tree_destroy(&t);
}

TEST(TreeRemoveTest, NotPresentMiddle) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  bool removed = tree_remove(&t, 9);

  EXPECT_FALSE(removed);
  EXPECT_EQ(tree_size(&t), std::size(origin));

  tree_destroy(&t);
}

TEST(TreeRemoveTest, NotPresentAfter) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (int val : origin) {
    tree_insert(&t, val);
  }

  bool removed = tree_remove(&t, 19);

  EXPECT_FALSE(removed);
  EXPECT_EQ(tree_size(&t), std::size(origin));

  tree_destroy(&t);
}

TEST(TreeRemoveTest, Stressed) {
  struct tree t;
  tree_create(&t);

  std::srand(0);

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();
    tree_insert(&t, value);
  }

  size_t expected = tree_size(&t);
  EXPECT_TRUE(expected > 0);

  std::srand(0); // reinit

  for (int i = 0; i < BIG_SIZE; ++i) {
    int value = rand();

    bool present = tree_contains(&t, value);

    if (present) {
      --expected;
    }

    bool removed = tree_remove(&t, value);

    EXPECT_EQ(present, removed);
    EXPECT_EQ(tree_size(&t), expected);
  }

  EXPECT_EQ(tree_size(&t), 0u);

  tree_destroy(&t);
}

/*
 * tree_walk_in_order
 */

static void check_tree(int value, void *user_data) {
  int *expected = static_cast<int *>(user_data);

  ASSERT_TRUE(expected != NULL);
  EXPECT_EQ(*expected, value);

  (*expected) += 2;
}

TEST(TreeWalkInOrderTest, Ordered) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  EXPECT_EQ(tree_size(&t), std::size(origin));

  int expected = 2;
  tree_walk_in_order(&t, check_tree, &expected);
  EXPECT_EQ(expected, 20);

  tree_destroy(&t);
}

static void check_once(int value, void *user_data) {
  int *count = static_cast<int *>(user_data);

  ASSERT_TRUE(count != NULL);
  ASSERT_TRUE(2 <= value && value <= 18);
  EXPECT_EQ(count[value / 2], 0);

  count[value / 2] = 1;
}

TEST(TreeWalkInOrderTest, Count) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  int count[10];

  std::memset(count, 0, sizeof count);
  tree_walk_in_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (std::size_t i = 1; i < std::size(count); ++i) {
    EXPECT_EQ(count[i], 1);
  }

  tree_destroy(&t);
}

/*
 * tree_walk_pre_order
 */

TEST(TreeWalkPreOrderTest, Count) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  int count[10];

  std::memset(count, 0, sizeof count);
  tree_walk_pre_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (std::size_t i = 1; i < std::size(count); ++i) {
    EXPECT_EQ(count[i], 1);
  }

  tree_destroy(&t);
}

/*
 * tree_walk_post_order
 */

TEST(TreeWalkPostOrderTest, Count) {
  static const int origin[] = { 16, 2, 8, 4, 10, 18, 6, 12, 14 };

  struct tree t;
  tree_create(&t);

  for (std::size_t i = 0; i < std::size(origin); ++i) {
    tree_insert(&t, origin[i]);
  }

  int count[10];

  std::memset(count, 0, sizeof count);
  tree_walk_post_order(&t, check_once, count);

  EXPECT_EQ(count[0], 0);

  for (std::size_t i = 1; i < std::size(count); ++i) {
    EXPECT_EQ(count[i], 1);
  }

  tree_destroy(&t);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
