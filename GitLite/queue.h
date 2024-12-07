#pragma once
#include<iostream>
using namespace std;
template<typename T>
struct Node {
	T data;
	Node<T>* next;
};

template<typename T>
class Queue {
private:
	Node<T>* front;
	Node<T>* rear;
public:
	Queue() {
		front = nullptr;
		rear = nullptr;
	}
	void enqueue(T item) {
		Node<T>* newNode = new Node<T>;
		newNode->data = item;
		newNode->next = nullptr;
		if (rear == nullptr) {
			front = newNode;
			rear = newNode;
			return;
		}
		rear->next = newNode;
		rear = newNode;
	}
	T dequeue() {
		if (front == nullptr) {
			return T(-1);
		}
		Node<T>* temp = front;
		front = front->next;
		if (front == nullptr) {
			rear = nullptr;
		}
		T data = temp->data;
		//delete temp;
		return data;
	}
	T Front() {
		if (front == nullptr) {
			return T(-1);
		}
		return front->data;
	}
	void print() {
		Node<T>* temp = front;
		while (temp != nullptr) {
			std::cout << temp->data << " ";
			temp = temp->next;
		}
		std::cout << std::endl;
	}
	bool is_empty() { return front == nullptr; }
	bool isPalindrome(char* str) {
		Queue<char> q;
		/*int i = 0;
		while (str[i]) {
			q.enqueue(str[i]);
			i++;
		}*/
		return checkpal(q, str, 0);
	}
	bool checkpal(Queue<char>& q, char* str, int i) {
		if (str[i] == 0) return true;
		q.enqueue(str[i]);
		if (!checkpal(q, str, i + 1)) return false;
		if (q.dequeue() != str[i]) return false;
		return true;
	}
};


void roundRobin(Queue<int>& q, int quantum) {
	while (!q.is_empty()) {
		int t = q.dequeue();
		cout << "Execution time: " << t<< endl;
		t-= quantum;
		cout << "Remaining time: " << t<< endl;
		if (t > 0) {
			cout << "Task is not completed, it is being re-scheduled." << endl<< endl;
			q.enqueue(t);
		}else {
			cout << "Task is completed, removing from queue." << endl << endl;
		}
	}
}