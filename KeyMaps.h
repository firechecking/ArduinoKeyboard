#ifndef __KEYMAPS__
#define __KEYMAPS__
#include <string.h>

#define default_layer_idx 0
#define NUM_LAYERS 2
#define CUSTOM_EMPTY 0x00
#define MO 0xE0
#define TG 0xE1
#define TO 0xE2
#define LOWER 0xE3

uint8_t keymaps[NUM_LAYERS * 44] = {'\n','/',';','p','=','.','l','o','-',',','k','i',MO,'m','j','u',KEY_RIGHT_SHIFT,'n','h','y',KEY_TAB,0x27,KEY_BACKSPACE,0x2F,' ','b','g','t',KEY_LEFT_GUI,'v','f','r',KEY_LEFT_ALT,'c','d','e',KEY_LEFT_CTRL,'x','s','w',KEY_ESC,'z','a','q','%','}',')',KEY_BACKSPACE,'^','{',KEY_RIGHT_ARROW,'&','*','#',KEY_DOWN_ARROW,KEY_UP_ARROW,LOWER,']',KEY_LEFT_ARROW,'@',KEY_RIGHT_SHIFT,'[','(','!',KEY_INSERT,'=',KEY_BACKSPACE,'$',' ','.','-','+',KEY_LEFT_GUI,'3','6','9','0','2','5','8',KEY_LEFT_CTRL,'1','4','7',TG,KEY_LEFT_ALT,KEY_PAGE_DOWN,KEY_PAGE_UP,};
uint8_t keymapsValue[NUM_LAYERS * 44] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0};
int layer_state[NUM_LAYERS] = {1, 0};
#endif