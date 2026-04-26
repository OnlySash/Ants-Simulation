#include "../include/environment.h"
#include "../include/ants.h"

int main() {
  environment_t env;
  ant_args args;
  args.env = &env;

  int size = 35;

  environment_init(&env, size, ENVIRONMENT_MAZE);
  environment_show(&env);

  ant_creation(&env);

  environment_dest(&env);
 

  return 0;
}
