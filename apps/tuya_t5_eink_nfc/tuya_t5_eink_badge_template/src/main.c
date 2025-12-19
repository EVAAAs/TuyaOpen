/**
 * @file main.c
 * @brief Main application entry point for badge template
 *
 * This is a template application for developers to start their projects.
 * It provides:
 * - Hardware initialization (SD card, buttons, display)
 * - LVGL initialization
 * - Button input as LVGL keypad (6 buttons: UP, DOWN, LEFT, RIGHT, ENTER, RETURN)
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */

#include "tuya_cloud_types.h"
#include "tal_api.h"
#include "tal_log.h"
#include "tkl_output.h"
#include "app_hardware.h"
#include "app_lvgl.h"
#include "app_input.h"
#include "lv_vendor.h"
#include <stdio.h>
#include <stdbool.h>

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/***********************************************************
************************macro define************************
***********************************************************/

/***********************************************************
***********************typedef define***********************
***********************************************************/

/***********************************************************
***********************variable define**********************
***********************************************************/
static lv_obj_t *sg_key_status_label = NULL;

/***********************************************************
********************function declaration********************
***********************************************************/
static void create_demo_screen(void);
static void update_key_status_text(const char *key_name, bool pressed);

/***********************************************************
***********************function define**********************
***********************************************************/

/**
 * @brief Update key status label text
 */
static void update_key_status_text(const char *key_name, bool pressed)
{
    if (sg_key_status_label == NULL) {
        return;
    }

    char status_text[64];
    if (pressed) {
        snprintf(status_text, sizeof(status_text), "Key: %s\nStatus: PRESSED", key_name);
    } else {
        snprintf(status_text, sizeof(status_text), "Key: %s\nStatus: RELEASED", key_name);
    }

    // Lock display before updating UI (thread safety)
    lv_vendor_disp_lock();
    lv_label_set_text(sg_key_status_label, status_text);
    lv_vendor_disp_unlock();
}

/**
 * @brief Create a simple demo screen to test LVGL and input
 */
static void create_demo_screen(void)
{
    // Create a title label
    lv_obj_t *title_label = lv_label_create(lv_scr_act());
    lv_label_set_text(title_label, "Badge Template");
    lv_obj_set_style_text_align(title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    // Create key status label that will be updated on button press/release
    sg_key_status_label = lv_label_create(lv_scr_act());
    lv_label_set_text(sg_key_status_label, "Key: None\nStatus: RELEASED");
    lv_obj_set_style_text_align(sg_key_status_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(sg_key_status_label);

    // Create instruction label
    lv_obj_t *info_label = lv_label_create(lv_scr_act());
    lv_label_set_text(info_label, "Buttons:\nUP/DOWN/LEFT/RIGHT\nENTER/RETURN");
    lv_obj_set_style_text_align(info_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(info_label, LV_ALIGN_BOTTOM_MID, 0, -20);

    PR_NOTICE("Demo screen created");
}

/**
 * @brief Main application initialization
 */
void user_main(void)
{
    OPERATE_RET rt = OPRT_OK;

    // Initialize logging
    tal_log_init(TAL_LOG_LEVEL_DEBUG, 1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);

    PR_NOTICE("========================================");
    PR_NOTICE("Tuya T5 E-Ink Badge Template");
    PR_NOTICE("========================================");
    PR_NOTICE("Project: %s", PROJECT_NAME);
    PR_NOTICE("Version: %s", PROJECT_VERSION);
    PR_NOTICE("Board: %s", PLATFORM_BOARD);

    // Initialize hardware (buttons, LED, display, SD card)
    rt = app_hardware_init();
    if (OPRT_OK != rt) {
        PR_ERR("Hardware initialization failed: %d", rt);
        return;
    }

    // Initialize LVGL display
    rt = app_lvgl_init();
    if (OPRT_OK != rt) {
        PR_ERR("LVGL initialization failed: %d", rt);
        return;
    }

    // Initialize button input as LVGL keypad
    // Note: This must be called after lv_vendor_init() but before lv_vendor_start()
    rt = app_input_init();
    if (OPRT_OK != rt) {
        PR_ERR("Input initialization failed: %d", rt);
        return;
    }

    // Register callback for key status updates
    app_input_set_status_callback(update_key_status_text);

    // Lock display before creating UI elements (thread safety)
    lv_vendor_disp_lock();

    // Create a simple demo screen
    create_demo_screen();

    // Unlock display after UI creation
    lv_vendor_disp_unlock();

    // Start LVGL task handler (this runs lv_task_handler() in a separate thread)
    app_lvgl_start(4, 4096);

    PR_NOTICE("Badge template initialized successfully");
    PR_NOTICE("You can now start building your application!");

    // Main loop - keep thread alive
    // LVGL runs in its own thread via lv_vendor_start(), but we keep this thread alive
    while (1) {
        tal_system_sleep(1000);
    }
}

/**
 * @brief Main entry point (Linux)
 */
#if OPERATING_SYSTEM == SYSTEM_LINUX
void main(int argc, char *argv[])
{
    user_main();

    while (1) {
        tal_system_sleep(500);
    }
}
#else

/* Tuya thread handle */
static THREAD_HANDLE ty_app_thread = NULL;

/**
 * @brief Application thread
 */
static void tuya_app_thread(void *arg)
{
    (void)arg;
    user_main();

    tal_thread_delete(ty_app_thread);
    ty_app_thread = NULL;
}

/**
 * @brief Tuya application main entry point
 */
void tuya_app_main(void)
{
    THREAD_CFG_T thrd_param = {4096, 4, "tuya_app_main"};
    tal_thread_create_and_start(&ty_app_thread, NULL, NULL, tuya_app_thread, NULL, &thrd_param);
}
#endif
