#ifndef RBTREE_H
#define RBTREE_H
#include <mutex>
#include <string>
#include <vector>
#include <iostream> 
#include <sstream>

#include <shared_mutex>
/**
 * For simplicity, fix value to type int, instead of using template
 * All key > 0
 */
#define RED true
#define BLACK false
struct Node {
    int key_;
    int value_;
    bool color_;
    Node *left_, *right_, *parent_;
    std::mutex latch_; 
    Node(int k, int v, bool c) : key_(k), value_(v), color_(c), 
                                  left_(nullptr), right_(nullptr), parent_(nullptr) {}
};


struct PrintableRBTree {
  int size_;                              // Display width for this subtree
  std::string keys_;                      // Display string (e.g., "10(B)")
  std::vector<PrintableRBTree> children_; // Left and right children

  /**
   * @brief BFS traverse and print the tree level by level
   * 
   * @param out_buf Output stream (e.g., std::cout)
   */
  void Print(std::ostream &out_buf) {
    std::vector<PrintableRBTree *> que = {this};
    while (!que.empty()) {
      std::vector<PrintableRBTree *> new_que;

      for (auto &t : que) {
        int padding = (t->size_ - t->keys_.size()) / 2;
        out_buf << std::string(padding, ' ');
        out_buf << t->keys_;
        out_buf << std::string(padding, ' ');

        for (auto &c : t->children_) {
          new_que.push_back(&c);
        }
      }
      out_buf << "\n";
      que = new_que;
    }
  }
};

class RBTree {
private:
    Node* root_;
    std::mutex global_latch_;  // Global for strategy 1, root_lock for strategy 2
    std::shared_mutex shared_mutex_; //All advanced thread should acquired this shared_mutex & fall into unique_lock if required rotation

    void Recolor(Node* parent, Node* grandparent, Node* uncle);
    void RotateLeft(Node* parent, Node* right_child);
    void RotateRight(Node* parent, Node* left_child);
    auto SearchPositionHelper(int key) -> Node*;
    
    void FixTreeStructure(Node* parent, Node* child);
    void FixTreeStructureAdvancedLock(Node* parent, Node* child);
    
public:
    RBTree();
    ~RBTree();
    
    auto InsertGlobalLock(int key, int value) -> bool;
    auto SearchGlobalLock(int key) -> int;

    auto InsertAdvancedLock(int key, int value) -> bool;
    auto SearchAdvancedLock(int key) -> int;


    /**
     * Testing purpose only
     */
    auto CheckTreeBalance() -> bool;
    auto CheckRedBalance(Node *r) -> bool; //check if any red node have red parent
    auto CheckBlackBalance(Node *r) -> bool; //check if path from root -> leaf has the same number of black node


  
    /**
     * Print Tree
     */
    void PrintTreePretty() {    
    if (root_ == nullptr) {
        std::cout << "(empty tree)\n";
        return;
    }
    PrintPrettyHelper(root_, "", false, std::cout);
    std::cout << "\n";
  }
  void PrintPrettyHelper(Node* node, std::string prefix, bool is_left, std::ostream& out) {
    if (node == nullptr) {
        return;
    }
    
    // Print right subtree first (appears at top)
    if (node->right_ != nullptr) {
        PrintPrettyHelper(node->right_, prefix + (is_left ? "│   " : "    "), false, out);
    }
    
    // Print current node
    out << prefix;
    out << (is_left ? "└── " : "┌── ");
    
    // Color the output
    if (node->color_ == RED) {
        out << "\033[31m" << node->key_ << "(R)\033[0m\n";  // Red text
    } else {
        out << node->key_ << "(B)\n";  // Normal text
    }
    
    // Print left subtree (appears at bottom)
    if (node->left_ != nullptr) {
        PrintPrettyHelper(node->left_, prefix + (is_left ? "    " : "│   "), true, out);
    }
  }
};


#endif