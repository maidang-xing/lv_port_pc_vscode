/**
 * @file page_manager.h
 * @brief Defines the structures and public APIs for a stack-based page management system.
 *
 * This manager treats each screen as a 'Page' object and handles the navigation
 * logic, including initialization, de-initialization, and transitions between pages.
 * This pattern is adapted from the OV-Watch project.
 */
#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "lvgl.h"

#define MAX_PAGE_DEPTH 10

// Forward declaration
struct Page_t;

/**
 * @struct Page_t
 * @brief Represents a single UI page (screen).
 *
 * This structure holds function pointers to manage the lifecycle of a page,
 * a pointer to the main LVGL object for that page, and other metadata.
 */
typedef struct Page_t {
    void (*init)(struct Page_t *page, void *user_data);
    void (*deinit)(struct Page_t *page);
    void (*input_handler)(uint32_t key);
    lv_obj_t *page_obj;
    const char* name; // For debugging
    void* user_data;  // To pass data to the page
} Page_t;


void PageManager_init(Page_t* home_page);
void PageManager_load(Page_t* new_page);
void PageManager_load_with_data(Page_t* new_page, void* user_data);
void PageManager_back(void);
void PageManager_handle_input(uint32_t key);

#endif // PAGE_MANAGER_H
