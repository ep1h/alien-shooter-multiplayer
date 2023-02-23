#ifndef TYPES_ENUMS
#define TYPES_ENUMS

/* 75 */
typedef enum enAnim
{
    ANI_STAND           = 0x0,
    ANI_STOP_MOVE       = 0x1,
    ANI_BUILD           = 0x1,
    ANI_GO              = 0x2,
    ANI_START_MOVE      = 0x3,
    ANI_L_ROTATE        = 0x4,
    ANI_R_ROTATE        = 0x5,
    ANI_OPEN            = 0x6,
    ANI_CLOSE           = 0x7,
    ANI_HIT             = 0x7,
    ANI_FIGHT           = 0x8,
    ANI_SALUT           = 0x9,
    ANI_STAND_OPEN      = 0xA,
    ANI_LOAD            = 0xB,
    ANI_CLASH_VERT      = 0xB,
    ANI_LINK            = 0xB,
    ANI_UNLOAD          = 0xC,
    ANI_LAZY            = 0xC,
    ANI_CLASH           = 0xC,
    ANI_WOUND           = 0xD,
    ANI_BIRTH           = 0xE,
    ANI_DEATH           = 0xF,
    ANI_DEATH2          = 0x10,
    ANI_MENUSTAND       = 0x0,
    ANI_MENUSTANDDOWN   = 0x0,
    ANI_MENUSTANDUP     = 0x1,
    ANI_MENUSELECTDOWN  = 0x2,
    ANI_MENUSELECTUP    = 0x3,
    ANI_MENUPRESSDOWN   = 0x4,
    ANI_MENUPRESSUP     = 0x5,
    ANI_MENUDISABLE     = 0x6,
    ANI_MENUDISABLEDOWN = 0x6,
    ANI_MENUDISABLEUP   = 0x7,
    ANI_FONT_NORMAL     = 0x0,
    ANI_FONT_NEXT_CHAR  = 0x1,
    ANI_FONT_NEXT_LINE  = 0x2,
} enAnim;

/* 78 */
typedef enum enVidSprType
{
    U_TERRAIN = 0x1,
    U_OBJECT  = 0x2,
    U_UNIT    = 0x4,
    U_AVIA    = 0x8,
    U_MENU    = 0x10,
    U_RAILWAY = 0x20,
    U_REGION  = 0x40,
    U_TACTIC  = 0x80,
    U_CANNON  = 0x200,
    U_SPRITE  = 0x400,
} enVidSprType;

/* 79 */
typedef enum enVidSprClass
{
    B_TERRAIN      = 0x0,
    B_OBJECT       = 0x1,
    B_UNIT         = 0x2,
    B_BUILDING     = 0x3,
    B_AVIA         = 0x4,
    B_CANNON       = 0x5,
    B_PRIMITIVE    = 0x6,
    B_PLAYER__     = 0x7,
    B_UNK_MB_OBJ__ = 0x8,
    B_SPRITE       = 0x9,
    B_FRAME        = 0xA,
    B_LINKER       = 0xC,
    B_TEXT         = 0x13,
    B_CIV_ROBOT    = 0x14,
    B_ENGINE       = 0x15,
    B_RAIL         = 0x16,
    B_REGION       = 0x17,
    B_DEPO         = 0x18,
    B_CREATURE     = 0x19,
    B_BALLOON      = 0x1A,
    B_MISSILE      = 0x1B,
} enVidSprClass;

