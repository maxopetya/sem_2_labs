//
// BSTree — всё в одном файле
//

#include <iostream>
#include <fstream>
#include <string>

namespace tree {

    class BSTree {
    private:
        // Структура узла дерева
        struct Node {
            int data;
            Node* left;
            Node* right;
            Node(int val) : data(val), left(nullptr), right(nullptr) {}
        };

        Node* root_; // Корень дерева

        // Рекурсивное копирование дерева
        Node* copyTree(Node* node) {
            if (!node) return nullptr;
            Node* newNode = new Node(node->data);
            newNode->left = copyTree(node->left);
            newNode->right = copyTree(node->right);
            return newNode;
        }

        // Рекурсивное удаление всех узлов дерева
        void destroyTree(Node* node) {
            if (!node) return;
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }

        // Рекурсивная вставка узла
        Node* insertNode(Node* node, int val) {
            if (!node) return new Node(val);
            if (val < node->data)
                node->left = insertNode(node->left, val);
            else if (val > node->data)
                node->right = insertNode(node->right, val);
            return node;
        }

        // Рекурсивное удаление узла по значению
        Node* removeNode(Node* node, int val) {
            if (!node) return nullptr;
            if (val < node->data) {
                node->left = removeNode(node->left, val);
            } else if (val > node->data) {
                node->right = removeNode(node->right, val);
            } else {
                // Узел найден — три случая удаления
                if (!node->left) {
                    Node* right = node->right;
                    delete node;
                    return right;
                }
                if (!node->right) {
                    Node* left = node->left;
                    delete node;
                    return left;
                }
                // У узла два потомка — заменяем минимальным из правого поддерева
                Node* minRight = findMin(node->right);
                node->data = minRight->data;
                node->right = removeNode(node->right, minRight->data);
            }
            return node;
        }

        // Поиск минимального узла в поддереве
        Node* findMin(Node* node) const {
            while (node && node->left) node = node->left;
            return node;
        }

        // Рекурсивный поиск значения в дереве
        bool searchNode(Node* node, int val) const {
            if (!node) return false;
            if (val == node->data) return true;
            if (val < node->data) return searchNode(node->left, val);
            return searchNode(node->right, val);
        }

        // Прямой обход (корень-левый-правый)
        void preorderTraversal(Node* node, std::ostream& os) const {
            if (!node) return;
            os << node->data << " ";
            preorderTraversal(node->left, os);
            preorderTraversal(node->right, os);
        }

        // Симметричный обход (левый-корень-правый)
        void inorderTraversal(Node* node, std::ostream& os) const {
            if (!node) return;
            inorderTraversal(node->left, os);
            os << node->data << " ";
            inorderTraversal(node->right, os);
        }

        // Обратный обход (левый-правый-корень)
        void postorderTraversal(Node* node, std::ostream& os) const {
            if (!node) return;
            postorderTraversal(node->left, os);
            postorderTraversal(node->right, os);
            os << node->data << " ";
        }

        // Визуальный вывод дерева (повёрнутое на 90 градусов)
        void printTree(Node* node, std::ostream& os, int level) const {
            if (!node) return;
            printTree(node->right, os, level + 1);
            for (int i = 0; i < level; ++i) os << "    ";
            os << node->data << std::endl;
            printTree(node->left, os, level + 1);
        }

        // Рекурсивная запись узлов в файл (прямой обход)
        void saveNode(Node* node, std::ofstream& file) const {
            if (!node) return;
            file << node->data << " ";
            saveNode(node->left, file);
            saveNode(node->right, file);
        }

    public:
        // Конструктор по умолчанию
        BSTree() : root_(nullptr) {}

        // Конструктор копирования
        BSTree(const BSTree& other) : root_(copyTree(other.root_)) {}

        // Конструктор перемещения
        BSTree(BSTree&& other) noexcept : root_(other.root_) {
            other.root_ = nullptr;
        }

        // Деструктор
        ~BSTree() {
            destroyTree(root_);
        }

        // Оператор присваивания копированием
        BSTree& operator=(const BSTree& other) {
            if (this != &other) {
                destroyTree(root_);
                root_ = copyTree(other.root_);
            }
            return *this;
        }

        // Оператор присваивания перемещением
        BSTree& operator=(BSTree&& other) noexcept {
            if (this != &other) {
                destroyTree(root_);
                root_ = other.root_;
                other.root_ = nullptr;
            }
            return *this;
        }

        // Добавление узла в дерево
        void insert(int val) { root_ = insertNode(root_, val); }

        // Удаление узла из дерева
        void remove(int val) { root_ = removeNode(root_, val); }

        // Удаление всего дерева
        void clear() {
            destroyTree(root_);
            root_ = nullptr;
        }

        // Поиск элемента в дереве
        bool search(int val) const { return searchNode(root_, val); }

        // Прямой обход
        void preorder(std::ostream& os = std::cout) const {
            preorderTraversal(root_, os);
            os << std::endl;
        }

        // Симметричный обход
        void inorder(std::ostream& os = std::cout) const {
            inorderTraversal(root_, os);
            os << std::endl;
        }

        // Обратный обход
        void postorder(std::ostream& os = std::cout) const {
            postorderTraversal(root_, os);
            os << std::endl;
        }

        // Сохранение дерева в файл
        void saveToFile(const std::string& filename) const {
            std::ofstream file(filename);
            if (!file.is_open()) return;
            saveNode(root_, file);
            file.close();
        }

        // Чтение дерева из файла
        void loadFromFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) return;
            clear();
            int val;
            while (file >> val) insert(val);
            file.close();
        }

        // Перегрузка оператора вывода в поток
        friend std::ostream& operator<<(std::ostream& os, const BSTree& bst) {
            bst.printTree(bst.root_, os, 0);
            return os;
        }
    };

} // namespace tree

int main() {
    // Создание дерева и добавление элементов
    tree::BSTree bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    // Вывод дерева через перегруженный оператор
    std::cout << "Tree:" << std::endl;
    std::cout << bst;

    // Обходы дерева
    std::cout << "Preorder: ";
    bst.preorder();
    std::cout << "Inorder: ";
    bst.inorder();
    std::cout << "Postorder: ";
    bst.postorder();

    // Поиск элементов
    std::cout << "Search 40: " << (bst.search(40) ? "found" : "not found") << std::endl;
    std::cout << "Search 99: " << (bst.search(99) ? "found" : "not found") << std::endl;

    // Демонстрация конструктора копирования
    tree::BSTree bst2(bst);
    std::cout << "Copied tree:" << std::endl;
    std::cout << bst2;

    // Демонстрация конструктора перемещения
    tree::BSTree bst3(std::move(bst2));
    std::cout << "Moved tree:" << std::endl;
    std::cout << bst3;

    // Удаление узла
    bst.remove(30);
    std::cout << "After removing 30:" << std::endl;
    std::cout << bst;

    // Сохранение в файл и загрузка из файла
    bst.saveToFile("tree.txt");
    tree::BSTree bst4;
    bst4.loadFromFile("tree.txt");
    std::cout << "Loaded from file:" << std::endl;
    std::cout << bst4;

    // Очистка дерева
    bst.clear();
    std::cout << "After clearing:" << std::endl;
    std::cout << bst;

    return 0;
}
