/*
 * algernon's ErgoDox EZ layout, please see the README.md file!
 */

#include "ergodox_ez.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "mousekey.h"
#include "timer.h"

/* Layers */

enum {
  BASE = 0,
  APPSEL,
  HUN,
  EMACS,
  OHLFT,
  OHRGT,
};

/* Macros */

enum {
  NONE = 0,
  // Buttons that do extra stuff
  A_GUI,
  A_ESC,

  // Function / number keys
  KF_1, // 1, F1
  KF_2, // 2, F2
  KF_3, // ...
  KF_4,
  KF_5,
  KF_6,
  KF_7,
  KF_8,
  KF_9,
  KF_10,
  KF_11, // =, F11

  // Application select keys
  APP_SLK, // Slack
  APP_EMCS, // Emacs
  APP_TERM, // Terminal
  APP_CHRM, // Chrome
  APP_MSIC, // Music

  // Diagonal mouse movement
  A_MUL,
  A_MUR,
  A_MDL,
  A_MDR,

  // Emacs layer keys
  AE_VIS, // Visual mode
  AE_PSTDEL, // Paste/Delete
  AE_CPYC, // Copy/Cut
  AE_EMACS, // Emacs copy & paste mode
  AE_TERM, // Terminal copy & paste mode
  AE_OTHER, // Other copy & paste mode
  AE_INS, // Insert mode
  AE_OVR, // Overwrite mode
  AE_APPND, // Append

  // Hungarian layer keys
  HU_AA, // Á
  HU_OO, // Ó
  HU_EE, // É
  HU_UU, // Ú
  HU_II, // Í
  HU_OE, // Ö
  HU_UE, // Ü
  HU_OEE, // Ő
  HU_UEE, // Ű

  // One-handed layout specials
  OH_BSSPC,
  OH_ENTSFT,
  OH_BASE,
  OH_LEFT,
  OH_RIGHT,
};

/* Fn keys */

enum {
  F_BSE = 0,
  F_HUN,
  F_GUI,
  F_SFT,
  F_ALT,
  F_CTRL
};

/* States & timers */

uint8_t m_visual_state = 0;
static uint16_t m_cutdel_timer;
static uint16_t m_copypaste_timer;

uint16_t gui_timer = 0;

uint16_t kf_timers[12];

uint16_t oh_base_timer = 0;
uint16_t oh_bsspc_timer = 0;
uint16_t oh_entsft_timer = 0;

enum {
  CP_EMACS = 0,
  CP_TERM  = 1,
  CP_OTHER = 2,
};

uint8_t cp_mode = CP_EMACS;

