/**
 * @file dino_game.c
 * Dino Game Component for AI Pocket Pet
 */

/*********************
 *      INCLUDES
 *********************/
#include "dino_game.h"
#include "ai_pocket_pet_app.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/
// Dino game tuning constants
#define DINO_JUMP_VY   12  /* positive = upward velocity magnitude */
#define DINO_GRAVITY    1  /* per-tick gravity (subtracts from vy) */
// Horizontal velocity applied when jumping (pixels per tick)
#define DINO_HORZ_VX    0
LV_IMG_DECLARE(ducky_game); // Declare the GIF image
/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    int dino_vy;       /* Vertical velocity (negative = up, positive = down) */
    int dino_y;        /* Vertical position (0 = ground level) */
    int dino_vx;       /* Horizontal velocity */
    int on_ground;     /* Boolean: 1 when on ground, 0 when in air */
    int obstacle_x;    /* Horizontal position of obstacle */
    int score;         /* Game score */
    int speed;         /* Game speed */
    int game_over;     /* Boolean: 1 when game over */
} dino_game_state_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void dino_game_stop_and_cleanup(void);
static void dino_game_timer_cb(lv_timer_t *tmr);
static void dino_game_event_cb(lv_event_t *e);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *g_game_screen = NULL;
static lv_obj_t *g_dino = NULL;
static lv_obj_t *g_obstacle = NULL;
static lv_obj_t *g_score_label = NULL;
static lv_timer_t *g_game_timer = NULL;
static dino_game_state_t g_gs;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void dino_game_stop_and_cleanup(void)
{
    // Save reference to main screen before doing any deletion
    lv_obj_t *main_screen = lv_demo_ai_pocket_pet_get_main_screen();
    lv_group_t *group = lv_group_get_default();

    // Delete timer first
    if (g_game_timer) {
        lv_timer_del(g_game_timer);
        g_game_timer = NULL;
    }

    // Delete game screen and all related objects
    if (g_game_screen) {
        // Remove event callbacks
        lv_obj_remove_event_cb(g_game_screen, dino_game_event_cb);

        // Clear references to child objects to avoid dangling pointers
        g_dino = NULL;
        g_obstacle = NULL;
        g_score_label = NULL;

        // Delete the screen itself
        lv_obj_del(g_game_screen);
        g_game_screen = NULL;
    }

    // Load main screen after cleanup
    if (main_screen) {
        lv_screen_load(main_screen);
        // Ensure the main screen can receive keyboard focus
        if (group) {
            // Make sure main screen is in the group and focused
            lv_group_focus_obj(main_screen);
        }
    }
}

/**
 * @brief 显示并启动恐龙游戏
 * @param 无
 * @return 无
 *
 * 创建游戏界面并启动游戏循环
 */
