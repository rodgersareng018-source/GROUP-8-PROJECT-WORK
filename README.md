# GROUP-8-PROJECT-WORK
C++ educational Support Vector Machine project developed by a group of eight members.

## EduSVM: Educational Support Vector Machine

A beginner-friendly, modular Support Vector Machine (SVM) C++ library designed to predict student performance metrics basing on attendance, study hours, and assignment scores. This project is built collaboratively as a team to demonstrate clean software engineering principles, class knowledge of object oriented programming, automated build testing, and mathematical optimization tracking.

## 🎯 Project Overview & Summary

EduSVM is built from scratch using strict object-oriented programming standards in C++. Instead of relying on external heavy machine learning frameworks, our group manually maps out structural matrix components, mathematical cost calculations, data normalization pipelines, and verification utilities. This provides a transparent look at how a linear binary Support Vector Machine model functions under the hood.

The software architecture completely covers the following fundamental system modules:
1.  **Dataset Loading:** Parsing and ingestion of structured CSV data records.
2.  **Data Preprocessing:** Integrity validation checks against empty, mismatch, or non-finite inputs.
3.  **Feature Scaling:** Min-max normalization mapping variable feature ranges evenly between `0.0` and `1.0`.
4.  **Hyperplane Representation & Margin Computation:** Mapping geometric decision boundaries and optimization gaps.
5.  **Hinge Loss:** Calculating individual classification loss margins ($L = \max(0, 1 - yf(x))$).
6.  **Regularization:** Tracking structural L2 norms to prevent training overfitting.
7.  **Gradient-Based Optimization:** Performing iterative weight vector updates via descending steps.
8.  **Support Vector Identification:** Isolating the critical edge data coordinates determining boundaries.
9.  **Decision Function:** Processing dot product operations ($w \cdot x + b$) across evaluation rows.
10. **Binary Classification:** Outputting clean categorical predictions ($-1$ or $+1$).
11. **Model Evaluation:** Computing overall model performance accuracy, precision, recall, and $F_1$-scores.

---

### Core Features
- **Data Integrity Pipeline:** Safe verification against empty rows, non-finite values (`NaN`/`Inf`), and layout errors.
- **Feature Scaling:** Min-max normalization mapping variable ranges evenly between `0.0` and `1.0`.
- **Mathematical Optimization:** Custom hyperplane calculations, hinge loss monitoring, and L2 regularization penalties.

## Recommended Project structure

project-name/
├── README.md
├── CMakeLists.txt
├── LICENSE
├── .gitignore
├── include/
│   └── project-name/
│       ├── module1.hpp
│       ├── module2.hpp
│       └── ...
├── src/
│   ├── module1.cpp
│   ├── module2.cpp
│   └── ...
├── tests/
│   ├── test_module1.cpp
│   ├── test_module2.cpp
│   └── ...
├── examples/
│   ├── example1.cpp
│   ├── example2.cpp
│   └── ...
├── reports/
│   ├── week-01.md
│   ├── week-02.md
│   ├── week-03.md
│   └── final-report.md
└── data/
    ├── input/
    └── output/


## 📅 Project Progress Tracker

### Foundations and Implementation Roadmap
*   **1 — Data Ingestion & Structures:** Established core data model structures, file system storage layers, and completed **Dataset Loading** mechanisms.
*   **2 — Structural Interfaces:** Formulated header blueprints for data handling, initial scaling matrices, and mathematical error monitoring components.
*   **3 — Core Integration & Verification:** Completed comprehensive validation checks inside `src/preprocessor.cpp` (**Data Preprocessing**), resolved branch merge blockages, and verified build stability.
*   **4 — Scaling & Spatial Modeling:** Activating **Feature Scaling** transformations alongside geometric **Hyperplane Representation & Margin Computation** configurations.
*   **5 — Mathematical Costs & Optimization:** Implementing **Hinge Loss** calculations, **Regularization** penalties, and building the iterative **Gradient-Based Optimization** training engine loop.
*   **6 — Prediction Pipeline & Support Vectors:** Isolating vector parameters via **Support Vector Identification**, activating the model's **Decision Function**, and wrapping up **Binary Classification** outputs.
*   **7 — Verification & Evaluation Release:** Launching the **Model Evaluation** metrics suite, generating comprehensive validation reports, and completing packaging tasks.