/* The Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |        `~ | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 | Apps |           | Apps | 6 F6 | 7 F7 | 8 F8 | 9 F9 | 0 F10| =     F11 |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |       Tab |   '  |   ,  |   .  |   P  |   Y  |   [  |           |  ]   |   F  |   G  |   C  |   R  |  L   | /         |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |         - |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | \         |
 * |-----------+------+------+------+------+------|   (  |           |  )   |------+------+------+------+------+-----------|
 * | Play/Pause|   ;  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |  Z   |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |  Home | End  | Down |  Up  |   :  |                                       |   -  | Left | Right| PgUp | PgDn  |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           |EMACS | 1HND |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           | LEAD |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      | ESC  |           | HUN  |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 KC_GRV             ,M(KF_1)     ,M(KF_2)     ,M(KF_3),M(KF_4),M(KF_5),KC_APP
,KC_TAB             ,KC_QUOT     ,KC_COMM     ,KC_DOT ,KC_P   ,KC_Y   ,KC_LBRC
,KC_MINS            ,KC_A        ,KC_O        ,KC_E   ,KC_U   ,KC_I
,KC_MPLY            ,KC_SCLN     ,KC_Q        ,KC_J   ,KC_K   ,KC_X   ,KC_LPRN
,KC_HOME            ,KC_END      ,KC_DOWN     ,KC_UP  ,KC_COLN

                                                            ,F(F_ALT),F(F_GUI)
                                                                     ,F(F_CTRL)
                                                    ,KC_BSPC,F(F_SFT),M(A_ESC)

                                                                // right hand
                                                               ,KC_APP  ,M(KF_6),M(KF_7),M(KF_8),M(KF_9)     ,M(KF_10)    ,M(KF_11)
                                                               ,KC_RBRC ,KC_F   ,KC_G   ,KC_C   ,KC_R        ,KC_L        ,KC_SLSH
                                                                        ,KC_D   ,KC_H   ,KC_T   ,KC_N        ,KC_S        ,KC_BSLS
                                                               ,KC_RPRN ,KC_B   ,KC_M   ,KC_W   ,KC_V        ,KC_Z        ,KC_MSTP
                                                                                ,KC_MINS,KC_LEFT,KC_RGHT     ,KC_PGUP     ,KC_PGDN

                                                               ,OSL(EMACS),M(OH_LEFT)
                                                               ,KC_LEAD
                                                               ,F(F_HUN),KC_ENT ,KC_SPC
    ),

/* Keymap 1: Application select layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |Music |Slack |Emacs |Term  |Chrome|      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[APPSEL] = KEYMAP(
// left hand
 KC_TRNS ,M(APP_MSIC),M(APP_SLK),M(APP_EMCS),M(APP_TERM),M(APP_CHRM),KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),


/* Keymap 2: Hungarian Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |  Ő   |      |  Ű   |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |   Á  |  Ó   |  É   |  Ú   |  Í   |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |  Ö   |      |  Ü   |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           | BASE |      |      |
 *                                  `--------------------'           `--------------------'
 */

[HUN] = KEYMAP(
// left hand
 KC_NO   ,KC_NO   ,KC_NO    ,KC_NO   ,KC_NO    ,KC_NO   ,KC_TRNS
,KC_NO   ,KC_NO   ,M(HU_OEE),KC_NO   ,M(HU_UEE),KC_NO   ,KC_NO
,KC_NO   ,M(HU_AA),M(HU_OO) ,M(HU_EE),M(HU_UU) ,M(HU_II)
,KC_TRNS ,KC_NO   ,M(HU_OE) ,KC_NO   ,M(HU_UE) ,KC_NO   ,KC_NO
,KC_TRNS ,KC_TRNS ,KC_TRNS  ,KC_TRNS ,KC_NO

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                        ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                                 ,KC_NO   ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_NO
                                                               ,F(F_BSE),KC_TRNS  ,KC_TRNS
    ),

/* Keymap 3: Spacemacs layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | MS Slow   | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 |ScrLCK|           |ScrLCK| 6 F6 | 7 F7 | 8 F8 | 9 F9 | 0 F10|    F11    |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | MS Normal |      | Home |  Up  | PgUp |      |Visual|           |Scroll|  $   |MsUpL | MsUp |MsUpR |  R   |PrintScreen|
 * |-----------+------+------+------+------+------| Mode |           |  Up  |------+------+------+------+------+-----------|
 * | MS Fast   |APPEND| Left | Down | Right| INS  |------|           |------|  D   |MsLeft| MsDn |MsRght|      |           |
 * |-----------+------+------+------+------+------| Cut  |           |Scroll|------+------+------+------+------+-----------|
 * | Play/Pause|      | End  | Down | PgDn |  X   | Copy |           | Down |      |MsDnL |  W   |MsDnR |      |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |EmacsM|TermM |OtherM|      |      |                                       |Vol Up|Vol Dn| Mute |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |  Alt | GUI  |           | BASE | MClk |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |Delete|      | Ctrl |           | Prev |Left  |Right |
 *                                  |      |LShift|------|           |------| Click| Click|
 *                                  |Paste |      | ESC  |           | Next |      |      |
 *                                  `--------------------'           `--------------------'
 */
