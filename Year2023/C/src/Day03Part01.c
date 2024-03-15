#include "LibThomas.h"
#include "stdlib.h"
#include <assert.h>
#include <string.h>
#include <strings.h>
#include "stdio.h"

typedef struct _Gear
{
    i32 SymbolPosition;
    i32 Number;
} Gear;

bool IsSymbol(char input)
{
    // Can't use a single range because dot (.) is NOT a symbol and is at 46
    // Can't do "an anything not 46"-check either because we might be next to another number...
    if (input > 31 && input < 46) return true;
    if (input == 47) return true;
    if (input > 57 && input < 65) return true;

    return false;
}

int main(void)
{
    StringArray *InputParts = File_ReadAllLines("Input/03.txt");

    // TEST DATA:
    //char* TestInput = "467..114..\n...*......\n..35..633.\n......#...\n617*......\n.....+.58.\n..592.....\n......755.\n...$.*....\n.664.598..";
    //String *TestString = String_Make(TestInput, 109);
    //StringArray *InputParts = String_Split(TestString, '\n');
    //String_Free(TestString);

    if (InputParts == NULL) return EXIT_FAILURE;
    assert(InputParts->Count == 140);

    i32 Part1Answer = 0;
    i32 Part2Answer = 0;
    Gear Gears[361*2] = {0};

    for(i32 LineIndex = 0; LineIndex < InputParts->Count; LineIndex++)
    {
        String *CurrentLine = InputParts->Contents[LineIndex];
        char* CurrentString = CurrentLine->Content;

        String* LinesToCheck[3] = {0};
        if (LineIndex > 0) LinesToCheck[0] = InputParts->Contents[LineIndex - 1];
        LinesToCheck[1] = CurrentLine;
        if (LineIndex < InputParts->Count - 1) LinesToCheck[2] = InputParts->Contents[LineIndex + 1];

        for(i32 StringIndex = 0; StringIndex < CurrentLine->Size;)
        {
            if (CurrentString[StringIndex] < 48 || CurrentString[StringIndex] > 57)
            {
                StringIndex++;
                continue;
            }

            char NumberBuffer[4] = {0};
            memcpy(&NumberBuffer, &CurrentString[StringIndex], 3);

            i32 Number = atoi(NumberBuffer);
            i32 DigitsToCheck = 0;
            i32 NumberStartPosition = StringIndex;
            i32 SymbolPositionsLeftToCheck = 0;

            if (Number < 10)
            {
                DigitsToCheck = 3;
                StringIndex += 2;
            }
            else if (Number < 100)
            {
                DigitsToCheck = 4;
                StringIndex += 3;
            }
            else
            {
                DigitsToCheck = 5;
                StringIndex += 4;
            }

            // To account for numbers against the edges. In that case check one less digit (column)
            if (NumberStartPosition == 0 || (CurrentLine->Size - DigitsToCheck) < DigitsToCheck)
            {
                DigitsToCheck--;
            }

            i32 LinesToCheckIndex = 0;
            while(LinesToCheckIndex < 3)
            {
                String* LineToCheck = LinesToCheck[LinesToCheckIndex];
                if (LineToCheck == NULL)
                {
                    LinesToCheckIndex++;
                    continue;
                }

                bool SymbolFound = false;
                SymbolPositionsLeftToCheck = DigitsToCheck;
                i32 SymbolCheckIndex = NumberStartPosition == 0 ? NumberStartPosition : NumberStartPosition -1;

                while(SymbolPositionsLeftToCheck > 0)
                {
                    char PotentialSymbol = LineToCheck->Content[SymbolCheckIndex];
                    SymbolFound = IsSymbol(PotentialSymbol);
                    if (SymbolFound)
                    {
                        Part1Answer += Number;
                        break;
                    }
                    SymbolCheckIndex++;
                    SymbolPositionsLeftToCheck--;
                }

                if (SymbolFound) break;
                LinesToCheckIndex++;
            }
        }
    }

    printf("Part 1 answer: %i", Part1Answer);
    assert(Part1Answer == 538046);

    return EXIT_SUCCESS;
}
