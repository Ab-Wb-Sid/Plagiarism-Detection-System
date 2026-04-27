#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <limits>
using namespace std;

// ------------------- ANSI Color Codes -------------------
const string RESET = "\033[0m";
const string RED = "\033[31m";      // Word-level (severity 1)
const string YELLOW = "\033[33m";   // Phrase-level (severity 2)
const string MAGENTA = "\033[35m";  // Sentence-level (severity 3)
const string BOLD_RED = "\033[1;31m";
const string BOLD_GREEN = "\033[1;32m";
const string BOLD_YELLOW = "\033[1;33m";
const string GREEN = "\033[32m";
const string CYAN = "\033[36m";
const string BLUE = "\033[34m";
const string BOLD_CYAN = "\033[1;36m";

// ------------------- Threshold Structure -------------------
struct ThresholdConfig {
    double lowThreshold;      // 0-15%: Original/Acceptable
    double moderateThreshold; // 15-40%: Low Similarity
    double highThreshold;     // 40-60%: Moderate Similarity
    double veryHighThreshold; // 60-85%: High Similarity
    // Above 85%: Very High Similarity/Potential Plagiarism

    ThresholdConfig() : lowThreshold(15.0), moderateThreshold(40.0),
        highThreshold(60.0), veryHighThreshold(85.0) {}

    ThresholdConfig(double low, double mod, double high, double veryHigh)
        : lowThreshold(low), moderateThreshold(mod),
        highThreshold(high), veryHighThreshold(veryHigh) {}
};

// ------------------- Severity Assessment -------------------
struct SeverityAssessment {
    string category;
    string color;
    string recommendation;
    bool flagForReview;
};

SeverityAssessment assessSimilarity(double similarityPercent, const ThresholdConfig& config) {
    SeverityAssessment assessment;

    if (similarityPercent < config.lowThreshold) {
        assessment.category = "ORIGINAL/ACCEPTABLE";
        assessment.color = BOLD_GREEN;
        assessment.recommendation = "Content appears original. No action needed.";
        assessment.flagForReview = false;
    }
    else if (similarityPercent < config.moderateThreshold) {
        assessment.category = "LOW SIMILARITY";
        assessment.color = GREEN;
        assessment.recommendation = "Minor similarities detected. Generally acceptable with proper citations.";
        assessment.flagForReview = false;
    }
    else if (similarityPercent < config.highThreshold) {
        assessment.category = "MODERATE SIMILARITY";
        assessment.color = YELLOW;
        assessment.recommendation = "Moderate similarities found. Review and ensure proper paraphrasing and citations.";
        assessment.flagForReview = true;
    }
    else if (similarityPercent < config.veryHighThreshold) {
        assessment.category = "HIGH SIMILARITY";
        assessment.color = BOLD_YELLOW;
        assessment.recommendation = "High similarity detected. Significant revision and proper attribution required.";
        assessment.flagForReview = true;
    }
    else {
        assessment.category = "CRITICAL - POTENTIAL PLAGIARISM";
        assessment.color = BOLD_RED;
        assessment.recommendation = "ALERT: Very high similarity! Immediate review and major revision needed.";
        assessment.flagForReview = true;
    }

    return assessment;
}

// ------------------- Display Visual Indicator -------------------
void displayProgressBar(double percent, const string& color) {
    int barWidth = 50;
    int pos = int(barWidth * (percent / 100.0));

    cout << color << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "#";
        else if (i == pos) cout << "#";
        else cout << "-";
    }
    cout << "] " << fixed << setprecision(2) << percent << "%" << RESET << "\n";
}

