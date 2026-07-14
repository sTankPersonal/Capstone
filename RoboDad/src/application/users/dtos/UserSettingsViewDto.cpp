#include "application/users/dtos/UserSettingsViewDto.h"

UserSettingsViewDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"] = id;
    if (firstName)             result["firstName"]            = *firstName;
    if (lastName)              result["lastName"]             = *lastName;
    if (dateOfBirth)           result["dateOfBirth"]          = *dateOfBirth;
    if (countryName)           result["countryName"]          = *countryName;
    if (currencyName)          result["currencyName"]         = *currencyName;
    if (languageName)          result["languageName"]         = *languageName;
    if (employmentStatusName)  result["employmentStatusName"] = *employmentStatusName;
    return result;
}
