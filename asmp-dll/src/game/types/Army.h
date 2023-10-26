/**
 * @file Army.h
 * @brief Provides an API for interacting with armies.
 *
 */

#ifndef TYPES_ARMY_H
#define TYPES_ARMY_H

#include "common.h"
#include "List.h"

typedef struct Army Army;
typedef struct Army_vtbl Army_vtbl;
typedef struct EntPlayer EntPlayer;

typedef Army*(CC_THISCALL* Army__destroy_t)(Army* this, int free);
typedef uint32_t(CC_THISCALL* Army__get_money_t)(Army* this);
typedef uint32_t(CC_THISCALL* Army__set_money_t)(Army* this, uint32_t money);
typedef uint32_t(CC_THISCALL* Army__set_clever_attack_t)(Army* this, int value);
typedef EntPlayer*(CC_THISCALL* Army__get_player_t)(Army* this);

typedef struct __cppobj Army
{
    Army_vtbl* __vftable /*VFT*/;
    uint32_t money;
    uint32_t field_8;
    uint32_t idx;
    EntPlayer* player;
    List list;
    uint32_t field_24;
    uint32_t field_28;
    uint32_t field_2C;
    uint32_t field_30;
    uint32_t field_34;
    uint32_t field_38;
    uint32_t field_3C;
    uint32_t field_40;
    uint32_t field_44;
    uint32_t field_48;
    uint32_t field_4C;
    uint32_t field_50[183];
} Army;

typedef struct __cppobj Army_vtbl
{
    Army*(CC_THISCALL* destroy)(Army* __hidden this, int free);
    void* f1;
    void* f2;
    void(CC_THISCALL* f3)(Army* __hidden this, int);
    void* f4;
    void* f5;
    void* f6;
    void(CC_THISCALL* f7)(Army* __hidden this);
    void(CC_THISCALL* f8)(Army* __hidden this);
} Army_vtbl;


/**
 * @brief Destroys an army instance.
 *
 * @param[in] ECX   Army instance pointer.
 * @param[in] free  Whether to free the memory allocated for the army.
 *
 * @return Army* Pointer to destroyed army instance.
 */
Army* Army__destroy(Army* ECX, int free);

/**
 * @brief Gets army money.
 *
 * @param[in] ECX Army instance pointer.
 *
 * @return uint32_t Army money.
 */
uint32_t Army__get_money(Army* ECX);

/**
 * @brief Sets army money.
 *
 * @param[in] ECX   Army instance pointer.
 * @param[in] money Army money.
 *
 * @return uint32_t Army money.
 */
uint32_t Army__set_money(Army* ECX, uint32_t money);

/**
 * @brief Sets army clever attack flag.
 *
 * @param[in] ECX   Army instance pointer.
 * @param[in] value Clever attack flag value.
 *
 * @return unk. // TODO: Reverse-engineer this function.
 */
uint32_t Army__set_clever_attack(Army* ECX, int value);

/**
 * @brief Gets army flagman player entity.
 *
 * @param[in] ECX Army instance pointer.
 *
 * @return EntPlayer* Army flagman player entity.
 */
EntPlayer* Army__get_player(Army* ECX);

#endif /* TYPES_ARMY_H */
