eco
===

eco is a generic C++ library of efficient components.

# Algorithms

## Linear search algorithms

`find_not` takes a `std::ranges::input_range`, a value, and optionally a
projection. It returns the first position in the range with a projected element
not equal to the given value. If all elements in the range are equal to the
given value, it returns the end position of the range.

Example: `<find_not, 1> [1, 2, 3] -> @2`

`find_n` takes a `std::ranges::input_iterator` and a count, a value, and
optionally a projection. It returns the first position in the range with a
projected element equal to the given value, along with the remaining count.

Example: `<find_n, 3, 2> [1, 2, 3] -> [@2, 1]`

`find_not_n` takes a `std::ranges::input_iterator` and a count, a value, and
optionally a projection. It returns the first position in the range with a
projected element not equal to the given value, along with the remaining count.

Example: `<find_not_n, 3, 2> [1, 2, 3] -> [@1, 2]`

`find_if_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It returns the first position in the
range with a projected element satisfying the given predicate, along with the
remaining count.

Example: `<find_if_n, 3, (>1)> [1, 2, 3] -> [@2, 1]`

`find_if_not_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It returns the first position in the
range with a projected element not satisfying the given predicate, along with
the remaining count.

Example: `<find_if_not_n, 3, (>2)> [1, 2, 3] -> [@1, 2]`

`find_if_unguarded` takes a `std::ranges::input_iterator`, a unary predicate,
and optionally a projection. It returns the first position in the range with a
projected element satisfying the given predicate. It is a precondition that at
least one element satisfies the given predicate.

Example: `<find_if_unguarded, 2> [1, 2, 3] -> @2`

`find_if_not_unguarded` takes a `std::ranges::input_iterator`, a unary
predicate, and optionally a projection. It returns the first position in the
range with a projected element not satisfying the given predicate. It is a
precondition that at least one element does not satisfy the given predicate.

Example: `<find_if_not_unguarded, 2> [1, 2, 3] -> @1`

`not_all_of` takes a `std::ranges::input_range`, a unary predicate, and
optionally a projection. It returns true iff not all of the projected elements
in the range satisfy the given predicate.

Example: `<not_all_of, (>3)> [1, 2, 3] -> true`

`all_of_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It returns true iff all of the projected
elements in the range satisfy the given predicate.

Example: `<all_of_n, 3, (<4)> [1, 2, 3] -> true`

`none_of_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It returns true iff none of the
projected elements in the range satisfy the given predicate.

Example: `<none_of_n, 3, (>3)> [1, 2, 3] -> true`

`not_all_of_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It returns true iff not all of the
projected elements in the range satisfy the given predicate.

Example: `<not_all_of_n, 3, (<2)> [1, 2, 3] -> true`

`any_of_n` takes a `std::ranges::input_iterator` and a count, a unary predicate,
and optionally a projection. It returns true iff any of the projected elements
in the range satisfy the given predicate.

Example: `<any_of_n, 3, (>1)> [1, 2, 3] -> true`

`count_not` takes a `std::ranges::input_range`, a value, and optionally a
projection. It counts the number of projected elements that are not equal to the
given predicate.

Example: `<count_not, (2)> [1, 2, 3] -> 2`

`count_if_not` takes a `std::ranges::input_range`, a unary predicate, and
optionally a projection. It counts the number of projected elements that don't
satisfy the given predicate.

Example: `<count_if_not, (<2)> [1, 2, 3] -> 2`

`count_n` takes a `std::ranges::input_iterator` and a count, a value, and
optionally a projection. It counts the number of projected elements that are
equal to the given value.

Example: `<count_n, 3, 2> [1, 2, 3] -> 1`

`count_not_n` takes a `std::ranges::input_iterator` and a count, a value, and
optionally a projection. It counts the number of projected elements that are
not equal to the given value.

Example: `<count_not_n, 3, 2> [1, 2, 3] -> 2`

`count_if_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It counts the number of projected
elements that satisfy the given predicate.

Example: `<count_if_n, 3, (<2)> [1, 2, 3] -> 1`