// ------------------- Generate Detailed Report -------------------
void generateReport(double similarityPercent, const SeverityAssessment& assessment,
    int wordMatches, int phraseMatches, int sentenceMatches,
    int totalTokens, const ThresholdConfig& config) {
    cout << "\n";
    cout << CYAN << "+====================================================================+\n";
    cout << "|           PLAGIARISM DETECTION ANALYSIS REPORT                     |\n";
    cout << "+====================================================================+" << RESET << "\n\n";

    // Similarity Score Section
    cout << BOLD_GREEN << "SIMILARITY SCORE:\n" << RESET;
    displayProgressBar(similarityPercent, assessment.color);
    cout << "\n";

    // Severity Assessment
    cout << BOLD_GREEN << "SEVERITY ASSESSMENT:\n" << RESET;
    cout << "Category: " << assessment.color << assessment.category << RESET << "\n";
    cout << "Status: ";
    if (assessment.flagForReview) {
        cout << BOLD_RED << "! FLAGGED FOR REVIEW" << RESET << "\n";
    }
    else {
        cout << BOLD_GREEN << "* ACCEPTABLE" << RESET << "\n";
    }
    cout << "\n";

    // Threshold Comparison
    cout << BOLD_GREEN << "THRESHOLD COMPARISON:\n" << RESET;
    cout << "+-----------------------------+-----------+----------+\n";
    cout << "| Threshold Level             | Range     | Status   |\n";
    cout << "+-----------------------------+-----------+----------+\n";

    auto printThresholdRow = [&](const string& name, double min, double max, bool passed) {
        cout << "| " << left << setw(27) << name << " | "
            << setw(9) << (to_string(int(min)) + "-" + to_string(int(max)) + "%") << " | ";
        if (passed) cout << GREEN << "  PASS  " << RESET;
        else cout << RED << "  FAIL  " << RESET;
        cout << " |\n";
        };

    printThresholdRow("Original/Acceptable", 0, config.lowThreshold,
        similarityPercent < config.lowThreshold);
    printThresholdRow("Low Similarity", config.lowThreshold, config.moderateThreshold,
        similarityPercent >= config.lowThreshold && similarityPercent < config.moderateThreshold);
    printThresholdRow("Moderate Similarity", config.moderateThreshold, config.highThreshold,
        similarityPercent >= config.moderateThreshold && similarityPercent < config.highThreshold);
    printThresholdRow("High Similarity", config.highThreshold, config.veryHighThreshold,
        similarityPercent >= config.highThreshold && similarityPercent < config.veryHighThreshold);
    printThresholdRow("Critical/Plagiarism", config.veryHighThreshold, 100.0,
        similarityPercent >= config.veryHighThreshold);

    cout << "+-----------------------------+-----------+----------+\n\n";

    // Match Statistics
    cout << BOLD_GREEN << "MATCH STATISTICS:\n" << RESET;
    cout << "Word-level matches:     " << RED << wordMatches << " tokens" << RESET << "\n";
    cout << "Phrase-level matches:   " << YELLOW << phraseMatches << " tokens" << RESET << "\n";
    cout << "Sentence-level matches: " << MAGENTA << sentenceMatches << " tokens" << RESET << "\n";
    cout << "Total tokens analyzed:  " << totalTokens << "\n";
    cout << "\n";

    // Recommendation
    cout << BOLD_GREEN << "RECOMMENDATION:\n" << RESET;
    cout << assessment.color << assessment.recommendation << RESET << "\n\n";

    // Additional Actions
    if (assessment.flagForReview) {
        cout << BOLD_YELLOW << "! REQUIRED ACTIONS:\n" << RESET;
        cout << "  * Manual review recommended\n";
        cout << "  * Check proper citations and attributions\n";
        cout << "  * Revise heavily matched sections\n";
        cout << "  * Consider paraphrasing flagged content\n\n";
    }

    cout << CYAN << "====================================================================\n" << RESET;
}

// ------------------- Utility: clean text (no regex) -------------------
string cleanText(const string& input) {
    string tmp;
    tmp.reserve(input.size());
    for (char c : input) {
        if (isalnum(static_cast<unsigned char>(c))) {
            tmp.push_back(char(tolower(static_cast<unsigned char>(c))));
        }
        else if (isspace(static_cast<unsigned char>(c))) {
            tmp.push_back(' ');
        }
        else {
            tmp.push_back(' ');
        }
    }
    string out;
    out.reserve(tmp.size());
    bool prevSpace = false;
    for (char c : tmp) {
        if (c == ' ') {
            if (!prevSpace) {
                out.push_back(' ');
                prevSpace = true;
            }
        }
        else {
            out.push_back(c);
            prevSpace = false;
        }
    }
    if (!out.empty() && out.front() == ' ') out.erase(out.begin());
    if (!out.empty() && out.back() == ' ') out.pop_back();
    return out;
}

