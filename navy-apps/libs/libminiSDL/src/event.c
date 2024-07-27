#include <NDL.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static uint8_t key_state[83] = {0};
int parse_event(uint8_t *_key, uint8_t *_type){
  char buf[60], buf_key[30];
  int len = sizeof(buf);
  int _len = NDL_PollEvent(buf, len);
  int i;
  uint8_t key = SDLK_NONE, type = SDL_KEYUP;
  if(_len){
    char * toke0 = strtok(buf, " ");
    char * toke1 = strtok(NULL, " ");
    switch(toke1[0]){
      case 'k':
        switch(toke1[1]){
          case 'u':
            type = SDL_KEYUP;
            break;
          case 'd':
            type = SDL_KEYDOWN;
            break;
          default:
            printf("%s\n", toke1[1]);
            assert(0);
        }
        break;
      default:
        printf("%s\n", toke1[0]);
        assert(0);
    }
    sscanf(toke0, "%s", buf_key);
    for (i = 0; i < sizeof(keyname) / sizeof(char*); ++ i) {
      if (strcmp(buf_key, keyname[i]) == 0) {
        key = i;
        break;
      }
    }
    if(key){
      *_key = key;
      *_type = type;
      return 1;
    }else{
      printf("unknow key %s\n", toke0);
      assert(0);
    }
    return 0;
  }else{
      if (_key) *_key = SDLK_NONE;
      if (_type) *_type = SDL_KEYUP;
      return 0;
  }
}

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  SDL_Init(3);
  uint8_t key, type;
  int ret = parse_event(&key, &type);
  ev->type = type;
  ev->key.type = type;
  ev->key.keysym.sym = key;
  if (type == SDL_KEYDOWN)
    key_state[key] = 1;
  else if (ev->type == SDL_KEYUP)
    key_state[key] = 0;
  return ret;
}


int SDL_WaitEvent(SDL_Event *ev) {
    unsigned buf_size = 32;
    char *buf = (char *)malloc(buf_size * sizeof(char));

    while (NDL_PollEvent(buf, buf_size) == 0); // wait ...
    printf("SDL_WaitEvent: buf = %s\n",buf);
    if (strncmp(buf, "kd", 2) == 0)
        ev->key.type = SDL_KEYDOWN;
    else
        ev->key.type = SDL_KEYUP;


    int flag = 0;
    for (unsigned i = 0; i < sizeof(keyname) / sizeof(keyname[0]); ++i) {
        if (strncmp(buf + 3, keyname[i], strlen(buf) - 4) == 0
                && strlen(keyname[i]) == strlen(buf) - 4) {
            flag = 1;
            ev->key.keysym.sym = i;
            break;
        }
    }

   

    free(buf);
    return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  if (numkeys != NULL) {
    int _numkeys = 0;
    for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++)
    {
      if (key_state[i] == 1){
        _numkeys += 1;
      }
    }
    *numkeys = _numkeys;
  }
  return key_state;
}
