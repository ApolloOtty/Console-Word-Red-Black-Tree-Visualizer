#include <iostream>
#include <string.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include<stdlib.h>

const int RED = 0;
const int BLACK = 1;

struct Node {
  std::string word;
  int color;
  Node *left;
  Node *right;
  Node *parent;
};

void deleteCase1(Node *&root, Node *&node);

bool isNil(Node* node) {
  return !node;
}

Node* successor(Node* z) {
  if (!isNil(z->right)) {
    Node* cur = z->right;
    while (!isNil(cur->left)) {
      cur = cur->left;
    }
    return cur;
  } else {
    Node* cur = z->parent;
    while (!isNil(cur) && z == cur->right) {
      z = cur;
      cur = cur->parent;
    }
    return cur;
  }
}

void printTreeShape(Node *root, int level) {
  if (!root) return;

  printTreeShape(root->right, level + 1);
  for (int i = 0; i < level; i++) {
    std::cout << "    ";
  }
  if (root->color == RED) {
    std::cout << "\033[1;31m" << root->word << "\033[0m" << "\n";
  } else {
    std::cout << "\033[1;30m" << root->word << "\033[0m" << "\n";
  }
  printTreeShape(root->left, level + 1);
}

void print(Node *root, int level) {
  if (root == NULL) {
    return;
  }
  print(root->right, level + 1);
  std::cout << std::string(4 * level, ' ') << root->word << (root->color == RED ? "\033[1;31m" : "\033[1;30m") << std::endl;
  print(root->left, level + 1);
}


// Recursive function to insert a word into the red-black tree
void insert(Node *&root, Node *&new_node) {
  if (!root) {
    root = new_node;
    return;
  }
   if (new_node->word.compare(root->word) < 0) {
    insert(root->left, new_node);
    root->left->parent = root;
  } else {
    insert(root->right, new_node);
    root->right->parent = root;
  }

}

// Function to rotate the tree left at a given node
void rotateLeft(Node *&root, Node *&node) {
  Node *right_child = node->right;
  node->right = right_child->left;

  if (node->right) {
    node->right->parent = node;
  }

  right_child->parent = node->parent;

  if (!node->parent) {
    root = right_child;
  } else if (node == node->parent->left) {
    node->parent->left = right_child;
  } else {
    node->parent->right = right_child;
  }

  right_child->left = node;
  node->parent = right_child;
}

// Function to rotate the tree right at a given node
void rotateRight(Node *&root, Node *&node) {
  Node *left_child = node->left;
  node->left = left_child->right;

  if (node->left) {
    node->left->parent = node;
  }

  left_child->parent = node->parent;

  if (!node->parent) {
    root = left_child;
  } else if (node == node->parent->right) {
    node->parent->right = left_child;
  } else {
    node->parent->left = left_child;
  }

  left_child->right = node;
  node->parent = left_child;
}


// Function to fix any violations of the red-black tree properties after insertion
void fixViolation(Node *&root, Node *&new_node) {
  Node *parent_node = NULL;
  Node *grand_parent_node = NULL;

  // Loop until the red-black properties are restored
  while ((new_node != root) && (new_node->color != BLACK) && (new_node->parent->color == RED)) {
    parent_node = new_node->parent;
    grand_parent_node = new_node->parent->parent;

    // If the parent node is the left child of the grand parent node
    if (parent_node == grand_parent_node->left) {
      Node *uncle_node = grand_parent_node->right;

      // Case 1: The uncle node is also red
      if (uncle_node && uncle_node->color == RED) {
        grand_parent_node->color = RED;
        parent_node->color = BLACK;
        uncle_node->color = BLACK;
        new_node = grand_parent_node;
      } else {
        // Case 2: The new node is the right child of its parent
        if (new_node == parent_node->right) {
          rotateLeft(root, parent_node);
          new_node = parent_node;
          parent_node = new_node->parent;
        }

        // Case 3: The new node is the left child of its parent
        rotateRight(root, grand_parent_node);
        std::swap(parent_node->color, grand_parent_node->color);
        new_node = parent_node;
      }
    } else {
      // If the parent node is the right child of the grand parent node
      Node *uncle_node = grand_parent_node->left;

      // Case 1: The uncle node is also red
      if (uncle_node && uncle_node->color == RED) {
        grand_parent_node->color = RED;
        parent_node->color = BLACK;
        uncle_node->color = BLACK;
        new_node = grand_parent_node;
      } else {
        // Case 2: The new node is the left child of its parent
        if (new_node == parent_node->left) {
          rotateRight(root, parent_node);
          new_node = parent_node;
          parent_node = new_node->parent;
        }

        // Case 3: The new node is the right child of its parent
        rotateLeft(root, grand_parent_node);
        std::swap(parent_node->color, grand_parent_node->color);
        new_node = parent_node;
      }
    }
  }

  root->color = BLACK;
    //printTreeShape(root, 0);
//std::cout<<"________________"<<std::endl;
}

