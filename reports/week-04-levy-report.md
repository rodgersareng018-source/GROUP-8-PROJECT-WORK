Week 4 Report: Review of Hinge Loss and Regularization
Name: Levy 
Group: Group 8 
 Branch: levy 
Task: To review the hinge-loss calculation and the regularization penalty, with formula checks and unit tests.
1. Task Summary
My Week 4 responsibility was to review the verification of the mathematical calculations. The assignment asked me to inspect the existing loss and regularization code (include/edusvm/loss.hpp, src/loss.cpp and the regularization files), check them against the standard formulas, and add unit tests. The expected deliverable is verified loss and regularization calculations with unit tests.
The two formulas I am verifying are:
•	Hinge loss: L = max(0, 1 − y · f(x)), where the label y must be 1 or −1 and f(x) = w · x + b.
•	L2 regularization penalty: R(w) = ½ Σ wᵢ².
2. Work Completed
•	Read the Week 4 plan and identified my assignment and deliverables.
•	Cloned the group repository and merged the latest main into my levy branch so that all Week 1, Week 2 and week 3 work was available, including loss.hpp, loss.cpp, regularization.hpp, regularization.cpp and the existing tests.
•	Reviewed the interface in loss.hpp: the function takes the weights, bias, features and label, and returns the loss together with an error message for invalid input.
•	Calculated expected values by hand so the code can be checked against known answers:
o	y = 1, f(x) = 2 gives a loss of 0.
o	y = 1, f(x) = 0.4 gives a loss of 0.6.
o	y = −1, f(x) = 0.5 gives a loss of 1.5.
o	A point exactly on the margin (y · f(x) = 1) gives a loss of 0.
o	Weights (2, 1) give a regularization penalty of 2.5, and zero weights give 0.
•	Checked on whether improvement can be done on non-finite input validation in hinge loss
4. Challenges and Solutions
The pushing the report through vs code was difficult, I had to use my group mate to review my work and push it.
5. Next Steps
1.	Finish checking loss.cpp and the regularization files against the formulas.
2.	reviewed tests to tests/test_loss.cpp and tests/test_regularization.cpp.
3.	Built and run all tests.