/* 80 */
typedef enum enEntAction
{
    ACT_ATTACK           = 0x20,
    ACT_MOVE             = 0x21,
    ACT_MOVE_TO          = 0x22,
    ACT_BUILD_UNIT       = 0x23,
    ACT_PATROL           = 0x24,
    ACT_COOR_ATTACK      = 0x25,
    ACT_RANDOM           = 0x26,
    ACT_STOP             = 0x27,
    ACT_PAUSE            = 0x28,
    ACT_ROTATE           = 0x29,
    ACT_CLEAR_COMMAND    = 0x2A,
    ACT_FLAGMAN_TRIGGER  = 0x2B,
    ACT_ADD_ITEM         = 0x36,
    ACT_DELETE_ITEM      = 0x37,
    ACT_HAVE_ITEM        = 0x38,
    ACT_DELETE_ALL_ITEM  = 0x39,
    ACT_GET_ITEM         = 0x3A,
    ACT_GET_ITEM_TYPE    = 0x3B,
    ACT_CHANGE_DIRECTION = 0x3C,
    ACT_CHANGE_ANIMATION = 0x3D,
    ACT_CHANGE_VID       = 0x3E,
    ACT_CHANGE_COOR      = 0x3F,
    ACT_BACKUP_COMMAND   = 0x46,
    ACT_GOTO_STACK       = 0x47,
    ACT_CLEAR_STACK      = 0x48,
    ACT_STOP_STACK       = 0x49,
    ACT_RESTORE_COMMAND  = 0x4A,
    ACT_COPY_STACK_TO    = 0x4B,
    ACT_NO_STACK         = 0x4C,
    ACT_SAVE             = 0x50,
    ACT_RESTORE          = 0x51,
    ACT_DAMAGE           = 0x55,
    ACT_REPAIR           = 0x56,
    ACT_GET_HP           = 0x57,
    ACT_SET_HP           = 0x58,
    ACT_GET_PERCENT_HP   = 0x59,
    ACT_GET_GOAL         = 0x5A,
    ACT_SET_GOAL_COOR    = 0x5B,
    ACT_GET_AMMO         = 0x5C,
    ACT_ADD_AMMO         = 0x5D,
    ACT_GET_BEHAVE       = 0x5E,
    ACT_SET_BEHAVE       = 0x5F,
    ACT_GET_ARMY         = 0x60,
    ACT_SET_ARMY         = 0x61,
    ACT_SET_INVISIBLE    = 0x62,
    ACT_GET_BATTLE_RANGE = 0x64,
    ACT_GET_LINK         = 0x65,
    ACT_SET_LINK         = 0x66,
    ACT_GET_UPLINK       = 0x67,
    ACT_SET_UPLINK       = 0x68,
    ACT_GET_TIMER        = 0x69,
    ACT_SET_TIMER        = 0x6A,
    ACT_GET_ZSPEED       = 0x6B,
    ACT_SET_ZSPEED       = 0x6C,
    ACT_GET_SPEED        = 0x6D,
    ACT_SET_SPEED        = 0x6E,
    ACT_GET_COMMAND      = 0x6F,
    ACT_SET_DEATH_TIMER  = 0x70,
    ACT_SET_TEXT         = 0x78,
    ACT_GET_TEXT_DESC    = 0x79,
    ACT_SET_TEXT_COUNT   = 0x7A,
    ACT_SET_FILE         = 0x7B,
    ACT_NEXT_COMMAND     = 0x82,
    ACT_LOGIC_RUN        = 0x83,
    ACT_UNDO_REMOVE      = 0x84,
    ACT_UNDO_INSERT      = 0x85,
    ACT_DESTROY_UNIT     = 0x86,
    ACT_PLAY_SFX         = 0x87,
    ACT_SELECT           = 0x2C,
    ACT_CHANGECOOR_Z     = 0x45,
    ACT_SETBUILDTIME     = 0x4D,
    ACT_GETBUILDTIME     = 0x4E,
    ACT_LINK_ENGINE      = 0x96,
    ACT_CLASH_ENGINE     = 0x97,
    ACT_FORCELINK_ENGINE = 0x98,
    ACT_TRAIN_BEHAVE     = 0x99,
    ACT_FIRST_ENGINE     = 0x9A,
    ACT_LAST_ENGINE      = 0x9B,
    ACT_NEXT_ENGINE      = 0x9C,
    ACT_IS_FIRST         = 0x9D,
    ACT_IN_TRAIN         = 0x9E,
    ACT_IS_TRAIN         = 0x9F,
    ACT_NONE             = 0xFF,
} enEntAction;

