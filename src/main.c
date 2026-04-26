#include "environment.h"

int main() {
  environment_t env;
  int size = 35;

  environment_init(&env, size, ENVIRONMENT_MAZE);
  environment_show(&env);
  environment_dest(&env);

  return 0;
}
