/**
 * @file ai_chat_main.h
 * @brief AI chat main module header
 *
 * This header file defines the types and functions for the AI chat main module,
 * which manages the overall AI chat functionality including mode configuration
 * and volume control.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_AI_CHAT_MODE_H__
#define __TUYA_AI_CHAT_MODE_H__

#include "tuya_cloud_types.h"
#include "tal_mutex.h"

#if defined(ENABLE_BUTTON) && (ENABLE_BUTTON == 1)
#include "tdl_button_manage.h"
#endif

#if defined(ENABLE_COMP_AI_AUDIO) && (ENABLE_COMP_AI_AUDIO == 1)
#include "ai_audio_player.h"
#endif

#include "ai_user_event.h"
#include "ai_manage_mode.h"
#include "ai_mode_hold.h"
#include "ai_mode_oneshot.h"
#include "ai_mode_wakeup.h"
#include "ai_mode_free.h"


#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/
typedef struct {
    AI_USER_EVENT_NOTIFY  evt_cb;
}AI_CHAT_MODE_CFG_T;


/***********************************************************
********************function declaration********************
***********************************************************/
/**
@brief Initialize AI chat module
@param cfg Chat mode configuration
@return OPERATE_RET Operation result
*/
OPERATE_RET ai_chat_init(AI_CHAT_MODE_CFG_T *cfg);

/**
@brief Set chat volume
@param volume Volume value (0-100)
@return OPERATE_RET Operation result
*/
OPERATE_RET ai_chat_set_volume(int volume);

/**
@brief Get chat volume
@return int Volume value (0-100)
*/
int ai_chat_get_volume(void);

#ifdef __cplusplus
}
#endif

#endif /* __TUYA_AI_CHAT_MODE_H__ */