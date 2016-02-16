#include<iostream>
#include<string>
#include<vector>
using namespace std;

/*
created by xinyao huang
this assignment includes all the extra credit(detecting exception and manually call constructor&destructor)
*/
template < class T > 
class myvector
{
private:
	int Size;//current length of the vector
	int Capacity;//current capacity of the vector
	T* x;

public:
	//default constructor
	myvector(int s = 0)
	{
		if (s < 0)
			throw out_of_range("bad size");
		Size = 0;//length will be zero no matter what capacity
		Capacity = s;
		x = new T[Capacity]();
	}

	//copy constructor
	myvector(const myvector<T>& orig)
	{
		Size = orig.size();
		Capacity = orig.capacity();
		x = new T[Capacity]();
		for (int i = 0; i < Size; i++)
			x[i] = orig.x[i];//x[i]=orig[i]	
	}

	//move constructor
	myvector(myvector<T>&& orig)//所谓move，就是当函数返回类型是自定义类时，自动调用这个类的move constructor
	{
		Size = orig.size();
		Capacity = orig.capacity();
		x = orig.x;
		orig.x = nullptr;//in case of orig calls the destructor
	}

	// array-copy constructor
	myvector(T orig[], int s)
	{
		if (s < 0)
			throw out_of_range("bad size");
		Size = s;
		Capacity = s * 2 + 1;
		x = new T[Capacity]();
		for (int i = 0; i < s; i++)
			x[i] = orig[i];
	}

	//destructor
	~myvector()
	{
		delete[] x;
		x = nullptr;
	}

	//overload copy=
	myvector<T>& operator=(const myvector<T>& orig)//???????<T>  
	{
		if (this != &orig)
		{

			Capacity = orig.capacity();
			Size = orig.size();
			delete[] x;
			x = new T[Capacity]();
			for (int i = 0; i < Size; i++)
				x[i] = orig.x[i];
		}
		return *this;
	}
	
	//overload move=
	myvector<T>& operator=(myvector<T>&& orig)//???????<T>  
	{
		if (this != &orig)
		{
			delete[] x;
			x = orig.x;
			orig.x = nullptr;
			Capacity = orig.capacity();
			Size = orig.size();
		}
		return *this;
	}

	//overload []
	T& operator[](int pos)
	{
		if (pos >= Size || pos < 0)
			throw out_of_range("out of range");
		return x[pos];
	}
	T operator[](int pos) const
	{
		if (pos >= Size || pos < 0)
			throw out_of_range("out of range");
		return x[pos];
	}
	T& at(int pos)
	{
		if (pos >= Size || pos < 0)
			throw out_of_range("out of range");
		return x[pos];
	}
	T at(int pos) const
	{
		if (pos >= Size || pos < 0)
			throw out_of_range("out of range");
		return x[pos];
	}
	int size() const
	{
		return Size;
	}
	int capacity() const
	{
		return Capacity;
	}
	bool empty() const
	{
		return Size == 0;
	}
	void resize(int s)//change size
	{
		if (s<0)
			throw out_of_range("bad size");
		else if (s <= Size)//shrink
		{
			for (int i = s; i < Size; i++)//call the destructor manually
				x[i].~T();
			Size = s;
		}
		else if (s <= Capacity)//extend
		{
			for (int i = Size; i < s; i++)
				x[i] = T();
			Size = s;
		}
		else
		{
			T* temp = x;
			Capacity = s * 2 + 1;
			x = new T[Capacity]();
			for (int i = 0; i < Size; i++)
				x[i] = temp[i];
			Size = s;
			delete[] temp;
		}
	}
	void reserve(int s)//change Capacity
	{
		if (s < Size)
			throw out_of_range("bad size");
		else if (s != Capacity)
		{
			T* temp = x;
			Capacity = s;
			x = new T[Capacity]();
			for (int i = 0; i < Size; i++)
				x[i] = temp[i];
			delete[] temp;
		}
	}
	void assign(int pos, const T& val)
	{
		if (pos<0 || pos>=Size)
			throw out_of_range("out of range");
		else
			x[pos] = val;
	}
	void push_back(const T& val)
	{
		if (Size == Capacity)
			reserve(Capacity * 2 + 1);
		x[Size++] = val;
	}
	T pop_back()
	{
		if (Size == 0)
			throw out_of_range("empty");
		return x[--Size];
	}
	void insert(int pos, const T& val)
	{
		if (pos < 0 || pos > Size)
			throw out_of_range("out of range");
		if (Size == Capacity)
			reserve(Capacity * 2 + 1);
		for (int i = Size - 1; i > pos - 1; i--)
			x[i + 1] = x[i];
		x[pos] = val;
		Size++;
	}
	void erase(int pos)
	{
		if (pos < 0 || pos >= Size)
			throw out_of_range("out of range");
		for (int i = pos; i < Size - 1; i++)
			x[i] = x[i + 1];
		Size--;

	}
	void erase(int begin, int end)//includes begin and end
	{
		if (begin>end || begin < 0 || end >= Size)
			throw out_of_range("bad size");
		int length = end - begin + 1;
		for (int i = begin, j = end + 1; j < Size; j++, i++)
			x[i] = x[j];
		Size -= length;
	}
	void clear()
	{
		Size = 0;
	}
};

template<class T>
void display(const myvector<T>& vect)
{
	cout << "Size:" << vect.size() << endl << "Capacity:" << vect.capacity() << endl;
	cout << "display: ";
	for (int i = 0; i < vect.size(); i++)
		cout << vect[i] << " ";
}

