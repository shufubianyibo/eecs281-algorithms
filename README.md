# EECS 281 — Algorithms and Data Structures

C++ graph search, priority queues, database queries, minimum spanning trees, and traveling salesperson algorithms.

Coursework portfolio of **Han Sun ([shufubianyibo](https://github.com/shufubianyibo))**. This repository collects source snapshots from local projects so they can be browsed directly from my personal GitHub profile.

## Main projects

### [Graph optimization: MST and traveling salesperson — Project 4](p4-among-us/)

A C++ command-line solver with three modes: a minimum spanning tree under region constraints, a fast heuristic tour, and an exact traveling salesperson search. The exact solver starts from an insertion-based tour improved with 2-opt, then uses branch-and-bound with an MST-based lower bound over the remaining vertices. This project brings together greedy construction, local improvement, and pruning of a combinatorial search space.

The core implementation is [`AmongUs.hpp`](p4-among-us/AmongUs.hpp); [`main.cpp`](p4-among-us/main.cpp) provides the command-line entry. Sample inputs and expected outputs cover the different modes.

### [SillyQL: an in-memory query engine — Project 3](p3-sillyql/)

A small relational database interpreter supporting table creation, insertion, conditional selection, deletion, joins, and index generation. Hash and ordered-tree indexes provide different lookup strategies, and index contents are rebuilt as the stored rows change. The project connects query semantics with concrete choices of data structure.

Start with [`command.hpp`](p3-sillyql/command.hpp) for table storage, filtering, and indexing, and [`silly.cpp`](p3-sillyql/silly.cpp) for command handling.

### [Priority queue implementations — Project 2B](p2b-priority-queues/)

Several interchangeable priority queues implement a common C++ interface, including sorted storage, a binary heap, and a pairing heap. The pairing heap adds node-based melding and priority updates, while the shared interface makes it possible to compare designs without changing client code.

Compare [`BinaryPQ.hpp`](p2b-priority-queues/BinaryPQ.hpp), [`PairingPQ.hpp`](p2b-priority-queues/PairingPQ.hpp), and [`SortedPQ.hpp`](p2b-priority-queues/SortedPQ.hpp).

Other substantial coursework includes [grid routing with stack/queue search](p1-rescue-the-countess/) and a [priority-driven simulation](p2-the-walking-deadline/). Smaller lab folders remain available in the full index below.

## All projects

| Project | Source files |
| --- | ---: |
| [Connected Components AG Problem](Connected%20Components%20AG%20Problem/) | 1 |
| [Hash Table Starter Files](Hash%20Table%20Starter%20Files/) | 2 |
| [p4-among-us](p4-among-us/) | 2 |
| [AVL Tree Starter Files](AVL%20Tree%20Starter%20Files/) | 2 |
| [Autograder Starter Files](Autograder%20Starter%20Files/) | 2 |
| [p3-sillyql](p3-sillyql/) | 4 |
| [p1-rescue-the-countess](p1-rescue-the-countess/) | 6 |
| [p2-the-walking-deadline](p2-the-walking-deadline/) | 7 |
| [l01-music-sorting-main](l01-music-sorting-main/) | 6 |
| [p2b-priority-queues](p2b-priority-queues/) | 7 |
| [p0-statistics](p0-statistics/) | 1 |
| [Anyfi__Starter_Files_export](Anyfi__Starter_Files_export/) | 3 |
| [String_Library_Starter_Files_export](String_Library_Starter_Files_export/) | 3 |

## Running the projects

Each directory is independent. Read its README, build file, tests, and `PORTFOLIO.md` before running. Original course infrastructure, data, or toolchains may be required. This publication checks source packaging; it does not certify that every historical project builds on a current machine.

## Attribution

These are individual and collaborative course projects. Course starter code, supplied tests, and third-party dependencies are included where needed. Original credits and licenses are retained. Each `PORTFOLIO.md` links to the source repository and records its base commit and commit authors when available. Source-file counts include tests and scaffolding and are not a measure of individually authored work. Snapshots were collected on 2026-11-17; original working directories were left unchanged.
