#include "math_sup.h"

double abs_double(double value) {
    if(value < 0) {
        return -1.0 * value;
    } else {
        return value;
    }
}