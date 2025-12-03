#include "RBTree.h"
#include <queue>


RBTree::RBTree() : root_(nullptr) {}

RBTree::~RBTree() {
    
}

void RBTree::RotateLeft(Node* parent, Node* right_child) {
    auto grandparent = parent->parent_;  // Get it here!
    auto left_right_child = right_child->left_;    
    parent->right_ = left_right_child;
    right_child->left_ = parent;

    //fix parent pointer
    right_child->parent_ = grandparent;
    parent->parent_ = right_child;
    if (left_right_child != nullptr){
        left_right_child->parent_ = parent;
    }

    //fix grandparent pointer
    if (grandparent == nullptr) {
        root_ = right_child;
    }
    else if (grandparent->left_ == parent) {
        grandparent->left_ = right_child;
    } else {
        grandparent->right_ = right_child;
    }
}

void RBTree::RotateRight(Node* parent, Node* left_child) {
    auto grandparent = parent->parent_;  // Get it here!
    auto right_left_child = left_child->right_;
    parent->left_ = right_left_child;
    left_child->right_ = parent;

    //fix parent pointer
    left_child->parent_ = grandparent;
    parent->parent_ = left_child; 
    if (right_left_child != nullptr){
        right_left_child->parent_ = parent;
    }

    //fix grandparent pointer
     if (grandparent == nullptr) {
        root_ = left_child;
    }
    else if (grandparent->left_ == parent) {
        grandparent->left_ = left_child;
    } else {
        grandparent->right_ = left_child;
    }
}

void RBTree::Recolor(Node* parent, Node* grandparent, Node* uncle){
    grandparent->color_ = RED;
    parent->color_ = BLACK;
    uncle->color_ = BLACK;
    if (root_ == grandparent){
        root_->color_ = BLACK;
    }
}

void RBTree::FixTreeStructure(Node* parent, Node* child){
    auto grandparent = parent->parent_;
    auto uncle = grandparent->left_;
    if (uncle == parent){
        uncle = grandparent->right_;
    }    
    if (uncle == nullptr || uncle->color_ == BLACK){
       //case 1: left - left
        if (grandparent->left_ == parent && parent->left_ == child){
            parent->color_ = BLACK;
            grandparent->color_ = RED;
            RotateRight(grandparent, parent);
        }
        // case 2: left-right
        else if (grandparent->left_ == parent && parent->right_ == child){
            RotateLeft(parent, child);
            child->color_ = BLACK;
            grandparent->color_ = RED;
            RotateRight(grandparent, child);
        }
        // case 3: right-left
        else if(grandparent->right_ == parent && parent->left_ == child){
            RotateRight(parent, child);
            child->color_ = BLACK;
            grandparent->color_ = RED;
            RotateLeft(grandparent, child);
        } 
        // case 4: right - right
        else {
            parent->color_ = BLACK;
            grandparent->color_ = RED;
            RotateLeft(grandparent, parent);
        }
    } 
    // case 5 - recolor
    else {
        Recolor(parent, grandparent, uncle);

        auto ggp = grandparent->parent_;
        if (ggp == nullptr){
            root_->color_ = BLACK;
        } else if (ggp->color_ == RED){
            FixTreeStructure(ggp, grandparent);
        }
    }
    return;
}

void RBTree::FixTreeStructureAdvancedLock(Node* parent, Node* child){
    auto grandparent = parent->parent_;
    grandparent->latch_.lock();

    auto uncle = grandparent->left_;
    if (uncle == parent){
        uncle = grandparent->right_;
    }  
    if (uncle != nullptr) {
        uncle->latch_.lock();
    }    

    auto ggp = grandparent->parent_;
    if (ggp != nullptr){
        ggp->latch_.lock();
    }

    if (uncle == nullptr || uncle->color_ == BLACK){
       //case 1: left - left
        if (grandparent->left_ == parent && parent->left_ == child){
            parent->color_ = BLACK;
            grandparent->color_ = RED;
            RotateRight(grandparent, parent);
        }
        // case 2: left-right
        else if (grandparent->left_ == parent && parent->right_ == child){
            RotateLeft(parent, child);
            child->color_ = BLACK;
            grandparent->color_ = RED;
            RotateRight(grandparent, child);
        }
        // case 3: right-left
        else if(grandparent->right_ == parent && parent->left_ == child){
            RotateRight(parent, child);
            child->color_ = BLACK;
            grandparent->color_ = RED;
            RotateLeft(grandparent, child);
        } 
        // case 4: right - right
        else {
            parent->color_ = BLACK;
            grandparent->color_ = RED;
            RotateLeft(grandparent, parent);
        }

        if (uncle != nullptr) uncle->latch_.unlock();
        grandparent->latch_.unlock();
        parent->latch_.unlock();
        child->latch_.unlock();
    } 
    // case 5 - recolor
    else {
        Recolor(parent, grandparent, uncle);
        uncle->latch_.unlock();
        parent->latch_.unlock();
        child->latch_.unlock();

        if (ggp == nullptr) {
            // Grandparent is root
            grandparent->color_ = BLACK;
            grandparent->latch_.unlock();
            return;
        }

        if (ggp->color_ == BLACK) {
            ggp->latch_.unlock();
            grandparent->latch_.unlock();
            return;
        }
        FixTreeStructureAdvancedLock(ggp, grandparent); // only held these 2 locks !
    }
    return;
}

/**
 * No-reinsertion. If find key exists -> return false
 */