`count_if_not_n` takes a `std::ranges::input_iterator` and a count, a unary
predicate, and optionally a projection. It counts the number of projected
elements that does not satisfy the given predicate.

Example: `<count_if_not_n, 3, (<2)> [1, 2, 3] -> 2`

## Fold algorithms

`fold_left` takes a `std::ranges::input_range`, an identity value,
a binary operator, and a projection, which defaults to `std::identity`.
It applies the operator to the projected elements in the range,
from left to right.
If the range is empty, it returns the identity value.

Example: `<fold_left, 0, +> [] -> 0`
Example: `<fold_left, 0, +> [1 2 3 4] -> 10`
Example: `<fold_left, 0, -> [] -> 0`
Example: `<fold_left, 0, -> [1 2 3 4] -> -8`
Example: `<fold_left, 1, *> [] -> 1`
Example: `<fold_left, 1, *> [1 2 3 4] -> 24`

`fold_left_nonempty` omits the identity value and requires the range to be
nonempty.

`fold_right` takes a `std::ranges::bidirectional_range`, an identity value,
a binary operator, and a projection, which defaults to `std::identity`.
It applies the operator to the projected elements in the range,
from right to left.
If the range is empty, it returns the identity value.

Example: `<fold_right, 0, +> [] -> 0`
Example: `<fold_right, 0, +> [1 2 3 4] -> 10`
Example: `<fold_right, 0, -> [] -> 0`
Example: `<fold_right, 0, -> [1 2 3 4] -> -2`
Example: `<fold_right, 1, *> [] -> 1`
Example: `<fold_right, 1, *> [1 2 3 4] -> 24`

`fold_right_nonempty` omits the identity value and requires the range to be
nonempty.

`fold_left_binary` takes two `std::input_range`s, an identity value,
two binary operators, and two projection functions,
which default to `std::identity`.
It pairwise maps all projected elements in the two ranges,
using the second operator, and reduces the results using the first operator,
from left to right.
It requires the second range not to be shorter than the first.

Example: `<fold_left_binary, 0, +, *> [0 1 2 3] [2 3 4 5] -> 26`
Example: `<fold_left_binary, 0, +, *> [0 1 2 3] [2 3 4 5 6] -> 26`
Example: `<fold_left_binary, 0, +, *> [] [2 3 4 5 6] -> 0`

`fold_left_binary_nonempty` omits the identity value and requires the range to
be nonempty.

`fold_right_binary` takes two `std::input_range`s, an identity value,
two binary operators, and two projection functions,
which default to `std::identity`.
It pairwise maps all projected elements in the two ranges,
using the second operator, and reduces the results using the first operator,
from right to left.
It requires the second range not to be shorter than the first.

Example: `<fold_right_binary, 0, +, *> [0 1 2 3] [2 3 4 5] -> 26`
Example: `<fold_right_binary, 0, +, *> [0 1 2 3] [2 3 4 5 6] -> 32`
Example: `<fold_right_binary, 0, +, *> [] [2 3 4 5] -> 0`

`fold_right_binary_nonempty` omits the identity value and requires the range to
be nonempty.

## List algorithms

`reverse_append` takes a forward range, a `std::forward_iterator` to the head of
a list, and a `forward_linker`.
It appends the forward range in reversed element order to the head, effectively
making the last element of the original range the new head.

Example: `[1 2 3 4] <reverse_append> [5 6 7 8] -> [4 3 2 1 5 6 7 8]`

## Tree algorithms

`tree_traverse_step` takes a `df_visit` enumeration and a
`bidirectional_bicursor`.
It traverses a binary tree in depth-first order, step by step, returning the
change in tree height on the taken traversal step.
The `df_visit` is updated to indicate the visitation step, (each node is visited
3 times, as `df_visit::pre`, `df_visit::in`, `df_visit::post`) and the
`bidirectional_bicursor` is updated to point to the tree node
visited.

`tree_weight` takes a `bidirectional_bicursor`.
It calculates the weight of a tree rooted at the cursor.

