#include "card.h"

void getCardHolderNameTest(void);
void getCardPANTest(void);
void getCardExpiryDateTest(void);


EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	*user_input = ' ';
	printf("Enter the name of the cardholder:\n");
	fgets(user_input, sizeof(user_input), stdin);
	int temp = strlen(user_input);
	if (user_input[temp - 1] == '\n') user_input[temp - 1] = 0;
	//checking user input for any non alphabetic chars other than the whitespace
	char alpha_check = 0;
	for (int i = 0; i < strlen(user_input); i++) {
		if (isalpha(user_input[i]) == 0) {
			if (isspace(user_input[i]) == 0) {
				alpha_check = 1;
				break;
			}
		}
	}
	//checking name length and non NULL input
	if (strlen(user_input) > 24 || strlen(user_input) < 20 || user_input == NULL || alpha_check) {
		return WRONG_NAME;
	}
	else
	{
		strcpy_s(cardData->cardHolderName, sizeof(cardData->cardHolderName), user_input);
		return CARD_OK;
	}
}

void getCardHolderNameTest(void) {
	ST_cardData_t test;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		switch (getCardHolderName(&test)) {
		case 0:
			strcpy_s(result, 30, "Passed");
			break;
		case 1:
			strcpy_s(result, 30, "Fail");
			break;
		default:
			strcpy_s(result, 30, "undefined Error");
			break;
		}
		printf("Tester Name:%sFunction Name: getCardHolderName\nTest case %d:\nInput Data:%s\nExpected result:%sActual result:%s\n"
			,tester_name, i, user_input, expected_case,result);
	}
}




EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	*user_input = ' ';
	printf("Enter card Expiry Date:\n");
	fgets(user_input, sizeof(user_input), stdin);

	//remove newline char from the user input to get correct string length
	int temp = strlen(user_input);
	if (user_input[temp - 1] == '\n') user_input[temp - 1] = 0;
	//length check
	if (strlen(user_input) != 5) {
		return WRONG_EXP_DATE;
	}
	//format check
	for (int i = 0; i < 5; i++) {
		if (isdigit(user_input[i]) == 0 && i != 2) {
			return WRONG_EXP_DATE;
		}
		if (i == 2 && user_input[i] != 47) {
			return WRONG_EXP_DATE;
		}
	}
	
	strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), user_input);
	return CARD_OK;
}

void getCardExpiryDateTest(void) {
	ST_cardData_t test;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		switch (getCardExpiryDate(&test)) {
		case 0:
			strcpy_s(result, 30, "PASS");
			break;
		case 2:
			strcpy_s(result, 30, "FAIL");
			break;
		default:
			strcpy_s(result, 30, "undefined Error");
			break;
		}
		printf("Tester Name :%sFunction Name: getCardExpiryDate \nTest case %d:\nInput Data:%s\nExpected result:%sActual result: %s\n-----------------\n"
			, tester_name, i, user_input, expected_case, result);
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	*user_input = ' ';
	printf("Enter card number:\n");
	fgets(user_input, sizeof(user_input), stdin);
	int temp = strlen(user_input);
	if (user_input[temp - 1] == '\n') user_input[temp - 1] = 0;
	//length check
	if (strlen(user_input) > 19 || strlen(user_input) < 16 || user_input == NULL) {
		return WRONG_PAN;
	}
	//numeric chars check
	for (int i = 0; i < strlen(user_input); i++) {
		if (isdigit(user_input[i]) == 0) {
			if (isspace(user_input[i]) == 0 ) {
				
				return WRONG_PAN;
			}
		}
	}
	strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), user_input);
	return CARD_OK;
	}

void getCardPANTest(void) {
	ST_cardData_t test;
	uint8_t tester_name[50];
	uint8_t expected_case[50];
	char result[30];
	printf("Enter your name:\n");
	fgets(tester_name, sizeof(tester_name), stdin);
	for (int i = 1; i < 6; i++) {
		printf("Enter expected result:\n");
		fgets(expected_case, sizeof(expected_case), stdin);
		switch(getCardPAN(&test)) {
		case 0:
			strcpy_s(result, 30, "PASS");
			break;
		case 3:
			strcpy_s(result, 30, "FAIL");
			break;
		default:
			strcpy_s(result, 30, "undefined Error");
			break;
		}
		printf("Tester Name :%sFunction Name: getCardPAN \nTest case %d:\nInput Data:%s\nExpected result : %sActual result: %s\n-----------------\n"
			, tester_name, i, user_input, expected_case, result);
	}
}