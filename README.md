# CrossWord-Generator
This project contains crossword puzzle generator written in C. Given a list of words, the
program automatically constructs a valid crossword grid such that every
intersecting pair of across and down words shares the correct letter at
their crossing point.

## Project Description

Most simple crossword tools either require a human to design the grid, or
let a user fill in an already-designed puzzle. This project instead
constructs a crossword from scratch: given a word list, it builds a
grid where words are placed so every intersection is letter-consistent,
using a backtracking search guided by a trie (for pattern matching against
partially-filled slots) and a length-bucketed hash table (for narrowing
word candidates by slot length before pattern matching).

## Goals

- Generate a structurally valid crossword grid from an input word list.
- Support topic-based word selection (e.g. animals, space) rather than a single fixed word bank.
- Support difficulty levels that change the structure of the generated grid (word length distribution, intersection density), not just a label.
- Practice multi-file C project organization, manual memory management, and non-trivial data structure design (trie + hash table + backtracking).

## Functional Requirements

1. Accept a word list as input (file-based, optionally filtered by topic).
2. Generate a crossword grid of a specified size using the input words.
3. Ensure every intersection between an across word and a down word shares the same letter.
4. Use backtracking to recover from dead-end placements and try alternative words.
5. Use a hash table to filter candidate words by length before pattern-matching.
6. Use a trie to match candidate words against known letter constraints at a slot.
7. Output the completed grid in a human-readable format.
8. Report failure gracefully if no valid grid can be constructed from the given word list/size.
9. Support a user-selected difficulty (Easy/Medium/Hard) that changes word length preference and grid density during generation.

## Non-Functional Requirements

- **Performance**: completes grid generation for a few hundred words on a
  15x15 grid within a few seconds.
- **Reliability**: reliably finds a valid grid when one exists for the
  given word list/size.
- **Memory usage**: trie and hash table structures are sized
  proportionally to word list size; all allocated memory is freed before
  program exit.
- **Portability**: compiles and runs with `gcc` on Linux with no external
  dependencies.
- **Usability**: output is readable directly in a terminal.
- **Maintainability**: code is split into logical modules (trie, hash
  table, grid, slot, main) rather than one file.

## Learning Outcomes

- Designed and implemented core data structures from scratch in C,
  including a trie and a hash table, rather than relying on a
  standard library.
- Applied backtracking search to a real constraint-satisfaction
  problem (filling a grid where every intersection must agree).
- Practiced manual memory management in C (malloc/free discipline,
  avoiding leaks) in a project large enough for it to matter.
- Structured a multi-file C project (headers in include/, sources in
  src/) with a Makefile, rather than a single monolithic file.
- Translated a project proposal with functional and non-functional
  requirements into an actual implementation plan.

  ## Prerequisites

- `gcc` (or any C11-compatible compiler)
- `make`
- Linux/macOS/WSL terminal (no OS-specific dependencies)

## Build & Run

```bash
make        # compiles the project into ./crossword
./crossword # run it
```

or in one step:

```bash
make run
```

To clean build artifacts:

```bash
make clean
```

## License

MIT — see LICENSE.