[EMACS] = KEYMAP(
// left hand
 KC_ACL0    ,M(KF_1)     ,M(KF_2)    ,M(KF_3) ,M(KF_4) ,M(KF_5) ,LGUI(KC_L)
,KC_ACL1    ,KC_NO       ,KC_HOME    ,KC_UP   ,KC_PGUP ,KC_NO   ,M(AE_VIS)
,KC_ACL2    ,M(AE_APPND) ,KC_LEFT    ,KC_DOWN ,KC_RIGHT,M(AE_INS)
,KC_MPLY    ,KC_NO       ,KC_END     ,KC_DOWN ,KC_PGDN ,KC_X    ,M(AE_CPYC)
,M(AE_EMACS),M(AE_TERM)  ,M(AE_OTHER),KC_NO   ,KC_NO
                                                        ,KC_TRNS ,KC_TRNS
                                                                 ,KC_TRNS
                                           ,M(AE_PSTDEL),KC_TRNS ,KC_TRNS

                                                                     // right hand
                                                                     ,LGUI(KC_L),M(KF_6) ,M(KF_7) ,M(KF_8) ,M(KF_9) ,M(KF_10) ,M(KF_11)
                                                                     ,KC_WH_U   ,KC_DLR  ,M(A_MUL),KC_MS_U ,M(A_MUR),M(AE_OVR),KC_PSCR
                                                                                ,KC_D    ,KC_MS_L ,KC_MS_D ,KC_MS_R ,KC_NO    ,KC_NO
                                                                     ,KC_WH_D   ,KC_NO   ,M(A_MDL),KC_W    ,M(A_MDR),KC_NO    ,KC_MSTP
                                                                                         ,KC_VOLU ,KC_VOLD ,KC_MUTE ,KC_NO    ,KC_NO

                                                                     ,KC_TRNS   ,KC_MS_BTN3
                                                                     ,KC_MPRV
                                                                     ,KC_MNXT   ,KC_BTN1 ,KC_BTN2
    ),

/* Keymap 5: One-handed, left side
 *
 * ,-----------------------------------------------------.
 * |        `~ | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 |A  BSE|
 * |-----------+------+------+------+------+-------------|
 * |       Tab |   '  |   ,  |   .  |   P  |   Y  |   [  |
 * |-----------+------+------+------+------+------|      |
 * |         - |   A  |   O  |   E  |   U  |   I  |------|
 * |-----------+------+------+------+------+------|   (  |
 * | Play/Pause|   ;  |   Q  |   J  |   K  |   X  |      |
 * `-----------+------+------+------+------+-------------'
 *     |  Home | End  | Down |  Up  | ESC  |
 *     `-----------------------------------'
 *                                         ,-------------.
 *                                         | LAlt | GUI  |
 *                                  ,------|------|------|
 *                                  |BackSp|LShift| Ctrl |
 *                                  |      |      |------|
 *                                  |Space |Enter |OTHER |
 *                                  `--------------------'
 */
[OHLFT] = KEYMAP(
// left hand
 KC_GRV     ,M(KF_1)     ,M(KF_2)     ,M(KF_3)   ,M(KF_4)    ,M(KF_5) ,M(OH_BASE)
,KC_TAB     ,KC_QUOT     ,KC_COMM     ,KC_DOT    ,KC_P       ,KC_Y    ,KC_LBRC
,KC_MINS    ,KC_A        ,KC_O        ,KC_E      ,KC_U       ,KC_I
,KC_MPLY    ,KC_SCLN     ,KC_Q        ,KC_J      ,KC_K       ,KC_X    ,KC_LPRN
,KC_HOME    ,KC_END      ,KC_DOWN     ,KC_UP     ,M(A_ESC)

                                                                ,KC_TRNS     ,KC_TRNS
                                                                             ,KC_TRNS
                                                    ,M(OH_BSSPC),M(OH_ENTSFT),M(OH_RIGHT)

                                                                // right hand
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                        ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                                ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO
    ),

