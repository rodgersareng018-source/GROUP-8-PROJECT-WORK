# dataset interface Review
## Files reviewed
-include/edusvm/dataset.hpp
-src/dataset.cpp

## Review checklist
- [x] Header and implementation agree.
- [x] Dataset features are numerical values.
- [x] Every student has one label.
- [x] All featre rows have equal length. 
- [x] Features are finite.
- [x] Labels are restricted to -1 and 1.
- [x] Module can be included by another C++ file.

## Findings
No issues were found. 
The header defines Dataset and declares validateDataset().
The implementation in src/dataset.cpp matches and correctly checksrow counts, feature counts, finite values, and label values (-1 and 1).

Reviewer: Member 2 - AKII RACHAEL ALORO - rachaelaloro-branch
Date: Week 1



