# Cursor Semantics

## Decision

A Cursor represents an insertion position between characters

Cursor::offset uses the "half-open boundary convention":

0 <= offset <= piece.length

offset == 0
Cursor is before the first character of the piece.

offset == piece.length
Cursor is after the last character of the piece.

## Insertion Semantics

insert(text) inserts text at the cursor position and moves the cursor
to immediately after the inserted text.
