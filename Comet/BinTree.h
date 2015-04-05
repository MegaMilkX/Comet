#pragma once

#include <vector>

unsigned int count_bin_data_storage(unsigned int depth);

template<class T>
class BinNode
{
public:
	BinNode(){ dataid = 0; }
	~BinNode(){}

	void SetValue(T val){ value = val; }

	unsigned int dataid;
	T value;
};

template<class T>
class BinTree
{
public:
	BinTree(){}
	~BinTree(){}

	void Init(unsigned int depth)
	{
		if (!depth)
			return;
		unsigned int sz = count_bin_data_storage(depth);
		printf("Data size is %i\n", sz);
		data.resize(sz);
		for (int i = 0; i < data.size(); i++)
		{
			data.at(i).dataid = i;
		}
	}

	std::vector<BinNode<T>>* Data(){ return (&data); }
	BinNode<T> *Root(){ return &(data[0]); }
	BinNode<T> *Parent(unsigned int id)
	{ 
		id++;
		id = id >> 1;
		id--;
		if (id > 0)
			return &(data[id]);
		else
			return 0;
	}
	BinNode<T> *LeftChild(unsigned int id)
	{
		id++;
		id = id << 1;
		id--;
		if (id < data.size())
			return &(data[id]);
		else
			return 0;
	}
	BinNode<T> *RightChild(unsigned int id)
	{ 
		id++;
		id = (id << 1) + 1;
		id--;
		if (id < data.size())
			return &(data[id]);
		else
			return 0;
	}
private:
	std::vector<BinNode<T>> data;

	void nodeInit(BinNode<T>* node, unsigned int id)
	{/*???*/}
};