/* 96 */
typedef enum enVid
{
    VID_000_TERRAIN_LOADING                       = 0x0,
    VID_002_MENU_FONT                             = 0x2,
    VID_003_MENU_FONT_BIG                         = 0x3,
    VID_004_MENU_FONT_SMALL                       = 0x4,
    VID_005_MENU_FONT_NUMBERS                     = 0x5,
    VID_009_UNIT_PLAYER_MALE_LEGS                 = 0x9,
    VID_040_UNIT_MONSTER_5_GREEN                  = 0x28,
    VID_042_TERRAIN_MONSTER_5_CORPSE              = 0x2A,
    VID_043_UNIT_MONSTER_5_YELLOW                 = 0x2B,
    VID_045_TERRAIN_MONSTER_5_CORPSE_YELLOW       = 0x2D,
    VID_046_UNIT_MONSTER_5_RED                    = 0x2E,
    VID_048_TERRAIN_MONSTER_5_CORPSE_RED          = 0x30,
    VID_049_TERRAIN_MONSTER_5_FROZEN              = 0x31,
    VID_050_UNIT_MONSTER_1_GREEN                  = 0x32,
    VID_052_TERRAIN_MONSTER_1_CORPSE              = 0x34,
    VID_053_UNIT_MONSTER_1_YELLOW                 = 0x35,
    VID_055_TERRAIN_MONSTER_1_CORPSE_YELLOW       = 0x37,
    VID_056_UNIT_MONSTER_1_RED                    = 0x38,
    VID_058_TERRAIN_MONSTER_1_CORPSE_RED          = 0x3A,
    VID_059_TERRAIN_MONSTER_1_FROZEN              = 0x3B,
    VID_060_UNIT_MONSTER_2_GREEN                  = 0x3C,
    VID_062_TERRAIN_MONSTER_2_CORPSE              = 0x3E,
    VID_063_UNIT_MONSTER_2_YELLOW                 = 0x3F,
    VID_065_TERRAIN_MONSTER_2_CORPSE_YELLOW       = 0x41,
    VID_066_UNIT_MONSTER_2_RED                    = 0x42,
    VID_068_TERRAIN_MONSTER_2_CORPSE_RED          = 0x44,
    VID_069_TERRAIN_MONSTER_2_FROZEN              = 0x45,
    VID_070_UNIT_MONSTER_3_GREEN                  = 0x46,
    VID_073_UNIT_MONSTER_3_YELLOW                 = 0x49,
    VID_076_UNIT_MONSTER_3_RED                    = 0x4C,
    VID_079_TERRAIN_MONSTER_3_FROZEN              = 0x4F,
    VID_080_UNIT_MONSTER_4_GREEN                  = 0x50,
    VID_082_TERRAIN_MONSTER_TANK_CORPSE           = 0x52,
    VID_083_UNIT_MONSTER_4_YELLOW                 = 0x53,
    VID_085_TERRAIN_MONSTER_TANK_CORPSE_YELLOW    = 0x55,
    VID_086_UNIT_MONSTER_4_RED                    = 0x56,
    VID_088_TERRAIN_MONSTER_TANK_CORPSE_RED       = 0x58,
    VID_089_TERRAIN_MONSTER_4_FROZEN              = 0x59,
    VID_090_UNIT_MONSTER_5_GREEN                  = 0x5A,
    VID_093_UNIT_MONSTER_5_YELLOW                 = 0x5D,
    VID_096_UNIT_MONSTER_5_RED                    = 0x60,
    VID_099_UNIT_MONSTER_5_FROZEN                 = 0x63,
    VID_108_TERRAIN_SHADOW_LITTLE                 = 0x6C,
    VID_110_TERRAIN_BLOOD_RED_GROUND_MEDIUM       = 0x6E,
    VID_120_TERRAIN_BLOOD_RED_GROUND_SMALL        = 0x78,
    VID_125_TERRAIN_PICKUP_HEALTH_FX              = 0x7D,
    VID_139_TERRAIN_PICKUP_AMMO_FX                = 0x8B,
    VID_152_TERRAIN_MUSHROOMS_RED                 = 0x98,
    VID_154_TERRAIN_BLOOD_GROUND_MEDIUM           = 0x9A,
    VID_164_TERRAIN_FENCE_PLASMA                  = 0xA4,
    VID_166_TERRAIN_BLOOD_GREEN_GROUND_MEDIUM     = 0xA6,
    VID_167_TERRAIN_BLOOD_GREEN_GROUND_SMALL      = 0xA7,
    VID_179_TERRAIN_ARROW_FX_DYNAMITE             = 0xB3,
    VID_180_TERRAIN_ARROW_FX                      = 0xB4,
    VID_198_TERRAIN_MUSHROOMS_GREEN               = 0xC6,
    VID_204_OBJECT_PICKUP_ARMOR_GREEN             = 0xCC,
    VID_205_OBJECT_PICKUP_ARMOR_YELLOW            = 0xCD,
    VID_206_OBJECT_PICKUP_ARMOR_RED               = 0xCE,
    VID_207_OBJECT_PICKUP_IMMORTALITY             = 0xCF,
    VID_210_OBJECT_PICKUP_HEALTH                  = 0xD2,
    VID_211_OBJECT_PICKUP_HEALTH_BIG              = 0xD3,
    VID_212_OBJECT_PICKUP_HEALTH_MEGA             = 0xD4,
    VID_230_OBJECT_PICKUP_MEDKIT                  = 0xE6,
    VID_231_OBJECT_PICKUP_DRONE                   = 0xE7,
    VID_232_OBJECT_PICKUP_ACCELERATION            = 0xE8,
    VID_233_OBJECT_PICKUP_DECELERATION            = 0xE9,
    VID_234_OBJECT_PICKUP_FREEZE                  = 0xEA,
    VID_235_OBJECT_PICKUP_LIFE                    = 0xEB,
    VID_236_OBJECT_PICKUP_BOMB                    = 0xEC,
    VID_237_OBJECT_PICKUP_BOMB_2                  = 0xED,
    VID_238_OBJECT_PICKUP_ADRENALINE              = 0xEE,
    VID_241_OBJECT_PICKUP_MONEY                   = 0xF1,
    VID_242_OBJECT_PICKUP_IMPLANT_HALTH           = 0xF2,
    VID_243_OBJECT_PICKUP_IMPLANT_STRENGTH        = 0xF3,
    VID_244_OBJECT_PICKUP_IMPLANT_VELOCITY        = 0xF4,
    VID_245_OBJECT_PICKUP_IMPLANT_ACCURACY        = 0xF5,
    VID_246_OBJECT_PICKUP_TORCH                   = 0xF6,
    VID_247_OBJECT_PICKUP_NIGHTVISION             = 0xF7,
    VID_260_OBJECT_UNK_MB_ALIEN_GUN               = 0x104,
    VID_261_OBJECT_UNK_MB_ALIEN_GUN               = 0x105,
    VID_262_OBJECT_PICKUP_WEAPON_SHOTGUN          = 0x106,
    VID_263_OBJECT_PICKUP_WEAPON_GRENADE_LAUNCHER = 0x107,
    VID_264_OBJECT_PICKUP_WEAPON_MINIGUN          = 0x108,
    VID_265_OBJECT_PICKUP_WEAPON_ROCKET_LAUNCHER  = 0x109,
    VID_266_OBJECT_PICKUP_WEAPON_FREEZE_RIFLE     = 0x10A,
    VID_267_OBJECT_PICKUP_WEAPON_PLASMA_RIFLE     = 0x10B,
    VID_268_OBJECT_PICKUP_WEAPON_FLAMETHROWER     = 0x10C,
    VID_269_OBJECT_PICKUP_WEAPON_MAGMA_MINIGUN    = 0x10D,
    VID_290_UNIT_DRONE                            = 0x122,
    VID_301_OBJECT_PICKUP_AMMO_DYNAMITE           = 0x12D,
    VID_302_OBJECT_PICKUP_AMMO_SHOTGUN            = 0x12E,
    VID_303_OBJECT_PICKUP_AMMO_GRENADE_LAUNCHER   = 0x12F,
    VID_304_OBJECT_PICKUP_AMMO_MINIGUN            = 0x130,
    VID_305_OBJECT_PICKUP_AMMO_ROCKET_LAUNCHER    = 0x131,
    VID_306_OBJECT_PICKUP_AMMO_FREEZE_RIFLE       = 0x132,
    VID_307_OBJECT_PICKUP_AMMO_PLASMA_RIFLE       = 0x133,
    VID_308_OBJECT_PICKUP_AMMO_FLAMETHROWER       = 0x134,
    VID_309_OBJECT_PICKUP_AMMO_MAGMA_MINIGUN      = 0x135,
    VID_452_TERRAIN_TELEPORT_FX                   = 0x1C4,
    VID_454_TERRAIN_TELEPORT_FX_2                 = 0x1C6,
    VID_500_TERRAIN_FLOOR_BASEMENT                = 0x1F4,
    VID_501_TERRAIN_FLOOR_GROUND                  = 0x1F5,
    VID_502_TERRAIN_FLOOR_BASEMENT_2              = 0x1F6,
    VID_503_TERRAIN_FLOOR_IRON                    = 0x1F7,
    VID_505_TERRAIN_FLOOR_GROUND_2                = 0x1F9,
    VID_506_TERRAIN_FLOOR_GRASS_3D                = 0x1FA,
    VID_507_TERRAIN_FLOOR_GRASS_2D                = 0x1FB,
    VID_549_UNIT_PLAYER_FEMALE_LEGS               = 0x225,
    VID_700_MENU_LOGO_TEAM                        = 0x2BC,
    VID_701_MENU_LOGO_GAME                        = 0x2BD,
    VID_703_MENU_LOGO_TEAM_SMALL                  = 0x2BF,
    VID_704_MENU_BKG                              = 0x2C0,
    VID_705_MENU_BUTTON_BACKGROUND                = 0x2C1,
    VID_706_MENU_PLAYER_MALE                      = 0x2C2,
    VID_707_MENU_PLAYER_FEMALE                    = 0x2C3,
    VID_708_MENU_EDITBOX                          = 0x2C4,
    VID_709_MENU_BKG_UNK                          = 0x2C5,
    VID_710_MENU_BKG_UNK                          = 0x2C6,
    VID_711_MENU_SHOP_BACKGROUND                  = 0x2C7,
    VID_712_MENU_SHOP_BUTTON_BACKGROUND           = 0x2C8,
    VID_713_MENU_SHOP_BUTTON_BACKGROUND_2         = 0x2C9,
    VID_714_MENU_BAR_SOUND_LEVEL                  = 0x2CA,
    VID_715_MENU_BAR_MUSIC_LEVEL                  = 0x2CB,
    VID_716_MENU_UNK                              = 0x2CC,
    VID_717_MENU_UNK                              = 0x2CD,
    VID_718_MENU_PLAYER_MALE                      = 0x2CE,
    VID_719_MENU_LOW_HP_INDICATOR                 = 0x2CF,
    VID_720_MENU_IMPLANT_RED                      = 0x2D0,
    VID_721_MENU_IMPLANT_BLUE                     = 0x2D1,
    VID_722_MENU_IMPLANT_YELLOW                   = 0x2D2,
    VID_723_MENU_IMPLANT_GREEN                    = 0x2D3,
    VID_724_MENU_ARMOR_GREEN                      = 0x2D4,
    VID_725_MENU_ARMOR_YELLOW                     = 0x2D5,
    VID_726_MENU_ARMOR_RAD                        = 0x2D6,
    VID_727_MENU_LIFE                             = 0x2D7,
    VID_728_MENU_TORCH                            = 0x2D8,
    VID_729_MENU_NIGHTVISION                      = 0x2D9,
    VID_730_MENU_MEDKIT                           = 0x2DA,
    VID_731_MENU_DRONE                            = 0x2DB,
    VID_732_MENU_ALIENGUN                         = 0x2DC,
    VID_733_MENU_SHOTGUN_AMMO                     = 0x2DD,
    VID_734_MENU_GRENADE_LAUNCHER_AMMO            = 0x2DE,
    VID_735_MENU_MINIGUN_AMMO                     = 0x2DF,
    VID_736_MENU_ROCKET_LAUNCHER_AMMO             = 0x2E0,
    VID_737_MENU_FREEZE_RIFLE_AMMO                = 0x2E1,
    VID_738_MENU_PLASMA_RIFLE_AMMO                = 0x2E2,
    VID_739_MENU_FLAMETHROWER_AMMO                = 0x2E3,
    VID_740_MENU_MAGMA_MINIGUN_AMMO               = 0x2E4,
    VID_741_MENU_GAME_HP_AP_BACKGROUND            = 0x2E5,
    VID_742_MENU_GAME_SCORE_BACKGROUND            = 0x2E6,
    VID_743_MENU_GAME_SCORE_BACKGROUND_2          = 0x2E7,
    VID_745_MENU_GAME_AMMO_PROGRESSBAR            = 0x2E9,
    VID_746_MENU_BACK_BLACKGROUND                 = 0x2EA,
    VID_747_MENU_CIRCLE_BACKGROUND_UNK            = 0x2EB,
    VID_748_MENU_DYNAMITE                         = 0x2EC,
    VID_749_MENU_BACKGROUND_GRAY_GRID_UNK         = 0x2ED,
    VID_759_TERRAIN_PICKUP_BONUS_FX               = 0x2F7,
    VID_782_TERRAIN_ROCK                          = 0x30E,
    VID_783_TERRAIN_ROCK_2                        = 0x30F,
    VID_785_TERRAIN_FLASHLIGHT_RAYS               = 0x311,
    VID_786_TERRAIN_ARROW                         = 0x312,
    VID_800_UNIT_MONSTER_GENERATOR_1_GREEN        = 0x320,
    VID_801_UNIT_MONSTER_GENERATOR_1_YELLOW       = 0x321,
    VID_802_UNIT_MONSTER_GENERATOR_1_RED          = 0x322,
    VID_803_UNIT_MONSTER_GENERATOR_2_GREEN        = 0x323,
    VID_804_UNIT_MONSTER_GENERATOR_2_YELLOW       = 0x324,
    VID_805_UNIT_MONSTER_GENERATOR_2_RED          = 0x325,
    VID_806_UNIT_MONSTER_GENERATOR_3_GREEN        = 0x326,
    VID_807_UNIT_MONSTER_GENERATOR_3_YELLOW       = 0x327,
    VID_808_UNIT_MONSTER_GENERATOR_3_RED          = 0x328,
    VID_809_UNIT_MONSTER_GENERATOR_4_GREEN        = 0x329,
    VID_810_UNIT_MONSTER_GENERATOR_4_YELLOW       = 0x32A,
    VID_811_UNIT_MONSTER_GENERATOR_4_RED          = 0x32B,
    VID_812_UNIT_MONSTER_GENERATOR_5_GREEN        = 0x32C,
    VID_813_UNIT_MONSTER_GENERATOR_5_YELLOW       = 0x32D,
    VID_814_UNIT_MONSTER_GENERATOR_5_RED          = 0x32E,
    VID_850_UNIT_MONSTER_1_BLUE                   = 0x352,
    VID_852_TERRAIN_MONSTER_1_CORPSE_BLUE         = 0x354,
    VID_853_UNIT_MONSTER_2_BLUE                   = 0x355,
    VID_855_TERRAIN_MONSTER_2_CORPSE_BLUE         = 0x357,
    VID_856_UNIT_MONSTER_3_BLUE                   = 0x358,
    VID_860_UNIT_MONSTER_4_BLUE                   = 0x35C,
    VID_862_TERRAIN_MONSTER_5_CORPSE_BLUE         = 0x35E,
    VID_863_UNIT_MONSTER_5_BLUE                   = 0x35F,
    VID_865_TERRAIN_MONSTER_TANK_BOSS_CORPSE      = 0x361,
    VID_865_UNIT_MONSTER_5_ARMED_BLUE             = 0x361,
    VID_867_UNIT_MONSTER_3_UPGRADED_BLUE          = 0x363,
    VID_900_TERRAIN_UNK                           = 0x384,
    VID_911_TERRAIN_UNK                           = 0x38F,
    VID_912_TERRAIN_SHOOT_POINTER_FX              = 0x390,
    VID_913_TERRAIN_UNK_MB_ARROW                  = 0x391,
    VID_914_TERRAIN_DYNAMITE_PTR                  = 0x392,
    VID_915_TERRAIN_UNK                           = 0x393,
    VID_916_TERRAIN_BLOOD_RED_BIG                 = 0x394,
    VID_917_TERRAIN_MONSTER_1_CORPSE_UNK          = 0x395,
    VID_918_TERRAIN_BLOOD_GREEN_GROUND_BIG        = 0x396,
    VID_949_MENU_HELP_SCREEN                      = 0x3B5,
    VID_950_MENU_HIGHSCORES_BACKGROUND            = 0x3B6,
    VID_954_MENU_PRESENTATION_SLIDES_BEGIN        = 0x3BA,
    VID_956_MENU_WEBSITE                          = 0x3BC,
    VID_957_MENU_PRESENTATION_SLIDES_END          = 0x3BD,
} enVid;

