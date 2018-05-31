Atom
====

[![Build Status](https://travis-ci.org/daddinuz/atom.svg?branch=master)](https://travis-ci.org/daddinuz/atom)

Atoms are pointers to a unique, immutable, sequence of zero or more arbitrary bytes.
There is only a single occurrence of any atom, which is why it’s called an atom.
Two atoms are identical if they point to the same location.
One of the advantages of atoms is that comparing two byte sequences for equality is performed by simply comparing pointers.
Another advantage is that using atoms saves space because there’s only one occurrence of each sequence.
Atoms are often used as keys in data structures that are indexed by sequences of arbitrary bytes instead of by integers.