/* Keymap 6: One-handed, right side
 *
 * ,-----------------------------------------------------.
 * | =     F11 | 0 F10| 9 F9 | 8 F8 | 7 F7 | 6 F6 |A  BSE|
 * |-----------+------+------+------+------+-------------|
 * |         / |   L  |   R  |   C  |   G  |   F  |   ]  |
 * |-----------+------+------+------+------+------|      |
 * |         \ |   S  |   N  |   T  |   H  |   D  |------|
 * |-----------+------+------+------+------+------|   )  |
 * |      Stop |   Z  |   V  |   W  |   M  |   B  |      |
 * `-----------+------+------+------+------+-------------'
 *     |  PgDn | PgUp | Right| Left | ESC  |
 *     `-----------------------------------'
 *                                         ,-------------.
 *                                         | LAlt | GUI  |
 *                                  ,------|------|------|
 *                                  |BackSp|LShift| Ctrl |
 *                                  |      |      |------|
 *                                  |Space |Enter |OTHER |
 *                                  `--------------------'
 */
[OHRGT] = KEYMAP(
// left hand
 M(KF_11)   ,M(KF_10)    ,M(KF_9)     ,M(KF_8)   ,M(KF_7)    ,M(KF_6) ,M(OH_BASE)
,KC_SLSH    ,KC_L        ,KC_R        ,KC_C      ,KC_G       ,KC_F    ,KC_RBRC
,KC_BSLS    ,KC_S        ,KC_N        ,KC_T      ,KC_H       ,KC_D
,KC_MSTP    ,KC_Z        ,KC_V        ,KC_W      ,KC_M       ,KC_B    ,KC_RPRN
,KC_PGDN    ,KC_PGUP     ,KC_RGHT     ,KC_LEFT   ,M(A_ESC)

                                                                ,KC_TRNS     ,KC_TRNS
                                                                             ,KC_TRNS
                                                    ,M(OH_BSSPC),M(OH_ENTSFT),M(OH_LEFT)

                                                                // right hand
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                        ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO
                                                                                ,KC_NO  ,KC_NO  ,KC_NO       ,KC_NO       ,KC_NO

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_NO
                                                               ,KC_NO   ,KC_NO  ,KC_NO
    ),

};

const uint16_t PROGMEM fn_actions[] = {
   [F_BSE]  = ACTION_LAYER_CLEAR(ON_PRESS)
  ,[F_HUN]  = ACTION_LAYER_INVERT(HUN, ON_PRESS)
  ,[F_GUI]  = ACTION_MACRO_TAP(A_GUI)
  ,[F_SFT]  = ACTION_MODS_ONESHOT (MOD_LSFT)
  ,[F_ALT]  = ACTION_MODS_ONESHOT (MOD_LALT)
  ,[F_CTRL] = ACTION_MODS_ONESHOT (MOD_LCTL)
};

