#pragma once
#include<iostream>
using namespace std;
template<typename T>
struct QNode {
	T data;
	QNode<T>* next;
};

template<typename T>
class Queue {
private:
	QNode<T>* front;
	QNode<T>* rear;
	int len = 0;
public:
	Queue() {
		front = nullptr;
		rear = nullptr;
	}
	void push(T item) {
		QNode<T>* newNode = new QNode<T>;
		newNode->data = item;
		newNode->next = nullptr;
		if (rear == nullptr) {
			front = newNode;
			rear = newNode;
			len++;
			return;
		}
		rear->next = newNode;
		rear = newNode;
		len++;
	}
	T pop() {
		if (front == nullptr) {
			return T(-1);
		}
		QNode<T>* temp = front;
		front = front->next;
		if (front == nullptr) {
			rear = nullptr;
		}
		T data = temp->data;
		//delete temp;
		len--;
		return data;
	}
	int size() {
		return len;
	}
	bool empty() {
		return len == 0;
	}
	T p() {
		if (front == nullptr) {
			return T(-1);
		}
		return front->data;
	}
	//void print() {
	//	QNode<T>* temp = front;
	//	while (temp != nullptr) {
	//		std::cout << temp->data << " ";
	//		temp = temp->next;
	//	}
	//	std::cout << std::endl;
	//}
	//bool is_empty() { return front == nullptr; }
	//bool isPalindrome(char* str) {
	//	Queue<char> q;
	//	/*int i = 0;
	//	while (str[i]) {
	//		q.enqueue(str[i]);
	//		i++;
	//	}*/
	//	return checkpal(q, str, 0);
	//}
	//bool checkpal(Queue<char>& q, char* str, int i) {
	//	if (str[i] == 0) return true;
	//	q.enqueue(str[i]);
	//	if (!checkpal(q, str, i + 1)) return false;
	//	if (q.dequeue() != str[i]) return false;
	//	return true;
	//}
};