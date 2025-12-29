/**
 * @file ai_skill.c
 * @brief AI skill module implementation
 *
 * This module implements AI skill processing, including emotion skills,
 * music/story skills, and play control skills.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_cloud_types.h"

#include "tal_api.h"
#include "cJSON.h"

#include "ai_user_event.h"
#include "skill_emotion.h"

#if defined(ENABLE_COMP_AI_AUDIO) && (ENABLE_COMP_AI_AUDIO == 1)
#include "ai_audio_player.h"
#include "skill_music_story.h"
#endif

#include "ai_skill.h"

/***********************************************************
************************macro define************************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/


/***********************************************************
***********************function define**********************
***********************************************************/
/**
@brief Process AI skill data from JSON
@param root JSON root object containing skill data
@param eof End of file flag
@return OPERATE_RET Operation result
*/
OPERATE_RET ai_agent_skills_process(cJSON *root, bool eof)
{
    OPERATE_RET rt = OPRT_OK;
    const cJSON *node = NULL;
    const char *code = NULL;

    /* Root is data:{}, parse code */
    node = cJSON_GetObjectItem(root, "code");
    code = cJSON_GetStringValue(node);
    if (!code) 
        return OPRT_OK;

    PR_NOTICE("text -> skill code: %s", code);
    if (strcmp(code, "emo") == 0 || strcmp(code, "llm_emo") == 0) {
        AI_AGENT_EMO_T *emo = NULL;
        if (ai_agent_parse_emo(root, &emo) == OPRT_OK) {
            ai_agent_play_emo(emo);
            ai_agent_parse_emo_free(emo);
        }
    } 
#if defined(ENABLE_COMP_AI_AUDIO) && (ENABLE_COMP_AI_AUDIO == 1)
    else if (strcmp(code, "music") == 0 ||
               strcmp(code, "story") == 0) {
        AI_AUDIO_MUSIC_T *music = NULL;
        if (ai_skill_parse_music(root, &music) == OPRT_OK) {
            ai_skill_parse_music_dump(music);
            ai_audio_play_music(music);
            ai_skill_parse_music_free(music);
        }
    } else if (strcmp(code, "PlayControl") == 0) {
        AI_AUDIO_MUSIC_T *music = NULL;
        if ((rt = ai_skill_parse_playcontrol(root, &music)) == 0) {
            ai_skill_parse_music_dump(music);
            ai_skill_playcontrol_music(music);
            ai_skill_parse_music_free(music);
        }
    }
#endif
    else if (strcmp(code, "alert") == 0) {
        /* TUYA_CALL_ERR_LOG(wukong_ai_parse_clock(root)); */
    }else {
        PR_NOTICE("skill %s not handled", code);
        /* PR_NOTICE("skill content %s ", cJSON_PrintUnformatted(root)); */

        ai_user_event_notify(AI_USER_EVT_SKILL, root);
    }

    return rt; 
}