#include "Utils.h"

// Print a String with a fixed length
//
String StringPad(String Str, int Length, char Filler) {
    int StrLen = Str.length();
    String Result = "";
    int AdditionalCharacters = 0;
    if (Length < 0) {
        AdditionalCharacters = -Length - StrLen;
        while (--AdditionalCharacters >= 0) {
            Result += Filler;
        }
    }
    Result += Str;
    if (Length > 0) {
        AdditionalCharacters = Length - StrLen;
        while (--AdditionalCharacters >= 0) {
            Result += Filler;
        }
    }
    return (Result);
}
