/**
 * @file snake_game.h
 * Snake Game Component for AI Pocket Pet
 */

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

/*********************
 *      INCLUDES
 *********************/
#include "ai_pocket_pet_app.h"

/*********************
 *      DEFINES
 *********************/

/*********************
 *      TYPEDEFS
 **********************/

/*********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief 显示并启动贪吃蛇游戏
 * @param 无
 * @return 无
 *
 * 创建游戏界面并启动游戏循环
 */
void snake_game_show(void);

/**
 * @brief 处理游戏键盘输入
 * @param key 按下的键值
 * @return 无
 *
 * 处理游戏中的按键输入事件
 */
void snake_game_key_input(int key);

/*********************
 *      MACROS
 **********************/

#endif /* SNAKE_GAME_H */