auto RBTree::InsertGlobalLock(int key, int value) -> bool {
    std::lock_guard<std::mutex> guard(global_latch_);   // RAII -> destructor called when out of scope -> auto unlock
    Node* new_node = new Node(key, value, RED);
    if (root_ == nullptr){
        root_ = new_node;
        root_->color_ = BLACK;
        return true;
    }

    auto parent_node = SearchPositionHelper(key);
    if (parent_node->key_ == key){
        return false;
    }

    if (key > parent_node->key_){ parent_node->right_ = new_node; }
    else { parent_node->left_ = new_node; }
    new_node->parent_ = parent_node;
    

    // need to fix rotation here !
    if (parent_node->color_ == RED){
        FixTreeStructure(parent_node, new_node);
    }

    return true;
}

/**
 * Return value. If cant find, return -1
 */
auto RBTree::SearchGlobalLock(int key) -> int{
    std::lock_guard<std::mutex> guard(global_latch_); 
    int val = -1;
    auto found_node = SearchPositionHelper(key);
    if (found_node && found_node->key_ == key){
        val = found_node->value_;
    }
    return val;
}

/**
 * Find the current node (if existed) or Find the parent for insertion
 */
auto RBTree::SearchPositionHelper(int key) -> Node*{
    Node* run_node = root_;
    Node* back_node = nullptr;
    bool found = false;
    while (run_node != nullptr){
        back_node = run_node;
        if (run_node->key_ == key){
            found = true;
            break;
        } else if (key > run_node->key_){
            run_node = run_node->right_;
        } else {
            run_node = run_node->left_;
        }        
    }
    if (found){
        back_node = nullptr;
        return run_node;
    }
    else {
        run_node = nullptr;
        return back_node;
    }
}

auto RBTree::InsertAdvancedLock(int key, int value) -> bool{
    Node* new_node = new Node(key, value, RED);
    std::shared_lock<std::shared_mutex> shared_lock(shared_mutex_);

    //case 1
    if (root_ == nullptr) {
        shared_lock.unlock();
        std::unique_lock<std::shared_mutex> unique_lock(shared_mutex_);
        root_ = new_node;
        root_->color_ = BLACK;
        return true;
    }

    //case 2
    auto curr = root_;
    curr->latch_.lock();
    while (curr != nullptr){
        if (key == curr->key_){
            curr->latch_.unlock();
            return false;
        }
        auto next_node = (key < curr->key_) ? curr->left_ : curr->right_;
        if (next_node == nullptr){
            break;
        } else {
            next_node->latch_.lock();       //crabbing: acquire first, then release
            curr->latch_.unlock();
            curr = next_node;
        }
    }
    if (curr->color_ != RED){
        if (key < curr->key_){
            curr->left_ = new_node;
        } else {
            curr->right_ = new_node;
        }
        new_node->parent_ = curr;
        curr->latch_.unlock();
        return true;
    }
    
    //case 3
    else {
        curr->latch_.unlock();
        shared_lock.unlock();
        std::unique_lock<std::shared_mutex> unique_lock(shared_mutex_);
        if (root_ == nullptr){
            root_ = new_node;
            root_->color_ = BLACK;
            return true;
        }

        auto parent_node = SearchPositionHelper(key);
        if (parent_node->key_ == key){
            return false;
        }
        if (key > parent_node->key_){ parent_node->right_ = new_node; }
        else { parent_node->left_ = new_node; }
        new_node->parent_ = parent_node;
        
        // need to fix rotation here !
        if (parent_node->color_ == RED){
            FixTreeStructure(parent_node, new_node);
        }
        return true;
    }
}

auto RBTree::SearchAdvancedLock(int key) -> int{
    std::shared_lock<std::shared_mutex> shared_lock(shared_mutex_);
    if (root_ == nullptr) {
        shared_lock.unlock();
        return -1;
    }
    auto curr = root_;
    curr->latch_.lock();
    int result = -1;

    while (curr != nullptr){
        if (key == curr->key_){
            result = curr->value_;
            curr->latch_.unlock();
            break;
        }
        auto next_node = (key < curr->key_) ? curr->left_ : curr->right_;
        if (next_node == nullptr){
            curr->latch_.unlock();
            break;
        } else {
            next_node->latch_.lock();       //crabbing: acquire first, then release
            curr->latch_.unlock();
            curr = next_node;
        }
    }
    return result;
}

auto RBTree::CheckTreeBalance() -> bool {
    std::lock_guard<std::mutex> guard(global_latch_); 
    auto r1 = root_;
    auto r2 = root_;
    return CheckBlackBalance(r1) && CheckRedBalance(r2);
}

auto RBTree::CheckBlackBalance(Node *r) -> bool {
    std::queue<std::pair<Node*, int>> qu;
    qu.push({r, 0});
    int black_count = -1;
    while (!qu.empty()){
        auto [curr, count] = qu.front();
        qu.pop();
        if (curr == nullptr){
            if (black_count == -1){
                black_count = count;
            }
            else if (count != black_count){
                return false;
            }
        }
        else {
            int next_count = count + (curr->color_ == BLACK ? 1 : 0);
            qu.push({curr->left_, next_count});
            qu.push({curr->right_, next_count});
        }
    }
    return true;
}

auto RBTree::CheckRedBalance(Node *r) -> bool {
    if (r == nullptr){
        return true;
    }
    auto p = r->parent_;
    if (r->color_ == RED && p != nullptr && p->color_ == RED){
        return false;
    }
    return CheckRedBalance(r->left_) && CheckRedBalance(r->right_);
}