/* 105 */
typedef enum enVidGetSetData
{
    VID_MAXHP               = 0x1,
    VID_GAMMA               = 0x12,
    VID_PROPERTY            = 0x16,
    VID_BATTLERANGE         = 0x17,
    VID_MAXUNIT             = 0x19,
    VID_AMMO                = 0x1A,
    VID_NAME                = 0x1B,
    VID_COUNT               = 0x1C,
    VID_KILLEDUNIT          = 0x1D,
    VID_KILLEDUNIT0         = 0x1E,
    VID_KILLEDUNIT1         = 0x1F,
    VID_KILLEDUNIT2         = 0x20,
    VID_KILLEDUNIT3         = 0x21,
    VID_COUNT0              = 0x22,
    VID_COUNT1              = 0x23,
    VID_COUNT2              = 0x24,
    VID_COUNT3              = 0x25,
    VID_MAXHP0              = 0x26,
    VID_MAXHP1              = 0x27,
    VID_MAXHP2              = 0x28,
    VID_MAXHP3              = 0x29,
    VID_HP_COEFF0           = 0x2A,
    VID_HP_COEFF1           = 0x2B,
    VID_HP_COEFF2           = 0x2C,
    VID_HP_COEFF3           = 0x2D,
    VID_SPRITETYPE          = 0x2E,
    VID_CLASS               = 0x2F,
    VID_SPEED               = 0x30,
    VID_LIFETIME            = 0x31,
    VID_DETECTRANGE         = 0x32,
    VID_WEAPONAIM           = 0x33,
    VID_EXCHANGEVID         = 0x34,
    VID_NO_DIR              = 0x35,
    VID_MOVE_MASK           = 0x36,
    VID_BUILDTIME           = 0x37,
    VID_HIDE                = 0x38,
    VID_NOT_CREATE_AS_CHILD = 0x39,
    VID_FRAME_SPEED         = 0x3A,
    VID_LINK                = 0x3B,
    VID_CHILD               = 0x3C,
    VID_NO_CHILD            = 0x5C,
    VID_DAMAGE              = 0x7C,
    VID_RECOLORUNIT         = 0x7D,
    VID_RECOLORUNIT0        = 0x7E,
    VID_RECOLORUNIT1        = 0x7F,
    VID_RECOLORUNIT2        = 0x80,
    VID_RECOLORUNIT3        = 0x81,
} enVidGetSetData;

