// cleaning.h
// Cleaning raw δεδομένων: μετατροπή string→double, τυποποίηση missing/invalid και σύνοψη ποιότητας

#pragma once

#include <vector>
#include <string>
#include "types.h"

// Σύνοψη cleaning για να γραφτεί στο report
struct CleaningSummary {
    std::vector<std::string> column_names;   // Ονόματα στηλών (ίδια σειρά με το DataSet)
    std::vector<std::size_t> valid_count;    // Πλήθος έγκυρων τιμών ανά στήλη
    std::vector<std::size_t> missing_count;  // Πλήθος missing/invalid (NaN) ανά στήλη
    std::size_t total_rows = 0;              // Σύνολο γραμμών (παίκτες)
};

// Μετατρέπει RawDataSet σε numeric DataSet
// - Missing tokens: "", "NULL", "N/A", "NA" (case-insensitive) -> NaN
// - Μη-αριθμητικά -> NaN
// - +/-inf -> NaN
// - Δεν αφαιρεί γραμμές/παίκτες (cleaning ανά κελί)
DataSet clean_to_numeric(const RawDataSet& raw, CleaningSummary& summary);