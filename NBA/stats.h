// stats.h
// Υπολογισμός στατιστικών (mean/median/variance) και συσχέτισης (correlation matrix)

#pragma once

#include "types.h"

// Υπολογίζει mean/median/variance για κάθε numeric στήλη
// - Αγνοεί missing τιμές (NaN)
// - Επιστρέφει StatsResult με valid_count ανά στήλη
StatsResult compute_basic_stats(const DataSet& ds);

// Υπολογίζει correlation matrix για όλα τα ζεύγη numeric στηλών
// - Pairwise deletion: για κάθε ζεύγος χρησιμοποιεί μόνο γραμμές όπου και οι 2 τιμές είναι έγκυρες
// - Επιστρέφει NxN πίνακα, με 1.0 στη διαγώνιο όταν υπάρχει αρκετό data
CorrelationResult compute_correlation_matrix(const DataSet& ds);