// ------------------- Tokenize by splitting on spaces -------------------
vector<string> tokenizeBySpace(const string& s) {
    vector<string> tokens;
    string current;
    for (char c : s) {
        if (c == ' ') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
        else {
            current.push_back(c);
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

// ------------------- Simple stemming (lightweight) -------------------
string stemWord(const string& w) {
    if (w.size() > 4) {
        if (w.size() >= 3 && w.substr(w.size() - 3) == "ing") return w.substr(0, w.size() - 3);
        if (w.size() >= 2 && w.substr(w.size() - 2) == "ed")  return w.substr(0, w.size() - 2);
        if (w.back() == 's') return w.substr(0, w.size() - 1);
    }
    else if (w.size() > 3) {
        if (w.back() == 's') return w.substr(0, w.size() - 1);
    }
    return w;
}

vector<string> stemTokens(const vector<string>& tokens) {
    vector<string> out;
    out.reserve(tokens.size());
    for (const auto& t : tokens) out.push_back(stemWord(t));
    return out;
}

// ------------------- Stopword removal -------------------
unordered_set<string> makeStopwords() {
    return { "the","is","in","and","to","a","of","for","on","at","by","with","an","that","this","it","as","are","was","were","be", "any"};
}

vector<string> removeStopwords(const vector<string>& tokens, const unordered_set<string>& sw) {
    vector<string> out;
    out.reserve(tokens.size());
    for (const auto& t : tokens) {
        if (!sw.count(t)) out.push_back(t);
    }
    return out;
}

// ------------------- Cosine similarity (uses frequency of tokens) ----
double cosineSimilarity(const vector<string>& A, const vector<string>& B) {
    unordered_map<string, int> f1, f2;
    for (const auto& w : A) f1[w]++;
    for (const auto& w : B) f2[w]++;
    double dot = 0;
    for (const auto& p : f1) {
        auto it = f2.find(p.first);
        if (it != f2.end()) dot += double(p.second) * double(it->second);
    }
    double m1 = 0, m2 = 0;
    for (const auto& p : f1) m1 += double(p.second) * double(p.second);
    for (const auto& p : f2) m2 += double(p.second) * double(p.second);
    if (m1 == 0 || m2 == 0) return 0.0;
    return dot / (sqrt(m1) * sqrt(m2));
}

// ------------------- K-gram rolling hash (Karp-Rabin style) ----------
unordered_set<long long> getHashes(const vector<string>& tokens, int K) {
    unordered_set<long long> H;
    if (tokens.size() < (size_t)K) return H;
    const long long P = 1000003LL;
    const long long MOD = 1000000007LL;
    vector<long long> hv(tokens.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        hv[i] = (long long)(std::hash<string>()(tokens[i]) & 0x7fffffff);
    }
    long long power = 1;
    for (int i = 0; i < K - 1; ++i) power = (power * P) % MOD;
    long long cur = 0;
    for (int i = 0; i < K; ++i) {
        cur = ((cur * P) + hv[i]) % MOD;
    }
    H.insert(cur);
    for (size_t i = K; i < hv.size(); ++i) {
        cur = (cur - (hv[i - K] * power) % MOD + MOD) % MOD;
        cur = ((cur * P) + hv[i]) % MOD;
        H.insert(cur);
    }
    return H;
}

// ------------------- Get shingles (actual sequences) -----------------
vector<string> getShingles(const vector<string>& tokens, int K) {
    vector<string> shingles;
    if (tokens.size() < (size_t)K) return shingles;
    shingles.reserve(tokens.size() - K + 1);
    for (size_t i = 0; i + K <= tokens.size(); ++i) {
        string s = tokens[i];
        for (int j = 1; j < K; ++j) {
            s += " ";
            s += tokens[i + j];
        }
        shingles.push_back(s);
    }
    return shingles;
}

// ------------------- Matched shingles (by comparing hashes) -----------
vector<string> matchedShingles(const vector<string>& refTokens, const vector<string>& tgtTokens, int K) {
    vector<string> matched;
    if (tgtTokens.size() < (size_t)K) return matched;
    unordered_set<long long> refHashes = getHashes(refTokens, K);
    const long long P = 1000003LL;
    const long long MOD = 1000000007LL;
    vector<long long> hv(tgtTokens.size());
    for (size_t i = 0; i < tgtTokens.size(); ++i) hv[i] = (long long)(std::hash<string>()(tgtTokens[i]) & 0x7fffffff);
    long long power = 1;
    for (int i = 0; i < K - 1; ++i) power = (power * P) % MOD;
    long long cur = 0;
    for (int i = 0; i < K; ++i) cur = ((cur * P) + hv[i]) % MOD;
    if (refHashes.count(cur)) {
        string s = tgtTokens[0];
        for (int j = 1; j < K; ++j) { s += " "; s += tgtTokens[j]; }
        matched.push_back(s);
    }
    for (size_t i = K; i < hv.size(); ++i) {
        cur = (cur - (hv[i - K] * power) % MOD + MOD) % MOD;
        cur = ((cur * P) + hv[i]) % MOD;
        if (refHashes.count(cur)) {
            size_t start = i - K + 1;
            string s = tgtTokens[start];
            for (int j = 1; j < K; ++j) { s += " "; s += tgtTokens[start + j]; }
            matched.push_back(s);
        }
    }
    return matched;
}

// ------------------- Mark plagiarized token positions ------------------
vector<int> markPlagiarism(const vector<string>& refTokens, const vector<string>& tgtTokens, int K, int level) {
    vector<int> mark(tgtTokens.size(), 0);
    if (tgtTokens.size() < (size_t)K) return mark;
    unordered_set<long long> refHashes = getHashes(refTokens, K);
    const long long P = 1000003LL;
    const long long MOD = 1000000007LL;
    vector<long long> hv(tgtTokens.size());
    for (size_t i = 0; i < tgtTokens.size(); ++i) hv[i] = (long long)(std::hash<string>()(tgtTokens[i]) & 0x7fffffff);
    long long power = 1;
    for (int i = 0; i < K - 1; ++i) power = (power * P) % MOD;
    long long cur = 0;
    for (int i = 0; i < K; ++i) cur = ((cur * P) + hv[i]) % MOD;
    if (refHashes.count(cur)) {
        for (int j = 0; j < K; ++j) mark[j] = max(mark[j], level);
    }
    for (size_t i = K; i < hv.size(); ++i) {
        cur = (cur - (hv[i - K] * power) % MOD + MOD) % MOD;
        cur = ((cur * P) + hv[i]) % MOD;
        if (refHashes.count(cur)) {
            size_t start = i - K + 1;
            for (size_t j = start; j <= i; ++j) mark[j] = max(mark[j], level);
        }
    }
    return mark;
}

// ------------------- Read file safe -------------------
bool readFileToString(const string& filename, string& out) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    stringstream ss;
    ss << in.rdbuf();
    out = ss.str();
    return true;
}

// ------------------- Get color based on severity level -------------------
string getColor(int level) {
    switch (level) {
    case 1: return RED;
    case 2: return YELLOW;
    case 3: return MAGENTA;
    default: return RESET;
    }
}

// ------------------- Input validation functions -------------------
bool isValidFilename(const string& filename) {
    if (filename.empty()) return false;
    if (filename.length() > 255) return false;

    // Check for invalid characters
    string invalidChars = "<>:\"|?*";
    for (char c : invalidChars) {
        if (filename.find(c) != string::npos) return false;
    }
    return true;
}

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getValidFilename(const string& prompt) {
    string filename;
    bool valid = false;

    while (!valid) {
        cout << prompt;
        getline(cin, filename);

        if (filename.empty()) {
            cout << RED << "Error: Filename cannot be empty!" << RESET << "\n";
            continue;
        }

        if (!isValidFilename(filename)) {
            cout << RED << "Error: Invalid filename! Avoid special characters like < > : \" | ? *" << RESET << "\n";
            continue;
        }

        if (!fileExists(filename)) {
            cout << RED << "Error: File '" << filename << "' does not exist!" << RESET << "\n";
            cout << YELLOW << "Please check the filename and try again." << RESET << "\n";
            continue;
        }

        valid = true;
    }

    return filename;
}

double getValidThreshold(const string& prompt, double minVal, double maxVal) {
    double value;
    bool valid = false;

    while (!valid) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                valid = true;
            }
            else {
                cout << RED << "Error: Value must be between " << minVal << " and " << maxVal << RESET << "\n";
            }
        }
        else {
            cout << RED << "Error: Invalid input! Please enter a numeric value." << RESET << "\n";
            clearInputBuffer();
        }
    }

    clearInputBuffer();
    return value;
}

