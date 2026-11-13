The sample dataset is `data/input/students.csv`.

It has six illustrative rows with `study_hours`, `attendance`,
`assignment_score`, and `label` columns (`1` = PASS, `-1` = FAIL). This is not
real student data. CSV loading is implemented by `DatasetLoader::loadFromCSV`
and `loadDatasetCsv`, and is covered by `tests/test_dataset_csv.cpp`.
