# KDTree

Balanced KD-Tree with multithreading

## Benchmarks for 3D KD-Tree:

CPU: 12th Gen Intel® Core™ i7-12700H × 20 | 16.0 GiB RAM

| Number of Points | KD-Tree Construction (Sequential) | KD-Tree Construction (Parallel) | KD-Tree Query |
|------------------|-----------------------------------|---------------------------------|---------------|
| 10,000           | 0.00165645                        | 0.00188793                      | 2.189e-06     |
| 100,000          | 0.0247329                         | 0.010698                        | 4.475e-06     |
| 1,000,000        | 0.247114                          | 0.0850132                       | 3.734e-06     |
| 10,000,000       | 2.9207                            | 1.12204                         | 5.224e-06     |
| 50,000,000       | 15.8889                           | 6.17333                         | 5.218e-06     |

## Benchmarks Many-to-many 3D KD-Tree Searches:

| Number of KD-Tree Points | Number of Target Points | Total Query Time (s) |
|--------------------------|-------------------------|----------------------|
| 10,000                   | 10,000                  | 0.00546097           |
| 100,000                  | 100,000                 | 0.0170865            |
| 1,000,000                | 1,000,000               | 0.0841204            |
| 10,000,000               | 10,000,000              | 1.53782              |