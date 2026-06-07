# DiaCare

A console-based C++ app that predicts diabetes risk using Logistic Regression and manages patients through a menu-driven hospital workflow. The ML model trains from scratch on startup — no external libraries, no frameworks.

---

## How it works

On launch, the program loads the Pima Indians Diabetes Dataset, trains a Logistic Regression model using Gradient Descent, then opens a menu for managing patients. The risk probability from the model isn't just a number — it controls where patients are placed in the ward list, how often they appear in the critical monitor, and whether they get flagged at all.

---

## Build & Run

**Requires:** A C++ compiler. Nothing else.

```bash
g++ main.cpp src/*.cpp -o diacare -static
```

Put `diabetes.csv` in a `data/` folder next to the executable. The model trains automatically on startup before the menu opens.

---

## Features

```
 1  About Model          — model weights and evaluation metrics
 2  Register Patient     — add to intake queue (name + age only)
 3  Process Next Patient — enter vitals, run prediction, admit to ward
 4  View Active Ward     — all patients sorted by risk (highest first)
 5  Navigate Ward        — move through ward with N/P keys
 6  New Checkup          — update vitals, re-run ML, re-sort ward
 7  View History         — full checkup history for a patient
 8  Risk Trajectory      — trend over last 3 visits, flags rapid deterioration
 9  Monitor Critical     — cycle through high-risk patients (P ≥ 0.60)
10  What-If Simulation   — test hypothetical vitals against current risk
11  Discharge Patient    — remove patient from system (undoable)
12  Undo Last Action     — reverse the last recorded action
13  View Queue           — patients waiting to be processed
14  Action History       — log of all actions, newest first
15  Search by ID         — fast patient lookup
```

---

## ML Pipeline

**Dataset:** Pima Indians Diabetes Dataset — 768 records, 8 features, binary outcome (diabetic / not diabetic).

**Preprocessing (all stats computed on training data only):**
- Impute impossible zero values with the non-zero training median
- Clip outliers to 5th–95th percentile range (Winsorization)
- Engineer two features: `Glucose_BMI = Glucose × BMI` and `Glucose_Age = Glucose / (Age + 1)`
- Standardize all features: `scaled = (value − mean) / std`

**Model:**
```
z    = bias + Σ(weights[j] × features[j])
ŷ    = 1 / (1 + e^−z)

loss gradient:
  error     = label − ŷ
  grad_w[j] = (1/n) × Σ(error × features[j])

update:
  weights[j] += lr × grad_w[j]
```

Classification threshold is **0.46** (not 0.5) — tuned to prioritize recall, since missing a diabetic patient is worse than a false alarm.

**Results on test set:** Accuracy 73.2% · Precision 58.6% · Recall 66.7%

---

## Data Structures

Each structure has one specific job — nothing is there just to fill a list:

| Structure | Job |
|-----------|-----|
| Queue | FIFO patient intake |
| Doubly Linked List | Active ward, sorted descending by risk probability |
| Singly Linked List | Per-patient checkup history (inside each Patient object) |
| Circular Linked List | Critical monitor — loops through high-risk patients |
| Stack | Undo — stores deep copies of patient state |
| BST | O(log n) patient lookup by ID |

One `Patient` object per patient in heap memory. Every structure holds a pointer to the same object — no duplicates.

---

## Risk Levels

| Level | Probability | What it means |
|-------|-------------|---------------|
| LOW | < 0.20 | Routine monitoring |
| MODERATE-LOW | 0.20 – 0.40 | Lifestyle changes recommended |
| MODERATE | 0.40 – 0.60 | Doctor visit within 2 weeks |
| HIGH | 0.60 – 0.80 | Doctor visit this week |
| CRITICAL | ≥ 0.80 | Immediate attention required |

Patients above 0.60 go into the critical monitor. Higher-risk patients get more slots in the cycle (up to 3×) so they surface more often.
