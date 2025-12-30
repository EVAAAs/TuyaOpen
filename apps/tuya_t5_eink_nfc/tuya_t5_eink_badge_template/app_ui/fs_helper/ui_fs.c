/**
 * @file ui_fs.c
 * @brief ui_fs module is used to
 * @version 0.1
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */

#include "ui.h"

#include "tal_api.h"

#include "tkl_fs.h"

#include <stdio.h>
#include <stdlib.h>

/***********************************************************
************************macro define************************
***********************************************************/
#define UI_FS_MALLOC(size) tal_psram_malloc(size)
#define UI_FS_FREE(ptr) tal_psram_free(ptr)

#define SDCARD_MOUNT_PATH "/sdcard"

/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
********************function declaration********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/


/***********************************************************
***********************function define**********************
***********************************************************/
void ui_fs_init(void)
{
    // int rt = tkl_fs_mount(SDCARD_MOUNT_PATH, DEV_SDCARD);
    // if (rt != 0) {
    //     PR_ERR("Failed to mount SD card: %d", rt);
    //     return;
    // }

    return;
}

const char *ui_fs_file_type_to_string(ui_fs_file_type_t type)
{
    switch (type) {
        case UI_FS_FILE_TYPE_DIRECTORY:
            return "DIRECTORY";
        case UI_FS_FILE_TYPE_FILE_TEXT:
            return "FILE_TEXT";
        case UI_FS_FILE_TYPE_FILE_PNG:
            return "FILE_PNG";
        case UI_FS_FILE_TYPE_FILE_EPUB:
            return "FILE_EPUB";
        case UI_FS_FILE_TYPE_UNKNOWN:
            return "UNKNOWN";
        default:
            return "UNKNOWN";
    }
}

int ui_fs_file_list_destroy(ui_fs_file_list_t *file_list)
{
    if (file_list == NULL) {
        return 0;
    }

    ui_fs_file_node_t *node = file_list->head;
    while (node != NULL) {
        ui_fs_file_node_t *next = node->next;
        UI_FS_FREE(node);
        node = next;
    }

    UI_FS_FREE(file_list);

    return 0;
}

// get file list
int ui_fs_get_file_list(const char *path, ui_fs_file_list_t **file_list_p)
{
    ui_fs_file_list_t *file_list = NULL;
    ui_fs_file_node_t *node = NULL;

    PR_DEBUG("ui_fs_get_file_list path: [%s]", path);

    if (path == NULL || file_list_p == NULL) {
        return -1;
    }

    TUYA_DIR dir;
    int rt = tkl_dir_open(path, &dir);
    if (rt != 0 || dir == NULL) {
        return -1;
    }

    file_list = UI_FS_MALLOC(sizeof(ui_fs_file_list_t));
    if (file_list == NULL) {
        tkl_dir_close(dir);
        return -1;
    }
    file_list->head = NULL;
    file_list->tail = NULL;
    file_list->node_count = 0;

    TUYA_FILEINFO info;
    while (tkl_dir_read(dir, &info) == 0) {
        const char *name = NULL;
        rt = tkl_dir_name(info, &name);
        if (rt != 0 || name == NULL) {
            continue;
        }

        // Skip . and .. entries
        if (name[0] == '.') {
            continue;
        }

        PR_DEBUG("ui_fs_get_file_list name: [%s]", name);

        node = UI_FS_MALLOC(sizeof(ui_fs_file_node_t));
        if (node == NULL) {
            goto __ERR;
        }
        memset(node->name, 0, UI_FS_FILE_LIST_MAX_SIZE);
        strncpy(node->name, name, UI_FS_FILE_LIST_MAX_SIZE - 1);
        node->name[UI_FS_FILE_LIST_MAX_SIZE - 1] = '\0';

        // set file type
        BOOL_T is_dir = FALSE;
        if (tkl_dir_is_directory(info, &is_dir) == 0 && is_dir) {
            node->type = UI_FS_FILE_TYPE_DIRECTORY;
        } else {
            // Check file extension (from the end of the filename)
            size_t name_len = strlen(name);
            if (name_len > 4 && strncmp(name + name_len - 4, ".txt", 4) == 0) {
                node->type = UI_FS_FILE_TYPE_FILE_TEXT;
            } else if (name_len > 4 && strncmp(name + name_len - 4, ".png", 4) == 0) {
                node->type = UI_FS_FILE_TYPE_FILE_PNG;
            } else if (name_len > 5 && strncmp(name + name_len - 5, ".epub", 5) == 0) {
                node->type = UI_FS_FILE_TYPE_FILE_EPUB;
            } else {
                node->type = UI_FS_FILE_TYPE_UNKNOWN;
            }
        }

        // set file size
        if (is_dir) {
            node->size = 0;
        } else {
            // Build full file path to get size
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, name);
            int file_size = tkl_fgetsize(full_path);
            node->size = (file_size >= 0) ? file_size : 0;
        }

        // set user data
        node->user_data = NULL;

        // set next node
        node->next = NULL;

        // add node to file list
        if (file_list->head == NULL) {
            file_list->head = node;
        } else {
            file_list->tail->next = node;
        }
        file_list->tail = node;
        file_list->node_count++;
    }

    tkl_dir_close(dir);
    *file_list_p = file_list;

    // DEBUG
    PR_DEBUG("--- file list count: [%d] ---", file_list->node_count);
    node = file_list->head;
    while (node != NULL) {
        // Format file size for better readability
        char size_str[32];
        if (node->type == UI_FS_FILE_TYPE_DIRECTORY) {
            snprintf(size_str, sizeof(size_str), "-");
        } else if (node->size < 1024) {
            snprintf(size_str, sizeof(size_str), "%u B", node->size);
        } else if (node->size < 1024 * 1024) {
            snprintf(size_str, sizeof(size_str), "%.2f KB", node->size / 1024.0);
        } else {
            snprintf(size_str, sizeof(size_str), "%.2f MB", node->size / (1024.0 * 1024.0));
        }

        PR_DEBUG("file name: [%s], file size: [%s], file type: [%s]",
                    node->name, size_str, ui_fs_file_type_to_string(node->type));
        node = node->next;
    }

    return 0;

