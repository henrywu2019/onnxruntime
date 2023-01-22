#include <bits/stdc++.h>
using namespace std;

vector<float> get_input(int mx=36){
  auto l= vector<float>(mx);
  std::iota(l.begin(), l.end(), 1);
  return l;
}

float O[16]={};

void extract(const vector<float>& v, vector<float>& D, int j,int H, int W, pair<int,int> hw){
  if(D.size()<hw.first*hw.second) D.resize(H*hw.second);
  int i=0;
  for(int x=0;x<H;x++){
    auto src = v.data() + j + x*W;
    ::memcpy(D.data()+i, src, hw.second* sizeof(float));
    i+=hw.second;
  }
}

/*
> g++ -O3 main.cpp -std=c++11 -mavx2
> ./a.out
48 | Compute Time: 1000 ns
474.000000, 519.000000, 564.000000, 609.000000,
744.000000, 789.000000, 834.000000, 879.000000,
1014.000000, 1059.000000, 1104.000000, 1149.000000,
1284.000000, 1329.000000, 1374.000000, 1419.000000,
 * */
int main(){
  auto I = get_input();
  auto F= get_input(9);

  vector<float> sliced_mat(6*4,0);
  auto start = std::chrono::high_resolution_clock::now();
  auto t1=start;
  for(int i=0;i<3;i++){
    //auto t0 = std::chrono::high_resolution_clock::now();
    extract(I, sliced_mat, i, 6, 6, {4,4}); //////
    //t1 = std::chrono::high_resolution_clock::now();
    //std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::nanoseconds >((t1 - t0)).count() << " ns" << std::endl; t0=t1;

    for(int k=0;k<3;k++){
      auto shifted_start = sliced_mat.data()+4*k;
      for(int m=0;m<1;m++){
        float scalar=F[k*3+i];
        // scalar * (shifted_start: shifted_start+16)
        for(int t=0;t<16;t++){
          auto z=*(shifted_start+t);
          O[t] += scalar * z;
        }
      }
    }
  }
  t1 = std::chrono::high_resolution_clock::now();
  std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::nanoseconds >((t1 - start)).count() << " ns" << std::endl;

  for(int t=0;t<16;t++){
    printf("%f, ",O[t]);
    if (t%4==3) printf("\n");
  }
  printf("\n");

  return 0;
}