---

## 💻 Technical Implementation Details

Our group successfully mapped out and implemented the 11 architectural requirements from the project brief using pure Object-Oriented C++ principles, entirely free from external machine learning dependencies:

### 1. Dataset Loading
- **Mechanism:** Implemented structured data models inside `include/edusvm/dataset.hpp` and `src/dataset.cpp`. 
- **Details:** The framework parses tabular structural comma-separated records, binding variable academic input feature dimensions (`study_hours`, `attendance`, `assignment_score`) as a row matrix (`std::vector<std::vector<double>>`) directly coupled to an aligned, 1-dimensional array of binary output classifications.

### 2. Data Preprocessing
- **Mechanism:** Handled via `include/edusvm/preprocessor.hpp` and `src/preprocessor.cpp`.
- **Details:** Validates raw inbound data matrices before optimization. The preprocessor enforces a gatekeeping pipeline that screens for three critical failure modes:
  - Validates that the database is not empty.
  - Ensures structural feature-to-label row alignment matches exactly ($N_{\text{rows}} == N_{\text{labels}}$).
  - Screens data points using `std::isnan` and `std::isinf` to actively discard corrupted or non-finite inputs.

### 3. Feature Scaling
- **Mechanism:** Engineered inside `include/edusvm/scaler.hpp` and `src/scaler.cpp`.
- **Details:** Utilizes **Min-Max Normalization** parameters calculated during model fitting. For every independent feature vector column, the library isolates absolute bounding boundaries ($x_{\min}, x_{\max}$) and safely transforms parameters to an even scale using:
  $$x_{\text{scaled}} = \frac{x - x_{\min}}{x_{\max} - x_{\min}}$$
  To protect against zero-division errors on constant-valued columns where $x_{\max} == x_{\min}$, the scaler catches the exception boundary and explicitly defaults the transformation coefficient to `0.0`.

### 4. Hyperplane Representation & Margin Computation
- **Mechanism:** Formulated in `include/edusvm/svm.hpp` and `src/svm.cpp`.
- **Details:** The separating structural decision hyperplane boundaries are mathematically represented by an adjustable weight vector $w$ (`std::vector<double>`) and a scalar bias factor $b$ (`double`). The software computes the model's **Functional Margin** across data tracking nodes by evaluating:
  $$\gamma_i = y_i (w \cdot x_i + b)$$
  where $y_i \in \{-1, +1\}$ denotes the authentic categorical label.

### 5. Hinge Loss
- **Mechanism:** Programmed in `include/edusvm/loss.hpp` and `src/loss.cpp`.
- **Details:** To measure classification optimization errors during training phases, the library computes structural **Hinge Loss** penalties for individual rows using a conditional lower bound:
  $$L(x) = \max(0.0, 1.0 - y_i(w \cdot x_i + b))$$
  If a student record falls safely beyond the target margins on the correct side of the decision boundary, the loss evaluates cleanly to `0.0`. Otherwise, it scales linearly relative to the margin violation distance.

### 6. Regularization
- **Mechanism:** Calculated inside `include/edusvm/regularization.hpp` and `src/regularization.cpp`.
- **Details:** Employs **L2 Regularization Penalties (Ridge)** to strictly penalize excessive model weights and prevent data overfitting. Guided by a customizable optimization parameter ($\lambda$), the penalty algorithm computes the squared Euclidean norm of the active weight vector:
  $$R(w) = \frac{\lambda}{2} \sum_{j=1}^{d} w_j^2$$

### 7. Gradient-Based Optimization
- **Mechanism:** Handled in the primary training loop inside `src/svm.cpp`.
- **Details:** Iteratively updates the structural hyperplane parameter variables over multiple training epochs. By computing partial sub-gradient derivatives of the joint objective function (Hinge Loss + L2 Penalty) with respect to weights and bias, the system applies descending steps scaled by a learning rate ($\eta$):
  $$w \leftarrow w - \eta \left( \lambda w - \sum_{i \in \text{misclassified}} y_i x_i \right)$$
  $$b \leftarrow b - \eta \left( - \sum_{i \in \text{misclassified}} y_i \right)$$

