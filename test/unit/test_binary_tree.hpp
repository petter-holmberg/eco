#ifndef ECO_TEST_BINARY_TREE_
#define ECO_TEST_BINARY_TREE_

import std;
import eco;

#include <cassert>

inline void
test_balanced_binary_tree()
{
  eco::balanced_binary_tree<int> x{9};

  assert(x.root() == 0);

  assert(x.has_left_child(x.root()));
  assert(x.left_child(x.root()) == 1);
  assert(x.has_left_child(x.left_child(x.root())));
  assert(x.left_child(x.left_child(x.root())) == 3);
  assert(x.has_left_child(x.left_child(x.left_child(x.root()))));
  assert(x.left_child(x.left_child(x.left_child(x.root()))) == 7);
  assert(!x.has_left_child(x.left_child(x.left_child(x.left_child(x.root())))));

  assert(x.has_right_child(x.root()));
  assert(x.right_child(x.root()) == 2);
  assert(x.has_right_child(x.right_child(x.root())));
  assert(x.right_child(x.right_child(x.root())) == 6);
  assert(!x.has_right_child(x.right_child(x.right_child(x.root()))));

  assert(x.parent(x.left_child(x.root())) == 0);
  assert(x.parent(x.right_child(x.root())) == 0);
  assert(x.parent(x.left_child(x.left_child(x.root()))) == 1);
  assert(x.parent(x.right_child(x.left_child(x.root()))) == 1);
  assert(x.parent(x.right_child(x.right_child(x.root()))) == 2);
  assert(x.parent(x.left_child(x.right_child(x.left_child(x.root())))) == 4);

  assert(x.is_leaf(x.root()) == false);
  assert(x.is_leaf(x.left_child(x.root())) == false);
  assert(x.is_leaf(x.left_child(x.left_child(x.root()))) == false);
  assert(x.is_leaf(x.right_child(x.left_child(x.left_child(x.root())))) == true);
  assert(x.is_leaf(x.right_child(x.root())) == false);
  assert(x.is_leaf(x.right_child(x.left_child(x.root()))) == true);

  assert(x.leaf_select(0) == 7);
  assert(x.leaf_select(1) == 8);
  assert(x.leaf_select(2) == 4);
  assert(x.leaf_select(3) == 5);
  assert(x.leaf_select(4) == 6);

  eco::balanced_binary_tree_cursor<int> cur{x};

  assert(!eco::is_left_successor(cur));
  assert(!eco::is_right_successor(cur));

  assert(has_left_successor(cur));
  auto curl = left_successor(cur);
  assert(eco::is_left_successor(curl));
  assert(!eco::is_right_successor(curl));

  assert(has_right_successor(cur));
  auto curr = right_successor(cur);
  assert(!eco::is_left_successor(curr));
  assert(eco::is_right_successor(curr));

  auto n1 = cur;
  assert(!eco::is_left_successor(n1));
  assert(!eco::is_right_successor(n1));
  auto n2 = left_successor(n1);
  assert(eco::is_left_successor(n2));
  assert(!eco::is_right_successor(n2));
  auto n3 = right_successor(cur);
  assert(!eco::is_left_successor(n3));
  assert(eco::is_right_successor(n3));
  auto n4 = left_successor(n1);
  assert(eco::is_left_successor(n4));
  assert(!eco::is_right_successor(n4));
  auto n5 = right_successor(n1);
  assert(!eco::is_left_successor(n5));
  assert(eco::is_right_successor(n5));
  auto n6 = left_successor(n3);
  assert(eco::is_left_successor(n6));
  assert(!eco::is_right_successor(n6));
  auto n7 = right_successor(n3);
  assert(!eco::is_left_successor(n7));
  assert(eco::is_right_successor(n7));
  auto n8 = left_successor(n4);
  assert(eco::is_left_successor(n8));
  assert(!eco::is_right_successor(n8));
  auto n9 = right_successor(n4);
  assert(!eco::is_left_successor(n9));
  assert(eco::is_right_successor(n9));

  assert(eco::tree_weight(cur) == 9);

  assert(eco::tree_height(cur) == 4);
}

