// test reading speed from a InputSplit
#include <cstdlib>
#include <cstdio>
#include <dmlc/io.h>
#include <dmlc/timer.h>
#include "../src/data/libsvm_parser.h"
#include <iostream>
template<class T>
void print(T* data, size_t n) {
  for (size_t  i= 0; i < n; i++) {
    std::cout<<data[i]<<" ";
  }
  std::cout<<"\n";
}
int main(int argc, char *argv[]) {
  if (argc < 5) {
    printf("Usage: <libsvm> partid npart nthread\n");
    return 0;
  }
  using namespace dmlc;
  InputSplit *split = InputSplit::Create(argv[1],
                                         atoi(argv[2]),
                                         atoi(argv[3]),
                                         "text");
  int aux_size=0;
  printf("ggg");
  int nthread = atoi(argv[4]);
  data::LibSVMExtParser<unsigned> parser(split, nthread, aux_size);
  double tstart = GetTime();
  size_t bytes_read = 0;
  size_t bytes_expect = 10UL << 20UL;
  size_t num_ex = 0;
  while (parser.Next()) {
    printf("start parse\n");
    bytes_read  = parser.BytesRead();
    auto& batch = parser.Value();
    num_ex += batch.size;
    print(batch.value, batch.offset[batch.size]);
    print(batch.aux, batch.size * batch.aux_size);
    double tdiff = GetTime() - tstart;
    if (bytes_read >= bytes_expect) {
      printf("%lu examples, %lu MB read, %g MB/sec\n",
             num_ex, bytes_read >> 20UL,
             (bytes_read >> 20UL) / tdiff);
      bytes_expect += 10UL << 20UL;
    }
  }
  printf("finished\n");
  return 0;
}
