#ifndef _HCoreINPUT_
#define _HCoreINPUT_

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct HCoreMouseInfo
{
	// 鼠标相对移动的位置
	int x,y;
	// 鼠标滚轮的滚动状态
	int z;
	// 鼠标按钮的状态
	unsigned char button[3];
};

struct HCoreJoystickInfo
{
	int x, y, z;
	int rx, ry, rz;
	int pov[4];
	unsigned char button[32];
};

enum HCoreInputDevice
{
	GINPUT_KEYBOARD,
	GINPUT_MOUSE,
	GINPUT_JOYSTICK
};

int HCoreCheckInputDevice(HCoreInputDevice input);
int HCoreInputInit(void);
int HCoreInputClose(void);
int HCoreInputRestore(void);

int HCoreReadKeyboard(char buffer[256]);
int HCoreReadMouse(HCoreMouseInfo *mouse);
int HCoreReadJoystick(HCoreJoystickInfo *jostick);
int HCoreReadJoystick(HCoreJoystickInfo *jostick, int whichone);

void HCoreReadKeyboard(void);
void HCoreRegisterKeyDown(int key, void (*pKeyDownFunc)(void));
void HCoreRegisterKeyUp  (int key, void (*pKeyDownUp)(void));
void HCoreRegisterKeyPressed(int key, void (*pKeyDownPressed)(void));

#define HCoreKEY_ESCAPE          0x01
#define HCoreKEY_1               0x02
#define HCoreKEY_2               0x03
#define HCoreKEY_3               0x04
#define HCoreKEY_4               0x05
#define HCoreKEY_5               0x06
#define HCoreKEY_6               0x07
#define HCoreKEY_7               0x08
#define HCoreKEY_8               0x09
#define HCoreKEY_9               0x0A
#define HCoreKEY_0               0x0B
#define HCoreKEY_MINUS           0x0C    /* - on main keyboard */
#define HCoreKEY_EQUALS          0x0D
#define HCoreKEY_BACK            0x0E    /* backspace */
#define HCoreKEY_TAB             0x0F
#define HCoreKEY_Q               0x10
#define HCoreKEY_W               0x11
#define HCoreKEY_E               0x12
#define HCoreKEY_R               0x13
#define HCoreKEY_T               0x14
#define HCoreKEY_Y               0x15
#define HCoreKEY_U               0x16
#define HCoreKEY_I               0x17
#define HCoreKEY_O               0x18
#define HCoreKEY_P               0x19
#define HCoreKEY_LBRACKET        0x1A
#define HCoreKEY_RBRACKET        0x1B
#define HCoreKEY_RETURN          0x1C    /* Enter on main keyboard */
#define HCoreKEY_LCONTROL        0x1D
#define HCoreKEY_A               0x1E
#define HCoreKEY_S               0x1F
#define HCoreKEY_D               0x20
#define HCoreKEY_F               0x21
#define HCoreKEY_G               0x22
#define HCoreKEY_H               0x23
#define HCoreKEY_J               0x24
#define HCoreKEY_K               0x25
#define HCoreKEY_L               0x26
#define HCoreKEY_SEMICOLON       0x27
#define HCoreKEY_APOSTROPHE      0x28
#define HCoreKEY_GRAVE           0x29    /* accent grave */
#define HCoreKEY_LSHIFT          0x2A
#define HCoreKEY_BACKSLASH       0x2B
#define HCoreKEY_Z               0x2C
#define HCoreKEY_X               0x2D
#define HCoreKEY_C               0x2E
#define HCoreKEY_V               0x2F
#define HCoreKEY_B               0x30
#define HCoreKEY_N               0x31
#define HCoreKEY_M               0x32
#define HCoreKEY_COMMA           0x33
#define HCoreKEY_PERIOD          0x34    /* . on main keyboard */
#define HCoreKEY_SLASH           0x35    /* / on main keyboard */
#define HCoreKEY_RSHIFT          0x36
#define HCoreKEY_MULTIPLY        0x37    /* * on numeric keypad */
#define HCoreKEY_LMENU           0x38    /* left Alt */
#define HCoreKEY_SPACE           0x39
#define HCoreKEY_CAPITAL         0x3A
#define HCoreKEY_F1              0x3B
#define HCoreKEY_F2              0x3C
#define HCoreKEY_F3              0x3D
#define HCoreKEY_F4              0x3E
#define HCoreKEY_F5              0x3F
#define HCoreKEY_F6              0x40
#define HCoreKEY_F7              0x41
#define HCoreKEY_F8              0x42
#define HCoreKEY_F9              0x43
#define HCoreKEY_F10             0x44
#define HCoreKEY_NUMLOCK         0x45
#define HCoreKEY_SCROLL          0x46    /* Scroll Lock */
#define HCoreKEY_NUMPAD7         0x47
#define HCoreKEY_NUMPAD8         0x48
#define HCoreKEY_NUMPAD9         0x49
#define HCoreKEY_SUBTRACT        0x4A    /* - on numeric keypad */
#define HCoreKEY_NUMPAD4         0x4B
#define HCoreKEY_NUMPAD5         0x4C
#define HCoreKEY_NUMPAD6         0x4D
#define HCoreKEY_ADD             0x4E    /* + on numeric keypad */
#define HCoreKEY_NUMPAD1         0x4F
#define HCoreKEY_NUMPAD2         0x50
#define HCoreKEY_NUMPAD3         0x51
#define HCoreKEY_NUMPAD0         0x52
#define HCoreKEY_DECIMAL         0x53    /* . on numeric keypad */
#define HCoreKEY_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define HCoreKEY_F11             0x57
#define HCoreKEY_F12             0x58
#define HCoreKEY_F13             0x64    /*                     (NEC PC98) */
#define HCoreKEY_F14             0x65    /*                     (NEC PC98) */
#define HCoreKEY_F15             0x66    /*                     (NEC PC98) */
#define HCoreKEY_KANA            0x70    /* (Japanese keyboard)            */
#define HCoreKEY_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define HCoreKEY_CONVERT         0x79    /* (Japanese keyboard)            */
#define HCoreKEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define HCoreKEY_YEN             0x7D    /* (Japanese keyboard)            */
#define HCoreKEY_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define HCoreKEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define HCoreKEY_PREVTRACK       0x90    /* Previous Track (HCoreKEY_CIRCUMFLEX on Japanese keyboard) */
#define HCoreKEY_AT              0x91    /*                     (NEC PC98) */
#define HCoreKEY_COLON           0x92    /*                     (NEC PC98) */
#define HCoreKEY_UNDERLINE       0x93    /*                     (NEC PC98) */
#define HCoreKEY_KANJI           0x94    /* (Japanese keyboard)            */
#define HCoreKEY_STOP            0x95    /*                     (NEC PC98) */
#define HCoreKEY_AX              0x96    /*                     (Japan AX) */
#define HCoreKEY_UNLABELED       0x97    /*                        (J3100) */
#define HCoreKEY_NEXTTRACK       0x99    /* Next Track */
#define HCoreKEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define HCoreKEY_RCONTROL        0x9D
#define HCoreKEY_MUTE            0xA0    /* Mute */
#define HCoreKEY_CALCULATOR      0xA1    /* Calculator */
#define HCoreKEY_PLAYPAUSE       0xA2    /* Play / Pause */
#define HCoreKEY_MEDIASTOP       0xA4    /* Media Stop */
#define HCoreKEY_VOLUMEDOWN      0xAE    /* Volume - */
#define HCoreKEY_VOLUMEUP        0xB0    /* Volume + */
#define HCoreKEY_WEBHOME         0xB2    /* Web home */
#define HCoreKEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define HCoreKEY_DIVIDE          0xB5    /* / on numeric keypad */
#define HCoreKEY_SYSRQ           0xB7
#define HCoreKEY_RMENU           0xB8    /* right Alt */
#define HCoreKEY_PAUSE           0xC5    /* Pause */
#define HCoreKEY_HOME            0xC7    /* Home on arrow keypad */
#define HCoreKEY_UP              0xC8    /* UpArrow on arrow keypad */
#define HCoreKEY_PRIOR           0xC9    /* PgUp on arrow keypad */
#define HCoreKEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define HCoreKEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define HCoreKEY_END             0xCF    /* End on arrow keypad */
#define HCoreKEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define HCoreKEY_NEXT            0xD1    /* PgDn on arrow keypad */
#define HCoreKEY_INSERT          0xD2    /* Insert on arrow keypad */
#define HCoreKEY_DELETE          0xD3    /* Delete on arrow keypad */
#define HCoreKEY_LWIN            0xDB    /* Left Windows key */
#define HCoreKEY_RWIN            0xDC    /* Right Windows key */
#define HCoreKEY_APPS            0xDD    /* AppMenu key */
#define HCoreKEY_POWER           0xDE    /* System Power */
#define HCoreKEY_SLEEP           0xDF    /* System Sleep */
#define HCoreKEY_WAKE            0xE3    /* System Wake */
#define HCoreKEY_WEBSEARCH       0xE5    /* Web Search */
#define HCoreKEY_WEBFAVORITES    0xE6    /* Web Favorites */
#define HCoreKEY_WEBREFRESH      0xE7    /* Web Refresh */
#define HCoreKEY_WEBSTOP         0xE8    /* Web Stop */
#define HCoreKEY_WEBFORWARD      0xE9    /* Web Forward */
#define HCoreKEY_WEBBACK         0xEA    /* Web Back */
#define HCoreKEY_MYCOMPUTER      0xEB    /* My Computer */
#define HCoreKEY_MAIL            0xEC    /* Mail */
#define HCoreKEY_MEDIASELECT     0xED    /* Media Select */