int getValidMenuChoice(int minChoice, int maxChoice) {
    int choice;
    bool valid = false;

    while (!valid) {
        cout << BOLD_CYAN << "\nEnter your choice: " << RESET;
        if (cin >> choice) {
            if (choice >= minChoice && choice <= maxChoice) {
                valid = true;
            }
            else {
                cout << RED << "Error: Please enter a number between " << minChoice << " and " << maxChoice << RESET << "\n";
            }
        }
        else {
            cout << RED << "Error: Invalid input! Please enter a number." << RESET << "\n";
            clearInputBuffer();
        }
    }

    clearInputBuffer();
    return choice;
}

char getValidYesNo(const string& prompt) {
    char response;
    bool valid = false;

    while (!valid) {
        cout << prompt;
        cin >> response;
        response = tolower(response);

        if (response == 'y' || response == 'n') {
            valid = true;
        }
        else {
            cout << RED << "Error: Please enter 'y' for yes or 'n' for no." << RESET << "\n";
        }
        clearInputBuffer();
    }

    return response;
}

// ------------------- Display Menu -------------------
void displayMainMenu() {
    cout << "\n";
    cout << BOLD_CYAN << "+=================================================================+\n";
    cout << "|                                                                 |\n";
    cout << "|         ADVANCED PLAGIARISM DETECTION SYSTEM v2.0               |\n";
    cout << "|                                                                 |\n";
    cout << "+=================================================================+" << RESET << "\n\n";

    cout << BOLD_GREEN << "MAIN MENU:" << RESET << "\n\n";
    cout << "  " << CYAN << "1." << RESET << " Run Plagiarism Detection (Standard Mode)\n";
    cout << "  " << CYAN << "2." << RESET << " Run Plagiarism Detection (Custom Thresholds)\n";
    cout << "  " << CYAN << "3." << RESET << " View Current Threshold Settings\n";
    cout << "  " << CYAN << "4." << RESET << " About This System\n";
    cout << "  " << CYAN << "5." << RESET << " Help & Instructions\n";
    cout << "  " << CYAN << "6." << RESET << " Exit\n";
    cout << "\n" << CYAN << "-----------------------------------------------------------------" << RESET << "\n";
}

