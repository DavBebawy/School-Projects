#Class Node, holds the value of the node and pointers for the left and right children
class Node
  attr_reader :value
  attr_accessor :left, :right

	#a node is nil if it's value is nil
  def nil?
    true if @value.nil?
  end

	#the node is initialized with a nil value, unless one is provided, and with nil left and right pointers
  def initialize(value = nil)
    @value = value
    left = nil
    right = nil
  end

	#overriding the to_s to display the value
  def to_s
    @value&.to_s
  end
end

class BST
  include Comparable

  #Initialize function: returns a new, empty BST. If a block of code is provided, this code is used to compare two objects in the tree during insertion, search, etc. This block should accept two parametersand return 1, -1, or 0, like Ruby’s <=> operator. If no block is provided, the <=> operator is used forcomparison.
  def initialize(&compare_proc)
    @compare_proc = compare_proc.nil? ? proc { |first, second| first <=> second } : compare_proc
    @root = Node.new
    @size = 0
  end

	 #Add function: adds a new item to the binary tree. Duplicate items are stored in the right subtree
  def add(item)
    @root.nil? ? @root = Node.new(item) : addHelper(@root, item)
    @size += 1
  end

	#Addhelper function: used to traverse the tree using the compare_proc and add the node at the correct location
  def addHelper(current, item)
    if @compare_proc.call(item, current.value) == -1
      if current.left.nil?
        current.left = Node.new(item)
      else
        addHelper(current.left, item)
      end
    else
      if current.right.nil?
        current.right = Node.new(item)
      else
        addHelper(current.right, item)
      end
    end
  end

  # empty? function: returns true if the tree is empty, otherwise returns false.
  def empty?
    @root.nil? ? true : false
  end

  # include? function : returns true if the item is found in the tree, otherwise returns false.
  def include?(item)
    @root.nil? ? false : includeHelper(@root, item)
  end

	#Addhelper function: used to traverse the tree using the compare_proc and returns true if the item is found
  def includeHelper(current, item)
    if current.nil?
      false
    elsif @compare_proc.call(item, current.value) == 0
      true
    elsif @compare_proc.call(item, current.value) == -1
      includeHelper(current.left, item)
    else
      includeHelper(current.right, item)
    end
  end

  # size: returns the number of items in the tree.
  attr_reader :size

  # each_inorder function: performs an in-order traversal of the tree, passing each item found to block.
  def each_inorder(&block)
    each_inorder_helper(@root, &block)
  end

	# each_inorder_helper function: unless current node is nil, recursively calls itself on the left child, yields on the current node using the block, and then recursively calls itself on the right child
  def each_inorder_helper(current, &block)
    unless current.nil?
      each_inorder_helper(current.left, &block)
      yield(current)
      each_inorder_helper(current.right, &block)
    end
  end

  # collect_inorder function: performs an in-order traversal of the tree, passing each item found to block. The values returned by block are collected into a new BST, which is returned by collect_inorder.
  def collect_inorder(&block)
    newBST = BST.new
    newBST.dub_compare_setter(@compare_proc)
    collect_inorder_helper(@root, newBST, &block)
    newBST
  end

	# each_inorder_helper function: unless current node is nil, recursively calls itself on the left child, adds a new node with the yield on the current node using the block, and then recursively calls itself on the right child
  def collect_inorder_helper(node, newBST, &block)
    unless node.nil?
      collect_inorder_helper(node.left, newBST, &block)
      newBST.add(yield(node.value))
      collect_inorder_helper(node.right, newBST, &block)
    end
  end

  # to_a function: returns a sorted array of all the elements in the BST.
  def to_a
    sorted_array = []
    to_a_helper(@root, sorted_array)
    sorted_array
  end

	# to_a_helper function: unless current node is nil, recursively calls itself on the left child, pushs the current node value to the sorted array, and then recursively calls itself on the right child
  def to_a_helper(node, sorted_array)
    unless node.nil?
      to_a_helper(node.left, sorted_array)
      sorted_array << node.value
      to_a_helper(node.right, sorted_array)
    end
  end

  # dup function: returns a new binary search tree with the same contents as the original tree. This operation performs a deep copy of the original tree.
  def dup
    newBST = BST.new
    newBST.dub_compare_setter(@compare_proc)
    dup_add(@root, newBST)
    newBST
  end

	# dup_add function: performs a preorder traversal of the tree, copying the current node before recursively calling itself on the left child then the right child
  def dup_add(node, newBST)
    unless node.nil?
      newBST.add(node.value)
      dup_add(node.left, newBST)
      dup_add(node.right, newBST)
    end
  end

	# dub_compare_setter function: is used to copy the compare proc
  def dub_compare_setter(compare_proc)
    @compare_proc = compare_proc
  end
end
