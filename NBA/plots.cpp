// plots.cpp
// Παραγωγή γραφημάτων μέσω gnuplot (PNG) 

#include "plots.h"

#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

// Γράφει bar chart μέσων όρων
static void write_mean_bar_plot(const std::string& dir, const StatsResult& stats) {
    // -------- Chart A: Counts --------
    {
        std::ofstream dat(dir + "/mean_counts.dat");
        dat << "metric value\n";
        for (const auto& c : {"pts", "reb", "ast", "gp"}) {
            auto it = stats.per_column.find(c);
            if (it != stats.per_column.end())
                dat << c << " " << it->second.mean << "\n";
        }
        dat.close();

        std::ofstream gp(dir + "/mean_counts.gp");
        gp <<
            "set terminal pngcairo size 900,600\n"
            "set output '" << dir << "/mean_counts.png'\n"
            "set title 'Μέσοι όροι (counts)'\n"
            "set style data histograms\n"
            "set style fill solid 0.7\n"
            "set boxwidth 0.6\n"
            "set grid ytics\n"
            "plot '" << dir << "/mean_counts.dat' using 2:xtic(1) title ''\n";
        gp.close();

        std::system(("gnuplot " + dir + "/mean_counts.gp").c_str());
    }

    // -------- Chart B: Rates --------
    {
        std::ofstream dat(dir + "/mean_rates.dat");
        dat << "metric value\n";
        for (const auto& c : {"usg_pct", "ts_pct"}) {
            auto it = stats.per_column.find(c);
            if (it != stats.per_column.end())
                dat << c << " " << it->second.mean << "\n";
        }
        dat.close();

        std::ofstream gp(dir + "/mean_rates.gp");
        gp <<
            "set terminal pngcairo size 600,500\n"
            "set output '" << dir << "/mean_rates.png'\n"
            "set title 'Μέσοι όροι (rates)'\n"
            "set style data histograms\n"
            "set style fill solid 0.7\n"
            "set boxwidth 0.5\n"
            "set grid ytics\n"
            "plot '" << dir << "/mean_rates.dat' using 2:xtic(1) title ''\n";
        gp.close();

        std::system(("gnuplot " + dir + "/mean_rates.gp").c_str());
    }
}

// Γράφει heatmap συσχέτισης
static void write_corr_heatmap(const std::string& dir, const CorrelationResult& corr) {
 // Επιλεγμένες στήλες (performance metrics)
std::vector<std::string> selected = {
    "pts", "usg_pct", "ts_pct", "ast", "ast_pct", "reb"
};

// Βρίσκει indices των επιλεγμένων στηλών
std::vector<size_t> idx;
for (const auto& name : selected) {
    auto it = std::find(corr.column_names.begin(),
                        corr.column_names.end(),
                        name);
    if (it != corr.column_names.end()) {
        idx.push_back(std::distance(corr.column_names.begin(), it));
    }
}

// Γράφει μειωμένο correlation matrix
std::ofstream dat(dir + "/corr.dat");
for (size_t i = 0; i < idx.size(); i++) {
    for (size_t j = 0; j < idx.size(); j++) {
        dat << i << " " << j
            << " " << corr.matrix[idx[i]][idx[j]] << "\n";
    }
    dat << "\n";
}
dat.close();

    // Script gnuplot για καθαρό heatmap
    std::ofstream gp(dir + "/corr.gp");
    gp <<
        "set terminal pngcairo size 1200,1000\n"
        "set output '" << dir << "/corr_heatmap.png'\n"
        "set title 'Correlation Heatmap'\n"
        "unset key\n"
        "set view map\n"
        "set size ratio -1\n"
        "set border lw 1\n"
        "set tics out\n"
        "set grid front lc rgb '#dddddd'\n"
        "set cbrange [-1:1]\n"
        "set palette defined (-1 '#2b6cb0', 0 'white', 1 '#c53030')\n"
        "set cbtics 0.5\n"
        "set xrange [-0.5:" << (corr.column_names.size() - 0.5) << "]\n"
        "set yrange [-0.5:" << (corr.column_names.size() - 0.5) << "]\n"
        "set xtics rotate by 45 right\n"
        "set xtics 0,1\n"
        "set ytics 0,1\n"
        "set xlabel ''\n"
        "set ylabel ''\n"
        "set pm3d map\n"
        "set pm3d interpolate 2,2\n"
        "splot '" << dir << "/corr.dat' using 1:2:3 with pm3d\n";
    gp.close();

    std::system(("gnuplot " + dir + "/corr.gp").c_str());
}

// Κεντρική συνάρτηση plots (αυτή καλεί το main.cpp)
bool generate_plots(const std::string& output_dir,
                    const DataSet& ds,
                    const StatsResult& stats,
                    const CorrelationResult& corr) {
    std::filesystem::create_directories(output_dir);

    // Κρατάμε τα bar charts
    write_mean_bar_plot(output_dir, stats);

    // Scatter plots (pairwise με νόημα)
    // pts vs usg_pct
    {
        std::ofstream dat(output_dir + "/pts_vs_usg.dat");
        size_t ix_pts = std::distance(ds.column_names.begin(),
                        std::find(ds.column_names.begin(), ds.column_names.end(), "pts"));
        size_t ix_usg = std::distance(ds.column_names.begin(),
                        std::find(ds.column_names.begin(), ds.column_names.end(), "usg_pct"));

        for (size_t i = 0; i < ds.row_count; i++) {
            double x = ds.columns[ix_usg][i];
            double y = ds.columns[ix_pts][i];
            if (!std::isnan(x) && !std::isnan(y))
                dat << x << " " << y << "\n";
        }
        dat.close();

        std::ofstream gp(output_dir + "/pts_vs_usg.gp");
        gp <<
            "set terminal pngcairo size 800,600\n"
            "set output '" << output_dir << "/pts_vs_usg.png'\n"
            "set title 'PTS vs USG%'\n"
            "set xlabel 'USG%'\n"
            "set ylabel 'Points'\n"
            "set grid\n"
            "plot '" << output_dir << "/pts_vs_usg.dat' using 1:2 notitle with points pt 7 ps 0.5\n";
        gp.close();

        std::system(("gnuplot " + output_dir + "/pts_vs_usg.gp").c_str());
    }

    // ast vs ast_pct
    {
        std::ofstream dat(output_dir + "/ast_vs_astpct.dat");
        size_t ix_ast = std::distance(ds.column_names.begin(),
                        std::find(ds.column_names.begin(), ds.column_names.end(), "ast"));
        size_t ix_ap  = std::distance(ds.column_names.begin(),
                        std::find(ds.column_names.begin(), ds.column_names.end(), "ast_pct"));

        for (size_t i = 0; i < ds.row_count; i++) {
            double x = ds.columns[ix_ap][i];
            double y = ds.columns[ix_ast][i];
            if (!std::isnan(x) && !std::isnan(y))
                dat << x << " " << y << "\n";
        }
        dat.close();

        std::ofstream gp(output_dir + "/ast_vs_astpct.gp");
        gp <<
            "set terminal pngcairo size 800,600\n"
            "set output '" << output_dir << "/ast_vs_astpct.png'\n"
            "set title 'AST vs AST%'\n"
            "set xlabel 'AST%'\n"
            "set ylabel 'Assists'\n"
            "set grid\n"
            "plot '" << output_dir << "/ast_vs_astpct.dat' using 1:2 notitle with points pt 7 ps 0.5\n";
        gp.close();

        std::system(("gnuplot " + output_dir + "/ast_vs_astpct.gp").c_str());
    }

    return true;
}