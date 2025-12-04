//
// Created by claud on 03/12/2025.
//

#include "Vol.h"

bool Vol::operator<(const Vol &v) const {
    return _id < v._id;
}
