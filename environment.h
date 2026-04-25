enum ENVIRONMENT_TYPES {
  ENVIRONMENT_FIELD,
  ENVIRONMENT_MAZE,
  ENVIRONMENT_COUNT
};

typedef struct {
  char **terrain;
  int size;
} environment_t;

void environment_init(environment_t *env, int size, int type);
void environment_show(environment_t *env);
void environment_dest(environment_t *env);