macro_t *ang_do_hun (keyrecord_t *record, uint16_t accent, uint16_t hun_char)
{
  uint8_t need_shift = 0;
  uint8_t hold_shift = 0;

  if (!record->event.pressed)
    return MACRO_NONE;

  layer_off (HUN);

  if (keyboard_report->mods & MOD_BIT (KC_LSFT)) {
    hold_shift = 1;
    need_shift = 1;
    unregister_code (KC_LSFT);
  }
  if ((get_oneshot_mods () & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out ()) {
    need_shift = 1;
    hold_shift = 0;
    unregister_code (KC_LSFT);
  }

  clear_oneshot_mods ();

  register_code (KC_RALT);
  unregister_code (KC_RALT);
  if (accent == (KC_DQT)) {
    register_code (KC_RSFT);
  }
  register_code (accent);
  unregister_code (accent);
  if (need_shift && accent != (KC_DQT)) {
    register_code (KC_RSFT);
  } else if (accent == (KC_DQT) && !need_shift) {
    unregister_code (KC_RSFT);
  }
  register_code (hun_char);
  unregister_code (hun_char);
  if (need_shift || accent == (KC_DQT))
    unregister_code (KC_RSFT);
  if (hold_shift)
    register_code (KC_LSFT);

  return MACRO_NONE;
}

void ang_handle_kf (keyrecord_t *record, uint8_t id)
{
  uint8_t code = id - KF_1;

  if (record->event.pressed) {
    kf_timers[code] = timer_read ();
  } else {
    uint8_t kc;

    if (timer_elapsed (kf_timers[code]) > TAPPING_TERM) {
      // Long press
      kc = KC_F1 + code;
    } else {
      if (id == KF_11)
        kc = KC_EQL;
      else
        kc = KC_1 + code;
    }

    register_code (kc);
    unregister_code (kc);
  }
}

/** LED ANIMATION LIBRARY */

typedef struct _led_state_t led_state_t;

struct _led_state_t
{
  uint8_t brightness;
  uint8_t target;
  int8_t step;
  void (*finish_cb) (led_state_t *state);
} _led_state_t;

static led_state_t leds[3];

void led_anim_init (void) {
  ergodox_led_all_set (0);
  ergodox_led_all_off ();
}

void led_anim_iterate (void) {
  for (int idx = 0; idx < 3; idx++) {
    if (leds[idx].step == 0)
      continue;
    if (leds[idx].target > leds[idx].brightness) {
      uint16_t n = (uint16_t)leds[idx].brightness + (uint16_t)leds[idx].step;
      if (n >= leds[idx].target)
        leds[idx].brightness = leds[idx].target;
      else
        leds[idx].brightness = (uint8_t) n;

      if (leds[idx].brightness >= leds[idx].target && leds[idx].finish_cb)
        leds[idx].finish_cb (&leds[idx]);
    } else {
      int16_t n = (int16_t)leds[idx].brightness + (int16_t)leds[idx].step;
      if (n <= leds[idx].target)
        leds[idx].brightness = leds[idx].target;
      else
        leds[idx].brightness = (uint8_t) n;

      if (leds[idx].brightness <= leds[idx].target && leds[idx].finish_cb)
        leds[idx].finish_cb (&leds[idx]);
    }

    if (leds[idx].brightness > 0)
      ergodox_right_led_on (idx + 1);
    else
      ergodox_right_led_off (idx + 1);
  }
}

void led_anim_fade_cb (led_state_t *state) {
  state->finish_cb = NULL;
}

void led_anim_breathe_cb (led_state_t *state) {
  if (state->target >= LED_BRIGHTNESS_HI)
    state->target = LED_BRIGHTNESS_LO;
  else
    state->target = LED_BRIGHTNESS_HI;
}

/** /LED ANIMATION LIBRARY */

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
      case A_ESC:
        if (record->event.pressed) {
          if ((get_oneshot_mods ()) && !has_oneshot_mods_timed_out ()) {
            clear_oneshot_mods ();
          } else {
            register_code (KC_ESC);
          }
        } else {
          unregister_code (KC_ESC);
        }
        break;

        /* Hungarian layer */
      case HU_AA:
        return ang_do_hun (record, KC_QUOT, KC_A);
      case HU_OO:
        return ang_do_hun (record, KC_QUOT, KC_O);
      case HU_EE:
        return ang_do_hun (record, KC_QUOT, KC_E);
      case HU_UU:
        return ang_do_hun (record, KC_QUOT, KC_U);
      case HU_II:
        return ang_do_hun (record, KC_QUOT, KC_I);
      case HU_OE:
        return ang_do_hun (record, KC_DQT, KC_O);
      case HU_UE:
        return ang_do_hun (record, KC_DQT, KC_U);
      case HU_OEE:
        return ang_do_hun (record, KC_EQL, KC_O);
      case HU_UEE:
        return ang_do_hun (record, KC_EQL, KC_U);

        /* Mouse movement */
      case A_MUL:
        if (record->event.pressed) {
          mousekey_on(KC_MS_UP);
          mousekey_on(KC_MS_LEFT);
        } else {
          mousekey_off(KC_MS_UP);
          mousekey_off(KC_MS_LEFT);
        }
        mousekey_send();
        break;

      case A_MUR:
        if (record->event.pressed) {
          mousekey_on(KC_MS_UP);
          mousekey_on(KC_MS_RIGHT);
        } else {
          mousekey_off(KC_MS_UP);
          mousekey_off(KC_MS_RIGHT);
        }
        mousekey_send();
        break;

      case A_MDL:
        if (record->event.pressed) {
          mousekey_on(KC_MS_DOWN);
          mousekey_on(KC_MS_LEFT);
        } else {
          mousekey_off(KC_MS_DOWN);
          mousekey_off(KC_MS_LEFT);
        }
        mousekey_send();
        break;

      case A_MDR:
        if (record->event.pressed) {
          mousekey_on(KC_MS_DOWN);
          mousekey_on(KC_MS_RIGHT);
        } else {
          mousekey_off(KC_MS_DOWN);
          mousekey_off(KC_MS_RIGHT);
        }
        mousekey_send();
        break;

        /* EMACS layer stuff */
      case AE_EMACS:
        if (record->event.pressed) {
          cp_mode = CP_EMACS;
        }
        break;

      case AE_TERM:
        if (record->event.pressed) {
          cp_mode = CP_TERM;
        }
        break;

      case AE_OTHER:
        if (record->event.pressed) {
          cp_mode = CP_OTHER;
        }
        break;

      case AE_VIS:
        if (cp_mode == CP_EMACS && record->event.pressed) {
          return MACRO(T(V), END);
        }
        break;

      case AE_CPYC:
        if (record->event.pressed) {
          m_copypaste_timer = timer_read ();
        } else {
          if (timer_elapsed (m_copypaste_timer) > TAPPING_TERM) {
            // Long press: Cut
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(X), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(X), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(X), U(RCTRL), END);
              break;
            }
          } else {
            // Short press: Copy
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(Y), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(C), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(C), U(RCTRL), END);
              break;
            }
          }
        }
        break;

      case AE_PSTDEL:
        if (record->event.pressed) {
          m_cutdel_timer = timer_read ();
        } else {
          if (timer_elapsed (m_cutdel_timer) > TAPPING_TERM) {
            // Long press: Delete
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(D), END);
              break;
            case CP_TERM:
            case CP_OTHER:
              return MACRO(T(DEL), END);
              break;
            }
          } else {
            // Short press: Paste
            switch (cp_mode) {
            case CP_EMACS:
              if (keyboard_report->mods & MOD_BIT(KC_RSFT)) {
                unregister_code (KC_RSFT);
                return MACRO(U(RSFT), T(RBRC), T(P), D(RSFT), END);
              }
              else
                return MACRO(T(P), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(V), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(V), U(RCTRL), END);
              break;
            }
          }
        }
        break;

      case AE_INS:
        if (record->event.pressed) {
          return MACRO(T(I), END);
        } else {
          layer_clear();
        }
        break;

      case AE_OVR:
        if (record->event.pressed) {
          return MACRO(T(R), END);
        } else {
          layer_clear();
        }
        break;

      case AE_APPND:
        if (record->event.pressed) {
          return MACRO(T(A), END);
        } else {
          layer_clear();
        }
        break;

        /* GUI & AppSel */
      case A_GUI:
        if (record->event.pressed) {
          register_code (KC_LGUI);
          if (record->tap.count && !record->tap.interrupted) {
            if (record->tap.count >= 2) {
              register_code (KC_W);
              layer_on (APPSEL);
              set_oneshot_layer (APPSEL, ONESHOT_START);
            }
          } else {
            record->tap.count = 0;
          }
          gui_timer = 0;
        } else {
          if (record->tap.count >= 2)
            {
              unregister_code (KC_W);
              clear_oneshot_layer_state (ONESHOT_PRESSED);
            }
          gui_timer = timer_read ();
        }
        break;

      case APP_SLK:
        return MACRODOWN(T(S), T(C), T(U), T(D), T(C), T(L), T(O), T(U), T(D), T(ENT), END);

      case APP_EMCS:
        return MACRODOWN(T(E), T(M), T(A), T(C), T(S), T(ENT), END);

      case APP_TERM:
        return MACRODOWN(T(T), T(E), T(R), T(M), T(ENT), END);

      case APP_CHRM:
        return MACRODOWN(T(C), T(H), T(R), T(O), T(M), T(ENT), END);

      case APP_MSIC:
        return MACRODOWN(T(R), T(H), T(Y), T(T), T(H), T(M), T(B), T(O), T(X), T(ENT), END);

        /* Function keys */
      case KF_1 ... KF_11:
        ang_handle_kf (record, id);
        break;

        /* 1HAND layout */
      case OH_BASE:
        if (record->event.pressed) {
          oh_base_timer = timer_read ();
        } else {
          if (timer_elapsed (oh_base_timer) > TAPPING_TERM) {
            layer_clear ();
          } else {
            return MACRO (T(APP), END);
          }
        }
        break;

      case OH_BSSPC:
        if (record->event.pressed) {
          oh_bsspc_timer = timer_read ();
        } else {
          if (timer_elapsed (oh_bsspc_timer) > TAPPING_TERM) {
            return MACRO (T(BSPC), END);
          } else {
            return MACRO (T(SPC), END);
          }
        }
        break;

      case OH_ENTSFT:
        if (record->event.pressed) {
          oh_entsft_timer = timer_read ();
        } else {
          if (timer_elapsed (oh_entsft_timer) > TAPPING_TERM) {
            if (keyboard_report->mods & MOD_BIT(KC_LSFT))
              unregister_code (KC_LSFT);
            else
              register_code (KC_LSFT);
          } else {
            return MACRO (T(ENT), END);
          }
        }
        break;

      case OH_LEFT:
        if (record->event.pressed) {
          leds[0].target = LED_BRIGHTNESS_LO;
          layer_move (OHLFT);
        }
        break;

      case OH_RIGHT:
        if (record->event.pressed) {
          leds[2].target = LED_BRIGHTNESS_LO;
          layer_move (OHRGT);
        }
        break;
      }
      return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  led_anim_init ();
};

