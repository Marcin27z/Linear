#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <cstring>

namespace aisdi {

	template<typename Type>
	class Vector {

	public:
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		using value_type = Type;
		using pointer = Type *;
		using reference = Type &;
		using const_pointer = const Type *;
		using const_reference = const Type &;

	private:
		pointer head, tail;
		size_type capacity;
		size_type size;

		void reserve(size_type allocSize) {
			head = new Type[allocSize + 1];
			tail = head;
			capacity = allocSize;
			size = 0;
		}

		void reReserve() {
			if (capacity == 0) {
				reserve(2);
				return;
			}
			pointer tmp = head;
			capacity = ((capacity - 1) * 2) + 1;
			head = new Type[capacity];
			for (size_type i = 0; i < size; i++) {
				*(head + i) = *(tmp + i);
			}
			delete[](tmp);
			tail = head + size;
		}

	public:
		class ConstIterator;

		class Iterator;

		using iterator = Iterator;
		using const_iterator = ConstIterator;

		Vector() {
			reserve(2);
		}

		Vector(std::initializer_list<Type> l) {
			reserve(l.size());
			size_type i = 0;
			for (auto it = l.begin(); it != l.end(); ++it) {
				append(*it);
			}
			size = l.size();
			tail = head + size;
		}

		Vector(const Vector &other) {
			reserve(other.getSize());
			for (auto it = other.begin(); it != other.end(); ++it) {
				append(*it);
			}
			size = other.size;
			tail = head + size;
		}

		Vector(Vector &&other) : head(other.head), tail(other.tail), capacity(other.capacity), size(other.size) {
			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
			other.capacity = 0;
		}

		~Vector() {
			delete[](head);
		}

		Vector &operator=(const Vector &other) {
			if (this == &other) return *this;
			delete[](head);
			reserve(other.getSize());
			for (auto it = other.begin(); it != other.end(); ++it) {
				append(*it);
			}
			size = other.size;
			tail = head + size;
			return *this;
		}

		Vector &operator=(Vector &&other) {
			if (this == &other) return *this;
			delete[](head);
			head = other.head;
			tail = other.tail;
			capacity = other.capacity;
			size = other.size;
			other.head = nullptr;
			other.tail = nullptr;
			other.size = 0;
			other.capacity = 0;
			return *this;
		}

		bool isEmpty() const {
			return size == 0;
		}

		size_type getSize() const {
			return size;
		}

		void append(const Type &item) {
			if (size == capacity) reReserve();
			*(tail) = item;
			size++;
			tail++;
		}

		void prepend(const Type &item) {
			if (size == capacity) {
				pointer tmp = head;
				capacity = ((capacity - 1) * 2) + 1;
				head = new Type[capacity];
				*head = item;
				for (size_type i = 1; i <= size; i++) {
					*(head + i) = *(tmp + i - 1);
				}
				delete[](tmp);
				size++;
				tail = head + size;
			} else {
				for (size_type i = size; i > 0; i--) {
					*(head + i) = *(head + i - 1);
				}
				*head = item;
				size++;
				tail++;
			}
		}

		void insert(const const_iterator &insertPosition, const Type &item) {
			if (size == capacity) {
				pointer tmp = head;
				capacity = ((capacity - 1) * 2) + 1;
				head = new Type[capacity]; //extend vector
				int offset = 0;
				for (size_type i = 0; i <=
															size; ++i) //copy old vector into new, if position == insert position put item and adjust copying other elements
				{
					if (tmp + i == insertPosition.getPosition()) {
						*(head + i) = item;
						offset = 1;
					} else {
						*(head + i) = *(tmp + i - offset);
					}
				}
				size++;
				tail = head + size;
			} else //shift elements until item
			{
				for (size_type i = size;; --i) {
					if (head + i == insertPosition.getPosition()) {
						*(head + i) = item;
						break;
					} else {
						*(head + i) = *(head + i - 1);
					}
					if (i == 0) break;
				}
				size++;
				tail = head + size;
			}
		}

		Type popFirst() {
			if (size == 0) throw std::logic_error("popFirst");
			Type tmp = *head;
			for (size_type i = 0; i < size - 1; ++i)
				*(head + i) = *(head + i + 1);
			size--;
			tail--;
			return tmp;
		}

		Type popLast() {
			if (size == 0) throw std::logic_error("popLast");
			size--;
			Type tmp = *(--tail);
			return tmp;
		}

		void erase(const const_iterator &position) {
			if (isEmpty()) throw std::out_of_range("erase");
			if (position.getPosition() == tail) throw std::out_of_range("erase");
			pointer tmp = head + (position.getPosition() - head);///!!!!!!!!!!!!
			while (tmp != tail) {
				*tmp = *(tmp + 1);
				tmp++;
			}

			size--;
			tail--;
		}

		void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
			pointer tmp = head + (firstIncluded.getPosition() - head); ///!!!!!!!!!!!!!!!!
			auto tmpIt = lastExcluded;
			size = size - (lastExcluded.getPosition() - firstIncluded.getPosition());
			tail = tail - (lastExcluded.getPosition() - firstIncluded.getPosition());
			while (tmp != tail) {
				*tmp = *tmpIt;
				tmpIt++;
				tmp++;
			}

		}

		iterator begin() {
			return iterator(ConstIterator(head, tail, head));
		}

		iterator end() {
			return iterator(ConstIterator(head, tail, tail));
		}

		const_iterator cbegin() const {
			return const_iterator(head, tail, head);
		}

		const_iterator cend() const {
			return const_iterator(head, tail, tail);
		}

		const_iterator begin() const {
			return cbegin();
		}

		const_iterator end() const {
			return cend();
		}
	};

	template<typename Type>
	class Vector<Type>::ConstIterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename Vector::value_type;
		using difference_type = typename Vector::difference_type;
		using pointer = typename Vector::const_pointer;
		using reference = typename Vector::const_reference;
	private:
		pointer begin;
		pointer end;
		pointer position;
	public:
		explicit ConstIterator() {}

		ConstIterator(pointer begin, pointer end, pointer position) : begin(begin), end(end), position(position) {}

		pointer getPosition() const {
			return position;
		}

		reference operator*() const {
			if (position == end) throw std::out_of_range("op*");
			return *position;
		}

		ConstIterator &operator++() {
			if (position == end) throw std::out_of_range("++op");
			position++;
			return *this;
		}

		ConstIterator operator++(int) {
			ConstIterator tmp = *this;
			++(*this);
			return tmp;
		}

		ConstIterator &operator--() {
			if (position == begin) throw std::out_of_range("--op");
			position--;
			return *this;
		}

		ConstIterator operator--(int) {
			ConstIterator tmp = *this;
			--(*this);
			return tmp;
		}

		ConstIterator operator+(difference_type d) const {
			ConstIterator tmp = *this;
			if (position + d > end) throw std::out_of_range("op--");
			tmp.position += d;
			return tmp;
		}

		ConstIterator operator-(difference_type d) const {
			ConstIterator tmp = *this;
			if (position - d < begin) throw std::out_of_range("op--");
			tmp.position -= d;
			return tmp;
		}

		bool operator==(const ConstIterator &other) const {
			return (position == other.position);
		}

		bool operator!=(const ConstIterator &other) const {
			return (position != other.position);
		}
	};

	template<typename Type>
	class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
	public:
		using pointer = typename Vector::pointer;
		using reference = typename Vector::reference;

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

#endif // AISDI_LINEAR_VECTOR_H