/* 107 */
typedef enum enTrainProp
{
    PROP_SPEED      = 0x1,
    PROP_WEAPON     = 0x2,
    PROP_LIFE       = 0x3,
    PROP_HP         = 0x4,
    PROP_AMMO       = 0x5,
    PROP_ACCELERATE = 0x6,
    PROP_BUILD_TIME = 0x7,
    PROP_FREE       = 0x9,
    PROP_AMMO_NO    = 0xA,
    PROP_AMMO_MAX   = 0xB,
} enTrainProp;

/* 110 */
typedef enum enInput
{
    INPUT_LCLICK = 0x1,
    INPUT_RCLICK = 0x2,
    INPUT_LDOWN  = 0x4,
    INPUT_RDOWN  = 0x8,
    INPUT_SHIFT  = 0x10,
    INPUT_CTRL   = 0x20,
    INPUT_LEFT   = 0x40,
    INPUT_RIGHT  = 0x80,
    INPUT_UP     = 0x100,
    INPUT_DOWN   = 0x200,
    INPUT_FIRST  = 0x400,
    INPUT_SECOND = 0x800,
} enInput;

/* 111 */
typedef enum enCursor
{
    CURSOR_OFF      = 0xFFFFFFFF,
    CURSOR_NORMAL   = 0x0,
    CURSOR_MOVE     = 0x2,
    CURSOR_CLASH    = 0x3,
    CURSOR_ATTACK   = 0x5,
    CURSOR_FIGHT    = 0x5,
    CURSOR_FIGHT2   = 0x6,
    CURSOR_SELECT   = 0x7,
    CURSOR_NOTMOVE  = 0x8,
    CURSOR_CYCLE    = 0x9,
    CURSOR_LINK     = 0xA,
    CURSOR_UNLINK   = 0xB,
    CURSOR_DELETE   = 0xF,
    CURSOR_HARDWARE = 0x100,
    CURSOR_SOFTWARE = 0x101,
} enCursor;

