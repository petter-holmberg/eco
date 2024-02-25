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

  assert(x.nodemap(x.root()) == 0);
  assert(x.nodemap(x.first_child(x.root())) == 1);
  assert(x.nodemap(x.last_child(x.root())) == 3);
  assert(x.nodemap(x.first_child(x.first_child(x.root()))) == 4);
  assert(x.nodemap(x.last_child(x.first_child(x.root()))) == 5);

  assert(x.nodeselect(0) == x.root());
  assert(x.nodeselect(1) == x.first_child(x.root()));
  assert(x.nodeselect(3) == x.last_child(x.root()));
  assert(x.nodeselect(4) == x.first_child(x.first_child(x.root())));
  assert(x.nodeselect(5) == x.last_child(x.first_child(x.root())));

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

  assert(eco::lca(x, x.root(), x.root()) == x.root());
  assert(eco::lca(x, x.first_child(x.root()), x.last_child(x.root())) == x.root());
  assert(eco::lca(x, x.first_child(x.first_child(x.root())), x.last_child(x.first_child(x.root()))) == x.first_child(x.root()));
}

#endif
