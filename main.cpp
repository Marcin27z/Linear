#include <cstddef>
#include <cstdlib>
#include <string>

#include "Vector.h"
#include<chrono>
#include<iostream>
#include "LinkedList.h"

/*namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

void perfomTest()
{
  LinearCollection<std::string> collection;
  collection.append("TODO");
}

} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}*/
using namespace aisdi;


int main()
{
	static  double testNumber = 2000;
	int append = 1000;
	int prepend =5000;
	int popLast = 50000;
	int popFirst = 5000;
	std::chrono::duration<double> vectorTime;
	std::chrono::duration<double> linkedListTime;
	/***************************************
	 * append comparision test
	****************************************/
	for(int i = 0;i < testNumber; i++)
	{
		Vector<int> vector;
		auto startVector = std::chrono::steady_clock::now();
		for(int j = 0; j < append; j++)
		{
			vector.append(j);
		}
		auto endVector = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_vector = endVector-startVector;
		vectorTime += elapsed_seconds_vector;
		LinkedList<int> list;
		auto startList = std::chrono::steady_clock::now();
		for(int j = 0; j < append; j++)
		{
			list.append(j);
		}
		auto endList = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_list = endList-startList;
		linkedListTime += elapsed_seconds_list;
	}
	std::cout<<"Vector append "<<append<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(vectorTime).count()/testNumber<<"\n";
	std::cout<<"LinkedList append "<<append<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(linkedListTime).count()/testNumber<<"\n";
	vectorTime = std::chrono::duration<double>::zero();
	linkedListTime = std::chrono::duration<double>::zero();
	/***************************************
  * prepend comparision test
  ****************************************/
	for(int i = 0;i < testNumber; i++)
	{
		Vector<int> vector;
		auto startVector = std::chrono::steady_clock::now();
		for(int j = 0; j < prepend; j++)
		{
			vector.prepend(j);
		}
		auto endVector = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_vector = endVector-startVector;
		vectorTime +=  elapsed_seconds_vector;
		LinkedList<int> list;
		auto startList = std::chrono::steady_clock::now();
		for(int j = 0; j < prepend; j++)
		{
			list.prepend(j);
		}
		auto endList = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_list = endList-startList;
		linkedListTime += elapsed_seconds_list;
	}
	std::cout<<"Vector prepend "<<prepend<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(vectorTime).count()/testNumber<<"\n";
	std::cout<<"LinkedList prepend "<<prepend<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(linkedListTime).count()/testNumber<<"\n";
	vectorTime = std::chrono::duration<double>::zero();
	linkedListTime = std::chrono::duration<double>::zero();
	 /***************************************
  * popBack comparision test
  ****************************************/
	for(int i = 0;i < testNumber; i++)
	{
		Vector<int> vector;
		for(int j = 0; j < popLast; j++)
		{
			vector.append(j);
		}
		auto startVector = std::chrono::steady_clock::now();
		for(int j = 0; j < popLast; j++)
		{
			vector.popLast();
		}
		auto endVector = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_vector = endVector-startVector;
		vectorTime += elapsed_seconds_vector;
		LinkedList<int> list;
		for(int j = 0; j < popLast; j++)
		{
			list.append(j);
		}
		auto startList = std::chrono::steady_clock::now();
		for(int j = 0; j < popLast; j++)
		{
			list.popLast();
		}
		auto endList = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_list = endList-startList;
		linkedListTime += elapsed_seconds_list;
	}
	std::cout<<"Vector popLast "<<popLast<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(vectorTime).count()/testNumber<<"\n";
	std::cout<<"LinkedList popLast "<<popLast<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(linkedListTime).count()/testNumber<<"\n";
	vectorTime = std::chrono::duration<double>::zero();
	linkedListTime = std::chrono::duration<double>::zero();
	/***************************************
	 * popFirst comparision test
	****************************************/
	for(int i = 0;i < testNumber; i++)
	{
		Vector<int> vector;
		for(int j = 0; j < popFirst; j++)
		{
			vector.append(j);
		}
		auto startVector = std::chrono::steady_clock::now();
		for(int j = 0; j < popFirst; j++)
		{
			vector.popFirst();
		}
		auto endVector = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_vector = endVector-startVector;
		vectorTime += elapsed_seconds_vector;
		LinkedList<int> list;
		for(int j = 0; j < popFirst; j++)
		{
			list.append(j);
		}
		auto startList = std::chrono::steady_clock::now();
		for(int j = 0; j < popFirst; j++)
		{
			list.popFirst();
		}
		auto endList = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_list = endList-startList;
		linkedListTime += elapsed_seconds_list;
	}
	std::cout<<"Vector popFirst "<<popFirst<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(vectorTime).count()/testNumber<<"\n";
	std::cout<<"LinkedList popFirst "<<popFirst<<" elements time: "<< std::chrono::duration_cast<std::chrono::microseconds>(linkedListTime).count()/testNumber<<"\n";
	vectorTime = std::chrono::duration<double>::zero();
	linkedListTime = std::chrono::duration<double>::zero();
	return 0;
}
