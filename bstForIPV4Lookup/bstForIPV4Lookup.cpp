#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>

using namespace std;

class TreeNode {
public:
    unsigned int ipAddress;  // Store IP address as an integer
    TreeNode* left;
    TreeNode* right;

    //TreeNode(unsigned int ip) : ipAddress(ip), left(nullptr), right(nullptr) {}
    TreeNode(unsigned int ip) {
        ipAddress = ip;
        left = right = NULL;
    }
};

class BinarySearchTree {
private:
    TreeNode* root;

    // Helper function to convert IP address to integer
    unsigned int ipToInt(const string& ip) {
        vector<int> parts;
        stringstream ss(ip);
        string item;
        while (getline(ss, item, '.')) {
            parts.push_back(stoi(item));
        }

        // Invalid IP address scenario
        if ((parts[0] >= 256) || (parts[1] >= 256) || (parts[2] >= 256) ||
            (parts[3] >= 256)) {
            return -1;
        }
        return (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
    }

    // Helper function to convert integer to IP address
    string intToIp(unsigned int num) {
        return to_string((num >> 24) & 255) + "." +
            to_string((num >> 16) & 255) + "." +
            to_string((num >> 8) & 255) + "." +
            to_string(num & 255);
    }

    // Helper function to insert a new IP address
    TreeNode* insert(TreeNode* node, unsigned int ip) {
        if (!node) {
            return new TreeNode(ip);
        }
        if (ip < node->ipAddress) {
            node->left = insert(node->left, ip);
        }
        else {
            node->right = insert(node->right, ip);
        }
        return node;
    }

    // Helper function to search for an IP address
    bool search(TreeNode* node, unsigned int ip) {
        if (!node) {
            return false;
        }
        if (ip == node->ipAddress) {
            return true;
        }
        if (ip < node->ipAddress) {
            return search(node->left, ip);
        }
        else {
            return search(node->right, ip);
        }
    }

    // Helper function to find the minimum value node
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    // Helper function to delete an IP address
    TreeNode* deleteNode(TreeNode* root, unsigned int ip) {
        if (!root) {
            return root;
        }
        if (ip < root->ipAddress) {
            root->left = deleteNode(root->left, ip);
        }
        else if (ip > root->ipAddress) {
            root->right = deleteNode(root->right, ip);
        }
        else {
            // Node with only one child or no child
            if (!root->left) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            }
            else if (!root->right) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }
            // Node with two children
            TreeNode* temp = minValueNode(root->right);
            root->ipAddress = temp->ipAddress;
            root->right = deleteNode(root->right, temp->ipAddress);
        }
        return root;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(const string& ip) {
        unsigned int ipInt = ipToInt(ip);
        if (ipInt == -1) {
            cout << "Out of bound" << endl;
            return;
        }
        else {
            cout << "Within bound" << endl;
        }
        root = insert(root, ipInt);
    }

    bool search(const string& ip) {
        unsigned int ipInt = ipToInt(ip);
        return search(root, ipInt);
    }

    void deleteNode(const string& ip) {
        unsigned int ipInt = ipToInt(ip);
        root = deleteNode(root, ipInt);
    }
};

// Example usage
int main() {
    BinarySearchTree bst;

    // Insert IP addresses
    bst.insert("192.168.1.5");
    bst.insert("10.8.2.5");
    bst.insert("256.8.2.5");

    // Search for IP addresses
    cout << "Search 192.168.1.5: " << (bst.search("192.168.1.5") ? "Found" : "Not Found") << endl;
    cout << "Search 10.0.0.1: " << (bst.search("10.0.0.1") ? "Found" : "Not Found") << endl;

    cout << "Search 256.8.2.5: " << (bst.search("256.8.2.5") ? "Found" : "Not Found") << endl;

    // Delete an IP address
    bst.deleteNode("192.168.1.5");
    cout << "Search 192.168.1.5 after deletion: " << (bst.search("192.168.1.5") ? "Found" : "Not Found") << endl;

    return 0;
}