`tree_height` takes a `bidirectional_bicursor`.
It calculates the height of a tree rooted at the cursor.

# Data structures

## Arrays

### `array`

`array` is a type constructor for dynamic arrays, similar to `std::vector`,
but with some key differences:

- The local storage of `array` is a single pointer, making storage of nested
arrays where many of the inner arrays are empty more efficient.
- Fewer member types and member functions, leveraging the capabilities of the
C++20 ranges library instead.
- Only designed to store value types and free of specializations with custom
semantics, like `std::vector<bool>`.
- The growth algorithm is configurable and by default grows the reserve area by
a factor of 1.5, which saves memory and when used with many allocators prevents
it from reusing previously allocated memory blocks.
- Support for custom allocators (as defined by concepts in this library) but
without support for storing stateful allocators in the `array` itself, which
makes it easier to reason about ownership semantics.
- No support for incomplete value type.

The type parameter `T` is the type of objects stored in the `array`.

`array<T>` models `std::ranges::contiguous_range<T>`.

`array<T>` models `std::semiregular` if `std::ranges::range_value_t<T>` does.

`array<T>` models `std::regular` if `std::ranges::range_value_t<T>` does.

`array<T>` models `std::totally_ordered` if `std::ranges::range_value_t<T>`
does.

The value parameter `ga` is the growth algorithm used when more space is needed.
`default_array_growth` is a function object that takes a non-negative capacity
and a lower bound. It returns `capacity + max(capacity / 2, lower_bound)`, which
gives an allocator-friendly growth factor that guarantees amortized constant
time stack operations.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` uses `malloc_allocator`.

The member type `value_type` is the type of objects stored in the `array`.

The member type `ssize_type` is a signed type large enough to represent the
maximum possible size of the `array`.

`array` can be constructed with an initial capacity, or constructed and assigned
from a  `std::ranges::forward_range`.

`array` supports the following operations:

- `swap(x)` swaps the `array` with `array` `x`.
- `bool{x}` returns `true` iff the `array` is not empty.
- `x[i]` accesses the element at index `i`.
- `begin()` returns an iterator to the beginning of the `array`.
- `end()` returns an iterator to the end of the `array`.
- `size()` returns the size of the `array`.
- `capacity()` returns the size of the allocated space in the `array`.
- `max_size()` returns the maximum number of elements the `array` can contain.
- `reserve(i)` ensures there is reserved space for up to `i` elements.
- `shrink_to_fit()` ensures `capacity() == size()`.
- `push_back(args)` appends an element constructed from `args`.
- `pop_back()` removes the last element.
- `append(range)` appends a `std::forward_range` of elements.
- `insert(pos, args)` inserts an element constructed from `args` after `pos`.
- `insert(pos, range)` inserts a `std::forward_range` of elements after `pos`.
- `erase(pos)` erases the element at `pos`.
- `erase(range)` erases a range of elements (`range` must be in the `array`).
- `clear()` erases all elements in the `array`, without changing capacity.
- `resize(x, size, value)` resizes `x`, appending `value`s if `size > x.size()`.

### `array_dict`

`array_dict` is a type constructor for associative arrays, where elements are
stored contiguously, but in an undefined order. The elements are indexed by a
key and offers the following tradeoffs compared to an `array`:

- Constant-time erasure of elements, suitable in situations where arbitrary
elements are frequently erased in between insertions.
- Elements are not necessarily stored in insertion order.
- A memory overhead compared to `array` due to the need to store key/value map.
- No range construction/insertion. Elements have to be inserted individually
with the caller tracking the returned keys.
- The comparison operators are slower because elements are unordered.
- Local storage is not a single pointer.

The type parameter `T` is the type of objects stored in the `array_dict`.

`array_dict<T>` models `std::ranges::contiguous_range<T>`.

`array_dict<T>` models `std::semiregular` if `std::ranges::range_value_t<T>`
does.

`array_dict<T>` models `std::regular` if `std::ranges::range_value_t<T>` does.

`array_dict<T>` models `std::totally_ordered` if `std::ranges::range_value_t<T>`
does.

The type parameter `Key` is the type of the key. It must model
`std::signed_integral`. `std::int32_t` is the default.

The value parameter `ga` is the growth algorithm used when more space is needed.
`default_array_growth` is the default.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` uses `malloc_allocator`.

