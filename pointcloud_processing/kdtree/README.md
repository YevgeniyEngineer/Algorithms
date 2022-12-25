# KDTree

Balanced KD-Tree with multithreading

## Benchmarks for 3D KD-Tree:

| Number of Points | KD-Tree Construction (Sequential) | KD-Tree Construction (Parallel) | KD-Tree Query |
|------------------|-----------------------------------|---------------------------------|---------------|
| 10,000           | 0.00165645                        | 0.00188793                      | 2.189e-06     |
| 100,000          | 0.0247329                         | 0.010698                        | 4.475e-06     |
| 1,000,000        | 0.247114                          | 0.0850132                       | 3.734e-06     |
| 10,000,000       | 2.9207                            | 1.12204                         | 5.224e-06     |
| 50,000,000       | 6.17333                           | 15.8889                         | 5.218e-06     |