// Function to calculate the Levenshtein distance between two strings
int levenshteinDistance(std::string s1, std::string s2) {
  int m = s1.length();
  int n = s2.length();

  int dp[m + 1][n + 1];

  for (int i = 0; i <= m; i++) {
    for (int j = 0; j <= n; j++) {
      if (i == 0) {
        dp[i][j] = j;
      } else if (j == 0) {
        dp[i][j] = i;
      } else if (s1[i - 1] == s2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = 1 + std::min(dp[i][j - 1], std::min(dp[i - 1][j], dp[i - 1][j - 1]));
      }
    }
  }

  return dp[m][n];
}


// Function to rotate the tree left at a given node

// Function to insert a new word into the red-black tree
// Function to insert a new word into the red-black tree
void insertWord(Node *&root, std::string word) {
  Node *new_node = new Node;
  new_node->word = word;
  new_node->color = RED;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;

  insert(root, new_node);
   printTreeShape(root, 0);
   std::cout<<"________________"<<std::endl;
  fixViolation(root, new_node);
   printTreeShape(root, 0);
}

// Function to find the closest word to a given word in the red-black tree
std::string closestWord(Node *root, std::string word) {
  if (!root) {
    return "";
  }

  // Check the current node
  int distance = levenshteinDistance(root->word, word);
  std::string closest = root->word;

  // Check the left child
  std::string left = closestWord(root->left, word);
  int left_distance = levenshteinDistance(left, word);
  if (left_distance < distance) {
    distance = left_distance;
    closest = left;
  }

  // Check the right child
  std::string right = closestWord(root->right, word);
  int right_distance = levenshteinDistance(right, word);
  if (right_distance < distance) {
    closest = right;
  }

  return closest;
}

// Function to search for a word in the red-black tree
Node* search(Node *root, std::string word) {
  if (!root) {
    return nullptr;
  }

  std::cout<<"- Currently at node -> "<<root->word<<std::endl;

  if (root->word == word) {
    return root;
  } else if (root->word > word) {
    return search(root->left, word);
  } else {
    return search(root->right, word);
  }
}



void deleteCase6(Node *&root, Node *&node) {
  Node *sibling = (node == node->parent->left ? node->parent->right : node->parent->left);

  sibling->color = node->parent->color;
  node->parent->color = BLACK;

  if (node == node->parent->left) {
    sibling->right->color = BLACK;
    rotateLeft(root, node->parent);
  } else {
    sibling->left->color = BLACK;
    rotateRight(root, node->parent);
  }
}



void deleteCase5(Node *&root, Node *&node) {
  Node *sibling = (node == node->parent->left ? node->parent->right : node->parent->left);

  if (sibling->color == BLACK) {
    if (node == node->parent->left &&
        (!sibling->right || sibling->right->color == BLACK) &&
        (sibling->left && sibling->left->color == RED)) {
      sibling->color = RED;
      sibling->left->color = BLACK;
      rotateRight(root, sibling);
    } else if (node == node->parent->right &&
               (!sibling->left || sibling->left->color == BLACK) &&
               (sibling->right && sibling->right->color == RED)) {
      sibling->color = RED;
      sibling->right->color = BLACK;
      rotateLeft(root, sibling);
    }
  }
  deleteCase6(root, node);
}



void deleteCase4(Node *&root, Node *&node) {
  Node *sibling = (node == node->parent->left ? node->parent->right : node->parent->left);

  if (node->parent->color == RED &&
      sibling->color == BLACK &&
      (!sibling->left || sibling->left->color == BLACK) &&
      (!sibling->right || sibling->right->color == BLACK)) {
    sibling->color = RED;
    node->parent->color = BLACK;
  } else {
    deleteCase5(root, node);
  }
}



void deleteCase3(Node *&root, Node *&node) {
  Node *sibling = (node == node->parent->left ? node->parent->right : node->parent->left);

  if (node->parent->color == BLACK &&
      sibling->color == BLACK &&
      (!sibling->left || sibling->left->color == BLACK) &&
      (!sibling->right || sibling->right->color == BLACK)) {
    sibling->color = RED;
    deleteCase1(root, node->parent);
  } else {
    deleteCase4(root, node);
  }
}