The member type `value_type` is the type of objects stored in the `array_dict`.

The member type `ssize_type` is a signed type large enough to represent the
maximum possible size of the `array_dict`.

`array_dict` can be constructed with an initial capacity.

`array_dict` supports the following operations:

- `swap(x)` swaps the `array_dict` with `array_dict` `x`.
- `bool{x}` returns `true` iff the `array_dict` is not empty.
- `x[key]` accesses the element with key `key`.
- `begin()` returns an iterator to the beginning of the `array_dict`.
- `end()` returns an iterator to the end of the `array_dict`.
- `size()` returns the size of the `array_dict`.
- `capacity()` returns the size of the allocated space in the `array_dict`.
- `max_size()` returns the maximum number of elements the `array_dict` can
contain.
- `reserve(i)` ensures there is reserved space for up to `i` elements.
- `shrink_to_fit()` ensures `capacity() == size()`.
- `has_key(key)` returns `true` iff `key` maps to an element in the
`array_dict`.
- `insert(args)` inserts an element constructed from `args`, returnings its key.
- `erase(key)` erases the element with the key `key`.
- `clear()` erases all elements in the `array_dict`, without changing capacity.

### `fixed_array`

`fixed_array` is a type constructor for dynamic arrays of a fixed bit size. It
stores `std::unsigned_integral` values (where some leftmost bits are unused)
compactly. The key differences compared to an `array` are.

- Only supports `std::unsigned_integral` values.
- The bit-packing makes element access slower, but the smaller memory footprint
makes the data more cache-friendly.
- Elements are stored in an underlying `array` of `value_type`. When setting a
capacity it may exceed the capacity required to store the given number of
elements.
- `bit_size_v` of the chosen `value_type` should ideally be a multiple of the
chosen bit size, to avoid values straddling two elements of the underlying
`array`.

The value parameter `w` is the bit size of elements stored in the array. It
satisfies `0 < w <= bit_size_v<T>`.

The type parameter `T` is the type of objects stored in the `array`. It must
model `std::unsigned_integral`.

`array<T>` models `std::ranges::random_access_range<T>`.

`array<T>` models `std::totally_ordered`.

