#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev)
{
  char buf[128];
  if (NDL_PollEvent(buf, 128) == 1)
  {
    //printf("SDL_PollEvent: %s", buf);
    if (strncmp(buf, "kd", 2) == 0)
    {
      ev->key.type = SDL_KEYDOWN;
    }
    else
    {
      ev->key.type = SDL_KEYUP;
    }

    for (unsigned i = 0; i < sizeof(keyname) / sizeof(keyname[0]); ++i)
    {
      if (strncmp(buf + 3, keyname[i], strlen(buf) - 4) == 0 && buf[3 + strlen(keyname[i])] == '\n')
      {
        //printf("keyname[%d]: %s\n", i, keyname[i]);
        ev->key.keysym.sym = i;
        break;
      }
    }
    return 1;
  }
  else
  {
    ev->key.type = SDL_USEREVENT; // avoid too many `Redirecting file open ...`
    ev->key.keysym.sym = 0;
  }
  return 0;
}

int SDL_WaitEvent(SDL_Event *event)
{
  while(SDL_PollEvent(event) == 0);
  return 1;
}


uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
