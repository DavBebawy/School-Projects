# Ruby Binary Search Tree

Binary Search Tree class with a few basic functions.

## Functions:
- `BST.new{compare_method}` returns a new, empty BST. If a block of code is provided, this code is used to compare two objects in the tree during insertion, search, etc.
- `add(item)` adds a new item to the binary tree, duplicate items are stored in the right subtree.
- `empty?` returns true if the tree is empty, otherwise returns false.
- `include?(item)` returns true if the item is found in the tree, otherwise returns false.
- `size` returns the number of items in the tree.
- `each_inorder{block}` performs an in-order traversal of the tree, passing each item found to block.
- `collect_inorder{block}` performs an in-order traversal of the tree, passing each item found to block, then returns it as a new BST.
- `to_a` returns a sorted array of all the elements in the BST.
- `dup` returns a deep copy BST with the same contents as the original tree.

## Run in GitPod
You can also run Ruby BST in Gitpod, a free online dev environment for GitHub:

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/DavBebawy/School-Project/Ruby-BST)