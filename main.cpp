#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "matplotlibcpp.h"

using uint = unsigned int;
namespace plt = matplotlibcpp;

struct uint2 {
  uint x;
  uint y;
  uint2(uint x, uint y) : x(x), y(y) {};
  uint2() : x(0), y(0) {};
  std::string toString() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }
};

uint2 operator>>(const uint2 obj, int shift) {
  uint2 result = uint2(obj.x >> shift, obj.y >> shift);
  return result;
}

uint2 operator&(const uint2 op1, const uint2 op2) {
  uint2 result = uint2(op1.x & op2.x, op1.y & op2.y);
  return result;
}

bool operator==(const uint2 op1, const uint2 op2) {
  bool result = (op1.x == op2.x) && (op1.y == op2.y);
  return result;
}

uint getBaseIndex(uint2 coord) {
  if (coord == uint2(0, 0))
    return 0;
  else if (coord == uint2(0, 1))
    return 1;
  else if (coord == uint2(1, 1))
    return 2;
  else
    return 3;
}

uint2 flip02(uint2 coord) {
  uint temp = coord.x;
  coord.x = coord.y;
  coord.y = temp;
  return coord;
}

uint2 flip13(uint2 coord) {
  if (coord == uint2(0, 0))
    return uint2(1, 1);
  else if (coord == uint2(1, 1))
    return uint2(0, 0);
  else
    return coord;
}

int getIndex(uint2 id, uint _order, bool debug=false) {
  // Get value of pixel at this coordinate, ensure within size
  uint hilbertWidth = 1u << _order;

  if (id.x < hilbertWidth && id.y < hilbertWidth) {
    // Find the index that the pixel belongs in
    uint index = 0;
    uint2 prevQuad;
    uint2 currQuad = uint2(2, 2);

    uint nflip02 = 0;
    uint nflip13 = 0;

    if (debug)
      std::cout << "Current coordinate: " << id.toString() << std::endl;

    // index is the sum of the previous squares before it
    for (int i = 0; i < (int)_order; i++) {
      uint currentOrder = _order - 1 - (uint)i;
      if (debug)
        std::cout << "Current Order: " << currentOrder << std::endl;

      prevQuad = currQuad;
      currQuad = (id >> currentOrder) & uint2(1, 1);

      nflip02 <<= 1;
      nflip13 <<= 1;

      if (debug) 
        std::cout << "Current: " << currQuad.toString() << std::endl;
      if (prevQuad == uint2(0, 0)) { 
        nflip02 |= 1;
        if (debug)
          std::cout << "Flipped02: " << currQuad.toString() << std::endl;
      } else if (prevQuad == uint2(1, 0)) {
        nflip13 |= 1;
        if (debug)
          std::cout << "Flipped13: " << currQuad.toString() << std::endl;
      }

      for (int n = i; n >= 0; n--) {
        if (nflip02 & (1 << n))
          currQuad = flip02(currQuad);
        if (nflip13 & (1 << n))
          currQuad = flip13(currQuad);
      }

      uint base = getBaseIndex(currQuad);
      uint area = 1 << (2 * currentOrder);

      index += base * area;
      if (debug)
        std::cout << "Current Index: " << index << std::endl;
    }

    return index;
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    return -1;
  }
  
  int order = std::stoi(argv[1]);
  int hilbertWidth = 1 << order;

  std::vector<uint2> hilbert(hilbertWidth * hilbertWidth);

  for(int x = 0; x < hilbertWidth; x++) {
    for(int y = 0; y < hilbertWidth; y++) {
      uint2 coord = uint2(x, y);
      hilbert.at(getIndex(coord, order)) = coord;
    }
  }


  std::vector<double> x(hilbert.size());
  std::vector<double> y(hilbert.size());

  for (int i = 0; i < hilbert.size(); i++) {
    std::cout << i << ": " << hilbert.at(i).toString() << std::endl;
    x.push_back(hilbert.at(i).x);
    y.push_back(hilbert.at(i).y);
  }

  getIndex(uint2(0, 1), order, true);

  plt::plot(x, y);
  plt::show();
}







