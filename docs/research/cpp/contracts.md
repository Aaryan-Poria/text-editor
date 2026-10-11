## Contracts in CPP

A C++ data structure is said to be a state machine with an invariant, and its public interface defines the legal ways users can interact with the state machine

- The interface should state preconditions, postconditions, error behavior, and ownership semantics.

A useful contract has roughly these dimensions:

1. Valid Object States
2. Preconditions
3. Postconditions
4. Const/mutation
5. Ownership/lifetime
6. Invalid input behaviour
7. Exception Behaviour
8. Complexity

Moreover, following one of the major programming techniques in CPP, [RAII](https://en.cppreference.com/cpp/language/raii) tells us two important things: If a constructor succeeds, it must create a valid object. Otherwise, in the case of failure, no object should have been created. We prefer creating an object in the required state rather than creating an incomplete state and fixing it afterward

Ownership is reflected through using concepts like: unique_ptr (sole ownership), shared_ptr (shared ownership), and & (non-owning functions). Plus, we favor returning values rather than exposing output parameters. This has become possible due to modern compiler optimizations, starting from C++11

- An exception must never leave an object violationg the invarianrt. Importantly, **Destructors should NOT throw**. This is also true for deallocation and similar cleanup operations.
