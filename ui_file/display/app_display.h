/**
 * @file app_display.h
 * @brief UI显示应用的头文件
 */

#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief 初始化显示相关的功能
 * @note 可以在这里添加任何需要在系统启动时初始化的UI相关内容
 */
void app_display_init(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*APP_DISPLAY_H*/