### 8. Support Vector Identification
- **Mechanism:** Integrated into the internal mathematical decision matrix loops inside `src/svm.cpp`.
- **Details:** Identifies crucial coordination points that directly dictate the positioning of margins. Data vectors that satisfy the condition $y_i(w \cdot x_i + b) \leq 1.0$ are isolated as the active **Support Vectors** that structurally define the geometric boundary lines.

### 9. Decision Function
- **Mechanism:** Declared as `edusvm::decisionFunction` within `src/svm.cpp`.
- **Details:** Computes the mathematical dot product combination of the optimized weights against an anonymous test student feature array, adding the structural internal bias:
  $$f(x) = \left( \sum_{j=1}^{d} w_j \cdot x_j \right) + b$$
  This yields a continuous linear classification score reflecting both spatial orientation and boundary distance.

### 10. Binary Classification
- **Mechanism:** Wrapped inside the ultimate prediction interfaces of `src/svm.cpp`.
- **Details:** Maps the continuous score generated by the decision function into crisp binary predictions. The sign step function yields:
  $$\text{Prediction}(x) = \begin{cases} +1 & \text{if } f(x) \geq 0.0 \quad (\text{PASS}) \\ -1 & \text{if } f(x) < 0.0 \quad (\text{FAIL}) \end{cases}$$

### 11. Model Evaluation
- **Mechanism:** Structured within the testing metrics suite inside `src/svm.cpp`.
- **Details:** Validates generalized model predictive power against unseen testing subsets by compiling a confusion matrix of True Positives ($TP$), False Positives ($FP$), True Negatives ($TN$), and False Negatives ($FN$) to output:
  - **Accuracy:** $\frac{TP + TN}{TP + TN + FP + FN}$
  - **Precision:** $\frac{TP}{TP + FP}$
  - **Recall:** $\frac{TP}{TP + FN}$
  - **$F_1$-Score:** $2 \cdot \frac{\text{Precision} \cdot \text{Recall}}{\text{Precision} + \text{Recall}}$



## 💻 Fresh Machine Installation & Setup Guide

Follow these exact steps to clone, configure, build, and run the project layout completely from scratch on any new machine.

### 1. Prerequisites
Ensure you have a working C++ compiler installed on your system. 
- **Windows:** Install [MSYS2 MinGW](https://msys2.org) to get `g++`.
- **Linux:** Run `sudo apt install build-essential` via terminal.

Verify your compiler version by running:
```bash
g++ --version
```

### 2. Download the Workspace
Clone the remote repository locally and jump directly into the workspace root folder:
```bash
git clone https://github.com
cd GROUP-8-PROJECT-WORK
```

### 3. Run Preprocessor Validation Tests
Since some environments might not have globally mapped `cmake` paths, use the direct, unified compilation string to compile and evaluate the preprocessing verification routines:
```bash
# Compile the module directly from the root workspace folder
g++ -std=c++17 -I include src/preprocessor.cpp tests/test_preprocessor.cpp -o test_preprocessor

# Run the test binary wrapper
./test_preprocessor
```

---

## 👥 Group 8 Contribution Directory

| Representative | Operational Branch | Assigned Project Core Scope Domains |
| :--- | :--- | :--- |
| **Martha** | `martha-branch` | Dataset Loading, Sample CSV Ingestion, Example Source Scripts |
| **Charity** | `Charity-branch` | Data Preprocessing, Row Alignment Checks, Integrity Testing |
| **Reacho** | `Reacho-branch` | Feature Scaling, Min-Max Normalization Parameters |
| **Komdit** | `komdit-branch` | Hyperplane Representation, Margin Computation, Decision Function |
| **Levy** | `levy-branch` | Hinge Loss Evaluation, L2 Regularization Penalties |
| **Rodgers** | `rodgers-branch` | Gradient-Based Optimization, Support Vector Identification |
| **Vivian** | `vivian-branch` | Binary Classification, Model Evaluation Analytics Metrics |
| **Monica** | `monica-branch` | Automated Compilation Configurations, Cross-Platform Automation |