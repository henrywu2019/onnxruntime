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

int main(){
  auto I = get_input();
  auto F= get_input(9);

  /*auto start = std::chrono::high_resolution_clock::now();
  auto t1=start;*/
  vector<float> sliced_mat(6*4,0);
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
  //t1 = std::chrono::high_resolution_clock::now();
  //std::cout << __LINE__ << " | Compute Time: " << std::chrono::duration_cast< std::chrono::nanoseconds >((t1 - start)).count() << " ns" << std::endl;

  /*for(int t=0;t<16;t++){
    printf("%f, ",O[t]);
    if (t%4==3) printf("\n");
  }
  printf("\n");*/

  return 0;
}

