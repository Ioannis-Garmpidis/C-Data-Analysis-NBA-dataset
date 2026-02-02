// reporting.h
// Εξαγωγή αποτελεσμάτων σε αρχεία (results.txt, timing_report.txt)

#pragma once

#include <string>
#include "types.h"
#include "cleaning.h"

// Γράφει τα στατιστικά και τον πίνακα συσχέτισης σε αρχείο κειμένου
// - Περιλαμβάνει και σύνοψη cleaning (valid/missing ανά στήλη)
bool write_results_txt(const std::string& file_path,
                       const CleaningSummary& cleaning,
                       const StatsResult& stats,
                       const CorrelationResult& corr);

// Γράφει τους χρόνους εκτέλεσης ανά στάδιο σε αρχείο κειμένου
bool write_timing_report_txt(const std::string& file_path,
                             const TimingResult& timing);