The value parameter `ga` is the growth algorithm used when more space is needed.
`default_array_growth` is the default.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` is the default.

The member type `value_type` is the type of objects stored in the `fixed_array`.

The member type `ssize_type` is a signed type large enough to represent the
maximum possible size of the `fixed_array`.

The member type `reference` is a proxy reference to elements of the
`fixed_array`.

The member type `iterator` is an iterator to elements of the `fixed_array`. It
models `std::random_access_iterator`.

The member type `const_iterator` is an iterator to elements of the `fixed_array`
with immutable access to the elements. It models `std::random_access_iterator`.

`fixed_array` can be constructed with an initial capacity, or constructed and
assigned from a `std::ranges::forward_range`.

- `swap(x)` swaps the `fixed_array` with `fixed_array` `x`.
- `bool{x}` returns `true` iff the `fixed_array` is not empty.
- `x[i]` accesses the element at index `i`.
- `begin()` returns an iterator to the beginning of the `fixed_array`.
- `cbegin()` returns an iterator to the beginning of the `fixed_array`.
- `end()` returns an iterator to the end of the `fixed_array`.
- `cend()` returns an iterator to the end of the `fixed_array`.
- `size()` returns the size of the `fixed_array`.
- `capacity()` returns the size of the allocated space in the `fixed_array`.
- `max_size()` returns the approximate maximum number of elements the
`fixed_array` can contain.
- `reserve(i)` ensures there is reserved space for up to `i` elements.
- `shrink_to_fit()` minimizes reserved space.
- `push_back(args)` appends an element constructed from `args`.
- `pop_back()` removes the last element.
- `append(range)` appends a `std::forward_range` of elements.
- `insert(pos, args)` inserts an element constructed from `args` after `pos`.
- `insert(pos, range)` inserts a `std::forward_range` of elements after `pos`.
- `erase(pos)` erases the element at `pos`.
- `erase(range)` erases a range of elements (`range` must be in the
`fixed_array`).
- `clear()` erases all elements in the `fixed_array`, without changing capacity.
- `resize(x, size, value)` resizes `x`, appending `value`s if `size > x.size()`.

## Bitvectors

`bitvector` describes a `std::regular` type with the following operations:

- `size()` returns the number of stored bits.
- `bitread(i)` returns bit `i`.
- `bitset(i)` sets bit `i` to `1`.
- `bitclear(i)` sets bit `i` to `0`.
- `init()` performs post-construction initialization for faster access.
- `rank_0(i)` returns the number of 0-bits in the range `[0, i)`.
- `rank_1(i)` returns the number of 1-bits in the range `[0, i)`.
- `select_0(i)` returns the position of the `i`:th 0-bit.
- `select_1(i)` returns the position of the `i`:th 1-bit.
- `init` preprocesses the `bitvector` for efficient `rank` and `select` queries.

`basic_bitvector` is a type constructor for a `bitvector` that stores bits in an
uncompressed form.

## Trees

### Binary trees

`binary_louds` (Level-Order Unary Degree Sequence) is a compact representation
of a binary tree. It can be constructed from a binary tree of a known size with
a pair of `bidirectional_bicursor`s.

- `root()` returns the root node of the tree.
- `parent(v)` returns the parent of node `v`.
- `has_left_child(v)` returns `true` iff node `v` has a left child.
- `has_right_child(v)` returns `true` iff node `v` has a right child.
- `is_leaf(v)` returns `true` iff node `v` is a leaf.
- `left_child(v)` returns the left child of node `v`.
- `right_child(v)` returns the right child of node `v`.
- `child_label(v)` returns the label of the edge leading to node `v`.

### Ordinal trees

`louds` (Level-Order Unary Degree Sequence) is a compact representation of an
ordinal tree. It can be constructed from a tree of a known size with a pair of
`linked_bicursor`s, where the `left_branch` points to the first child of a node
and `right branch` points to the next sibling.

- `root()` returns the root node of the tree.
- `first_child(v)` returns the first child of node `v`.
- `last_child(v)` returns the last child of node `v`.
- `next_sibling(v)` returns the next sibling of node `v`.
- `prev_sibling(v)` returns the previous sibling of node `v`.
- `parent(v)` returns the parent of node `v`.
- `is_leaf(v)` returns `true` iff node `v` is a leaf.
- `nodemap(v)` returns a unique identifier of node `v`, called its _index_.
- `nodeselect(i)` returns the node with index `i`.
- `children(v)` returns the number of children of node `v`.
- `child(v, n)` returns the `n`:th child of node `v`.
- `child_rank(v)` returns the `n` such that node `v` is the `n`:th child of
its parent.
- `lca(t, u, v)` returns the lowest common ancestor of nodes `u` and `v` in `t`.

## Sequences

Components for implemention of sequence types, such as containers.

### Extents

`extent` is a type constructor for storage of arrays of objects in a contiguous
memory region, where elements are prefixed by a header that keeps track of the
number of elements stored and the number of elements that can be stored without
a reallocation. Additional metadata can also be stored in the header.
`extent` models `std::contiguous_range` and owns the elements it stores. It
supports resizing of the memory region and provides automatic growth when
inserting elements.

`extent` can be customized for many different use cases.

The type parameter `T` is the type of objects stored in the memory region.

The type parameter `Size` is the type
used to store size and capacity in the header. By using a smaller type when the
maximum number of elements is small the header size can be reduced. By default
it is equivalent to `std::ptrdiff_t`.

The type parameter `Metadata` is the type of an optional extra field in the
header. By default it is `std::monostate` and no bytes are used for it in the
header. It can be used for optimizing element operations when the data encoding
requires it.

The value parameter `copier` is an object with member functions `copy`, `move`,
`move_backward`, and `destroy` that is used to copy, move and delete
elements when necessary. By default it is `default_array_copy`, which provides
efficient and exception-safe range operations on (possibly) uninitialized arrays
of its value type.

The value parameter `ga` is the growth algorithm used when more space is needed.
By default it is `default_array_growth`.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` uses `malloc_allocator`.

