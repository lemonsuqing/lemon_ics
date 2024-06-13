#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

#define KEY_NUM 256

static int key_state[KEY_NUM];

void update_key_state(int keycode, int down) {
  key_state[keycode] = down;
}

int is_key_down(int keycode) {
  return key_state[keycode];
}

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t k = inl(KBD_ADDR);
  int down = (k & KEYDOWN_MASK) != 0;
  int keycode = down ? (k & ~KEYDOWN_MASK) : AM_KEY_NONE;
  update_key_state(keycode, down);
  kbd->keydown = down;
  kbd->keycode = keycode;
}

