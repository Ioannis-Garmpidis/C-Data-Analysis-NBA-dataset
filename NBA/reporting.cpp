// reporting.cpp
// Υλοποίηση εξαγωγής αποτελεσμάτων σε αρχεία κειμένου

#include "reporting.h"

#include <fstream>
#include <iomanip>
#include <limits>
#include <cmath>

// Γράφει double με σταθερή μορφή ή "NaN" αν λείπει
static void write_value(std::ostream& os, double v, int precision = 4) {
    if (std::isnan(v)) {
        os << "NaN";
    } else {
        os << std::fixed << std::setprecision(precision) << v;
    }
}

bool write_results_txt(const std::string& file_path,
                       const CleaningSummary& cleaning,
                       const StatsResult& stats,
                       const CorrelationResult& corr) {
    std::ofstream out(file_path);
    if (!out.is_open()) return false;

    out << "ΑΠΟΤΕΛΕΣΜΑΤΑ ΑΝΑΛΥΣΗΣ\n";
    out << "===================\n\n";

    // Σύνοψη cleaning
    out << "1) ΣΥΝΟΨΗ CLEANING\n";
    out << "-----------------\n";
    out << "Σύνολο παικτών (γραμμές): " << cleaning.total_rows << "\n\n";
    out << std::left << std::setw(18) << "Στήλη"
        << std::right << std::setw(12) << "Valid"
        << std::right << std::setw(12) << "Missing"
        << "\n";
    out << std::string(42, '-') << "\n";

    for (std::size_t i = 0; i < cleaning.column_names.size(); i++) {
        out << std::left << std::setw(18) << cleaning.column_names[i]
            << std::right << std::setw(12) << cleaning.valid_count[i]
            << std::right << std::setw(12) << cleaning.missing_count[i]
            << "\n";
    }
    out << "\n";

    // Βασικά στατιστικά
    out << "2) ΒΑΣΙΚΑ ΣΤΑΤΙΣΤΙΚΑ (ανά numeric στήλη)\n";
    out << "--------------------------------------\n";
    out << std::left << std::setw(18) << "Στήλη"
        << std::right << std::setw(12) << "Mean"
        << std::right << std::setw(12) << "Median"
        << std::right << std::setw(12) << "Variance"
        << std::right << std::setw(12) << "N(valid)"
        << "\n";
    out << std::string(66, '-') << "\n";

    for (const auto& col : cleaning.column_names) {
        auto it = stats.per_column.find(col);
        if (it == stats.per_column.end()) continue;

        const ColumnStats& s = it->second;

        out << std::left << std::setw(18) << col;

        out << std::right << std::setw(12);
        write_value(out, s.mean);

        out << std::right << std::setw(12);
        write_value(out, s.median);

        out << std::right << std::setw(12);
        write_value(out, s.variance);

        out << std::right << std::setw(12) << s.valid_count << "\n";
    }
    out << "\n";

    // Correlation matrix
    out << "3) ΠΙΝΑΚΑΣ ΣΥΣΧΕΤΙΣΗΣ (Correlation Matrix)\n";
    out << "-----------------------------------------\n";
    out << "Pearson correlation για όλα τα ζεύγη στηλών (pairwise, αγνοεί NaN).\n\n";

    // Header γραμμή (ονόματα στηλών)
    out << std::left << std::setw(10) << "";
    for (const auto& name : corr.column_names) {
        out << std::right << std::setw(10) << name.substr(0, 9);
    }
    out << "\n";

    // Γραμμές πίνακα
    for (std::size_t i = 0; i < corr.column_names.size(); i++) {
        out << std::left << std::setw(10) << corr.column_names[i].substr(0, 9);
        for (std::size_t j = 0; j < corr.column_names.size(); j++) {
            out << std::right << std::setw(10);
            write_value(out, corr.matrix[i][j], 3);
        }
        out << "\n";
    }

    out << "\n";
    return true;
}

bool write_timing_report_txt(const std::string& file_path,
                             const TimingResult& timing) {
    std::ofstream out(file_path);
    if (!out.is_open()) return false;

    out << "TIMING REPORT\n";
    out << "=============\n\n";

    out << std::left << std::setw(18) << "Load (ms):"   << timing.load_ms   << "\n";
    out << std::left << std::setw(18) << "Clean (ms):"  << timing.clean_ms  << "\n";
    out << std::left << std::setw(18) << "Stats (ms):"  << timing.stats_ms  << "\n";
    out << std::left << std::setw(18) << "Export (ms):" << timing.export_ms << "\n";
    out << std::left << std::setw(18) << "Plots (ms):"  << timing.plots_ms  << "\n\n";

    out << "Total (ms): "
        << (timing.load_ms + timing.clean_ms + timing.stats_ms + timing.export_ms + timing.plots_ms)
        << "\n";

    return true;
}