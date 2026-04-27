# 📄 Advanced Plagiarism Detection System

A high-performance **C++-based plagiarism detection engine** that analyzes textual similarity across multiple levels (word, phrase, and sentence) using **Cosine Similarity** and **K-gram fingerprinting**. The system provides real-time analysis, severity classification, and structured reporting—making it suitable for academic and research applications.

---

## 🚀 Overview

This project implements a modular plagiarism detection pipeline that:

* Processes and normalizes textual data
* Extracts meaningful linguistic features
* Computes similarity using vector-space modeling
* Detects copied sequences using hashing techniques
* Generates a detailed plagiarism report with severity levels

---

## 🧠 Core Features

### 🔍 Multi-Level Similarity Detection

* Word-level matching
* Phrase-level matching (via K-grams)
* Sentence-level semantic comparison

### ⚡ Real-Time Analysis

* Fast preprocessing and similarity computation
* Immediate feedback on document comparison

### 📊 Severity Classification

* Configurable thresholds
* Categories such as:

  * ORIGINAL / ACCEPTABLE
  * LOW SIMILARITY
  * MODERATE SIMILARITY
  * CRITICAL (Potential Plagiarism)

### 🎯 Precision-Oriented Processing

* Stopword removal
* Lightweight stemming
* Token normalization

### 📈 Detailed Reporting

* Similarity percentage
* Severity category
* Review recommendations

---

## 🏗️ System Architecture

```
Input Documents
       ↓
Text Preprocessing (Cleaning + Normalization)
       ↓
Tokenization
       ↓
Stopword Removal & Stemming
       ↓
Feature Extraction (K-grams + Frequencies)
       ↓
Cosine Similarity Computation
       ↓
Severity Assessment
       ↓
Report Generation
```

---

## ⚙️ Technologies Used

* **Language:** C++
* **Compiler:** GCC / MinGW
* **IDE:** Visual Studio
* **Platform:** Windows

### 📦 Data Structures

* `vector` → token storage
* `unordered_map` → frequency vectors
* `unordered_set` → hash-based fingerprinting
* `string` → text processing

---

## 🧪 Algorithmic Workflow

1. Read input documents
2. Clean and normalize text
3. Tokenize into words
4. Remove stopwords
5. Apply stemming
6. Generate K-grams
7. Compute cosine similarity
8. Classify similarity severity
9. Generate structured report

---

## 📌 Key Algorithms

### 🔹 Cosine Similarity

* Represents documents as vectors
* Measures angle similarity between them
* Effective for semantic comparison

### 🔹 K-Gram Hashing (Shingling)

* Breaks text into overlapping sequences
* Uses hashing for efficient comparison
* Detects copied phrases accurately

---

## 📊 Example Output

```
Similarity Score: 67.5%
Category: MODERATE SIMILARITY
Recommendation: Review suggested for potential plagiarism.
```

---

## ⚠️ Challenges Faced

* Handling large documents efficiently
* Detecting paraphrased (non-exact) plagiarism
* Balancing performance with accuracy

---

## 🔮 Future Enhancements

* 🤖 Machine Learning for paraphrase detection
* 📄 Support for PDF and Word documents
* 🌐 Graphical User Interface (GUI)
* ☁️ Web-based deployment

---

## 👥 Contributors

* **Abdul Rafay** → Algorithm optimization, testing, Cosine Similarity
* **Abdul Wahab** → Tokenization logic, Rabin-Carp Algorithm, error handling, reporting system
* **Ebbad** Ur Rehman → Core idea, hashing, report writing
* **Faizan Afzal** → Implementation refinement, configuration tuning, Stemming

---

## 📜 License

This project is intended for **educational and academic use**. You may modify and extend it for learning purposes.

---

## ⭐ Final Note

This system demonstrates how classical algorithms like **Cosine Similarity** combined with **hash-based fingerprinting** can produce an effective plagiarism detection tool without requiring heavy machine learning models.

If you find this project useful, consider giving it a ⭐ on GitHub!

---
