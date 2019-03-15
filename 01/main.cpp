#include "numbers.dat"
#include <iostream>
#include <cmath>

bool isPrime(int x)
{
    if(x == 2) return true;
    for (int i = 2; i <= std::sqrt(x); i++) {
        if (x % i == 0) return false;
    }
    if(x == 1) return false;
    return true;
}

int num(int left_ind, int right_ind)
{
    int sum = 0, pred = 0;
    for(int i = left_ind; i <= right_ind ; i++)
    {
        //jump over if equal
        if(i > 0 && Data[i] == Data[i - 1])
        {
            if(Data[i - 1] == pred) sum++;
            continue;
        }
        if(isPrime(Data[i]))
        {
            //save the previous prime number
            pred = Data[i];
            sum++;
        }
    }
    return sum;
}

int BinSearch(int key) 
{
  int left = 0;
  int right = Size - 1;
 
  while (left <= right) {
    int med = (left + right) / 2;
    if (Data[med] == key) return med;
    if (Data[med] < key) left = med + 1;
    if (Data[med] > key) right = med - 1;
  }
  return -1;
}

int main(int argc, char* argv[])
{
    //input check
    if((argc % 2 == 0) || (argc == 1))return -1;
    for (int i = 1; i < argc; i += 2)
    {
        if(argv[i] > argv[i + 1]) return -1;
        if(isdigit(std::atoi(argv[i])) || isdigit(std::atoi(argv[i + 1]))) return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        //search for left and right indexes
        int left_ind = BinSearch(std::atoi(argv[i]));
        i++;
        int right_ind = BinSearch(std::atoi(argv[i]));

        std::cout << num(left_ind, right_ind) << std::endl;
    }
    return 0;
}