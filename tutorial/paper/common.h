#ifndef ORT_COMMON_HEADER
#define ORT_COMMON_HEADER
#include <bits/stdc++.h>
#include <sys/resource.h>
#include <unistd.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


void print_output(float* Output, int h, int w, int output_channel, bool all = false);

void get_input(float* l, int n, int c, int w, int h, float channel_delta = 0.1, float cell_delta = 1, float batch_delta=0.2);

int print_peak_mem();

void append_file(const string& s);

#define REP(i,begin,end) for(int i=begin;i<end;i++)
#define REP2(i,begin,end,step) for(int i=begin;i<end;i+=step)

#endif