/*
 *  Alternate names for keys, to facilitate transition from DOS.
 */
#define HCoreKEY_BACKSPACE       HCoreKEY_BACK            /* backspace */
#define HCoreKEY_NUMPADSTAR      HCoreKEY_MULTIPLY        /* * on numeric keypad */
#define HCoreKEY_LALT            HCoreKEY_LMENU           /* left Alt */
#define HCoreKEY_CAPSLOCK        HCoreKEY_CAPITAL         /* CapsLock */
#define HCoreKEY_NUMPADMINUS     HCoreKEY_SUBTRACT        /* - on numeric keypad */
#define HCoreKEY_NUMPADPLUS      HCoreKEY_ADD             /* + on numeric keypad */
#define HCoreKEY_NUMPADPERIOD    HCoreKEY_DECIMAL         /* . on numeric keypad */
#define HCoreKEY_NUMPADSLASH     HCoreKEY_DIVIDE          /* / on numeric keypad */
#define HCoreKEY_RALT            HCoreKEY_RMENU           /* right Alt */
#define HCoreKEY_UPARROW         HCoreKEY_UP              /* UpArrow on arrow keypad */
#define HCoreKEY_PGUP            HCoreKEY_PRIOR           /* PgUp on arrow keypad */
#define HCoreKEY_LEFTARROW       HCoreKEY_LEFT            /* LeftArrow on arrow keypad */
#define HCoreKEY_RIGHTARROW      HCoreKEY_RIGHT           /* RightArrow on arrow keypad */
#define HCoreKEY_DOWNARROW       HCoreKEY_DOWN            /* DownArrow on arrow keypad */
#define HCoreKEY_PGDN            HCoreKEY_NEXT            /* PgDn on arrow keypad */

#endif // _HCoreINPUT_
