#pragma once
#include "latlon.h"
#include <iostream>

namespace LatLon {

inline void test(const double lat1, const double lon1, const double lat2,
                 const double lon2) {
  const double d{distance(lat1, lon1, lat2, lon2)};
  const double b{bearing(lat1, lon1, lat2, lon2)};
  double lat_m, lon_m;
  midpoint(lat_m, lon_m, lat1, lon1, lat2, lon2);
  double lat_d, lon_d;
  destination(lat_d, lon_d, lat1, lon1, d, b);
  double x1, y1;
  cartesian(x1, y1, lat1, lon1, lat_m, lon_m);
  double x2, y2;
  cartesian(x2, y2, lat2, lon2, lat_m, lon_m);

  std::cout << "Position_1: " << lat1 << " " << lon1 << std::endl
            << "Position_2: " << lat2 << " " << lon2 << std::endl
            << "Distance: " << d << std::endl
            << "Bearing: " << b << std::endl
            << "Midpoint: " << lat_m << " " << lon_m << std::endl
            << "Destination: " << lat_d << " " << lon_d << std::endl
            << "Cartesian_1: " << x1 << " " << y1 << std::endl
            << "Cartesian_2: " << x2 << " " << y2 << std::endl;
}

inline void test_1() {
  test(dmsToDeg(50, 03, 59, 'N'), dmsToDeg(5, 42, 53, 'W'),
       dmsToDeg(58, 38, 38, 'N'), dmsToDeg(3, 04, 12, 'W'));
}

inline void test_2() {
  const double lat1{43.58223};
  const double lon1{39.70199};
  double lat2, lon2;
  destination(lat2, lon2, lat1, lon1, 111.0, 222.0);
  test(lat1, lon1, lat2, lon2);
}

} // namespace LatLon
