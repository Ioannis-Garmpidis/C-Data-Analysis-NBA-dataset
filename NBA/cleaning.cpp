// cleaning.cpp
// Υλοποίηση cleaning: κανόνες missing/invalid και μετατροπή raw tokens σε numeric τιμές

#include "cleaning.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <string>

// Αφαιρεί κενά από αρχή/τέλος (trim)
static std::string trim_copy(const std::string& s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;

    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;

    return s.substr(start, end - start);
}

// Μετατρέπει σε UPPER για εύκολους ελέγχους tokens
static std::string upper_copy(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return s;
}

// Επιστρέφει true αν το token πρέπει να θεωρηθεί missing/invalid
static bool is_missing_token(const std::string& raw) {
    std::string t = upper_copy(trim_copy(raw));
    return t.empty() || t == "NULL" || t == "N/A" || t == "NA";
}

// Μετατρέπει string σε double, αλλιώς επιστρέφει NaN
static double parse_double_or_nan(const std::string& raw) {
    if (is_missing_token(raw)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    std::string t = trim_copy(raw);
    try {
        std::size_t pos = 0;
        double v = std::stod(t, &pos);

        // Αν μείνουν χαρακτήρες που δεν είναι κενά, το θεωρούμε invalid
        for (std::size_t i = pos; i < t.size(); i++) {
            if (!std::isspace(static_cast<unsigned char>(t[i]))) {
                return std::numeric_limits<double>::quiet_NaN();
            }
        }

        // Τυποποίηση μη-πεπερασμένων τιμών
        if (std::isinf(v) || std::isnan(v)) {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return v;
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

DataSet clean_to_numeric(const RawDataSet& raw, CleaningSummary& summary) {
    DataSet ds;
    ds.column_names = raw.column_names;
    ds.columns.assign(raw.columns.size(), {});
    ds.row_count = raw.row_count;

    summary.column_names = raw.column_names;
    summary.total_rows = raw.row_count;
    summary.valid_count.assign(raw.columns.size(), 0);
    summary.missing_count.assign(raw.columns.size(), 0);

    // Μετατροπή ανά στήλη (ίδια πλήθος γραμμών/παίκτες)
    for (std::size_t c = 0; c < raw.columns.size(); c++) {
        ds.columns[c].reserve(raw.columns[c].size());

        for (const std::string& token : raw.columns[c]) {
            double v = parse_double_or_nan(token);

            if (std::isnan(v)) {
                summary.missing_count[c]++;
            } else {
                summary.valid_count[c]++;
            }

            ds.columns[c].push_back(v);
        }
    }

    return ds;
}