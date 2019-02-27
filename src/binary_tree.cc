// binary_tree.cc

// Copyright 2019 E. Decker
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "wvewht.h" // wvewht = "word_vector_evaluation_with_hash_table"

BinaryTree::BinaryTree() {}

BinaryTree::~BinaryTree() {
  if (root_ != NULL)
    DestroyTree(root_);
}

void BinaryTree::DestroyTree(struct Item *current_item) {
  if (current_item->left != NULL)
    DestroyTree(current_item->left);
  if (current_item->right != NULL)
    DestroyTree(current_item->right);
  delete current_item;
}

void BinaryTree::AddIndex(const int index_to_add) {
  struct Item *new_item = new struct Item;
  new_item->index = index_to_add;
  if (root_ == NULL)
    root_ = new_item;
  else {
    struct Item *current_item = root_;
    while (true) {
      if (current_item->index == index_to_add) // avoids multiple additions of the same index
        return;
      if (index_to_add < current_item->index) { // if "index_to_add" is smaller than "current_item->index" it will be stored on the left of "current_item"
        if (current_item->left == NULL) {
          current_item->left = new_item;
          return;
        } else
          current_item = current_item->left;
      } else { // if "index_to_add" is bigger than "current_item->index" it will be stored on the right of "current_item"
        if (current_item->right == NULL) {
          current_item->right = new_item;
          return;
        } else
          current_item = current_item->right;
      }
    }
  }
}

bool BinaryTree::IndexIsAlreadyStored(const int index_to_check_for) {
// Checks if the "index_to_check_for" is stored in the binary tree and returns
// "true" if so and "false" otherwise.
  if (root_ == NULL)
    return false;
  if (root_->index == index_to_check_for) // if "root_" contains the "index_to_check_for"
    return true;
  struct Item *current_item = root_;
  while (true) {
    if (index_to_check_for < current_item->index) {
      if (current_item->left == NULL) // if "index_to_check_for" couldn't be found in the binary tree
        return false;
      else if (current_item->left->index == index_to_check_for)
        return true;
      else
        current_item = current_item->left;
    } else {
      if (current_item->right == NULL) // if "index_to_check_for" couldn't be found in the binary tree
        return false;
      else if (current_item->right->index == index_to_check_for)
        return true;
      else
        current_item = current_item->right;
    }
  }
}
