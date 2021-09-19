/* Copyright © 2021 Chee Bin HOH. All rights reserved.
 *
 * ADT binary search tree
 */

#ifndef BTREE_H_HAS_INCLUDED

#define BTREE_H_HAS_INCLUDED

#include "btree-internal.h"
#include "avlbstree.h"
#include "llist.h"


typedef void (bTreeTraversalCallback)(struct TreeNode *node, int pos, void *data);


struct TreeNode * addTreeNode(struct TreeNode *root, int val);
struct TreeNode * findTreeNode(struct TreeNode *root, int val);
struct TreeNode * treeRebalance(struct TreeNode *root);
struct TreeNode * delTreeNode(struct TreeNode *root, int val);
void              freeTreeNode(struct TreeNode *root);


struct TreeNode *buildBinaryTree(struct ListNode *inorder, struct ListNode *postorder);

void printTreeNodeInTreeTopology(struct TreeNode *root);

int  findTotalNumberOfTreeNode(struct TreeNode *root);
void traverseTreeNodePreOrder(struct TreeNode *root, bTreeTraversalCallback func, void *data);
void traverseTreeNodeInOrder(struct TreeNode *root, bTreeTraversalCallback func, void *data);
void traverseTreeNodePostOrder(struct TreeNode *root, bTreeTraversalCallback func, void *data);

int determineMaxDepthLevel(struct TreeNode *root);
int determineMinDepthLevel(struct TreeNode *root);

struct TreeNode * findLeastCommonAncestor(struct TreeNode *root, int val1, int val2);
int               findMaximumSumPathToLeafValue(struct TreeNode *root, int val);
int               isTreeSubTree(struct TreeNode *tree1, struct TreeNode *tree2);
int               isTreeBinarySearchTree(struct TreeNode *root);
int               isTreeSymmetric(struct TreeNode *root);

struct ListNode * getInOrderList(struct TreeNode *root);
struct ListNode * getPreOrderList(struct TreeNode *root);
struct ListNode * getPostOrderList(struct TreeNode *root);


#endif
