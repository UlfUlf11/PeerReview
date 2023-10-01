enum class DateCheckFailReason {
    YEAR_TOO_SMALL = 0, 
    YEAR_TOO_BIG = 1,
    MONTH_TOO_SMALL = 2,
    MONTH_TOO_BIG = 3,
    DAY_TOO_SMALL = 4,
    DAY_TOO_BIG = 5,
    HOUR_TOO_SMALL = 6,
    HOUR_TOO_BIG = 7,
    MINUTE_TOO_SMALL = 8,
    MINUTE_TOO_BIG = 9,
    SECOND_TOO_SMALL = 10,
    SECOND_TOO_BIG = 11,
};


//Возвращаемое значение имеет две альтернативы — true или код ошибки.
std::variant<true, DateCheckFailReason> CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
        return static_cast<DateCheckFailReason>(0);
    }
    if (dt.year > 9999) {
        return static_cast<DateCheckFailReason>(1);
    }

    if (dt.month < 1) {
        return static_cast<DateCheckFailReason>(2);
    }
    if (dt.month > 12) {
        return static_cast<DateCheckFailReason>(3);
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (dt.day < 1) {
        return static_cast<DateCheckFailReason>(4);
    }
    if (dt.day > month_lengths[dt.month - 1]) {
        return static_cast<DateCheckFailReason>(5);
    }

    if (dt.hour < 0) {
        return static_cast<DateCheckFailReason>(6);
    }
    if (dt.hour > 23) {
        return static_cast<DateCheckFailReason>(7);
    }

    if (dt.minute < 0) {
        return static_cast<DateCheckFailReason>(8);
    }
    if (dt.minute > 59) {
        return static_cast<DateCheckFailReason>(9);
    }

    if (dt.second < 0) {
        return static_cast<DateCheckFailReason>(10);
    }
    if (dt.second > 59) {
        return static_cast<DateCheckFailReason>(11);
    }

    return true;
}