#ifndef ECO_TEST_ORDINAL_TREE_
#define ECO_TEST_ORDINAL_TREE_

import std;
import eco;

#include <cassert>

inline void
test_louds()
{
  ptree_node node_1{1, nullptr, nullptr};
  ptree_node node_2{2, nullptr, nullptr};
  node_1.first_child = &node_2;
  ptree_node node_3{3, nullptr, nullptr};
  node_2.next_sibling = &node_3;
  ptree_node node_4{4, nullptr, nullptr};
  node_3.next_sibling = &node_4;
  ptree_node node_5{5, nullptr, nullptr};
  node_2.first_child = &node_5;
  ptree_node node_6{6, nullptr, nullptr};
  node_5.next_sibling = &node_6;
  ptree_node node_7{7, nullptr, nullptr};
  node_3.first_child = &node_7;
  ptree_node node_8{8, nullptr, nullptr};
  node_7.next_sibling = &node_8;
  ptree_node node_9{9, nullptr, nullptr};
  node_7.first_child = &node_9;
  ptree_node node_10{10, nullptr, nullptr};
  node_9.next_sibling = &node_10;
  ptree_node node_11{11, nullptr, nullptr};
  node_8.first_child = &node_11;
  ptree_node node_12{12, nullptr, nullptr};
  node_9.first_child = &node_12;
  ptree_node node_13{13, nullptr, nullptr};
  node_11.first_child = &node_13;
  ptree_node node_14{14, nullptr, nullptr};
  node_13.next_sibling = &node_14;
  ptree_node node_15{15, nullptr, nullptr};
  node_14.next_sibling = &node_15;
  ptree_node node_16{16, nullptr, nullptr};
  node_15.next_sibling = &node_16;
  ptree_node node_17{17, nullptr, nullptr};
  node_12.first_child = &node_17;
  ptree_node node_18{18, nullptr, nullptr};
  node_17.next_sibling = &node_18;
  ptree_node node_19{19, nullptr, nullptr};
  node_18.next_sibling = &node_19;
  ptree_node node_20{20, nullptr, nullptr};
  node_14.first_child = &node_20;

  ptree_cursor root;
  root.value = &node_1;

  ptree_cursor nil;

  assert(root);
  assert(has_left_successor(root));
  assert(!has_right_successor(root));
  assert(left_successor(root).value->value == 2);

  eco::louds x{root, nil, 20};

  assert(x.root() == 2);

  assert(x.first_child(x.root()) == 6);
  assert(x.first_child(x.first_child(x.root())) == 13);

  assert(x.last_child(x.root()) == 12);
  assert(x.last_child(x.first_child(x.root())) == 14);

  assert(x.next_sibling(x.first_child(x.root())) == 9);
  assert(x.next_sibling(x.first_child(x.first_child(x.root()))) == 14);

  assert(x.prev_sibling(x.last_child(x.root())) == 9);
  assert(x.prev_sibling(x.last_child(x.first_child(x.root()))) == 13);

  assert(x.parent(x.first_child(x.root())) == 2);
  assert(x.parent(x.last_child(x.root())) == 2);
  assert(x.parent(x.first_child(x.first_child(x.root()))) == 6);
  assert(x.parent(x.last_child(x.first_child(x.root()))) == 6);

  assert(x.is_leaf(x.root()) == false);
  assert(x.is_leaf(x.first_child(x.root())) == false);
  assert(x.is_leaf(x.first_child(x.first_child(x.root()))) == true);
  assert(x.is_leaf(x.last_child(x.root())) == true);

  assert(x.node_map(x.root()) == 0);
  assert(x.node_map(x.first_child(x.root())) == 1);
  assert(x.node_map(x.last_child(x.root())) == 3);
  assert(x.node_map(x.first_child(x.first_child(x.root()))) == 4);
  assert(x.node_map(x.last_child(x.first_child(x.root()))) == 5);

  assert(x.node_select(0) == x.root());
  assert(x.node_select(1) == x.first_child(x.root()));
  assert(x.node_select(3) == x.last_child(x.root()));
  assert(x.node_select(4) == x.first_child(x.first_child(x.root())));
  assert(x.node_select(5) == x.last_child(x.first_child(x.root())));

  assert(x.children(x.root()) == 3);
  assert(x.children(x.first_child(x.root())) == 2);
  assert(x.children(x.last_child(x.root())) == 0);
  assert(x.children(x.first_child(x.first_child(x.root()))) == 0);
  assert(x.children(x.next_sibling(x.first_child(x.root()))) == 2);

  assert(x.child(x.root(), 0) == 6);
  assert(x.child(x.first_child(x.root()), 0) == 13);
  assert(x.child(x.first_child(x.root()), 1) == 14);
  assert(x.child(x.next_sibling(x.first_child(x.root())), 0) == 15);
  assert(x.child(x.next_sibling(x.first_child(x.root())), 1) == 18);

  assert(x.child_rank(x.first_child(x.root())) == 0);
  assert(x.child_rank(x.first_child(x.first_child(x.root()))) == 0);
  assert(x.child_rank(x.last_child(x.first_child(x.root()))) == 1);
  assert(x.child_rank(x.last_child(x.root())) == 2);

  assert(x.lca(x.root(), x.root()) == x.root());
  assert(x.lca(x.first_child(x.root()), x.last_child(x.root())) == x.root());
  assert(x.lca(x.first_child(x.first_child(x.root())), x.last_child(x.first_child(x.root()))) == x.first_child(x.root()));

  auto pos = x.begin();
  auto end = x.end();

  assert(*pos == 6);
  ++pos;
  assert(*pos == 13);
  ++pos;
  assert(*pos == 13);
  ++pos;
  assert(*pos == 14);
  ++pos;
  assert(*pos == 14);
  ++pos;
  assert(*pos == 6);
  ++pos;
  assert(*pos == 9);
  ++pos;
  assert(*pos == 15);
  ++pos;
  assert(*pos == 20);
  ++pos;
  assert(*pos == 28);
  ++pos;
  assert(*pos == 37);
  ++pos;
  assert(*pos == 37);
  ++pos;
  assert(*pos == 38);
  ++pos;
  assert(*pos == 38);
  ++pos;
  assert(*pos == 39);
  ++pos;
  assert(*pos == 39);
  ++pos;
  assert(*pos == 28);
  ++pos;
  assert(*pos == 20);
  ++pos;
  assert(*pos == 22);
  ++pos;
  assert(*pos == 22);
  ++pos;
  assert(*pos == 15);
  ++pos;
  assert(*pos == 18);
  ++pos;
  assert(*pos == 23);
  ++pos;
  assert(*pos == 32);
  ++pos;
  assert(*pos == 32);
  ++pos;
  assert(*pos == 33);
  ++pos;
  assert(*pos == 40);
  ++pos;
  assert(*pos == 40);
  ++pos;
  assert(*pos == 33);
  ++pos;
  assert(*pos == 35);
  ++pos;
  assert(*pos == 35);
  ++pos;
  assert(*pos == 36);
  ++pos;
  assert(*pos == 36);
  ++pos;
  assert(*pos == 23);
  ++pos;
  assert(*pos == 18);
  ++pos;
  assert(*pos == 9);
  ++pos;
  assert(*pos == 12);
  ++pos;
  assert(*pos == 12);
  ++pos;
  assert(pos == end);

  --pos;
  assert(*pos == 12);
  --pos;
  assert(*pos == 12);
  --pos;
  assert(*pos == 9);
  --pos;
  assert(*pos == 18);
  --pos;
  assert(*pos == 23);
  --pos;
  assert(*pos == 36);
  --pos;
  assert(*pos == 36);
  --pos;
  assert(*pos == 35);
  --pos;
  assert(*pos == 35);
  --pos;
  assert(*pos == 33);
  --pos;
  assert(*pos == 40);
  --pos;
  assert(*pos == 40);
  --pos;
  assert(*pos == 33);
  --pos;
  assert(*pos == 32);
  --pos;
  assert(*pos == 32);
  --pos;
  assert(*pos == 23);
  --pos;
  assert(*pos == 18);
  --pos;
  assert(*pos == 15);
  --pos;
  assert(*pos == 22);
  --pos;
  assert(*pos == 22);
  --pos;
  assert(*pos == 20);
  --pos;
  assert(*pos == 28);
  --pos;
  assert(*pos == 39);
  --pos;
  assert(*pos == 39);
  --pos;
  assert(*pos == 38);
  --pos;
  assert(*pos == 38);
  --pos;
  assert(*pos == 37);
  --pos;
  assert(*pos == 37);
  --pos;
  assert(*pos == 28);
  --pos;
  assert(*pos == 20);
  --pos;
  assert(*pos == 15);
  --pos;
  assert(*pos == 9);
  --pos;
  assert(*pos == 6);
  --pos;
  assert(*pos == 14);
  --pos;
  assert(*pos == 14);
  --pos;
  assert(*pos == 13);
  --pos;
  assert(*pos == 13);
  --pos;
  assert(*pos == 6);
}