void deleteCase2(Node *&root, Node *&node) {
  Node *sibling = (node == node->parent->left ? node->parent->right : node->parent->left);

  if (sibling->color == RED) {
    node->parent->color = RED;
    sibling->color = BLACK;
    if (node == node->parent->left) {
      rotateLeft(root, node->parent);
    } else {
      rotateRight(root, node->parent);
    }
  }

  deleteCase3(root, node);
}


void deleteCase1(Node *&root, Node *&node) {
  if (node->parent) {
    deleteCase2(root, node);
  }
}

void deleteNode(Node *&root, Node *node) {
  // When the node has two children
  if (node->left && node->right) {
    // Find the successor of the node
    Node *successornode = successor(node);

    // Copy the value of the successornode to the node
    node->word = successornode->word;

    // Delete the successornode
    deleteNode(root, successornode);
  } else {
    // Store the child of the node
    Node *child = (node->left ? node->left : node->right);

    // If the node is black, we need to fix any violations of the
    // Red-Black Tree properties after deletion
    if (node->color == BLACK) {
      if (child && child->color == RED) {
        child->color = BLACK;
      } else {
        deleteCase1(root, node);
      }
    }

    // Replace the node with its child
    if (node->parent) {
      if (node == node->parent->left) {
        node->parent->left = child;
      } else {
        node->parent->right = child;
      }
    } else {
      root = child;
    }

    if (child) {
      child->parent = node->parent;
    }

    delete node;
  }
  printTreeShape(root, 0);
  std::cout<<"____________________"<<std::endl;
}

void inorder(Node* root) {
  if (root == nullptr) {
    return;
  }
  inorder(root->left);
  std::cout <<"\033[32m"<< root->word <<"\033[0m"<< std::endl;
  inorder(root->right);
}

int black_height(Node* root) {
  if (root == nullptr) {
    return 0;
  }
  return std::max(black_height(root->left), black_height(root->right)) + 1;
}


std::string successor(Node* root, Node* node) {
  if (node == nullptr || root == nullptr) {
    return nullptr;
  }

  if (node->left != nullptr) {
    // If the node has a left child, its predecessor is the rightmost node in its left subtree
    Node* current = node->left;
    while (current->right != nullptr) {
      current = current->right;
    }
    return current->word;
  } else {
    // If the node does not have a left child, its predecessor is the first ancestor that is the right child of its parent
    Node* current = node;
    Node* parent = current->parent;
    while (parent != nullptr && current == parent->left) {
      current = parent;
      parent = current->parent;
    }
    return parent->word;
  }
}


std::string predecessor(Node* root, Node* node) {
  if (node == nullptr || root == nullptr) {
    return nullptr;
  }

  if (node->right != nullptr) {
    // If the node has a right child, its predecessor is the leftmost node in its right subtree
    Node* current = node->right;
    while (current->left != nullptr) {
      current = current->left;
    }
    return current->word;
  } else {
    // If the node does not have a right child, its predecessor is the first ancestor that is the left child of its parent
    Node* current = node;
    Node* parent = current->parent;
    while (parent != nullptr && current == parent->right) {
      current = parent;
      parent = current->parent;
    }
    return parent->word;
  }
}

std::string maximum(Node* root) {
  if (root == nullptr) {
    return nullptr;
  }
  if (root->right == nullptr) {
    return root->word;
  }
  return maximum(root->right);
}

int depth(Node* root) {
  if (root == nullptr) return 0;
  return 1 + std::max(depth(root->left), depth(root->right));
}

int main() {
    int choice=0;
  Node *root = NULL;

  std::ifstream file("cuvinte.txt");
   std::vector<std::string> words;

    // Read the words from the file and store them in the vector
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }

  file.close();

/*while (file >> word) {
  std::cout<<"\033[0m";
  std::cout<<"Inserting "<<word<<std::endl<<std::endl;
  //print(root);
  std::cout<<"________________"<<std::endl;
  insertWord(root, word);
  print(root);
  std::cout<<"________________"<<std::endl;
}
*/

 for (const auto& w : words) {
   //std::cout<<"\033[0m";
   std::cout<<std::endl<<std::endl;
     std::cout<<"Inserting "<<w<<std::endl<<std::endl;
   insertWord(root, w);
    }

//deleteNode(root, search(root, "classroom"));

  // Insert some words into the red-black tree
  //insertWord(root, "hello");
  //insertWord(root, "world");
  //insertWord(root, "apple");
  //insertWord(root, "banana");
  //insertWord(root, "cherry");

  // Check the spelling of a word and suggest correct spellings if it is not spelled correctly
