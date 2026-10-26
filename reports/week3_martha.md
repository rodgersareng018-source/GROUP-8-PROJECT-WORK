Example_dataset.cpp weeek3 report

I made a small example showing how to build and validate an 'edusvm::Dataset'.

How it works

1. Build the data. 'buildSampleDataset()' fills a 'Dataset' by hand with six students. Each row holds two features (study hours per week, attendance %). Each label is '+1' (passed) or '-1' (failed).
2. Validate it. 'report()' calls 'edusvm::validateDataset()'. On success it prints the student and feature counts. On failure it prints the error message.
3. Break it on purpose. 'main()' runs three cases:
   a valid dataset (passes)
   a label of '0' (fails: labels must be -1 or +1)
   a row with a missing feature (fails: inconsistent feature counts)