inline void
test_binary_louds()
{
  ptree_node node_1{1, nullptr, nullptr};
  ptree_node node_2{2, nullptr, nullptr};
  node_2.parent = &node_1;
  node_1.first_child = &node_2;
  ptree_node node_3{3, nullptr, nullptr};
  node_3.parent = &node_1;
  node_1.next_sibling = &node_3;
  ptree_node node_4{4, nullptr, nullptr};
  node_4.parent = &node_2;
  node_2.first_child = &node_4;
  ptree_node node_5{5, nullptr, nullptr};
  node_5.parent = &node_2;
  node_2.next_sibling = &node_5;
  ptree_node node_6{6, nullptr, nullptr};
  node_6.parent = &node_3;
  node_3.next_sibling = &node_6;
  ptree_node node_7{7, nullptr, nullptr};
  node_7.parent = &node_5;
  node_5.first_child = &node_7;
  ptree_node node_8{8, nullptr, nullptr};
  node_8.parent = &node_5;
  node_5.next_sibling = &node_8;
  ptree_node node_9{9, nullptr, nullptr};
  node_9.parent = &node_6;
  node_6.first_child = &node_9;
  ptree_node node_10{10, nullptr, nullptr};
  node_10.parent = &node_7;
  node_7.next_sibling = &node_10;
  ptree_node node_11{11, nullptr, nullptr};
  node_11.parent = &node_9;
  node_9.first_child = &node_11;
  ptree_node node_12{12, nullptr, nullptr};
  node_12.parent = &node_9;
  node_9.next_sibling = &node_12;

  ptree_cursor root;
  root.value = &node_1;

  assert(root);
  assert(has_left_successor(root));
  assert(has_right_successor(root));
  assert(left_successor(root).value->value == 2);

  eco::binary_louds x{root, 12};

  assert(x.root() == 0);

  assert(x.has_left_child(x.root()));
  assert(x.left_child(x.root()) == 1);
  assert(x.has_left_child(x.left_child(x.root())));
  assert(x.left_child(x.left_child(x.root())) == 3);

  assert(x.has_right_child(x.root()));
  assert(x.right_child(x.root()) == 2);
  assert(x.has_right_child(x.left_child(x.root())));
  assert(x.right_child(x.left_child(x.root())) == 4);

  assert(x.parent(x.left_child(x.root())) == 0);
  assert(x.parent(x.right_child(x.root())) == 0);
  assert(x.parent(x.left_child(x.left_child(x.root()))) == 1);
  assert(x.parent(x.right_child(x.left_child(x.root()))) == 1);
  assert(x.parent(x.right_child(x.right_child(x.root()))) == 2);
  assert(x.parent(x.left_child(x.right_child(x.left_child(x.root())))) == 4);

  assert(x.is_leaf(x.root()) == false);
  assert(x.is_leaf(x.left_child(x.root())) == false);
  assert(x.is_leaf(x.left_child(x.left_child(x.root()))) == true);
  assert(x.is_leaf(x.right_child(x.root())) == false);

  assert(x.child_label(x.root()) == -1);
  assert(x.child_label(x.left_child(x.root())) == 0);
  assert(x.child_label(x.right_child(x.root())) == 1);
  assert(x.child_label(x.right_child(x.left_child(x.root()))) == 1);
  assert(x.child_label(x.right_child(x.right_child(x.root()))) == 1);

  eco::binary_louds_cursor cur{x};

  assert(!eco::is_left_successor(cur));
  assert(!eco::is_right_successor(cur));

  assert(has_left_successor(cur));
  auto curl = left_successor(cur);
  assert(eco::is_left_successor(curl));
  assert(!eco::is_right_successor(curl));

  assert(has_right_successor(cur));
  auto curr = right_successor(cur);
  assert(!eco::is_left_successor(curr));
  assert(eco::is_right_successor(curr));

  auto n1 = cur;
  assert(!eco::is_left_successor(n1));
  assert(!eco::is_right_successor(n1));
  auto n2 = left_successor(n1);
  assert(eco::is_left_successor(n2));
  assert(!eco::is_right_successor(n2));
  auto n3 = right_successor(cur);
  assert(!eco::is_left_successor(n3));
  assert(eco::is_right_successor(n3));
  auto n4 = left_successor(n2);
  assert(eco::is_left_successor(n4));
  assert(!eco::is_right_successor(n4));
  auto n5 = right_successor(n2);
  assert(!eco::is_left_successor(n5));
  assert(eco::is_right_successor(n5));
  auto n6 = right_successor(n3);
  assert(!eco::is_left_successor(n6));
  assert(eco::is_right_successor(n6));
  auto n7 = left_successor(n5);
  assert(eco::is_left_successor(n7));
  assert(!eco::is_right_successor(n7));
  auto n8 = right_successor(n5);
  assert(!eco::is_left_successor(n8));
  assert(eco::is_right_successor(n8));
  auto n9 = left_successor(n6);
  assert(eco::is_left_successor(n9));
  assert(!eco::is_right_successor(n9));
  auto n10 = right_successor(n7);
  assert(!eco::is_left_successor(n10));
  assert(eco::is_right_successor(n10));
  auto n11 = left_successor(n9);
  assert(eco::is_left_successor(n11));
  assert(!eco::is_right_successor(n11));
  auto n12 = right_successor(n9);
  assert(!eco::is_left_successor(n12));
  assert(eco::is_right_successor(n12));

  assert(eco::tree_weight(cur) == 12);

  assert(eco::tree_height(cur) == 5);
}

#endif
