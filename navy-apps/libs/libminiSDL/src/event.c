#include <NDL.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  unsigned buf_size = 32;
  char *buf = (char *)malloc(buf_size * sizeof(char));
  int ndl_flag = NDL_PollEvent(buf, buf_size);
  // printf("NDL_PollEvent(buf, buf_size) = %d\n", ndl_flag);
  if (ndl_flag == 5) {
    if (strncmp(buf, "kd", 2) == 0) {
        ev->key.type = SDL_KEYDOWN;
    } else {
        ev->key.type = SDL_KEYUP;
    }
    printf("buf = %s\n",buf);
    // printf("ev->key.type: %d\n", ev->key.type);
    int flag = 0;
    for (unsigned i = 0; i < sizeof(keyname) / sizeof(keyname[0]); ++i) {
      // printf("i = %d\n",i);
      if (strncmp(buf + 3, keyname[i], strlen(buf) - 4) == 0
            && strlen(keyname[i]) == strlen(buf) - 4) {
        flag = 1;
        printf("ev->key.keysym.sym: %d\n", i);
        ev->key.keysym.sym = i;
        break;
      }
    }

    // Handle special cases for keys like space
    // if (!flag) {
    //   printf("buf = %s\n",buf);
    //   if (strncmp(buf + 3, "SPACE", strlen(buf) - 4) == 0) {
    //     ev->key.keysym.sym = SDLK_SPACE;  // Use SDLK_SPACE for SDL space key
    //     flag = 1;
    //   } else if (strncmp(buf + 3, "ENTER", strlen(buf) - 4) == 0) {
    //     ev->key.keysym.sym = SDLK_RETURN;  // Use SDLK_RETURN for SDL enter key
    //     flag = 1;
    //   }
    // }
    
    free(buf);
    return 1;
  } else {
      return 0;
  }
}

int SDL_WaitEvent(SDL_Event *ev) {
    unsigned buf_size = 32;
    char *buf = (char *)malloc(buf_size * sizeof(char));

    while (NDL_PollEvent(buf, buf_size) == 0); // wait ...
    // printf("SDL_WaitEvent: buf = %s\n",buf);
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
  return NULL;
}