void displayAbout() {
    cout << "\n";
    cout << BOLD_CYAN << "+=================================================================+\n";
    cout << "|                     ABOUT THIS SYSTEM                           |\n";
    cout << "+=================================================================+" << RESET << "\n\n";

    cout << BOLD_GREEN << "Advanced Plagiarism Detection System v2.0\n" << RESET;
    cout << "\nThis system uses multiple algorithms to detect plagiarism:\n\n";
    cout << "  " << CYAN << "*" << RESET << " Word-level matching (single word detection)\n";
    cout << "  " << CYAN << "*" << RESET << " Phrase-level matching (3-word sequences)\n";
    cout << "  " << CYAN << "*" << RESET << " Sentence-level matching (5-word sequences)\n";
    cout << "  " << CYAN << "*" << RESET << " Cosine similarity analysis\n";
    cout << "  " << CYAN << "*" << RESET << " Stemming and stopword removal\n";
    cout << "  " << CYAN << "*" << RESET << " Rolling hash algorithm (Karp-Rabin)\n\n";

    cout << BOLD_GREEN << "Features:\n" << RESET;
    cout << "  " << CYAN << "*" << RESET << " Color-coded similarity highlighting\n";
    cout << "  " << CYAN << "*" << RESET << " Comprehensive analysis reports\n";
    cout << "  " << CYAN << "*" << RESET << " Customizable threshold settings\n";
    cout << "  " << CYAN << "*" << RESET << " Detailed match statistics\n\n";

    cout << "Press Enter to return to main menu...";
    cin.get();
}

