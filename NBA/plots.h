// plots.h
// Δημιουργία γραφημάτων (PNG) από τα αποτελέσματα ανάλυσης

#pragma once

#include <string>
#include "types.h"

// Παράγει βασικά γραφήματα και τα αποθηκεύει ως PNG
// - Bar chart: μέσοι όροι βασικών metrics
// - Heatmap: correlation matrix
bool generate_plots(const std::string& output_dir,
                    const DataSet& ds,
                    const StatsResult& stats,
                    const CorrelationResult& corr);