/**
 * @file page_manager.c
 * @brief Implements the logic for the stack-based page management system.
 */
#include "page_manager.h"
#include <stdio.h>

// The stack to manage the navigation history of pages.
typedef struct {
    Page_t* pages[MAX_PAGE_DEPTH];
    uint8_t top;
} PageStack_t;

static PageStack_t g_page_stack;

// Internal stack operations

static void page_stack_init(PageStack_t* stack) {
    stack->top = 0;
}

static int page_stack_push(PageStack_t* stack, Page_t* page) {
    if (stack->top >= MAX_PAGE_DEPTH) {
        printf("Page stack overflow!\n");
        return -1;
    }
    stack->pages[stack->top++] = page;
    return 0;
}

static Page_t* page_stack_pop(PageStack_t* stack) {
    if (stack->top == 0) {
        printf("Page stack underflow!\n");
        return NULL;
    }
    return stack->pages[--stack->top];
}

static Page_t* page_stack_peek(PageStack_t* stack) {
    if (stack->top == 0) {
        return NULL;
    }
    return stack->pages[stack->top - 1];
}

// Public API Implementations

void PageManager_init(Page_t* home_page) {
    page_stack_init(&g_page_stack);
    page_stack_push(&g_page_stack, home_page);
    if (home_page->init) {
        home_page->init(home_page, NULL);
    }
    if (home_page->page_obj) {
        lv_disp_load_scr(home_page->page_obj);
    }
}

void PageManager_load_with_data(Page_t* new_page, void* user_data) {
    if (!new_page) return;

    Page_t* current_page = page_stack_peek(&g_page_stack);
    if (current_page && current_page->deinit) {
        current_page->deinit(current_page);
    }

    if (page_stack_push(&g_page_stack, new_page) == 0) {
        new_page->user_data = user_data;
        if (new_page->init) {
            new_page->init(new_page, user_data);
        }
        if (new_page->page_obj) {
            lv_scr_load_anim(new_page->page_obj, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0, false);
        }
    }
}

void PageManager_load(Page_t* new_page) {
    PageManager_load_with_data(new_page, NULL);
}

void PageManager_back(void) {
    if (g_page_stack.top <= 1) { // Can't go back from home page
        return;
    }

    Page_t* current_page = page_stack_pop(&g_page_stack);
    if (current_page && current_page->deinit) {
        current_page->deinit(current_page);
    }

    Page_t* previous_page = page_stack_peek(&g_page_stack);
    if (previous_page) {
        // Re-initialize the previous page to restore its state
        if (previous_page->init) {
            previous_page->init(previous_page, previous_page->user_data);
        }
        if(previous_page->page_obj) {
            lv_scr_load_anim(previous_page->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0, false);
        }
    }
}

void PageManager_handle_input(uint32_t key) {
    Page_t* current_page = page_stack_peek(&g_page_stack);
    if (current_page && current_page->input_handler) {
        current_page->input_handler(key);
    }
}
