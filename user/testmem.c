#include "kernel/types.h"
#include "user/user.h"

int main() {
  printf("Virtual Pages: %d\n", countvp());
  printf("Physical Pages: %d\n", countpp());
  exit(0);
}
