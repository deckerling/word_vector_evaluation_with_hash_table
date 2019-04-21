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

void BinaryTree::DestroyTree(Node* current_node) {
  if (current_node->left != NULL)
    DestroyTree(current_node->left);
  if (current_node->right != NULL)
    DestroyTree(current_node->right);
  delete current_node;
}

void BinaryTree::AddIndex(const unsigned index_to_add) {
  Node* new_node = new Node(index_to_add);
  if (root_ == NULL)
    root_ = new_node;
  else {
    Node* current_node = root_;
    while (true) {
      if (current_node->index == index_to_add) // avoids multiple additions of the same index
        return;
      if (index_to_add < current_node->index) { // if "index_to_add" is smaller than "current_node->index" it will be stored on the left of "current_node"
        if (current_node->left == NULL) {
          current_node->left = new_node;
          return;
        } else
          current_node = current_node->left;
      } else { // if "index_to_add" is bigger than "current_node->index" it will be stored on the right of "current_node"
        if (current_node->right == NULL) {
          current_node->right = new_node;
          return;
        } else
          current_node = current_node->right;
      }
    }
  }
}

bool BinaryTree::IndexIsAlreadyStored(const unsigned index_to_check_for) {
// Checks if the "index_to_check_for" is stored in the binary tree and returns
// "true" if so and "false" otherwise.
  if (root_ == NULL)
    return false;
  if (root_->index == index_to_check_for) // if "root_" contains the "index_to_check_for"
    return true;
  Node* current_node = root_;
  while (true) {
    if (index_to_check_for < current_node->index) {
      if (current_node->left == NULL) // if "index_to_check_for" couldn't be found in the binary tree
        return false;
      else if (current_node->left->index == index_to_check_for)
        return true;
      else
        current_node = current_node->left;
    } else {
      if (current_node->right == NULL) // if "index_to_check_for" couldn't be found in the binary tree
        return false;
      else if (current_node->right->index == index_to_check_for)
        return true;
      else
        current_node = current_node->right;
    }
  }
}