inline void
test_bp_tree()
{
  eco::basic_bitvector b{40};
  b.bit_set(0);
  b.bit_set(1);
  b.bit_set(2);
  b.bit_set(4);
  b.bit_set(7);
  b.bit_set(8);
  b.bit_set(9);
  b.bit_set(10);
  b.bit_set(11);
  b.bit_set(13);
  b.bit_set(15);
  b.bit_set(19);
  b.bit_set(22);
  b.bit_set(23);
  b.bit_set(24);
  b.bit_set(26);
  b.bit_set(27);
  b.bit_set(30);
  b.bit_set(32);
  b.bit_set(37);

  eco::bp_tree x{std::ranges::begin(b), std::ranges::end(b)};

  assert(x.root() == 0);

  assert(x.first_child(x.root()) == 1);
  assert(x.first_child(x.first_child(x.root())) == 2);

  assert(x.last_child(x.root()) == 37);
  assert(x.last_child(x.first_child(x.root())) == 4);

  assert(x.next_sibling(x.first_child(x.root())) == 7);

  assert(x.prev_sibling(x.last_child(x.root())) == 7);

  assert(x.parent(x.first_child(x.root())) == 0);
  assert(x.parent(x.last_child(x.root())) == 0);
  assert(x.parent(x.first_child(x.first_child(x.root()))) == 1);
  assert(x.parent(x.last_child(x.first_child(x.root()))) == 1);

  assert(x.is_leaf(x.root()) == false);
  assert(x.is_leaf(x.first_child(x.root())) == false);
  assert(x.is_leaf(x.first_child(x.first_child(x.root()))) == true);
  assert(x.is_leaf(x.last_child(x.root())) == true);

  assert(x.node_map(x.root()) == 0);
  assert(x.node_map(x.first_child(x.root())) == 1);
  assert(x.node_map(x.last_child(x.root())) == 19);
  assert(x.node_map(x.first_child(x.first_child(x.root()))) == 2);
  assert(x.node_map(x.last_child(x.first_child(x.root()))) == 3);

  assert(x.node_select(0) == x.root());
  assert(x.node_select(1) == x.first_child(x.root()));
  assert(x.node_select(2) == x.first_child(x.first_child(x.root())));
  assert(x.node_select(3) == x.last_child(x.first_child(x.root())));
  assert(x.node_select(4) == x.next_sibling(x.first_child(x.root())));

  assert(x.preorder(10) == 7);
  assert(x.preorder_select(7) == 10);

  assert(x.postorder(8) == 9);
  assert(x.postorder_select(9) == 8);

  assert(x.depth(8) == 3);

  assert(x.subtree(8) == 6);

  assert(!x.is_ancestor(x.node_select(9), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(8), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(7), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(6), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(5), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(4), x.node_select(8)));
  assert(!x.is_ancestor(x.node_select(3), x.node_select(8)));
  assert(!x.is_ancestor(x.node_select(2), x.node_select(8)));
  assert(!x.is_ancestor(x.node_select(1), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(0), x.node_select(8)));

  assert(x.level_ancestor(19, 2) == 7);

  assert(x.height(22) == 3);

  assert(x.children(7) == 2);

  assert(x.child(x.root(), 0) == 1);
  assert(x.child(x.root(), 1) == 7);
  assert(x.child(x.root(), 2) == 37);
  assert(x.child(7, 0) == 8);
  assert(x.child(7, 1) == 22);

  assert(x.child_rank(22) == 2);

  assert(x.lca(10, 23) == 7);

  auto pos = x.begin();
  auto end = x.end();

  assert(*pos == 1);
  ++pos;
  assert(*pos == 2);
  ++pos;
  assert(*pos == 2);
  ++pos;
  assert(*pos == 4);
  ++pos;
  assert(*pos == 4);
  ++pos;
  assert(*pos == 1);
  ++pos;
  assert(*pos == 7);
  ++pos;
  assert(*pos == 8);
  ++pos;
  assert(*pos == 9);
  ++pos;
  assert(*pos == 10);
  ++pos;
  assert(*pos == 11);
  ++pos;
  assert(*pos == 11);
  ++pos;
  assert(*pos == 13);
  ++pos;
  assert(*pos == 13);
  ++pos;
  assert(*pos == 15);
  ++pos;
  assert(*pos == 15);
  ++pos;
  assert(*pos == 10);
  ++pos;
  assert(*pos == 9);
  ++pos;
  assert(*pos == 19);
  ++pos;
  assert(*pos == 19);
  ++pos;
  assert(*pos == 8);
  ++pos;
  assert(*pos == 22);
  ++pos;
  assert(*pos == 23);
  ++pos;
  assert(*pos == 24);
  ++pos;
  assert(*pos == 24);
  ++pos;
  assert(*pos == 26);
  ++pos;
  assert(*pos == 27);
  ++pos;
  assert(*pos == 27);
  ++pos;
  assert(*pos == 26);
  ++pos;
  assert(*pos == 30);
  ++pos;
  assert(*pos == 30);
  ++pos;
  assert(*pos == 32);
  ++pos;
  assert(*pos == 32);
  ++pos;
  assert(*pos == 23);
  ++pos;
  assert(*pos == 22);
  ++pos;
  assert(*pos == 7);
  ++pos;
  assert(*pos == 37);
  ++pos;
  assert(*pos == 37);
  ++pos;
  assert(pos == end);

  --pos;
  assert(*pos == 37);
  --pos;
  assert(*pos == 37);
  --pos;
  assert(*pos == 7);
  --pos;
  assert(*pos == 22);
  --pos;
  assert(*pos == 23);
  --pos;
  assert(*pos == 32);
  --pos;
  assert(*pos == 32);
  --pos;
  assert(*pos == 30);
  --pos;
  assert(*pos == 30);
  --pos;
  assert(*pos == 26);
  --pos;
  assert(*pos == 27);
  --pos;
  assert(*pos == 27);
  --pos;
  assert(*pos == 26);
  --pos;
  assert(*pos == 24);
  --pos;
  assert(*pos == 24);
  --pos;
  assert(*pos == 23);
  --pos;
  assert(*pos == 22);
  --pos;
  assert(*pos == 8);
  --pos;
  assert(*pos == 19);
  --pos;
  assert(*pos == 19);
  --pos;
  assert(*pos == 9);
  --pos;
  assert(*pos == 10);
  --pos;
  assert(*pos == 15);
  --pos;
  assert(*pos == 15);
  --pos;
  assert(*pos == 13);
  --pos;
  assert(*pos == 13);
  --pos;
  assert(*pos == 11);
  --pos;
  assert(*pos == 11);
  --pos;
  assert(*pos == 10);
  --pos;
  assert(*pos == 9);
  --pos;
  assert(*pos == 8);
  --pos;
  assert(*pos == 7);
  --pos;
  assert(*pos == 1);
  --pos;
  assert(*pos == 4);
  --pos;
  assert(*pos == 4);
  --pos;
  assert(*pos == 2);
  --pos;
  assert(*pos == 2);
  --pos;
  assert(*pos == 1);
  --pos;
  assert(*pos == 0);
}

