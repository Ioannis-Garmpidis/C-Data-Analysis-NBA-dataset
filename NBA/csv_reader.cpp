// csv_reader.cpp
// Ανάγνωση CSV με header και φόρτωση επιλεγμένων στηλών σε RawDataSet

#include "csv_reader.h"

#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>

// Αφαιρεί κενά από αρχή/τέλος (trim)
static std::string trim_copy(const std::string& s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;

    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;

    return s.substr(start, end - start);
}

// Βασικό split CSV γραμμής με υποστήριξη για quotes
// - Χειρίζεται πεδία τύπου "a,b" ως ένα πεδίο
static std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> out;
    std::string cur;
    bool in_quotes = false;

    for (std::size_t i = 0; i < line.size(); i++) {
        char c = line[i];

        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }

        if (c == ',' && !in_quotes) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

RawDataSet read_csv_raw(const std::string& file_path,
                        const std::vector<std::string>& selected_columns) {
    RawDataSet raw;
    raw.column_names = selected_columns;
    raw.columns.assign(selected_columns.size(), {});
    raw.row_count = 0;

    std::ifstream in(file_path);
    if (!in.is_open()) {
        // Αν δεν ανοίξει το αρχείο, επιστρέφει κενό dataset
        return raw;
    }

    std::string header_line;
    if (!std::getline(in, header_line)) {
        // Κενό αρχείο
        return raw;
    }

    // Χαρτογράφηση: όνομα στήλης -> index στο CSV
    std::vector<std::string> header = split_csv_line(header_line);
    std::unordered_map<std::string, std::size_t> name_to_index;
    name_to_index.reserve(header.size());

    for (std::size_t i = 0; i < header.size(); i++) {
        std::string col = trim_copy(header[i]);
        name_to_index[col] = i;
    }

    // Βρίσκει indices για τις στήλες που μας ενδιαφέρουν
    std::vector<std::size_t> selected_idx(selected_columns.size(),
                                          static_cast<std::size_t>(-1));

    for (std::size_t i = 0; i < selected_columns.size(); i++) {
        auto it = name_to_index.find(selected_columns[i]);
        if (it != name_to_index.end()) {
            selected_idx[i] = it->second;
        }
    }

    // Διαβάζει γραμμές δεδομένων
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields = split_csv_line(line);

        // Για κάθε επιλεγμένη στήλη, αποθηκεύει raw token (trimmed)
        for (std::size_t c = 0; c < selected_columns.size(); c++) {
            std::size_t idx = selected_idx[c];

            if (idx == static_cast<std::size_t>(-1) || idx >= fields.size()) {
                raw.columns[c].push_back(""); // λείπει πεδίο -> κενό token
                continue;
            }

            raw.columns[c].push_back(trim_copy(fields[idx]));
        }

        raw.row_count++;
    }

    return raw;
}