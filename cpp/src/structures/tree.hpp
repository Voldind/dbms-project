#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "queue.hpp"

template <typename T>
class CompleteBinaryTree {
   private:
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* m_root;

    bool find(Node* node, const T& value) const {
        if (node == nullptr) return false;
        if (node->data == value) return true;
        return find(node->left, value) || find(node->right, value);
    }
    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    void preOrder(Node* node) const {
        if (node == nullptr) return;
        std::cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        std::cout << node->data << " ";
        inOrder(node->right);
    }
    void postOrder(Node* node) const {
        if (node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->data << " ";
    }

   public:
    CompleteBinaryTree() : m_root(nullptr) {}
    ~CompleteBinaryTree() { clear(m_root); }
    bool find(const T& value) const { return find(m_root, value); }
    void clear() {
        clear(m_root);
        m_root = nullptr;
    }
    void insert(const T& value) {
        if (m_root == nullptr) {
            m_root = new Node(value);
            return;
        }

        Queue<Node*> q;
        q.push(m_root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current->left == nullptr) {
                current->left = new Node(value);
                return;
            } else {
                q.push(current->left);
            }

            if (current->right == nullptr) {
                current->right = new Node(value);
                return;
            } else {
                q.push(current->right);
            }
        }
    }
    void printPreOrder() const {
        preOrder(m_root);
        std::cout << std::endl;
    }
    void printInOrder() const {
        inOrder(m_root);
        std::cout << std::endl;
    }
    void printPostOrder() const {
        postOrder(m_root);
        std::cout << std::endl;
    }
    void print() const {
        if (m_root == nullptr) {
            std::cout << "[]" << std::endl;
            return;
        }
        Queue<Node*> q;
        q.push(m_root);
        std::cout << "[ ";
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            std::cout << current->data << " ";

            if (current->left != nullptr) q.push(current->left);
            if (current->right != nullptr) q.push(current->right);
        }
        std::cout << "]" << std::endl;
    }
    bool isComplete() const {
        if (m_root == nullptr) return true;

        Queue<Node*> q;
        q.push(m_root);

        bool flag_null_encountered = false;

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current == nullptr) {
                flag_null_encountered = true;
            } else {
                if (flag_null_encountered) {
                    return false;
                }
                q.push(current->left);
                q.push(current->right);
            }
        }
        return true;
    }
    std::vector<T> to_vector() const {
        std::vector<T> res;
        if (m_root == nullptr) return res;

        Queue<Node*> q;
        q.push(m_root);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            res.push_back(current->data);
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        return res;
    }
};