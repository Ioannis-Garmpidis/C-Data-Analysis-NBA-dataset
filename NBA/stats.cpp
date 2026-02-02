// stats.cpp
// Υλοποίηση βασικών στατιστικών και correlation matrix

#include "stats.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

// Υπολογισμός mean αγνοώντας NaN
static double mean_ignore_nan(const std::vector<double>& v, std::size_t& valid_count) {
    double sum = 0.0;
    valid_count = 0;

    for (double x : v) {
        if (!std::isnan(x)) {
            sum += x;
            valid_count++;
        }
    }
    return (valid_count > 0) ? (sum / valid_count) : std::numeric_limits<double>::quiet_NaN();
}

// Υπολογισμός median αγνοώντας NaN
static double median_ignore_nan(std::vector<double> v) {
    v.erase(std::remove_if(v.begin(), v.end(),
                           [](double x) { return std::isnan(x); }),
            v.end());

    if (v.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    std::sort(v.begin(), v.end());
    std::size_t n = v.size();

    if (n % 2 == 0) {
        return (v[n / 2 - 1] + v[n / 2]) / 2.0;
    }
    return v[n / 2];
}

// Υπολογισμός sample variance (N-1) αγνοώντας NaN
static double variance_ignore_nan(const std::vector<double>& v, double mean) {
    double acc = 0.0;
    std::size_t count = 0;

    for (double x : v) {
        if (!std::isnan(x)) {
            double d = x - mean;
            acc += d * d;
            count++;
        }
    }

    if (count < 2) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return acc / (count - 1);
}

// Υπολογισμός Pearson correlation για δύο στήλες
static double pearson_corr(const std::vector<double>& x,
                           const std::vector<double>& y) {
    double sum_x = 0.0, sum_y = 0.0;
    double sum_x2 = 0.0, sum_y2 = 0.0;
    double sum_xy = 0.0;
    std::size_t n = 0;

    for (std::size_t i = 0; i < x.size() && i < y.size(); i++) {
        if (!std::isnan(x[i]) && !std::isnan(y[i])) {
            sum_x += x[i];
            sum_y += y[i];
            sum_x2 += x[i] * x[i];
            sum_y2 += y[i] * y[i];
            sum_xy += x[i] * y[i];
            n++;
        }
    }

    if (n < 2) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double num = n * sum_xy - sum_x * sum_y;
    double den_x = n * sum_x2 - sum_x * sum_x;
    double den_y = n * sum_y2 - sum_y * sum_y;

    if (den_x <= 0.0 || den_y <= 0.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return num / std::sqrt(den_x * den_y);
}

StatsResult compute_basic_stats(const DataSet& ds) {
    StatsResult result;

    for (std::size_t c = 0; c < ds.columns.size(); c++) {
        ColumnStats stats;

        stats.mean = mean_ignore_nan(ds.columns[c], stats.valid_count);
        stats.median = median_ignore_nan(ds.columns[c]);
        stats.variance = variance_ignore_nan(ds.columns[c], stats.mean);

        result.per_column[ds.column_names[c]] = stats;
    }

    return result;
}

CorrelationResult compute_correlation_matrix(const DataSet& ds) {
    CorrelationResult corr;
    corr.column_names = ds.column_names;

    std::size_t n = ds.columns.size();
    corr.matrix.assign(n, std::vector<double>(n, 0.0));

    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = i; j < n; j++) {

            if (i == j) {
                corr.matrix[i][j] = 1.0;
            } else {
                double r = pearson_corr(ds.columns[i], ds.columns[j]);
                corr.matrix[i][j] = r;
                corr.matrix[j][i] = r;
            }
        }
    }

    return corr;
}