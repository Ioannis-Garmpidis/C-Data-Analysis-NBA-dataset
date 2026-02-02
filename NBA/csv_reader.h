// csv_reader.h
// Ανάγνωση CSV και φόρτωση raw (string) δεδομένων σε RawDataSet

#pragma once

#include <string>
#include <vector>
#include "types.h"

// Διαβάζει CSV αρχείο και επιστρέφει raw dataset
// - Χρησιμοποιεί header για ονόματα στηλών
// - Κρατά μόνο επιλεγμένες στήλες
// - Δεν κάνει μετατροπές τύπων (αυτές γίνονται στο cleaning)
RawDataSet read_csv_raw(
    const std::string& file_path,
    const std::vector<std::string>& selected_columns
);