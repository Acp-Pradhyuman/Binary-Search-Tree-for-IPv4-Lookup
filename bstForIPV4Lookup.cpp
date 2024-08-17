#include <iostream>
#include <string>
// #include <sstream>
// #include <vector>
// #include <bitset>

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
        // vector<int> parts;

        // Initialized to 0.0.0.0
        int parts[4] = {0};
        // stringstream ss(ip);
        // string item;
        // while (getline(ss, item, '.')) {
        //     parts.push_back(stoi(item));
        // }

        int num = 0, index = 0;
        if(!(ip[0] >= '0' && ip[0] <= '9')) {
            return -1;
        }
        
        // for(int i=0; i<ip.size(); i++) {
        //     if(index >= 4) {
        //         return -1;
        //     }

        //     if(ip[i] >= '0' && ip[i] <= '9') {
        //         num = num*10 + ip[i] - '0';
        //     }
        //     else if((index == 3) && !ip[i]) {
        //         parts[index] = num;
        //     }
        //     else if(ip[i] == '.') {
        //         parts[index++] = num;
        //         num = 0;
        //     }
        //     else {
        //         return -1;
        //     }
        // }

        for (char ch : ip) {
            if (ch >= '0' && ch <= '9') {
                num = num * 10 + (ch - '0');
            } else if (ch == '.') {
                if (index >= 4 || num > 255) 
                return -1;  // More than 4 parts or segment > 255
                parts[index++] = num;
                num = 0;
            } else {
                return -1;  // Invalid character
            }
        }

        if (index >= 4 || num > 255) return -1;
        // last num is not added within the loop 
        // since we have null instead of "."
        parts[index] = num;

        // // Invalid IP address scenario
        // if ((parts[0] >= 256) || (parts[1] >= 256) || (parts[2] >= 256) ||
        //     (parts[3] >= 256)) {
        //     return -1;
        // }
        return (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
    }

    // Helper function to convert integer to IP address
    string intToIp(unsigned int num) {
        // Convert the integer to the dotted-decimal IP address format
        return to_string((num >> 24) & 0xFF) + "." +
            to_string((num >> 16) & 0xFF) + "." +
            to_string((num >> 8) & 0xFF) + "." +
            to_string(num & 0xFF);
    }

    // Helper function to insert a new IP address
    TreeNode* insert(TreeNode* node, unsigned int ip) {
        if (!node) {
            return new TreeNode(ip);
        }
        if (ip < node->ipAddress) {
            node->left = insert(node->left, ip);
        }
        else if (ip > node->ipAddress)  {
            node->right = insert(node->right, ip);
        }
        else {
            cout << "IP address " << intToIp(ip) << " already exists" << endl;
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

    // User API's for insert, search, delete and inorder travresal

    void insert(const string& ip) {
        unsigned int ipInt = ipToInt(ip);
        if (ipInt == -1) {
            cout << "Invalid IP: " << ip << endl;
            return;
        }
        else {
            cout << "Valid IP: " << ip << endl;
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

    void inorderTraversal() {
        // morris traversal variation
        // advantage is O(1) space complexity
        // i.e we avoid the stack space due to recursion
        TreeNode* curr = root;

        // LNR
        while(curr) {
            if(!curr->left) {
                cout<<intToIp(curr->ipAddress)<<", ";
                curr = curr->right;
            } else {
                TreeNode* pre = curr->left;
                while(pre->right && pre->right != curr) {
                    pre = pre->right;
                }

                if(!pre->right) {
                    // create connection from predecessor to current node
                    pre->right = curr;
                    curr = curr->left;
                } else {
                    // remove connection from predecessor to current node
                    pre->right = NULL;
                    cout<<intToIp(curr->ipAddress)<<", ";
                    curr = curr->right;
                }
            }
        }
    }
};

// Example usage
int main() {
    BinarySearchTree bst;

    // Insert IP addresses
    cout << "Insert IP addresses: " << endl;
    bst.insert("192.168.1.5");
    bst.insert("10.8.2.5");
    bst.insert("256.8.2.5");


    // Search for IP addresses
    cout << endl << "Search for IP addresses: " << endl;
    cout << "Search 192.168.1.5: " << (bst.search("192.168.1.5") ? "Found" : "Not Found") << endl;
    cout << "Search 10.0.0.1: " << (bst.search("10.0.0.1") ? "Found" : "Not Found") << endl;
    cout << "Search 256.8.2.5: " << (bst.search("256.8.2.5") ? "Found" : "Not Found") << endl;

    // Delete an IP address
    cout << endl << "Delete an IP address: " << endl;
    bst.deleteNode("192.168.1.5");
    cout << "Search 192.168.1.5 after deletion: " << (bst.search("192.168.1.5") ? "Found" : "Not Found") << endl;

    cout << endl;
    bst.insert("10.8.2.5");
    bst.insert("10.0.0.1");
    bst.insert("140.0.0.1");
    bst.insert("5.80.35.6");

    // Inorder traversal
    cout << endl << "Inorder traversal: " << endl;
    bst.inorderTraversal();
    cout << endl;

    return 0;
}