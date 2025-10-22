#include "cJSON.h"
#include "puzzle_state.h"
#include <microhttpd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	bool valid == (isValid(word, puzzle))

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

char *getMessage(int validResult, int foundResult) {
	if (result == -3) {
		return strdup("INVALID: Word must be at least 4 letters");
	} else if (result == -2) {
		return strdup("INVALID: Word must contain the required letter");
	} else if (result == -1) {
		return strdup("INVALID: Word contains one or more letters not in the current hive");
	} else if (foundResult < 0) {
		return strdup("INVALID: Word is not in the current dictionary");
	} else {
		return strdup("");
	}
}
