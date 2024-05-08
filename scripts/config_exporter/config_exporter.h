/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __DUMP_H__
#define __DUMP_H__

#include <config.h>
#include <ccan/json/json.h>
#include <stdio.h>

/**
 * @brief Convert a Config struct to a JSON object.
 * 
 * @param config The Config struct to convert.
 * @return The JSON object.
 */
struct JsonNode *config_to_json(struct config *config);

/**
 * @brief Dump the contents of a Config struct to a file.
 * 
 * @param config The Config struct to dump.
 * @param path The path to the file to dump to.
 * @param minimize Whether to minimize the JSON output.
 */
void dump_config(struct config *config, FILE *file, bool minimize);

#endif //__DUMP_H__