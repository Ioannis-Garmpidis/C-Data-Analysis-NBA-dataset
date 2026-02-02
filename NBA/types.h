// types.h
// Κοινές δομές δεδομένων που χρησιμοποιούνται σε όλο το pipeline ανάλυσης

#pragma once

#include <string>
#include <vector>
#include <unordered_map>


// Αποθηκεύει raw (string) δεδομένα όπως διαβάστηκαν από το CSV
struct RawDataSet {
    std::vector<std::string> column_names;              // Ονόματα επιλεγμένων στηλών
    std::vector<std::vector<std::string>> columns;      // Raw τιμές ανά στήλη (string tokens)
    std::size_t row_count = 0;                          // Συνολικός αριθμός γραμμών (παίκτες)
};

// Αποθηκεύει το numeric dataset μετά το διάβασμα CSV και το cleaning
struct DataSet {
    std::vector<std::string> column_names;        // Ονόματα numeric στηλών που αναλύονται
    std::vector<std::vector<double>> columns;     // Τιμές ανά στήλη (NaN = missing/invalid)
    std::size_t row_count = 0;                    // Συνολικός αριθμός γραμμών (παίκτες)
};

// Στατιστικά μεγέθη για μία numeric στήλη
struct ColumnStats {
    double mean = 0.0;                            // Αριθμητικός μέσος όρος
    double median = 0.0;                          // Διάμεσος
    double variance = 0.0;                        // Διακύμανση δείγματος (N-1)
    std::size_t valid_count = 0;                  // Πλήθος έγκυρων (μη-missing) τιμών
};

// Συγκεντρωτικά στατιστικά για όλες τις numeric στήλες
struct StatsResult {
    std::unordered_map<std::string, ColumnStats> per_column; // Στατιστικά ανά όνομα στήλης
};

// Αποτελέσματα συσχέτισης (correlation) μεταξύ όλων των numeric στηλών
struct CorrelationResult {
    std::vector<std::string> column_names;        // Σειρά στηλών όπως εμφανίζονται στον πίνακα
    std::vector<std::vector<double>> matrix;      // Πίνακας συσχέτισης NxN
};

// Χρόνοι εκτέλεσης για κάθε στάδιο του pipeline
struct TimingResult {
    double load_ms = 0.0;                         // Χρόνος φόρτωσης CSV
    double clean_ms = 0.0;                        // Χρόνος cleaning δεδομένων
    double stats_ms = 0.0;                        // Χρόνος υπολογισμού στατιστικών
    double export_ms = 0.0;                       // Χρόνος εξαγωγής αποτελεσμάτων
    double plots_ms = 0.0;                        // Χρόνος δημιουργίας γραφημάτων
};