void dino_game_show(void)
{
    if (g_game_screen) return;  // Already running

    srand((unsigned int)time(NULL));

    g_gs.game_over = 0;
    g_gs.on_ground = 1;
    g_gs.dino_vy = 0;
    g_gs.dino_y = 0;
    g_gs.dino_vx = 0;
    g_gs.obstacle_x = 200;
    g_gs.speed = 4;
    g_gs.score = 0;

    g_game_screen = lv_obj_create(NULL);
    lv_obj_set_size(g_game_screen, AI_PET_SCREEN_WIDTH, AI_PET_SCREEN_HEIGHT);
    lv_obj_set_style_bg_color(g_game_screen, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(g_game_screen, LV_OPA_COVER, 0);

    // Score
    g_score_label = lv_label_create(g_game_screen);
    lv_label_set_text(g_score_label, "SCORE: 0");
    lv_obj_align(g_score_label, LV_ALIGN_TOP_MID, 0, 6);
    lv_obj_set_style_text_font(g_score_label, &lv_font_montserrat_14, 0);

    // Ground
    lv_obj_t *ground = lv_obj_create(g_game_screen);
    lv_obj_set_size(ground, lv_obj_get_width(g_game_screen), 6);
    lv_obj_set_style_bg_color(ground, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(ground, LV_OPA_COVER, 0);
    lv_obj_align(ground, LV_ALIGN_BOTTOM_MID, 0, -30);

    // Create Dino using GIF widget
    g_dino = lv_gif_create(g_game_screen);
    lv_gif_set_src(g_dino, &ducky_game);
    lv_obj_add_flag(g_dino, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(g_dino, LV_OBJ_FLAG_SCROLLABLE);

    /* Set size based on screen height */
    // lv_coord_t max_height = lv_obj_get_height(g_game_screen) - 50;  /* leave space for ground */
    lv_coord_t scaled_h = 50;  /* cap at 50px high */
    lv_coord_t scaled_w = 50;  /* maintain aspect ratio */
    lv_obj_set_size(g_dino, scaled_w, scaled_h);

    /* Position the dino */
    lv_obj_set_x(g_dino, 20);
    lv_obj_set_y(g_dino, lv_obj_get_height(g_game_screen) - 30 - scaled_h);

    // /* Position container */
    // lv_obj_set_x(g_dino, 20);
    // lv_obj_set_y(g_dino, lv_obj_get_height(g_game_screen) - 30 - scaled_h);

    // Obstacle
    g_obstacle = lv_obj_create(g_game_screen);
    /* Make obstacles proportional to dino size */
    lv_coord_t obstacle_h = scaled_h * 0.50;  /* 50% of dino height */
    lv_coord_t obstacle_w = scaled_w * 0.25;  /* 25% of dino width */
    lv_obj_set_size(g_obstacle, obstacle_w, obstacle_h);
    lv_obj_set_style_bg_color(g_obstacle, lv_color_black(), 0);
    lv_obj_set_x(g_obstacle, g_gs.obstacle_x);
    lv_obj_set_y(g_obstacle, lv_obj_get_height(g_game_screen) - 30 - obstacle_h);

    // Events and group
    lv_obj_add_event_cb(g_game_screen, dino_game_event_cb, LV_EVENT_ALL, NULL);
    lv_group_t *grp = lv_group_get_default();
    if (grp) {
        lv_group_add_obj(grp, g_game_screen);
        lv_group_focus_obj(g_game_screen);
    }

    lv_screen_load(g_game_screen);

    // Start timer
    g_game_timer = lv_timer_create(dino_game_timer_cb, 20, NULL);
}

static void dino_game_timer_cb(lv_timer_t *tmr)
{
    (void)tmr;
    if (!g_game_screen || g_gs.game_over) return;

    if (!g_gs.on_ground) {
        // Positive vy means upward; gravity reduces vy each tick
        g_gs.dino_vy -= DINO_GRAVITY;
        g_gs.dino_y += g_gs.dino_vy;
        // When dino_y drops back to or below 0, we're on the ground
        if (g_gs.dino_y <= 0) {
            g_gs.dino_y = 0;
            g_gs.dino_vy = 0;
            g_gs.on_ground = 1;
            // stop horizontal motion on landing
            // g_gs.dino_vx = 0;
        }
        lv_coord_t base_y = lv_obj_get_height(g_game_screen) - 30;
        lv_obj_set_y(g_dino, base_y - 50 - g_gs.dino_y);
    }

    // Horizontal position update (allow small forward movement during jump)
    if (g_gs.dino_vx != 0) {
        lv_coord_t cur_x = lv_obj_get_x(g_dino);
        lv_coord_t next_x = cur_x + g_gs.dino_vx;
        // clamp to screen
        if (next_x < 0) next_x = 0;
        if (next_x > (lv_obj_get_width(g_game_screen) - lv_obj_get_width(g_dino)))
            next_x = lv_obj_get_width(g_game_screen) - lv_obj_get_width(g_dino);
        lv_obj_set_x(g_dino, next_x);
    }

    char buf[32];
    g_gs.obstacle_x -= g_gs.speed;
    if (g_gs.obstacle_x < -40) {
        g_gs.obstacle_x = lv_obj_get_width(g_game_screen) + (rand() % 30);
        g_gs.score += 1;
        if (g_gs.score % 2 == 0 && g_gs.speed < 30) g_gs.speed += 1;

        snprintf(buf, sizeof(buf), "SCORE: %d", g_gs.score);
        lv_label_set_text(g_score_label, buf);
    }
    lv_obj_set_x(g_obstacle, g_gs.obstacle_x);

    // Collision
    lv_area_t dino_coords;
    lv_area_t obs_coords;
    lv_obj_get_coords(g_dino, &dino_coords);
    lv_obj_get_coords(g_obstacle, &obs_coords);
    // Add a small buffer to prevent overly sensitive collision detection
    const int collision_buffer = 2;
    if (!(dino_coords.x2 < obs_coords.x1 + collision_buffer ||
          dino_coords.x1 > obs_coords.x2 - collision_buffer ||
          dino_coords.y2 < obs_coords.y1 + collision_buffer ||
          dino_coords.y1 > obs_coords.y2 - collision_buffer)) {
        printf("dino_x2=%d, obs_x1=%d, dino_x1=%d, obs_x2=%d\n",
                 dino_coords.x2, obs_coords.x1, dino_coords.x1, obs_coords.x2);
        printf("dino_y2=%d, obs_y1=%d, dino_y1=%d, obs_y2=%d\n",
                 dino_coords.y2, obs_coords.y1, dino_coords.y1, obs_coords.y2);
        g_gs.game_over = 1;
        snprintf(buf, sizeof(buf), "GAME OVER: %d", g_gs.score);
        lv_label_set_text(g_score_label, buf);
    }
}

/**
 * @brief 处理游戏键盘输入
 * @param key 按下的键值
 * @return 无
 *
 * 处理游戏中的按键输入事件
 */
void dino_game_key_input(int key)
{
    if (!g_game_screen) return;

    if (key == KEY_UP) {
        if (!g_gs.game_over && g_gs.on_ground) {
            g_gs.dino_vy = DINO_JUMP_VY;
            g_gs.dino_vx = DINO_HORZ_VX;
            g_gs.on_ground = 0;
        }
    }
    // Exit game with ESC
    if (key == KEY_ESC) {
        dino_game_stop_and_cleanup();
    }
}

static void dino_game_event_cb(lv_event_t *e)
{
    if (!e) return;
    if (lv_event_get_code(e) == LV_EVENT_KEY) {
        int key = lv_event_get_key(e);
        printf("Key: %d\n", key);
        if (key == KEY_UP) {
            if (!g_gs.game_over && g_gs.on_ground) {
                g_gs.dino_vy = DINO_JUMP_VY;
                g_gs.dino_vx = DINO_HORZ_VX;
                g_gs.on_ground = 0;
            }
        }
        // Exit game with ESC
        if (key == KEY_ESC) {
            dino_game_stop_and_cleanup();
        }
    } else if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // Handle jumping with touch/click
        if (!g_gs.game_over && g_gs.on_ground) {
            g_gs.dino_vy = DINO_JUMP_VY;
            g_gs.dino_vx = DINO_HORZ_VX;
            g_gs.on_ground = 0;
        }
    }
}
