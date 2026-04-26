#include "../include/ants.h"
#include "../include/environment.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *ant_movement(void *arg){
  ant_args *data = (ant_args *)arg;

  int pos[2];
  
  if (find_free_tile(data->env, pos)) {
    printf("Hormiga %d en (%d, %d)\n",
          data->ant_id, pos[0], pos[1]);
    if (!data->env || !data->env->terrain) return 0;
    }
  return NULL;
}

void ant_creation(environment_t *env){
  
  pthread_t t_ids[ant_num];
  ant_args args[ant_num];
  //pthread_mutex_init(&mutex, NULL); 

  // Crear cada hilo
  for (int t = 0; t < ant_num; ++t) {
    
    args[t].env = env;     
    args[t].ant_id = t;

    pthread_create(&t_ids[t], NULL, ant_movement, &args[t]);
  }

  for (int t = 0; t < ant_num; ++t) {
    pthread_join(t_ids[t], NULL);
  }
}