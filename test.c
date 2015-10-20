#include "data.h"
#include <stdio.h>

int main(int argc, char **argv) {
  puts("Loading data from header.");
  Data *data = load_data();
  puts("data loaded.");
}

