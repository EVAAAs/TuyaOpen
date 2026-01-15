/**
 * @file skill_emotion.h
 * @brief Emotion skill module header
 *
 * This header file defines the types and functions for parsing and playing
 * emotion expressions from AI skill responses.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __SKILL_EMOTION_H__
#define __SKILL_EMOTION_H__

#include "tuya_cloud_types.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/
#define EMOJI_NEUTRAL      "NEUTRAL"
#define EMOJI_SAD          "SAD"
#define EMOJI_ANGRY        "ANGRY"
#define EMOJI_SURPRISE     "SURPRISE"
#define EMOJI_CONFUSED     "CONFUSED"
#define EMOJI_THINKING     "THINKING"
#define EMOJI_HAPPY        "HAPPY"
#define EMOJI_TOUCH        "TOUCH"
#define EMOJI_FEARFUL      "FEARFUL"
#define EMOJI_DISAPPOINTED "DISAPPOINTED"
#define EMOJI_ANNOYED      "ANNOYED"
#define EMOJI_SLEEP        "SLEEP"
#define EMOJI_WAKEUP       "WAKEUP"
#define EMOJI_LEFT         "LEFT"
#define EMOJI_RIGHT        "RIGHT"
#define EMOJI_WINK         "WINK"

/***********************************************************
***********************typedef define***********************
***********************************************************/
typedef struct {
    uint8_t emo_cnt;
    char **text;
    char **emotion;
} AI_AGENT_EMO_T;


/***********************************************************
********************function declaration********************
***********************************************************/
/**
@brief Parse emotion data from JSON
@param json JSON object containing emotion data
@param emo Pointer to store parsed emotion structure
@return OPERATE_RET Operation result
*/
OPERATE_RET ai_agent_parse_emo(cJSON *json, AI_AGENT_EMO_T **emo);

/**
@brief Play emotion expression
@param emo Pointer to emotion structure
@return OPERATE_RET Operation result
*/
OPERATE_RET ai_agent_play_emo(AI_AGENT_EMO_T *emo);

/**
@brief Free emotion structure memory
@param emo Pointer to emotion structure to free
@return None
*/
void ai_agent_parse_emo_free(AI_AGENT_EMO_T *emo);


#ifdef __cplusplus
}
#endif

#endif /* __SKILL_EMOTION_H__ */