__ERR:
    ui_fs_file_list_destroy(file_list);

    return -1;
}

/**
 * @brief Sort file list by type first, then by name
 *
 * Sorting rules:
 * 1. Sort by file type (DIRECTORY < FILE_PNG < FILE_TEXT < FILE_EPUB < UNKNOWN)
 * 2. When file type is the same, sort by file name (case-insensitive alphabetical order)
 *
 * @param file_list Pointer to the file list to be sorted
 * @return 0 on success, -1 on failure
 */
int ui_fs_file_list_sort(ui_fs_file_list_t *file_list)
{
    if (file_list == NULL) {
        return -1;
    }

    // Nothing to sort if less than 2 nodes
    if (file_list->node_count < 2) {
        return 0;
    }

    // Use insertion sort algorithm to sort the linked list
    ui_fs_file_node_t *sorted = NULL;  // Head of sorted list
    ui_fs_file_node_t *node = file_list->head;

    while (node != NULL) {
        ui_fs_file_node_t *next = (ui_fs_file_node_t *)node->next;
        node->next = NULL;

        // Insert node into sorted list
        if (sorted == NULL) {
            // First node in sorted list
            sorted = node;
        } else {
            ui_fs_file_node_t *prev = NULL;
            ui_fs_file_node_t *curr = sorted;

            // Find the correct position to insert
            while (curr != NULL) {
                // Compare by file type first
                if (node->type < curr->type) {
                    break;
                } else if (node->type == curr->type) {
                    // Same type, compare by name (case-insensitive)
                    int cmp_result = strcasecmp(node->name, curr->name);
                    if (cmp_result < 0) {
                        break;
                    } else if (cmp_result == 0) {
                        // Names are equal (case-insensitive), use case-sensitive comparison for stability
                        if (strcmp(node->name, curr->name) < 0) {
                            break;
                        }
                    }
                }
                prev = curr;
                curr = (ui_fs_file_node_t *)curr->next;
            }

            if (prev == NULL) {
                // Insert at the beginning
                node->next = sorted;
                sorted = node;
            } else {
                // Insert after prev
                node->next = prev->next;
                prev->next = node;
            }
        }

        node = next;
    }

    // Update the file list head and tail
    file_list->head = sorted;

    // Find the tail by traversing to the last node
    ui_fs_file_node_t *tail = sorted;
    while (tail != NULL && tail->next != NULL) {
        tail = (ui_fs_file_node_t *)tail->next;
    }
    file_list->tail = tail;

    return 0;
}

ui_fs_file_node_t *ui_fs_file_list_get_node_by_index(ui_fs_file_list_t *file_list, uint32_t index)
{
    if (file_list == NULL || index >= file_list->node_count) {
        return NULL;
    }

    ui_fs_file_node_t *node = file_list->head;
    for (uint32_t i = 0; i < index; i++) {
        node = node->next;
    }
    return node;
}

int ui_fs_read_file(const char *path, uint32_t offset, void *buf, uint32_t size)
{
    (void) offset;

    if (path == NULL || buf == NULL || size == 0) {
        return -1;
    }

    return 0;

// __ERR:
//     return -1;
}