//  std::string word;
std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
std::string yon;
while(choice!=0){
    if(choice==1){
                    std::cout<<std::endl;

        std::cout<<"Type the word you want to delete: ";
        std::cin>>word;
         if (search(root, word)) {
                deleteNode(root, search(root, word));
                  std::ifstream in_file;
                  std::ofstream out_file;
                  std::string word_to_delete = word;

                  // Open the input and output files
                  in_file.open("cuvinte.txt");
                  out_file.open("cuvinteOutput.txt");

                  // Read the words from the input file one by one
                  while (in_file >> word) {
                    // If the current word is not the word to delete, write it to the output file
                    if (word != word_to_delete) {
                      out_file << word << std::endl;
                    }
                  }
                  in_file.close();
                  out_file.close();

                  std::ifstream in_file2;
                  std::ofstream out_file2;
                  std::string line;

                  // Open the input file in input mode and the output file in output mode
                  in_file2.open("cuvinteOutput.txt");
                  out_file2.open("cuvinte.txt");
                    while (std::getline(in_file2, line)) {
                            // Write the line to the output file
                            out_file2 << line << std::endl;
                          }

                  in_file.close();
                  out_file.close();

std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
         }else {

             std::cout<<"The word doesn't exist in the tree! Do you want to add it? (y/n)";
            std::cin>>yon;
            if(yon=="y"){
                insertWord(root, word);
std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
            }
            else if (yon=="n"){
std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
            }else {
                std::cout<<"That is not a valid option! Type y(yes) or n(no): ";
            }
         }

    }else if (choice==2){
                std::cout<<std::endl;

        std::cout<<"\033[0m";
        std::cout << "Enter a word to check its spelling: ";
        std::cin >> word;
        if (search(root, word)) {
    std::cout <<"\033[32m"<< "The word is spelled correctly. \033[0m" << std::endl<< std::endl;
  } else {
    std::cout <<"\033[33m"<< "The word is not spelled correctly. \033[0m" << std::endl;
    std::cout <<"\033[45m"<< "Did you mean: " << closestWord(root, word) << "?\033[0m" << std::endl<< std::endl;
  }
std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
std::cout<<std::endl;
    }else if (choice==3){
        std::cout<<std::endl;
        std::cout<<"Type the word you want to insert into the tree: ";
        std::cin>>word;
        if(search (root, word)){
            std::cout<<"The word already exists in the tree!";
        }else{
            std::cout<<"Inserting "<<word<<std::endl;
            insertWord(root, word);
            std::ofstream file;
             file.open("cuvinte.txt", std::ios_base::app);

            // Write the word to the file, followed by a newline character
            file << word << std::endl;

            // Close the file
            file.close();
                    std::cout<<std::endl;
std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
        }
    }else if (choice==4){
                std::cout<<std::endl;
        inorder(root);
std::cout<<std::endl<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
    }
    else if(choice==5){
                    std::cout<<std::endl;

        std::cout<<"\033[32m"<<black_height(root)<<"\033[0m"<<std::endl;
std::cout<<std::endl<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
    }
    else if(choice==6){
                    std::cout<<std::endl;

        std::cout<<"Type the node you want to see the predecessor for: ";
            std::cin>>word;
        std::cout<<"\033[32m"<<successor(root, search(root, word))<<"\033[0m"<<std::endl<<std::endl;
std::cout<<std::endl<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
    }
    else if (choice==7){
                    std::cout<<std::endl;

        std::cout<<"Type the node you want to see the predecessor for: ";
            std::cin>>word;
        std::cout<<"\033[32m"<<predecessor(root, search(root, word))<<"\033[0m"<<std::endl<<std::endl;
std::cout<<std::endl<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
    }
    else if (choice==8){
        std::cout<<std::endl;

        std::cout<<"\033[32m"<<depth(root)<<"\033[0m"<<std::endl;
        std::cout<<"1. Delete a node"<<std::endl
         <<"2. Check word"<<std::endl
         <<"3. Insert word"<<std::endl
         <<"4. Inorder traversal of the tree"<<std::endl
         <<"5. Black height of the tree"<<std::endl
         <<"6. Get successor of node"<<std::endl
         <<"7. Get predecessor of node"<<std::endl
         <<"8. Get depth of tree"<<std::endl;

std::cout<<"Pick an option: ";
std::cin>>choice;
    }
    else {std::cout<<"The chosen option is not correct! Pick an option 1-6: ";
        std::cin>>choice;
    }

}
  return 0;
}

