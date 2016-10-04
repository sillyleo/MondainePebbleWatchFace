#include "pebble.h"
  
static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -4, 16 },
    { 6, 16 },
    { 4, -66 },
    { -2, -66 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 16 },
    { 7, 16 },
    { 5, -45 },
    { -3, -45 }
  }
};
