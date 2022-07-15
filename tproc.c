#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

static const char OUTPUT_FILE_1[] = "text3.txt";
static const char OUTPUT_FILE_2[] = "text4.txt";

int TextProcessingFcn(int *lentxt1, int *matches, char *matchASCII,
		char *input1, char *input2, int *i1type, int *i2type);
void ResultPrintFcn(int *matches, int *lentxt1), InputTypeFcn(int *i1type,
		int *i2type, char *input1, char *input2);

int main() {

	int lentxt1 = 0, matches = 0, i1type, i2type;
	char matchASCII[256] = { 0 }, input1[100], input2[100];

	InputTypeFcn(&i1type, &i2type, input1, input2);
	TextProcessingFcn(&lentxt1, &matches, matchASCII, input1, input2, &i1type,
			&i2type);
	ResultPrintFcn(&matches, &lentxt1);

	return 0;
}

void InputTypeFcn(int *i1type, int *i2type, char *input1, char *input2) {
	int i, link1match = 0, text1match = 0, link2match = 0, text2match = 0;
	char https[8] = { 'h', 't', 't', 'p', 's', ':', '/', '/' }, txt[4] = { '.',
			't', 'x', 't' };

	fgets(input1, 100, stdin);
	input1[strlen(input1) - 1] = '\0';

	fgets(input2, 100, stdin);
	input2[strlen(input2) - 1] = '\0';

	for (i = 0; i < 100; i++) {
		if (input1[i] == https[i] && i < 8) {
			link1match += 1;
		} else if (input1[strlen(input1) - i] == txt[4 - i] && i < 4) {
			text1match += 1;
		}

		if (input2[i] == https[i] && i < 8) {
			link2match += 1;
		} else if (input2[strlen(input2) - i] == txt[4 - i] && i < 4) {
			text2match += 1;
		}
	}

	if (link1match == 8) {
		*i1type = 1;
		printf("\nFIRST INPUT: INTERNET\n");
	} else if (text1match == 4) {
		*i1type = 2;
		printf("\nFIRST INPUT: TEXT FILE\n");
	} else {
		*i1type = 3;
		printf("\nFIRST INPUT: CONSOLE\n");
	}

	if (link2match == 8) {
		*i2type = 1;
		printf("SECOND INPUT: INTERNET\n\n");
	} else if (text2match == 4) {
		*i2type = 2;
		printf("SECOND INPUT: TEXT FILE\n\n");
	} else {
		*i2type = 3;
		printf("SECOND INPUT: CONSOLE\n\n");
	}
}

int TextProcessingFcn(int *lentxt1, int *matches, char *matchASCII,
		char *input1, char *input2, int *i1type, int *i2type) {
	FILE *text1;
	FILE *text2;
	CURL *curl;

	int c;

	if (*i1type == 1) {
		text1 = fopen(OUTPUT_FILE_1, "wb");

		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, input1);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, text1);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(text1);

		text1 = fopen(OUTPUT_FILE_1, "r");
	} else if (*i1type == 2) {
		text1 = fopen(input1, "r");
	} else if (*i1type == 3) {
		text1 = fopen(OUTPUT_FILE_1, "w");

		fputs(input1, text1);
		fclose(text1);

		text1 = fopen(OUTPUT_FILE_1, "r");
	}

	if (*i2type == 1) {
		text2 = fopen(OUTPUT_FILE_2, "wb");

		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, input2);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, text2);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(text2);

		text2 = fopen(OUTPUT_FILE_2, "r");
	} else if (*i2type == 2) {
		text2 = fopen(input2, "r");
	} else if (*i2type == 3) {
		text2 = fopen(OUTPUT_FILE_2, "w");

		fputs(input2, text2);
		fclose(text2);

		text2 = fopen(OUTPUT_FILE_2, "r");
	}

	if (text1 == NULL || text2 == NULL) {
		printf("Error.\n");
		return -1;
	}

	while ((c = getc(text2)) != EOF) {
		matchASCII[c] = 1;
	}

	while ((c = getc(text1)) != EOF) {
		*lentxt1 += 1;
		if (matchASCII[c] == 1) {
			*matches += 1;
		}
	}

	fclose(text1);
	fclose(text2);

	return 0;
}

// fcn for displaying results
void ResultPrintFcn(int *matches, int *lentxt1) {
	// calculating the percentage of text 1 that can be created using only chars from text 2
	double percentage = (double) (*matches * 100) / *lentxt1;

	// displaying the results after both texts have been processed
	printf("Matches: %d \n", (int) *matches);
	printf("Number of characters found in first input source: %d \n", *lentxt1);

	printf(
			"The percentage of text 1 that can be created using only characters from"
					" text 2 is: %.2f%% \n", percentage);
}