/* 112 */
typedef enum enGetSpriteType
{
    GETSPRITE_VID     = 0x800,
    GETSPRITE_CLASS   = 0x1000,
    GETSPRITE_HASH    = 0x8000,
    GETSPRITE_ARMY0   = 0x10000,
    GETSPRITE_ARMY1   = 0x20000,
    GETSPRITE_ARMY2   = 0x40000,
    GETSPRITE_ARMY3   = 0x80000,
    GETSPRITE_TERRAIN = 0x100000,
    GETSPRITE_OBJECT  = 0x200000,
    GETSPRITE_UNIT    = 0x400000,
    GETSPRITE_AVIA    = 0x800000,
    GETSPRITE_MENU    = 0x1000000,
    GETSPRITE_RAILWAY = 0x2000000,
    GETSPRITE_REGION  = 0x4000000,
    GETSPRITE_CANNON  = 0x20000000,
    GETSPRITE_SPRITE  = 0x40000000,
    GETSPRITE_FREE    = 0x80000000,
} enGetSpriteType;

/* 114 */
typedef enum enEffect
{
    EFF_NONE         = 0x0,
    EFF_NUKE         = 0x1,
    EFF_SCROLL       = 0x2,
    EFF_FADE         = 0x3,
    EFF_OPEN_WINDOW  = 0x4,
    EFF_ALPHAAPPEAR  = 0x5,
    EFF_PIXELAPPEAR  = 0x6,
    EFF_LINESHIFT    = 0x7,
    EFF_SCALE_WINDOW = 0x8,
    EFF_FADE_IN      = 0x9,
    EFF_FADE_OUT     = 0xA,
    EFF_SHIFT_GAMMA  = 0xB,
} enEffect;

/* 115 */
typedef enum enEnvironment
{
    ENV_SHADOW      = 0x1,
    ENV_LONGSHADOW  = 0x2,
    ENV_EARTHQUAKE  = 0x4,
    _ENV_SNOWSTORM  = 0x8,
    ENV_FOG         = 0x20,
    ENV_GROUND_SNOW = 0x40,
    ENV_SQUALL      = 0x80,
    ENV_FOGINV      = 0x100,
    ENV_FOGAPPEAR   = 0x200,
    ENV_STOP_RAIN   = 0x400,
    ENV_BORN_RAIN   = 0x800,
    ENV_RAIN        = 0xC00,
    ENV_MASK_RAIN   = 0xC00,
    ENV_STOP_SNOW   = 0x4000,
    ENV_BORN_SNOW   = 0x8000,
    ENV_SNOW        = 0xC000,
    ENV_MASK_SNOW   = 0xC000,
    ENV_STOP        = 0x80000000,
    ENV_STOPALL     = 0xFFFFFFFF,
} enEnvironment;

#endif /* GAME_ENUMS */