void displayHelp() {
    cout << "\n";
    cout << BOLD_CYAN << "+=================================================================+\n";
    cout << "|                   HELP & INSTRUCTIONS                           |\n";
    cout << "+=================================================================+" << RESET << "\n\n";

    cout << BOLD_GREEN << "How to Use:\n" << RESET;
    cout << "\n1. Prepare two text files:\n";
    cout << "   " << CYAN << "*" << RESET << " Reference file (original/source document)\n";
    cout << "   " << CYAN << "*" << RESET << " Target file (document to check for plagiarism)\n\n";

    cout << "2. Select option 1 or 2 from the main menu:\n";
    cout << "   " << CYAN << "*" << RESET << " Option 1: Uses default thresholds\n";
    cout << "   " << CYAN << "*" << RESET << " Option 2: Allows custom threshold configuration\n\n";

    cout << "3. Enter the filenames when prompted\n";
    cout << "   " << CYAN << "*" << RESET << " Example: document.txt, essay.txt, paper.doc\n\n";

    cout << "4. Review the generated report\n";
    cout << "   " << CYAN << "*" << RESET << " Check similarity percentage\n";
    cout << "   " << CYAN << "*" << RESET << " Review color-coded matches\n";
    cout << "   " << CYAN << "*" << RESET << " Follow recommendations\n\n";

    cout << BOLD_GREEN << "Color Coding:\n" << RESET;
    cout << "   " << RED << "Red" << RESET << "     = Word-level matches\n";
    cout << "   " << YELLOW << "Yellow" << RESET << "  = Phrase-level matches\n";
    cout << "   " << MAGENTA << "Magenta" << RESET << " = Sentence-level matches\n\n";

    cout << BOLD_GREEN << "Threshold Levels (Default):\n" << RESET;
    cout << "   0-15%   = Original/Acceptable\n";
    cout << "   15-40%  = Low Similarity\n";
    cout << "   40-60%  = Moderate Similarity\n";
    cout << "   60-85%  = High Similarity\n";
    cout << "   85-100% = Critical/Potential Plagiarism\n\n";

    cout << "Press Enter to return to main menu...";
    cin.get();
}

void displayThresholdSettings(const ThresholdConfig& config) {
    cout << "\n";
    cout << BOLD_CYAN << "+=================================================================+\n";
    cout << "|                 CURRENT THRESHOLD SETTINGS                      |\n";
    cout << "+=================================================================+" << RESET << "\n\n";

    cout << BOLD_GREEN << "Active Thresholds:\n" << RESET;
    cout << "\n+-----------------------------+------------+\n";
    cout << "| Threshold Level             | Value      |\n";
    cout << "+-----------------------------+------------+\n";
    cout << "| Low Threshold               | " << setw(8) << fixed << setprecision(1) << config.lowThreshold << "% |\n";
    cout << "| Moderate Threshold          | " << setw(8) << config.moderateThreshold << "% |\n";
    cout << "| High Threshold              | " << setw(8) << config.highThreshold << "% |\n";
    cout << "| Very High Threshold         | " << setw(8) << config.veryHighThreshold << "% |\n";
    cout << "+-----------------------------+------------+\n\n";

    cout << "Press Enter to return to main menu...";
    cin.get();
}

