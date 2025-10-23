#include "cJSON.h"
#include "helpers.h"
#include "puzzle_state.h"
#include <microhttpd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Variables that front ends needs access to:
	- hive
	- required letter
	- solved words (solvedList->words)
	- user userwords (userWordList->words)
	- user score
	- user pangrams
	- user perfect pangrams
	- user bingo

Games initlization should provide JSON response of:

*/

char *parseUserWord(const char *json_str) {
	cJSON *root = cJSON_Parse(json_str);
	if (root == NULL) {
		return strdup("{\"error\": \"Invalid JSON\"}");
	}
	cJSON *word_node = cJSON_GetObjectItem(root, "word");
	if (!cJSON_IsString(word_node)) {
		cJSON_Delete(root);
		return strdup("{\"error\": \"Missing or invalid 'word' field\"}");
	}
	const char *word = word_node->valuestring;

	cJSON *response = cJSON_CreateObject();
	getResponse(word, response);

	char *responseStr = cJSON_PrintUnformatted(response);

	cJSON_Delete(root);
	cJSON_Delete(response);

	return strdup(responseStr);
}

char *parseHiveOptions(const char *json_str) {
	cJSON *root = cJSON_Parse(json_str);

	cJSON *size_node = cJSON_GetObjectItem(root, "hive-size");

	if (!cJSON_IsNumber(size_node)) {
		cJSON_Delete(root);
		return strdup("{\"error\": \"Missing or invalid 'hive-size' field\"}");
	}

	int hiveSize = size_node->valueint;

	cJSON *response = cJSON_CreateObject();

	getHiveResponse(hiveSize, response);

	char *responseStr = cJSON_PrintUnformatted(response);

	cJSON_Delete(root);
	cJSON_Delete(response);

	return strdup(responseStr);
}

char *processWord(const char *json_str, const Puzzle *puzzle) {
	cJSON *root = cJSON_Parse(json_str);
	if (root == NULL) {
		return strdup("{\"error\": \"Invalid JSON\"}");
	}
	cJSON *word_node = cJSON_GetObjectItem(root, "word");
	if (!cJSON_IsString(word_node)) {
		cJSON_Delete(root);
		return strdup("{\"error\": \"Missing or invalid 'word' field\"}");
	}
	const char *word = word_node->valuestring;
	printf("Frontend submitted %s\n", word);
	getResponse(char *word, cJSON *response);

	char *responseStr = cJSON_PrintUnformatted(response);

	cJSON_Delete(root);
	cJSON_Delete(response);

	return responseStr;
}
