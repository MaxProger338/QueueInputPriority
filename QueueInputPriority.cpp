#include <iostream>
#include <initializer_list>
using namespace std;

template <typename T>
class QueuePriorityInput
{
    private:
        class Node
        {
            public:
                T value;
                int priority;

                Node() : Node(0, 0) {}
                Node(T value, int priority) : value{ value }, priority{ priority } {}

                Node& operator=(const Node& src)
                {
                    if (this == &src)
                        return *this;

                    this->value    = src.value;
                    this->priority = src.priority;

                    return *this;
                }

                friend bool operator>(const Node& node1, const Node& node2)
                {
                    return node1.priority > node2.priority;
                }

                friend bool operator<(const Node& node1, const Node& node2)
                {
                    return node1.priority < node2.priority;
                }

                friend bool operator==(const Node& node1, const Node& node2)
                {
                    return node1.priority == node2.priority;
                }

                friend bool operator!=(const Node& node1, const Node& node2)
                {
                    return node1.priority != node2.priority;
                }
        };

        Node* _arr;
        Node* _top;
        Node* _end;

    public:
        explicit QueuePriorityInput(int size)
        {
            _arr = new Node[size];
            _top = _arr - 1;
            _end = _arr + size - 1;
        }

        QueuePriorityInput(int size, initializer_list<Node> list): QueuePriorityInput(size)
        {
            if (list.size() > size)
                throw "Out of range";

            for (auto i : list)
            {
                enqueue(i.value, i.priority);
            }
        }

        QueuePriorityInput(const QueuePriorityInput& queue)
        {
            _arr = new Node[queue.getQueueSize()];
            for (size_t i = 0; i < queue.getQueueSize(); i++)
            {
                _arr[i] = queue._arr[i];
            }

            _end = _arr + queue.getQueueSize() - 1;
            _top = _end - (queue.getQueueSize() - queue.getCurrentSize());
        }

        QueuePriorityInput(QueuePriorityInput&& queue)
        {
            _arr = queue._arr;
            _top = queue._top;
            _end = queue._end;

            queue._arr = nullptr;
            queue._top = nullptr;
            queue._end = nullptr;
        }

        ~QueuePriorityInput()
        {
            delete[] _arr;
            _arr = nullptr;
            _top = nullptr;
            _end = nullptr;
        }

        bool isFull() const
        {
            return _top == _end;
        }

        bool isEmpty() const
        {
            return _top + 1 == _arr;
        }

        int getCurrentSize() const
        {
            return _top - _arr + 1;
        }

        int getQueueSize() const
        {
            return _end - _arr + 1;
        }

        QueuePriorityInput& clear()
        {
            _top = _arr - 1;
            return *this;
        }

        QueuePriorityInput& enqueue(T value, int priority)
        {
            Node node{ value, priority };

            if (isEmpty())
            {
                *++_top = node;
                return *this;
            }

            if (isFull())
                return *this;

            Node* position = _arr;
            while (node < *position || node == *position)
            {
                position++;
            }

            _top++;

            Node* current = _top;
            while (current > position)
            {
                *current = *(current - 1);
                current--;
            }

            *position = node;

            return *this;
        }

        T dequeue()
        {
            if (isEmpty())
                throw "Out of range";

            T res         = _arr->value;

            Node* current = _arr;
            while (current < _top)
            {
                *current = *(current + 1);
                current++;
            }

            _top--;

            return res;
        }

        T peek()
        {
            if (isEmpty())
                throw "Out of range";

            return _arr->value;
        }

        friend ostream& operator<<(ostream& out, const QueuePriorityInput& queue)
        {
            Node* current = queue._arr;
            while (current <= queue._top)
            {
                out << current->value << '(' << current->priority << ')' << '\t';
                current++;
            }
            out << endl;

            return out;
        }
};

int main()
{
    QueuePriorityInput<int> q(3);

    q.enqueue(4, 1);
    q.enqueue(14, 2);
    q.enqueue(17, 3);

    cout << q << endl;

    return 0;
}