inline void
test_dfuds()
{
  ptree_node node_1{1, nullptr, nullptr};
  ptree_node node_2{2, nullptr, nullptr};
  node_1.first_child = &node_2;
  ptree_node node_3{3, nullptr, nullptr};
  node_2.next_sibling = &node_3;
  ptree_node node_4{4, nullptr, nullptr};
  node_3.next_sibling = &node_4;
  ptree_node node_5{5, nullptr, nullptr};
  node_2.first_child = &node_5;
  ptree_node node_6{6, nullptr, nullptr};
  node_5.next_sibling = &node_6;
  ptree_node node_7{7, nullptr, nullptr};
  node_3.first_child = &node_7;
  ptree_node node_8{8, nullptr, nullptr};
  node_7.next_sibling = &node_8;
  ptree_node node_9{9, nullptr, nullptr};
  node_7.first_child = &node_9;
  ptree_node node_10{10, nullptr, nullptr};
  node_9.next_sibling = &node_10;
  ptree_node node_11{11, nullptr, nullptr};
  node_8.first_child = &node_11;
  ptree_node node_12{12, nullptr, nullptr};
  node_9.first_child = &node_12;
  ptree_node node_13{13, nullptr, nullptr};
  node_11.first_child = &node_13;
  ptree_node node_14{14, nullptr, nullptr};
  node_13.next_sibling = &node_14;
  ptree_node node_15{15, nullptr, nullptr};
  node_14.next_sibling = &node_15;
  ptree_node node_16{16, nullptr, nullptr};
  node_15.next_sibling = &node_16;
  ptree_node node_17{17, nullptr, nullptr};
  node_12.first_child = &node_17;
  ptree_node node_18{18, nullptr, nullptr};
  node_17.next_sibling = &node_18;
  ptree_node node_19{19, nullptr, nullptr};
  node_18.next_sibling = &node_19;
  ptree_node node_20{20, nullptr, nullptr};
  node_14.first_child = &node_20;

  ptree_cursor root;
  root.value = &node_1;

  ptree_cursor nil;

  assert(root);
  assert(has_left_successor(root));
  assert(!has_right_successor(root));
  assert(left_successor(root).value->value == 2);

  eco::dfuds x{root, nil, 20};

  assert(x.root() == 3);

  assert(x.first_child(x.root()) == 7);
  assert(x.first_child(x.first_child(x.root())) == 10);

  assert(x.last_child(x.root()) == 41);
  assert(x.last_child(x.first_child(x.root())) == 11);

  assert(x.next_sibling(x.first_child(x.root())) == 12);

  assert(x.prev_sibling(x.last_child(x.root())) == 12);

  assert(x.parent(x.first_child(x.root())) == 3);
  assert(x.parent(x.last_child(x.root())) == 3);
  assert(x.parent(x.first_child(x.first_child(x.root()))) == 7);
  assert(x.parent(x.last_child(x.first_child(x.root()))) == 7);

  assert(x.is_leaf(x.root()) == false);
  assert(x.is_leaf(x.first_child(x.root())) == false);
  assert(x.is_leaf(x.first_child(x.first_child(x.root()))) == true);
  assert(x.is_leaf(x.last_child(x.root())) == true);

  assert(x.node_map(x.root()) == 0);
  assert(x.node_map(x.first_child(x.root())) == 1);
  assert(x.node_map(x.last_child(x.root())) == 19);
  assert(x.node_map(x.first_child(x.first_child(x.root()))) == 2);
  assert(x.node_map(x.last_child(x.first_child(x.root()))) == 3);

  assert(x.node_select(0) == x.root());
  assert(x.node_select(1) == x.first_child(x.root()));
  assert(x.node_select(2) == x.first_child(x.first_child(x.root())));
  assert(x.node_select(3) == x.last_child(x.first_child(x.root())));
  assert(x.node_select(4) == x.next_sibling(x.first_child(x.root())));

  assert(x.preorder(12) == 4);
  assert(x.preorder_select(4) == 12);

  assert(x.subtree(12) == 15);

  assert(!x.is_ancestor(x.node_select(9), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(8), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(7), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(6), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(5), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(4), x.node_select(8)));
  assert(!x.is_ancestor(x.node_select(3), x.node_select(8)));
  assert(!x.is_ancestor(x.node_select(2), x.node_select(8)));
  assert(!x.is_ancestor(x.node_select(1), x.node_select(8)));
  assert(x.is_ancestor(x.node_select(0), x.node_select(8)));

  assert(x.children(12) == 2);

  assert(x.child(x.root(), 0) == 7);
  assert(x.child(x.root(), 1) == 12);
  assert(x.child(x.root(), 2) == 41);
  assert(x.child(7, 0) == 10);
  assert(x.child(7, 1) == 11);

  assert(x.child_rank(28) == 2);

  assert(x.lca(20, 30) == 12);

  auto pos = x.begin();
  auto end = x.end();

  assert(*pos == 7);
  ++pos;
  assert(*pos == 10);
  ++pos;
  assert(*pos == 10);
  ++pos;
  assert(*pos == 11);
  ++pos;
  assert(*pos == 11);
  ++pos;
  assert(*pos == 7);
  ++pos;
  assert(*pos == 12);
  ++pos;
  assert(*pos == 15);
  ++pos;
  assert(*pos == 18);
  ++pos;
  assert(*pos == 20);
  ++pos;
  assert(*pos == 24);
  ++pos;
  assert(*pos == 24);
  ++pos;
  assert(*pos == 25);
  ++pos;
  assert(*pos == 25);
  ++pos;
  assert(*pos == 26);
  ++pos;
  assert(*pos == 26);
  ++pos;
  assert(*pos == 20);
  ++pos;
  assert(*pos == 18);
  ++pos;
  assert(*pos == 27);
  ++pos;
  assert(*pos == 27);
  ++pos;
  assert(*pos == 15);
  ++pos;
  assert(*pos == 28);
  ++pos;
  assert(*pos == 30);
  ++pos;
  assert(*pos == 35);
  ++pos;
  assert(*pos == 35);
  ++pos;
  assert(*pos == 36);
  ++pos;
  assert(*pos == 38);
  ++pos;
  assert(*pos == 38);
  ++pos;
  assert(*pos == 36);
  ++pos;
  assert(*pos == 39);
  ++pos;
  assert(*pos == 39);
  ++pos;
  assert(*pos == 40);
  ++pos;
  assert(*pos == 40);
  ++pos;
  assert(*pos == 30);
  ++pos;
  assert(*pos == 28);
  ++pos;
  assert(*pos == 12);
  ++pos;
  assert(*pos == 41);
  ++pos;
  assert(*pos == 41);
  ++pos;
  assert(pos == end);

  --pos;
  assert(*pos == 41);
  --pos;
  assert(*pos == 41);
  --pos;
  assert(*pos == 12);
  --pos;
  assert(*pos == 28);
  --pos;
  assert(*pos == 30);
  --pos;
  assert(*pos == 40);
  --pos;
  assert(*pos == 40);
  --pos;
  assert(*pos == 39);
  --pos;
  assert(*pos == 39);
  --pos;
  assert(*pos == 36);
  --pos;
  assert(*pos == 38);
  --pos;
  assert(*pos == 38);
  --pos;
  assert(*pos == 36);
  --pos;
  assert(*pos == 35);
  --pos;
  assert(*pos == 35);
  --pos;
  assert(*pos == 30);
  --pos;
  assert(*pos == 28);
  --pos;
  assert(*pos == 15);
  --pos;
  assert(*pos == 27);
  --pos;
  assert(*pos == 27);
  --pos;
  assert(*pos == 18);
  --pos;
  assert(*pos == 20);
  --pos;
  assert(*pos == 26);
  --pos;
  assert(*pos == 26);
  --pos;
  assert(*pos == 25);
  --pos;
  assert(*pos == 25);
  --pos;
  assert(*pos == 24);
  --pos;
  assert(*pos == 24);
  --pos;
  assert(*pos == 20);
  --pos;
  assert(*pos == 18);
  --pos;
  assert(*pos == 15);
  --pos;
  assert(*pos == 12);
  --pos;
  assert(*pos == 7);
  --pos;
  assert(*pos == 11);
  --pos;
  assert(*pos == 11);
  --pos;
  assert(*pos == 10);
  --pos;
  assert(*pos == 10);
  --pos;
  assert(*pos == 7);
  --pos;
  assert(*pos == 3);
}

#endif