LEADER_EXTERNS();

void ang_do_unicode (void) {
  register_code (KC_RCTL);
  register_code (KC_RSFT);
  register_code (KC_U);
  unregister_code (KC_U);
  unregister_code (KC_RSFT);
  unregister_code (KC_RCTL);
}

void ang_tap (uint16_t codes[]) {
  for (int i = 0; codes[i] != 0; i++) {
    register_code (codes[i]);
    unregister_code (codes[i]);
  }
}

#define TAP_ONCE(code) \
  register_code (code); \
  unregister_code (code)

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  uint8_t layer = biton32(layer_state);

  if (gui_timer && timer_elapsed (gui_timer) > TAPPING_TERM)
    unregister_code (KC_LGUI);

  switch (layer) {
  case OHLFT:
    leds[0].finish_cb = led_anim_breathe_cb;
    leds[0].target = LED_BRIGHTNESS_LO;
    leds[1].finish_cb = led_anim_fade_cb;
    leds[1].target = LED_BRIGHTNESS_HI;
    leds[2].finish_cb = led_anim_fade_cb;
    leds[2].target = 0;
    break;

  case OHRGT:
    leds[0].finish_cb = led_anim_fade_cb;
    leds[0].target = 0;
    leds[1].finish_cb = led_anim_fade_cb;
    leds[1].target = LED_BRIGHTNESS_HI;
    leds[2].finish_cb = led_anim_breathe_cb;
    leds[2].target = LED_BRIGHTNESS_LO;
    break;

  case HUN:
    leds[0].target = 0;
    leds[0].finish_cb = led_anim_fade_cb;
    leds[1].target = LED_BRIGHTNESS_LO;
    leds[1].finish_cb = led_anim_fade_cb;
    leds[2].target = LED_BRIGHTNESS_LO;
    leds[2].finish_cb = led_anim_fade_cb;
    break;

  case EMACS:
    leds[0].target = LED_BRIGHTNESS_LO;
    leds[0].finish_cb = led_anim_fade_cb;
    leds[1].target = LED_BRIGHTNESS_LO;
    leds[1].finish_cb = led_anim_fade_cb;
    leds[2].target = 0;
    leds[2].finish_cb = led_anim_fade_cb;
    break;
  }

  if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
    leds[0].target = LED_BRIGHTNESS_HI;
    if (keyboard_report->mods & MOD_BIT(KC_LSFT))
      leds[0].finish_cb = led_anim_fade_cb;
    else
      leds[0].finish_cb = led_anim_breathe_cb;
  }

  if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
    leds[1].target = LED_BRIGHTNESS_HI;
    if (keyboard_report->mods & MOD_BIT(KC_LALT))
      leds[1].finish_cb = led_anim_fade_cb;
    else
      leds[1].finish_cb = led_anim_breathe_cb;
  }

  if (keyboard_report->mods & MOD_BIT(KC_LCTRL) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
    leds[2].target = LED_BRIGHTNESS_HI;
    if (keyboard_report->mods & MOD_BIT(KC_LCTRL))
      leds[2].finish_cb = led_anim_fade_cb;
    else
      leds[2].finish_cb = led_anim_breathe_cb;
  }

  LEADER_DICTIONARY() {
    leading = false;
    leader_end ();

    SEQ_ONE_KEY (KC_U) {
      ang_do_unicode ();
    }

    SEQ_ONE_KEY (KC_L) {
      /* λ */
      ang_do_unicode ();

      uint16_t codes[] = {KC_0, KC_3, KC_B, KC_B, KC_ENT, 0};
      ang_tap (codes);
    }

    SEQ_ONE_KEY (KC_S) {
      ang_do_unicode (); TAP_ONCE (KC_A); TAP_ONCE (KC_F); TAP_ONCE (KC_SPC);
      TAP_ONCE (KC_BSLS);
      register_code (KC_RSFT); TAP_ONCE (KC_MINS); TAP_ONCE (KC_9); unregister_code (KC_RSFT);
      ang_do_unicode (); TAP_ONCE (KC_3); TAP_ONCE (KC_0); TAP_ONCE (KC_C); TAP_ONCE (KC_4); TAP_ONCE (KC_SPC);
      register_code (KC_RSFT); TAP_ONCE (KC_0); TAP_ONCE (KC_MINS); unregister_code (KC_RSFT);
      TAP_ONCE (KC_SLSH);
      ang_do_unicode (); TAP_ONCE (KC_A); TAP_ONCE (KC_F); TAP_ONCE (KC_SPC);
    }

    SEQ_TWO_KEYS (KC_W, KC_M) {
      register_code (KC_LALT);
      register_code (KC_F2);
      unregister_code (KC_F2);
      unregister_code (KC_LALT);

      _delay_ms (1000);

      uint16_t codes[] = {KC_M, KC_A, KC_X, KC_MINS, KC_F, KC_O, KC_C, KC_U, KC_S, KC_E, KC_D, KC_ENT, 0};
      ang_tap (codes);
      register_code (KC_LGUI);
      register_code (KC_UP);
      unregister_code (KC_UP);
      unregister_code (KC_LGUI);
    }
  }

  led_anim_iterate ();
}
