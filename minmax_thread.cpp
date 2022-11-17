#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>

using namespace std;
constexpr long long value= 1000000;   
mutex myMutex;

void minMax(long long& min, long long& max, const vector<int>& val, 
   unsigned long long beg, unsigned long long end)
{
    long long local_minimum = val[1];
    long long local_maximum = val[1];
    for (auto it= beg; it < end; ++it)
    {         
         if (val[it] < local_minimum)
         {
            local_minimum = val[it];
         }
         if (val[it] > local_minimum)
         {
            local_maximum = val[it];
         }
    }
    lock_guard<mutex> myLock(myMutex);
    //cout << "Local Max: " << local_maximum << endl;
    //cout << "Local Min: " << local_minimum << endl;
    if(min > local_minimum)
    {
      min = local_minimum;
    }
    
    if(max < local_maximum)
    {
      max = local_maximum;
    }
}

int main(){

  cout << endl;

  vector<int> randValues;
  randValues.reserve(value);

  mt19937 engine (0);
  uniform_int_distribution<> uniformDist(10,100);
  for ( long long i=0 ; i< value ; ++i)
     randValues.push_back(uniformDist(engine));
  long long min = randValues[0];
  long long max = randValues[0];
  auto start = chrono::system_clock::now();

  int threads = 4;
  thread t[threads];
  long long slice = value / threads;
  int startIdx=0;
  for (int i = 0; i < threads; ++i) 
  {
    cout << "Thread[" << i << "] - slice ["
         << startIdx << ":" << startIdx+slice-1 << "]" << endl;
    t[i] = thread(minMax, ref(min), ref(max), ref(randValues), startIdx, startIdx+slice-1);
    startIdx += slice;
  }

  for (int i = 0; i < threads; ++i)
     t[i].join();

  chrono::duration<double> dur= chrono::system_clock::now() - start;
  cout << "Time for addition " << dur.count() << " seconds" << endl;
  cout << "Result Max: " << max << endl;
  cout << "Result Min: " << min << endl;

  cout << endl;

}