// ------------------- Core Plagiarism Detection Function -------------------
void runPlagiarismDetection(bool useCustomThresholds) {
    cout << "\n";
    cout << BOLD_CYAN << "+=================================================================+\n";
    cout << "|               PLAGIARISM DETECTION ANALYSIS                     |\n";
    cout << "+=================================================================+" << RESET << "\n\n";

    // Get filenames with validation
    string refFile = getValidFilename(CYAN + "Enter reference filename: " + RESET);
    string tgtFile = getValidFilename(CYAN + "Enter target filename: " + RESET);

    // Configure thresholds
    ThresholdConfig config;

    if (useCustomThresholds) {
        cout << "\n" << BOLD_GREEN << "CUSTOM THRESHOLD CONFIGURATION:\n" << RESET;
        cout << YELLOW << "Note: Thresholds should be in ascending order\n" << RESET;

        config.lowThreshold = getValidThreshold("Enter low threshold (0-100, default 15): ", 0.0, 100.0);
        config.moderateThreshold = getValidThreshold("Enter moderate threshold (0-100, default 40): ", config.lowThreshold, 100.0);
        config.highThreshold = getValidThreshold("Enter high threshold (0-100, default 60): ", config.moderateThreshold, 100.0);
        config.veryHighThreshold = getValidThreshold("Enter very high threshold (0-100, default 85): ", config.highThreshold, 100.0);

        cout << GREEN << "\nCustom thresholds configured successfully!\n" << RESET;
    }

    // Read files
    string refRaw, tgtRaw;
    cout << "\n" << CYAN << "Reading files..." << RESET << "\n";

    if (!readFileToString(refFile, refRaw)) {
        cerr << BOLD_RED << "ERROR: Cannot open reference file: " << refFile << RESET << "\n";
        cout << "\nPress Enter to return to main menu...";
        cin.get();
        return;
    }

    if (!readFileToString(tgtFile, tgtRaw)) {
        cerr << BOLD_RED << "ERROR: Cannot open target file: " << tgtFile << RESET << "\n";
        cout << "\nPress Enter to return to main menu...";
        cin.get();
        return;
    }

    cout << GREEN << "Files loaded successfully!\n" << RESET;
    cout << CYAN << "Processing text..." << RESET << "\n";

    // Clean and tokenize
    string refClean = cleanText(refRaw);
    string tgtClean = cleanText(tgtRaw);

    vector<string> refTokensRaw = tokenizeBySpace(refClean);
    vector<string> tgtTokensRaw = tokenizeBySpace(tgtClean);

    if (refTokensRaw.empty() || tgtTokensRaw.empty()) {
        cerr << BOLD_RED << "ERROR: One of the files has no tokens after cleaning.\n" << RESET;
        cout << "\nPress Enter to return to main menu...";
        cin.get();
        return;
    }

    cout << GREEN << "Text processed successfully!\n" << RESET;
    cout << CYAN << "Analyzing similarity..." << RESET << "\n";

    vector<string> refTokensMatch = stemTokens(refTokensRaw);
    vector<string> tgtTokensMatch = stemTokens(tgtTokensRaw);

    unordered_set<string> stopwords = makeStopwords();
    vector<string> refTokensForSim = removeStopwords(refTokensMatch, stopwords);
    vector<string> tgtTokensForSim = removeStopwords(tgtTokensMatch, stopwords);

    double cosineSim = cosineSimilarity(refTokensForSim, tgtTokensForSim);

    vector<int> ks = { 1, 3, 5 };
    vector<string> levelName = { "Word-level", "Phrase-level", "Sentence-level" };
    vector<int> levelValue = { 1, 2, 3 };

    vector<int> finalMark(tgtTokensMatch.size(), 0);

    cout << "\n" << CYAN << "================ Matched Shingles =================" << RESET << "\n";
    for (size_t idx = 0; idx < ks.size(); ++idx) {
        int K = ks[idx];
        int level = levelValue[idx];
        auto matched = matchedShingles(refTokensMatch, tgtTokensMatch, K);
        cout << "\n" << BOLD_GREEN << "--- " << levelName[idx] << " (k=" << K << ") ---\n" << RESET;
        if (matched.empty()) {
            cout << GREEN << "No matches found.\n" << RESET;
        }
        else {
            unordered_set<string> uniq;
            for (const auto& s : matched) {
                if (!uniq.count(s)) {
                    cout << getColor(level) << s << RESET << "\n";
                    uniq.insert(s);
                }
            }
        }
        auto marks = markPlagiarism(refTokensMatch, tgtTokensMatch, K, level);
        for (size_t i = 0; i < finalMark.size(); ++i) finalMark[i] = max(finalMark[i], marks[i]);
    }

    int countWord = 0, countPhrase = 0, countSent = 0;
    for (int v : finalMark) {
        if (v == 1) ++countWord;
        else if (v == 2) ++countPhrase;
        else if (v == 3) ++countSent;
    }

    // Calculate similarity percentage
    int totalMatchedTokens = countWord + countPhrase + countSent;
    double similarityPercent = 0.0;
    if (tgtTokensRaw.size() > 0) {
        similarityPercent = (totalMatchedTokens * 100.0) / tgtTokensRaw.size();
    }

    // Generate comprehensive report
    SeverityAssessment assessment = assessSimilarity(similarityPercent, config);
    generateReport(similarityPercent, assessment, countWord, countPhrase,
        countSent, (int)tgtTokensRaw.size(), config);

    // Display additional metrics
    cout << "\n" << CYAN << "ADDITIONAL METRICS:\n" << RESET;
    cout << "Cosine Similarity (semantic): " << fixed << setprecision(2)
        << (cosineSim * 100.0) << "%\n";
    cout << "Token Match Similarity (exact): " << similarityPercent << "%\n\n";

    // Display highlighted text
    cout << "\n" << BOLD_GREEN << "--- Highlighted Target Text (Color-coded by severity) ---\n" << RESET;
    cout << RED << "[Red = Word-level] " << YELLOW << "[Yellow = Phrase-level] "
        << MAGENTA << "[Magenta = Sentence-level]" << RESET << "\n\n";

    int currentLevel = 0;
    for (size_t i = 0; i < tgtTokensRaw.size(); ++i) {
        int tokenLevel = finalMark[i];

        if (tokenLevel != currentLevel) {
            if (currentLevel > 0) {
                cout << RESET;
            }
            if (tokenLevel > 0) {
                cout << getColor(tokenLevel);
            }
            currentLevel = tokenLevel;
        }

        cout << tgtTokensRaw[i];
        if (i + 1 < tgtTokensRaw.size()) cout << " ";
    }

    if (currentLevel > 0) cout << RESET;
    cout << "\n\n";

    // Ask if user wants to save report
    char saveReport = getValidYesNo("\nWould you like to save this report to a file? (y/n): ");

    if (saveReport == 'y') {
        string reportFilename;
        cout << CYAN << "Enter filename for the report (e.g., report.txt): " << RESET;
        getline(cin, reportFilename);

        ofstream reportFile(reportFilename);
        if (reportFile.is_open()) {
            reportFile << "PLAGIARISM DETECTION ANALYSIS REPORT\n";
            reportFile << "====================================\n\n";
            reportFile << "Reference File: " << refFile << "\n";
            reportFile << "Target File: " << tgtFile << "\n\n";
            reportFile << "Similarity Score: " << fixed << setprecision(2) << similarityPercent << "%\n";
            reportFile << "Category: " << assessment.category << "\n";
            reportFile << "Recommendation: " << assessment.recommendation << "\n\n";
            reportFile << "Match Statistics:\n";
            reportFile << "  Word-level matches: " << countWord << " tokens\n";
            reportFile << "  Phrase-level matches: " << countPhrase << " tokens\n";
            reportFile << "  Sentence-level matches: " << countSent << " tokens\n";
            reportFile << "  Total tokens: " << tgtTokensRaw.size() << "\n";

            reportFile.close();
            cout << GREEN << "Report saved successfully to " << reportFilename << "!\n" << RESET;
        }
        else {
            cout << RED << "Error: Could not save report to file.\n" << RESET;
        }
    }

    cout << "\nPress Enter to return to main menu...";
    cin.get();
}

// ------------------- Main program -------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ThresholdConfig globalConfig; // Default configuration
    bool running = true;

    cout << BOLD_GREEN << "\nWelcome to the Advanced Plagiarism Detection System!\n" << RESET;
    cout << CYAN << "Initializing..." << RESET << "\n";

    while (running) {
        displayMainMenu();
        int choice = getValidMenuChoice(1, 6);

        switch (choice) {
        case 1:
            runPlagiarismDetection(false);
            break;

        case 2:
            runPlagiarismDetection(true);
            break;

        case 3:
            displayThresholdSettings(globalConfig);
            break;

        case 4:
            displayAbout();
            break;

        case 5:
            displayHelp();
            break;

        case 6:
            cout << "\n" << BOLD_GREEN << "Thank you for using the Plagiarism Detection System!\n" << RESET;
            cout << CYAN << "Exiting...\n" << RESET;
            running = false;
            break;

        default:
            cout << RED << "Invalid choice. Please try again.\n" << RESET;
        }
    }

    return 0;
}