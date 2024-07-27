#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

#define fname_size 128
static char fname[128];

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal) - lemonsuqing\n\n");

sh_printf("   _                                            _             \n");
sh_printf("  | | ___ _ __ ___   ___  _ __  ___ _   _  __ _(_)_ __   __ _ \n");
sh_printf("  | |/ _ \ '_ ` _ \ / _ \| '_ \/ __| | | |/ _` | | '_ \ / _` |\n");
sh_printf("  | |  __/ | | | | | (_) | | | \__ \ |_| | (_| | | | | | (_| |\n");
sh_printf("  |_|\___|_| |_| |_|\___/|_| |_|___/\__,_|\__, |_|_| |_|\__, |\n");
sh_printf("                                             |_|        |___/ \n");

}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  if(cmd == NULL){
    return;
  }
  else if(strncmp(cmd, "echo", 4) == 0){
    if(strlen(cmd) == 5) sh_printf("\n");
    else sh_printf("%s", cmd + 5);
  }
  else{
    if(strlen(cmd) > fname_size){
      sh_printf("the commad is to long\n");
      return;
    }
    memset(fname, 0, fname_size);
    strncpy(fname, cmd, strlen(cmd) - 1);
    printf("%s\n", fname);
    execvp(fname, NULL);
  }
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    // printf("132\n");
    if (SDL_PollEvent(&ev)) {
      // printf("ev.type=%d", ev.type);
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        // printf("ev.type=%d", ev.type);
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
