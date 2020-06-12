#include <iostream>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

int main(){
  std::random_device r;
  std::default_random_engine e{r()};
  std::uniform_real_distribution<float> u(-1, 1);

  int size = 1<<26;
  std::vector<double> vec(size);
  #pragma omp parallel for
  for (int i = 0; i < size; ++i)
      vec[i] = u(e);

  for (int i = 0; i < 1000; ++i){
    vec[rand()%size] = NAN;
    vec[rand()%size] = log(0);
  }

  struct timeval start, end;
  double diff;

  gettimeofday(&start, nullptr);
  std::vector<bool> res(size);
  // for (int j = 0; j < 10000; ++j){
 #pragma omp parallel for
    for (int i = 0; i < size; ++i)
    {
      res[i] = isfinite(vec[i]);
    }
  // }
  gettimeofday(&end, nullptr);
  diff = end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1e6;
  cout << endl << "comsume: " << diff << " s" << endl;

  cout << endl;
  return 0;
}