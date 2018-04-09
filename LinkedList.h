#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

	template<typename Type>
	class LinkedList {
	private:
		class Node {
		public:
			Type data;
			Node *next, *prev;

			Node(const Type &value) : data(value), next(nullptr), prev(nullptr) {}

			Node() : next(nullptr), prev(nullptr) {}
		};

		Node *head, *tail;

	public:
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using value_type = Type;
		using pointer = Type *;
		using reference = Type &;
		using const_pointer = const Type *;
		using const_reference = const Type &;

		class ConstIterator;

		class Iterator;

		using iterator = Iterator;
		using const_iterator = ConstIterator;
		size_type size = 0;

		LinkedList() {
			Node *sentinel = new Node();
			head = sentinel;
			tail = sentinel;
		}

		LinkedList(std::initializer_list<Type> l) {
			Node *sentinel = new Node();
			head = sentinel;
			tail = sentinel;
			for (auto it = l.begin(); it != l.end(); ++it)
				append(*it);
		}

		LinkedList(const LinkedList &other) {
			Node *sentinel = new Node();
			head = sentinel;
			tail = sentinel;
			for (auto it = other.begin(); it != other.end(); ++it) {
				append(*it);
			}
		}

		LinkedList(LinkedList &&other) : head(other.head), tail(other.tail), size(other.size) {
			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
		}

		~LinkedList() {
			if (head != tail) {
				auto tmp = head;
				for (auto pos = head->next; pos != tail->next; pos = pos->next) {
					tmp = pos->prev;
					delete (tmp);
				}
			}
			delete (tail);
		}

		LinkedList &operator=(const LinkedList &other) {
			if (this == &other) return *this;
			erase(this->begin(), this->end());
			for (auto it = other.begin(); it != other.end(); ++it) append(*it);
			return *this;
		}

		LinkedList &operator=(LinkedList &&other) {
			if (this == &other) return *this;
			erase(this->begin(), this->end());
			delete (tail);
			head = other.head;
			tail = other.tail;
			size = other.size;
			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
			return *this;
		}

		bool isEmpty() const {
			return size == 0;
		}

		size_type getSize() const {
			return size;
		}

		void append(const Type &item) {

			Node *newNode = new Node(item);
			if (isEmpty()) {
				head = newNode;
				newNode->next = tail;
				tail->prev = newNode;
				tail->next = nullptr;
			} else {
				tail->prev->next = newNode;
				newNode->next = tail;
				newNode->prev = tail->prev;
				tail->prev = newNode;
			}
			size++;
			return;
		}

		void prepend(const Type &item) {
			Node *newNode = new Node(item);
			if (isEmpty()) {
				head = newNode;
				newNode->next = tail;
				tail->prev = newNode;
				tail->next = nullptr;
			} else {
				head->prev = newNode;
				newNode->next = head;
				head = newNode;
			}
			size++;
			return;
		}

		void insert(const const_iterator &insertPosition, const Type &item) {
			if (insertPosition.getCurrent() == head) {
				prepend(item);
				return;
			}
			Node *newNode = new Node(item);
			insertPosition.getCurrent()->prev->next = newNode;
			newNode->prev = insertPosition.getCurrent()->prev;
			insertPosition.getCurrent()->prev = newNode;
			newNode->next = insertPosition.getCurrent();
			size++;
			return;
		}

		Type popFirst() {
			if (isEmpty()) throw std::out_of_range("popFirst");
			Type tmpData = head->data;
			Node *tmp = head->next;
			delete (head);
			head = tmp;
			size--;
			return tmpData;
		}

		Type popLast() {
			if (isEmpty()) throw std::out_of_range("popLast");
			Node *tmp = tail->prev;
			Type tmpData = tmp->data;
			tail->prev = tail->prev->prev;
			if (tail->prev != nullptr) tail->prev->next = tail;
			delete (tmp);
			size--;
			return tmpData;
		}

		void erase(const const_iterator &possition) {
			if (possition.getCurrent()->next == nullptr) throw std::out_of_range("erase");
			if (possition.getCurrent()->prev == nullptr) {
				Node *tmp = head->next;
				delete (head);
				head = tmp;
				tmp->prev = nullptr;
				size--;
				return;
			}
			possition.getCurrent()->next->prev = possition.getCurrent()->prev;
			possition.getCurrent()->prev->next = possition.getCurrent()->next;
			delete (possition.getCurrent());
			size--;
			return;
		}

		void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
			/*for (auto it = firstIncluded; it != lastExcluded; ++it) {
				erase(it);
			}*/

			if(firstIncluded != begin()) {
				auto tmpIt = firstIncluded;
				tmpIt--;
				for (auto pos = firstIncluded.getCurrent()->next; pos != lastExcluded.getCurrent()->next; pos = pos->next) {
					delete (pos->prev);
					size--;
				}
				tmpIt.getCurrent()->next = lastExcluded.getCurrent();
				lastExcluded.getCurrent()->prev = tmpIt.getCurrent();
			}
			else{
				for (auto pos = firstIncluded.getCurrent()->next; pos != lastExcluded.getCurrent()->next; pos = pos->next) {
					delete (pos->prev);
					size--;
				}
				head = lastExcluded.getCurrent();
				lastExcluded.getCurrent()->prev = nullptr;
			}

		}

		iterator begin() {
			return iterator(head);
		}

		iterator end() {
			return iterator(tail);
		}

		const_iterator cbegin() const {
			return const_iterator(head);
		}

		const_iterator cend() const {
			return const_iterator(tail);
		}

		const_iterator begin() const {
			return cbegin();
		}

		const_iterator end() const {
			return cend();
		}
	};

	template<typename Type>
	class LinkedList<Type>::ConstIterator {
	private:
		Node *current;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename LinkedList::value_type;
		using difference_type = typename LinkedList::difference_type;
		using pointer = typename LinkedList::const_pointer;
		using reference = typename LinkedList::const_reference;


		explicit ConstIterator() {}

		ConstIterator(Node *node) : current(node) {}

		Node *getCurrent() const {
			return current;
		}

		reference operator*() const {
			if (current->next == nullptr) throw std::out_of_range("op*");
			return current->data;
		}

		ConstIterator &operator++() {
			if (current->next == nullptr) throw std::out_of_range("op++");
			current = current->next;
			return *this;
		}

		ConstIterator operator++(int) {
			ConstIterator tmp = *this;
			++(*this);
			return tmp;
		}

		ConstIterator &operator--()
		{
			if (current->prev == nullptr) throw std::out_of_range("op--");
			current = current->prev;
			return *this;
		}

		ConstIterator operator--(int) {
			ConstIterator tmp = *this;
			--(*this);
			return tmp;
		}

		ConstIterator operator+(difference_type d) const {
			ConstIterator tmp = *this;
			for (int i = 0; i < d; i++) {
				tmp++;
			}
			return tmp;
		}

		ConstIterator operator-(difference_type d) const {
			ConstIterator tmp = *this;
			for (int i = 0; i < d; i++) {
				tmp--;
			}
			return tmp;
		}

		bool operator==(const ConstIterator &other) const {
			return (this->current == other.current);
		}

		bool operator!=(const ConstIterator &other) const {
			return !(this->current == other.current);
		}
	};

	template<typename Type>
	class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
	public:
		using pointer = typename LinkedList::pointer;
		using reference = typename LinkedList::reference;

		explicit Iterator() {}

		Iterator(const ConstIterator &other)
				: ConstIterator(other) {}

		Iterator &operator++() {
			ConstIterator::operator++();
			return *this;
		}

		Iterator operator++(int) {
			auto result = *this;
			ConstIterator::operator++();
			return result;
		}

		Iterator &operator--() {
			ConstIterator::operator--();
			return *this;
		}

		Iterator operator--(int) {
			auto result = *this;
			ConstIterator::operator--();
			return result;
		}

		Iterator operator+(difference_type d) const {
			return ConstIterator::operator+(d);
		}

		Iterator operator-(difference_type d) const {
			return ConstIterator::operator-(d);
		}

		reference operator*() const {
			// ugly cast, yet reduces code duplication.
			return const_cast<reference>(ConstIterator::operator*());
		}
	};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
