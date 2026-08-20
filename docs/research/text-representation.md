## How is text represented in memory

### 20/08/2026

Approach-1:

Simplest approach is using a vector of lines, used in [Build your own text editor](https://viewsourcecode.org/snaptoken/kilo/)

Problem with this approach:

Memory Cost: Potentially proportional to the whole document

Modification Cost: As the strings are stored contiguously, inserting involves shifting the elemnets throughout the array

Approach-2:

[Rope](<https://en.wikipedia.org/wiki/Rope_(data_structure)>)

Concepts to investigate:

[Copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write)
// fork() is an example of this approach

Problem with this approach:

The rope data structure is complex and complicated, and difficult to maintain in a project like this.

Moreover, every parent node will take up extra memory, and a pure rope implementation (without any buffer) will make a new node for every character, which is slow

Approach-3:

[Gap Buffer](https://en.wikipedia.org/wiki/Gap_buffer)

A dynamic array that allows efficient insertion and deletion operations clustered near the same location. Useful because of the fact that most changes in text editors are localized at or around the cursor. Used in editors like Emacs

Approach-4:

[Piece Table](https://en.wikipedia.org/wiki/Piece_table)
Initially a reference (or "span") to the whole of the original file is created, which represents the as yet unchanged file. Subsequent inserts and deletes replace a span by combinations of one, two, or three references to sections of either the original document or to a buffer holding inserted text. The text of the original document is held in one immutable block, and the text of each subsequent insert is stored in new immutable blocks.

This is supposed to be elegant, simple and fast, with the added benefit of making operations like Undo/Redo easy, beacuse the piece table history can be looked up

### Questions to Investigate:

1. Efficieny of operations in actual mathematical terms (Asymptotic notation), and what operations to look at ?
2. What happens when the changes are not localized in a Gap Buffer? What if gap is moved ?
3. Benefit of the immutability approach of the Piece Table

### Investigation:

1.  | Operation       |    Vector of Lines |   Gap Buffer\* |     Rope |                   Piece Table |
    | --------------- | -----------------: | -------------: | -------: | ----------------------------: |
    | Insertion       |    O(N) worst case | O(1) amortized |  O(logN) | O(1) at the cursor's position |
    | Deletion        |    O(N) worst case | O(1) amortized |  O(logN) | O(1) at the cursor's position |
    | Cursor Movement | O(1) per character |           O(D) |  O(logN) |                        O(1)\^ |
    | Random Access   |               O(1) |           O(1) |  O(logN) |                          O(P) |
    | Find Line       |               O(N) |           O(N) | O(N)\*\* |                    O(N)\*\*\* |

    **where**

    **N=Size of whole document**

    **D=Distance travered from starting position**

    **P=Number of pieces**

\*Gap Buffer also has a resize operation in case gap gets full. The new gap allocated is usually 2 times the old gap, and is an O(N) operation
\*\*Modern editors like Zed are able to bring it down to O(logN), usig SumTrees
\*\*\*Can be bought down to O(logP) using a Balanced Binary Tree implementation

\^ Assuming it is implemented as a simple variable

2. The gap needs to be shifted the whole distance, resulting in the above listed O(D) complexity.
3. Immutability rule is a key property for how the operations are designed

**Given the above research, piece table is the data structure I will be using in my approach**
