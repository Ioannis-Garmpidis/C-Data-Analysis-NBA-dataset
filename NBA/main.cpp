// main.cpp
// Κεντρικό entry point: εκτελεί όλο το pipeline ανάλυσης δεδομένων

#include <iostream>
#include <vector>

#include "types.h"
#include "timing.h"
#include "csv_reader.h"
#include "cleaning.h"
#include "stats.h"
#include "reporting.h"
#include "plots.h"

int main() {

    // -------- Ρυθμίσεις --------
    const std::string input_csv = "/Users/ioannisgarmpidis/Desktop/csv files/all_seasons.csv";
    const std::string results_txt = "results.txt";
    const std::string timing_txt  = "timing_report.txt";

    // Numeric στήλες που θα αναλυθούν
    std::vector<std::string> numeric_columns = {
        "age", "player_height", "player_weight",
        "gp", "pts", "reb", "ast",
        "net_rating", "oreb_pct", "dreb_pct",
        "usg_pct", "ts_pct", "ast_pct"
    };

    TimingResult timing;

    // -------- 1) Load CSV --------
    RawDataSet raw;
    {
        ScopedTimer t(timing.load_ms);
        raw = read_csv_raw(input_csv, numeric_columns);
    }

    // -------- 2) Cleaning --------
    DataSet ds;
    CleaningSummary cleaning;
    {
        ScopedTimer t(timing.clean_ms);
        ds = clean_to_numeric(raw, cleaning);
    }

    // -------- 3) Στατιστικά --------
    StatsResult stats;
    CorrelationResult corr;
    {
        ScopedTimer t(timing.stats_ms);
        stats = compute_basic_stats(ds);
        corr  = compute_correlation_matrix(ds);
    }

    // -------- 4) Export αποτελεσμάτων --------
    {
        ScopedTimer t(timing.export_ms);
        write_results_txt(results_txt, cleaning, stats, corr);
        write_timing_report_txt(timing_txt, timing);
    }

   // -------- 5) Plots --------
{
    ScopedTimer t(timing.plots_ms);
   generate_plots("charts", ds, stats, corr);
}

    std::cout << "Η ανάλυση ολοκληρώθηκε επιτυχώς.\n";
    std::cout << "Αποτελέσματα: " << results_txt << "\n";
    std::cout << "Χρόνοι: " << timing_txt << "\n";

    return 0;
}