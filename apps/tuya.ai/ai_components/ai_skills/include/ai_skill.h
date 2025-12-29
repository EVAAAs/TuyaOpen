/**
 * @file ai_skill.h
 * @brief AI skill module header
 *
 * This header file defines the functions for processing AI skills such as
 * emotion, music, story, and play control skills.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __AI_SKILL_H__
#define __AI_SKILL_H__

#include "tuya_cloud_types.h"
#include "cJSON.h"
#include "skill_emotion.h"

#if defined(ENABLE_COMP_AI_AUDIO) && (ENABLE_COMP_AI_AUDIO == 1)
#include "skill_music_story.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
************************macro define************************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
********************function declaration********************
***********************************************************/
/* Skills examples: */
/* {"bizId":"asr-1741763201372","bizType":"ASR","eof":1,"data":{"text":"This is ASR text!"}} */
/* {"bizId":"nlg-1741763173046","bizType":"NLG","eof":0,"data":{"content":"This is NLG response text!","appendMode":"append","finish":false}} */
/* {"bizId":"skill-........emo","bizType":"SKILL","eof":1,"data":{"code":"emo","skillContent":{"emotion": ["sad", "happy"], "text":["😢","😀"]}}} */
/* {"bizId":"skill-......music","bizType":"SKILL","eof":1,"data":{"code":"music","skillContent":{"playList": [item1, item2]}}} */
/**
@brief Process AI skill data from JSON
@param root JSON root object containing skill data
@param eof End of file flag
@return OPERATE_RET Operation result
*/
OPERATE_RET ai_agent_skills_process(cJSON *root, bool eof);

#ifdef __cplusplus
}
#endif

#endif /* __AI_SKILL_H__ */
