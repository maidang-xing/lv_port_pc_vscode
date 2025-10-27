/**
 * @file main_screen.h
 * @brief Declaration of the main screen for the application
 *
 * This file contains the declarations for the main screen which displays
 * the main AI Pocket Pet interface including status bar, pet area, and
 * menu system. This is the primary screen after the startup sequence.
 *
 * The main screen includes:
 * - Screen initialization and deinitialization functions
 * - Screen structure definition for the screen manager
 * - Main UI components initialization
 *
 * @copyright Copyright (c) 2024 LVGL PC Simulator Project
 */

#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "screen_manager.h"

/***********************************************************
***********************Type Definitions********************
***********************************************************/
typedef enum {
    AI_PET_STATE_NORMAL,        // Normal state (walk, blink, stand)
    AI_PET_STATE_SLEEP,         // Sleeping animation
    AI_PET_STATE_DANCE,         // Dancing animation
    AI_PET_STATE_EAT,           // Eating animation
    AI_PET_STATE_BATH,          // Bathing animation
    AI_PET_STATE_TOILET,        // Toilet animation
    AI_PET_STATE_SICK,          // Sick animation
    AI_PET_STATE_HAPPY,         // Happy emotion
    AI_PET_STATE_ANGRY,         // Angry emotion
    AI_PET_STATE_CRY,           // Crying emotion
    // Legacy states for backward compatibility
    AI_PET_STATE_IDLE = AI_PET_STATE_NORMAL,
    AI_PET_STATE_WALKING = AI_PET_STATE_NORMAL,
    AI_PET_STATE_BLINKING = AI_PET_STATE_NORMAL,
    AI_PET_STATE_EATING = AI_PET_STATE_EAT,
    AI_PET_STATE_SLEEPING = AI_PET_STATE_SLEEP,
    AI_PET_STATE_PLAYING = AI_PET_STATE_DANCE
} ai_pet_state_t;

typedef enum {
    AI_PET_MENU_MAIN,
    AI_PET_MENU_INFO,
    AI_PET_MENU_FOOD,
    AI_PET_MENU_BATH,
    AI_PET_MENU_HEALTH,
    AI_PET_MENU_SLEEP
} ai_pet_menu_t;

typedef struct {
    uint8_t health;    // 0-100
    uint8_t hungry;    // 0-100
    uint8_t happy;     // 0-100
    uint16_t age_days; // Age in days
    float weight_kg;   // Weight in kg (decimal)
    char name[16];     // Pet name
} ai_pet_stats_t;

typedef enum {
    PET_EVENT_FEED_HAMBURGER,
    PET_EVENT_DRINK_WATER,
    PET_EVENT_FEED_PIZZA,
    PET_EVENT_FEED_APPLE,
    PET_EVENT_FEED_FISH,
    PET_EVENT_FEED_CARROT,
    PET_EVENT_FEED_ICE_CREAM,
    PET_EVENT_FEED_COOKIE,
    PET_EVENT_TOILET,
    PET_EVENT_TAKE_BATH,
    PET_EVENT_SEE_DOCTOR,
    PET_EVENT_SLEEP,
    PET_EVENT_WAKE_UP,
    PET_EVENT_WIFI_SCAN,
    PET_EVENT_I2C_SCAN,
    PET_STAT_RANDOMIZE,
    PET_EVENT_MAX
} pet_event_type_t;
/**********************************************************/
extern Screen_t main_screen;

void main_screen_init(void);
void main_screen_deinit(void);

void simple_demo_set_wifi_strength(uint8_t strength);
void simple_demo_set_cellular_status(uint8_t strength, bool connected);
void simple_demo_set_battery_status(uint8_t level, bool charging);

void simple_pet_area_set_animation(ai_pet_state_t state);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MAIN_SCREEN_H*/
