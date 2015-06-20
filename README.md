# Dining Philosophers

> This problem illustrates how to implement mutual exclusion between processes that compete for access to overlapping sets of shared variables.

**Aim**: using Monitors, simulate:

- `N > 2` philosphers in a circular table.
- N forks
- A philosopher `thinks` or `eats`.
  -   getting to the `eat` state requires him to grab its left-side fork as well as its right-side one.
- Philosophers have a different degree of importance, indicated by a weight (`p_i, i \in [1,...,N) > 1`)
- Every philospher wastes the same amount of time to each when they can get to the `eat` state, but wastes a random quantity thinking.

Each philosopher is a thread that competes for shared resources. We must guarantee justice and non-starvation.


## Monitor Specifics

Allowed methods: `emtpy(cv)`, `wait(cv)`, `wait(cv, rank)`, `signal(cv)`, `signal_all(cv)`, `minrank(cv)`


## Input

```sh
usage:
      $ ./main f R U|P

F     destination of the file specifying the
      number of philosophers and their weights

R     number of portions to be eaten

U|P   whether it should include weights or it
      runs uniformly
```


