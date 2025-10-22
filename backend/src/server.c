#include "cJSON.h"
#include "helpers.h"
#include "puzzle_state.h"
#include <microhttpd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	cJSON *reponse = cJSON_CreateObject();
	getResponse(word, response);
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
	if (checkWord)

		cJSON *response = cJSON_CreateObject();
	cJSON_AddBoolToObject(response, "valid", valid);
	cJSON_AddNumberToObject(response, "score", score);
	cJSON_AddStringToObject(response, "message", message);
	cJSON_AddBoolToObject(response, "word", word);

	char *responseStr = cJSON_PrintUnformatted(response);

	cJSON_Delete(root);
	cJSON_Delete(response);

	return responseStr;
}
