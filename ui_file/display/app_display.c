/**
 * @file app_display.c
 * @brief UI显示应用的入口文件
 */

/*********************
 *      INCLUDES
 *********************/
#include "app_display.h"
#include "lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "ui/screen_manager.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * @brief 初始化显示相关的功能
 * @note 可以在这里添加任何需要在系统启动时初始化的UI相关内容
 */
void app_display_init(void)
{
    screens_init();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