### List pools

`forward_list_pool` is a type constructor for a pool of singly linked list nodes,
stored in a contiguous memory region. It uses `extent` to store the list nodes.
The lifetime of elements in the list nodes is bound to the lifetime of the pool,
not individual lists in it.

`forward_list_pool` can be customized for many different use cases.

The type parameter `T` is the type of objects stored in the elements of the pool.

The type parameter `Size` is the type used to store links in the list nodes.
Using a type not larger than required to store the maximum number of nodes in
the pool minimizes the memory footprint and makes the pool more cache-friendly.

`list_pool` is a type constructor for a pool of doubly linked list nodes,
stored in a contiguous memory region. It uses `extent` to store the list nodes.
The lifetime of elements in the list nodes is bound to the lifetime of the pool,
not individual lists in it.

`list_pool` can be customized for many different use cases.

The type parameter `T` is the type of objects stored in the elements of the pool.

The type parameter `Size` is the type used to store links in the list nodes.
Using a type not larger than required to store the maximum number of nodes in
the pool minimizes the memory footprint and makes the pool more cache-friendly.

# Memory

Low-level memory components used in the implementation of eco data structures.

## Memory regions

`memory_view` is a view of a (logically) contiguous memory region beginning at
`first` and containing `size` bytes of data. It is the type used throughout the
library to describe memory regions with arbitrary contents.
`memory_view` models `std::totally_ordered` and is ordered first by memory
address and second by size.
`memory_view` also models `std::ranges::contiguous_range`.
A boolean test on a `memory_view` checks if the contained memory address is not
equal to `nullptr`.
`begins_in(x)` returns `true` iff the `memory_view` begins in `memory_view` `x`.
`ends_in(x)` returns `true` iff the `memory_view` ends in `memory_view` `x`.
`is_in` returns `true` iff the `memory_view` begins and ends in `memory_view`
`x`.

## Allocators

Allocators are objects responsible for dynamically allocating contiguous memory
regions. Unlike standard library allocators, eco allocators are not concerned
with the types of the data the memory is allocated for. eco allocators are not
necessarily capable of multiple allocations, deallocations or ownership
detection.

`allocator` describes a type with a member function `allocate` that is used to
allocate raw memory and returns a `memory_view` of that memory. It returns
a `memory_view` testable as `false` if allocation fails.

`deallocatable_allocator` describes an `allocator` type with a member function
`deallocate` that is used to deallocate previously allocated memory by the same
allocator object. It returns `false` if deallocation fails.

`reallocatable_allocator` describes an `allocator` type with a member function
`reallocate` that can reallocate memory previously allocated by the same object.
It returns a memory view testable as `false` if reallocation fails.

`ownership_aware_allocator` describes an `allocator` type with a member function
`is_owner` that takes a `memory_view` and tells if the associated memory was
allocated with this object or not.

### malloc_allocator

`malloc_allocator` is a type that uses `std::malloc` and associated functions
to manage dynamic memory. It models `deallocatable_allocator` and
`reallocatable_allocator`.

This is the default allocators used in eco containers.

### arena_allocator

`arena_allocator` is a type constuctor for allocators that pre-allocate a given
number of bytes on construction, using a given `deallocatable_allocator`. It
models `deallocatable_allocator`.

This allocator is useful for situations where allocation and deallocation must
be as fast as possible and the total amount of memory that will be used is
known beforehand.

`can_allocate(n)` returns `true` iff `n` bytes can still be allocated.

`allocate` allocates from the beginning of previously unused memory in this
arena.

`deallocate` does nothing.

`deallocate_all` deallocates the entire arena.
