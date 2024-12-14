template <typename T>
class Stack {
private:
    struct StackNode {
        T data;
        StackNode* next;

        StackNode(T nodeData) : data(nodeData), next(nullptr) {}
    };

    StackNode* top;

public:
    Stack() : top(nullptr) {}

    // Push an item onto the stack
    void push(T item) {
        StackNode* newStackNode = new StackNode(item);
        newStackNode->next = top;
        top = newStackNode;
    }

    // Pop an item from the stack and return it
    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty, cannot pop.");
        }
        StackNode* temp = top;
        T item = top->data;
        top = top->next;
        delete temp;
        return item;
    }

    // Peek at the top item of the stack without removing it
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Stack is empty, cannot peek.");
        }
        return top->data;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }

    // Destructor to clean up memory
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};
